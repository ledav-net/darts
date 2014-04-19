
#include <allproc.h>

#include <ncurses.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define	P_NAME		"Darts"
#define P_VERSION	"1.1.0"

#define	MAX_COLS	128
#define MAX_ROWS	48

#include "matrix_def.h"
#include "finish_def.h"

struct	_player		{	char		name[16];		/* Player's name					*/
				WINDOW		*main,			/* player's main window					*/
						*stat,			/* statistical sub-window (top)				*/
						*list,			/* Points list sub-window (left)			*/
						*total,			/* Total points + finish help (center) 			*/
						*input;			/* Input's window (bottom)				*/
				unsigned	won,			/* Number of games won					*/
						lost;			/* Number of games lost					*/
				int		lastpnts,		/* Player's last points					*/
						bestPoint,		/* Best point over the games				*/
						bestFinish;		/* Best finish over the games				*/
				unsigned	score,
						turn;			/* Player's actual remaining points			*/
			};

struct	_game		{	WINDOW		*main;			/* Main window						*/
				int		playersnbr,		/* Number of players					*/
						startpoints,		/* Starting points (501,701, ..)			*/
						playerstart;		/* Starting player number				*/
				int		gametype,		/* Gametype (learn, normal, compet)			*/
						gamenbr;		/* Current playing game number				*/
			};


void	gameboard_draw(struct _player *);				/* Draw the player's game board				*/
void	gameboard_show(struct _player *);
void	gameboard_focus(struct _player *, struct _player *);		/* Focus on specified player's game board		*/
void	gameboard_readinput(struct _player *, char *);			/* Read player's score					*/
void	gameboard_update(struct _player *, int);			/* Update content of specifid player with -n points	*/
void	gameboard_reset(struct _player *);

void	main_clear(struct _game *);					/* Clear the main screen				*/
void	main_show(struct _game *);
void	main_redraw(struct _game *);
void	main_get_infos(struct _game *);					/* get game informations				*/

void	popup(char *);							/* Popup a centralized box with message and wait for a  */
									/* Key to be pressed					*/

void	wdrawbox(WINDOW *, char, char, int, int, int, int);		/* Draw a box						*/

struct _player *	init_players(struct _game *);			/* Allocate all player's structures and init datas	*/

struct _game	game;

#ifdef	_DEBUG
FILE	*debugfile;
#define	printdebug(a, b...)	fprintf(debugfile, a"\n", b), fflush(debugfile)
#define	fdebug(a)		fputs(a"\n", debugfile)
#define assert(a)		if ( a ) { printf("Assert Error line %d !\n", __LINE__); exit(1); }
#else
#define	printdebug(a, b...)
#define fdebug(a)
#define assert(a)
#endif

void	wdrawbox(WINDOW *win, char vch, char hch, int ny, int nx, int y, int x)
{
	wmove(win, y, x);		waddch(win, '+'); whline(win, hch, nx-2);
	wmove(win, y, x+nx-1);		waddch(win, '+');
	wmove(win, y+ny-1, x);		waddch(win, '+'); whline(win, hch, nx-2);
	wmove(win, y+ny-1, x+nx-1);	waddch(win, '+');

	wmove(win, y+1, x);		wvline(win, vch, ny-2);
	wmove(win, y+1, x+nx-1);	wvline(win, vch, ny-2);
}

void	popupask(char *msg, char *buf, int maxlen)
{
	WINDOW  *pop;
	int      nlines, n, ncols, x, y;
	
	n = strlen(msg);
	
	nlines = 5;			/* 4 lines for border and empty line							*/
	ncols  = n + maxlen + 6 + 1;	/* 6 cols  "           "          "   + 1 for space between question and answer		*/

	x = (MAX_COLS - ncols - 1) >> 1;
	y = (MAX_ROWS - nlines - 1) >> 1;
	
	pop = newwin(nlines, ncols, y, x);
	wborder(pop,'*','*','=','=','*','*','*','*');

	mvwaddstr (pop, 2, 3, msg);
	mvwgetnstr(pop, 2, 3 + n + 1, buf, maxlen);

	delwin(pop);
}

void	popup(char *msg)
{
	WINDOW	*pop,  *popder;
	int	nlines, ncols, x, y;
	char	*lf;

	if ( (lf = strchr(msg, '\n')) ) {	/* Find the bigger sub-string (if it exists) */
		int	t;
		char	*plf = msg;
		nlines = ncols = 0;
		do {
			nlines++;
			if ( (t = lf-plf) > ncols ) ncols = t;
			plf = ++lf;
		}
		while ( (lf = strchr(lf, '\n')) );

		if ( (t = strlen(plf)) ) {
			nlines++;
			if ( t > ncols ) ncols = t;
		}
	}else{
		ncols  = strlen(msg);
		nlines = 1;
	}

	nlines += 4;	/* 4 -> 2 lines * 2 for border and empty line	*/
	ncols  += 4;	/* 4 -> 2 cols      "           "        column	*/

	x = (MAX_COLS - ncols - 1) >> 1;
	y = (MAX_ROWS - nlines - 1) >> 1;

	pop    = newwin(nlines, ncols, y, x);
	popder = derwin(pop, nlines - 4, ncols - 3, 2, 2);
	
	wborder(pop,'*','*','=','=','*','*','*','*');
	waddstr(popder, msg);
	wrefresh(pop);
	
	noecho(); wgetch(popder); echo();

	delwin(popder);
	delwin(pop);
}

void	show_help(void)
{
	popup(
	"Here are the commands you can use:\n"
	"\n"
	"N -> Next player's turn\n"
	"B -> Rollback the previous action\n"
	"R -> Redraw screen\n"
	"Q -> Quit\n"
	"\n"
	"H -> Show this help screen");
}

void    main_get_infos(struct _game *game)
{
	char	b[4];

	do {
		popupask("Nombre de joueurs (1-4) ?", b, 1);
		game->playersnbr = atoi(b);
	}
	while ( ! between(game->playersnbr, 1, 4)   );

	do {
		popupask("Point de depart (2-901) ?", b, 3);
		game->startpoints = atoi(b);
	}
	while ( ! between(game->startpoints, 2, 901) );

	do {
		popupask("Type de partie (1=normal, 2=competition) ?", b, 1);
		game->gametype = atoi(b);
	}
	while ( ! between(game->gametype, 1, 2) );
}

void	main_clear(struct _game *g)
{
	attr_t	sattr;
	short	spair;
	wclear(g->main);
	wmove(g->main, MAX_ROWS, 0);
	wattr_get(g->main, &sattr, &spair, NULL);
	wattr_set(g->main, A_BOLD, 4, NULL);
	waddstr(g->main, P_NAME" v"P_VERSION", (C) 2002 by David De Grave. All rights reserved.");
	wattr_set(g->main, sattr, spair, NULL);
}

void	main_show(struct _game *g)
{
	touchwin(g->main);
	wrefresh(g->main);
	fdebug("main_show() called");
}

void    gameboard_focus(struct _player *prev, struct _player *next)
{
	wmove(next->input, 1, 5);
}

void	gameboard_show(struct _player *p)
{
	touchwin(p->main);
	wrefresh(p->main);
	fdebug("gameboard_show() called");
}

void	put_matrix(WINDOW *win, const char mtx[MTX_LINES][MTX_COLS+1], int y, int x)
{
	register int	i;
	for ( i=0 ; i < MTX_LINES ; i++ ) mvwaddstr(win, y+i, x, mtx[i]);
}

void	gameboard_drawstatus(struct _player *p)
{
	wclear(p->stat);

	mvwprintw(p->stat, 0, 1, "Name: %-30s", p->name);
	mvwprintw(p->stat, 1, 1, " Won: %-2d  Lost: %-2d  Best point: %-3d  Best finish: %-3d"
				, p->won, p->lost, p->bestPoint, p->bestFinish);

	mvwchgat(p->stat, 0,  7, 30, A_BOLD, 2, NULL);

	mvwchgat(p->stat, 1,  7,  3, A_BOLD, 1, NULL);
	mvwchgat(p->stat, 1, 17,  3, A_BOLD, 1, NULL);   
	mvwchgat(p->stat, 1, 33,  3, A_BOLD, 1, NULL); 
	mvwchgat(p->stat, 1, 51,  5, A_BOLD, 1, NULL);  

}

bool	is_finishable(int score)
{
	if ( score == 50 ) return true;
	if ( score <= 40 && (score % 2) == 0 ) return true;
	return false;
}

void	gameboard_drawtotal(struct _player *p)
{
	register int d, i;

	wclear(p->total);

	if ( is_finishable(p->score) )	wcolor_set(p->total, 3, 0);
	else				wcolor_set(p->total, 1, 0);

	d = p->score;
	for ( i=3 ; i && d ; i-- ) {
		put_matrix(p->total, matrix[d%10], 1, 8 + ((i-1)*MTX_COLS));
		d /= 10;
	}

	if ( game.gametype < 2 && p->score <= 170 ) {
		wcolor_set(p->total, 2, 0);
		mvwprintw(p->total, 1+MTX_LINES+3,1, "Best Finish: %s", bestfinish[p->score]);
	}
}

void	gameboard_addtolist(struct _player *p, int value)
{
	wprintw(p->list, " %d. %d\n", ++p->turn, value);
}

void	gameboard_update(struct _player *p, int value)
{
	if ( p->bestPoint < value ) {
		p->bestPoint = value;
		gameboard_drawstatus(p);
	}

	p->score    -= value;
	p->lastpnts  = value;

	gameboard_addtolist(p, value);
	gameboard_drawtotal(p);
}

void	gameboard_rollback(struct _player *p)
{
	int	x, y;

	if ( p->turn > 0 && p->lastpnts > 0 ) {
		p->score    += p->lastpnts;
		p->turn--;
		
		gameboard_addtolist(p, - p->lastpnts);
		gameboard_drawtotal(p);

		p->turn--;
		p->lastpnts  = -1;
	}
}

void	gameboard_draw(struct _player *p)
{
	wborder(p->main, '|','|','-','-',0,0,0,0);

	wdrawbox(p->main, '|', '-', 4,  MAX_COLS/2,  0,  0);	/* Stats	*/
	wdrawbox(p->main, '|', '-', 17, 12,          4,  0);	/* List		*/
	wdrawbox(p->main, '|', '-', 3,  MAX_COLS/2, 21,  0);	/* Input	*/

	mvwaddstr(p->input, 0, 1, "Score (H for Help) ?");

	gameboard_drawtotal(p);
	gameboard_drawstatus(p);
}

void	gameboard_readinput(struct _player *p, char *buf)
{
	mvwgetnstr(p->input, 0, 22, buf, 3);
	mvwaddstr (p->input, 0, 22, "     ");
}

struct _player *       init_players(struct _game *game)
{
	struct _player	*players;
	int		p, x, y;

	players = calloc(game->playersnbr, sizeof(struct _player));

	for ( p=0 ; p < game->playersnbr ; p++ ) {
		x = p % 2;
		y = (p & 2) >> 1;

		players[p].main  = derwin(game->main, (MAX_ROWS/2), (MAX_COLS/2), y*(MAX_ROWS/2), x*(MAX_COLS/2));

		players[p].stat  = derwin(players[p].main,  2,    (MAX_COLS/2)-2,  1,  1);
		players[p].list  = derwin(players[p].main, 15,                10,  5,  1);
		players[p].total = derwin(players[p].main, 15, (MAX_COLS/2)-2-13,  5, 13);
		players[p].input = derwin(players[p].main,  1,    (MAX_COLS/2)-2, 22,  1);

		players[p].score = game->startpoints;
		players[p].turn  = 0;

		scrollok(players[p].list, TRUE);

		/* Colors set */
		wattron(players[p].total, A_BOLD);

		printdebug("[%p] [%p] [%p] [%p] [%p]"
			, players[p].main,  players[p].stat, players[p].list, players[p].total, players[p].input);

		gameboard_draw(&players[p]);
		gameboard_show(&players[p]);

		wattr_on(players[p].stat, COLOR_PAIR(2) | A_BOLD, NULL);
		mvwgetnstr(players[p].stat, 0, 7, players[p].name, sizeof(players[0].name));
	}

	return players;
}

void	reset_players(struct _game *g, struct _player *p)
{
	int	i;

	for ( i=0 ; i < g->playersnbr ; i++ ) {
		p[i].lastpnts = 0;
		p[i].score    = g->startpoints,
		p[i].turn     = 0;
		p[i].lost     = g->gamenbr - p[i].won;

		wclear(p[i].list);
		gameboard_draw(&p[i]);
	}
}

int	main(void)
{
	struct _player	*players, *p;
	char		buf[256];
	int		i, actual_player = 0, last_player = 0;

	#ifdef	_DEBUG
	if ( (debugfile = fopen("debug.log","wt")) == NULL ) {
		puts("Can't create debug file in current directory !");
		exit(-1);
	}
	#endif

	game.main = initscr();

	if ( has_colors() ) {
		printdebug("Colors enabled !","");
		start_color();
		init_pair(1, COLOR_CYAN,  COLOR_BLACK); /* Total		*/
		init_pair(2, COLOR_WHITE, COLOR_BLACK); /* Player's name	*/
		init_pair(3, COLOR_RED,   COLOR_BLACK); /* Player is on finish	*/
		init_pair(4, COLOR_BLACK, COLOR_BLACK); /* Gray color           */
	}

	main_get_infos(&game);
	main_clear(&game);
	main_show(&game);

	players = init_players(&game); /* Initialize & show all the game boards */

	while (1) {
		if ( actual_player == game.playersnbr ) actual_player = 0;
		p = &players[actual_player];

		gameboard_focus(&players[last_player], p);
		gameboard_readinput(p, buf);

		printdebug("Readed: %s", buf);

		if ( ! isnumber(buf) ) {
			fdebug("Not a number !");
			switch ( toupper(buf[0]) ) {
				case 'Q': goto endgame;					/* (Q) Quit				*/
				case 'N': actual_player++; break;			/* (N) Next player's turn		*/
				case 'B': gameboard_rollback(&players[last_player]);	/* (B) Rollback the previous action	*/
					  gameboard_show(&players[last_player]);
					  actual_player = last_player;
					  break;
				case 'R': main_show(&game);
					  break;
				case 'H': show_help();
					  main_show(&game);
					  break;
			}
		}else{
			fdebug("This is a Number ...");
			i = atoi(buf);

			if ( i <= p->score && i <= 180 ) {
				gameboard_update(p, i);
				gameboard_show(p);
			}else{
				beep();
				continue;
			}

			if ( p->score == 0 ) {
				sprintf(buf, "%s WINS THE GAME !!!", p->name);
				popup(buf);

				popupask("Restart the same game ?", buf, 1);
				if ( toupper(buf[0]) == 'N' ) goto endgame;

				if ( p->bestFinish < i ) p->bestFinish = i;

				p->won++;
				game.gamenbr++;
				last_player = actual_player = 0;

				reset_players(&game, players);
				main_show(&game);
			}
			else	last_player = actual_player++;
		}
	}
endgame:
	endwin();
	return 0;
}
