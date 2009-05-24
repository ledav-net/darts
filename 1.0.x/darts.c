
#include <ncurses.h>
#include <malloc.h>
#include <string.h>

#define	P_NAME		"Darts"
#define P_VERSION	"1.0.0"

#define	MAX_COLS	132
#define MAX_LINES	48

#include "matrix_def.h"
#include "finish_def.h"

struct	_player		{	char		name[16];		/* Player's name					*/
				WINDOW		*main,			/* player's main window					*/
						*stat,			/* statistical sub-window (top)				*/
						*list,			/* Points list sub-window (left)			*/
						*total,			/* Total points + finish help (center) 			*/
						*input;			/* Input's window (bottom)				*/
				unsigned	won,			/* Number of winning games				*/
						lost;			/* Number of loosed games				*/
				int		lastpnts;		/* Player's last points					*/
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
void	gameboard_focus(struct _player *);				/* Focus on specified player's game board		*/
void	gameboard_readinput(struct _player *, char *);			/* Read player's score					*/
void	gameboard_update(struct _player *, int);			/* Update content of specifid player with -n points	*/
void	gameboard_reset(struct _player *);

void	main_clear(struct _game *);					/* Clear the main screen				*/
#define	main_show(a)	wrefresh((a)->main)				/* Show modifications on screen				*/
void	main_draw(struct _game *);					/* draw the main screen					*/
void	main_get_infos(struct _game *);					/* get game informations				*/

void	popup(char *);							/* Popup a centralized box with message and wait for a  */
									/* Key to be pressed					*/

void	wdrawbox(WINDOW *, char, char, int, int, int, int);		/* Draw a box						*/

struct _player *	init_players(struct _game *);			/* Allocate all player's structures and init datas	*/

struct _game	game;

#ifdef	_DEBUG
FILE	*debugfile;
#define	printdebug(a, b...)	fprintf(debugfile, a"\n", b)
#define	fdebug(a)		fputs(a"\n", debugfile)
#else
#define	printdebug(a, b...)
#define fdebug(a)
#endif

void	wdrawbox(WINDOW *win, char vch, char hch, int ny, int nx, int y, int x)
{
	wmove(win, y, x);		waddch(win, 'A'); whline(win, hch, nx-2);
	wmove(win, y, x+nx-1);		waddch(win, 'B');
	wmove(win, y+ny-1, x);		waddch(win, 'C'); whline(win, hch, nx-2);
	wmove(win, y+ny-1, x+nx-1);	waddch(win, 'D');

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
	y = (MAX_LINES - nlines - 1) >> 1;
	
	pop = newwin(nlines, ncols, y, x);
	wborder(pop,'*','*','=','=',0,0,0,0);

	wmove(pop, 2, 3); waddstr(pop, msg);
	wmove(pop, 2, 3 + n + 1);
	wgetnstr(pop, buf, 50);

	delwin(pop);
}

void	popup(char *msg)
{
	WINDOW	*pop;
	int	 nlines, n, ncols, x, y;

	n = strlen(msg);

	nlines = (n / 50.0 + 0.99) + 4;		/* 4 lines for border and empty line	*/
	ncols  = n + 4;				/* 4 cols  "           "          "	*/

	x = (MAX_COLS - ncols - 1) >> 1;
	y = (MAX_LINES - nlines - 1) >> 1;

	pop = newwin(nlines, ncols, y, x);
	wborder(pop,'*','*','=','=',0,0,0,0);

	wmove(pop, 2, 2); waddstr(pop, msg);

	noecho(); wgetch(pop); echo();

	delwin(pop);	
}

void    main_draw(struct _game *game)
{
	wborder(game->main,'|','|','-','-',0,0,0,0);
	wmove(game->main,10,40); waddstr(game->main,"Nombre de joueur  :");
	wmove(game->main,11,40); waddstr(game->main,"Points de départ  :");
	wmove(game->main,12,40); waddstr(game->main,"Type de partie    :");
}

void    main_get_infos(struct _game *game)
{
	do wmove(game->main,10,60); while ( wscanw(game->main,"%d", &game->playersnbr) == 0 || game->playersnbr <= 0 || game->playersnbr > 4 );
	do wmove(game->main,11,60); while ( wscanw(game->main,"%d", &game->startpoints) == 0 || game->startpoints <= 0 || game->startpoints > 901 );
	do wmove(game->main,12,60); while ( wscanw(game->main,"%d", &game->gametype ) == 0 );

	game->gamenbr = 1;
}

void	main_clear(struct _game *g)
{
	wclear(g->main);
	wmove(g->main, MAX_LINES-1, 0);
	waddstr(g->main, P_NAME" v"P_VERSION", (C) 2002 by David De Grave. All rights reserved.");
}

void    gameboard_focus(struct _player *player)
{
	wmove(player->input,1,5);
}

void	gameboard_show(struct _player *p)
{
	touchwin(p->main);
	wrefresh(p->main);

	fdebug("voilaquoi");
}

void	put_matrix(WINDOW *win, const char mtx[MTX_LINES][MTX_COLS+1], int y, int x)
{
	register int	i;
	for ( i=0 ; i < MTX_LINES ; i++ ) mvwaddstr(win, y+i, x, mtx[i]);
}

void	gameboard_drawtotal(struct _player *p)
{
	int	d, r, i=2;

	wclear(p->total);

	d = p->score;

	do
	{
		r = d % 10;
		put_matrix(p->total, matrix[r], 1, 8+(i*MTX_COLS));
		d = d / 10;
	}
	while ( d && i-- );

	if ( game.gametype < 2 && p->score <= 170 )
	{
		mvwprintw(p->total, 1+MTX_LINES+3,1, "Best Finish: %s", bestfinish[p->score]);
	}
}

void	gameboard_addtolist(struct _player *p, int value)
{
	wprintw(p->list, " %d. %d\n", ++p->turn, value);
}

void	gameboard_update(struct _player *p, int value)
{
	p->score    -= value;
	p->lastpnts  = value;

	gameboard_addtolist(p, value);
	gameboard_drawtotal(p);
}

void	gameboard_rollback(struct _player *p)
{
	int	x, y;

	if ( p->turn > 0 && p->lastpnts > 0 )
	{
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

	wdrawbox(p->main, '|', '-', 4,  66,  0,  0);	/* Stats	*/
	wdrawbox(p->main, '|', '-', 17, 12,  4,  0);	/* List		*/
	wdrawbox(p->main, '|', '-', 3,  66, 21,  0);	/* Input	*/

	gameboard_drawtotal(p);

	wmove(p->stat,  0, 1); wprintw(p->stat,  "Name: %-30s won: %d  lost: %d", p->name, p->won, p->lost);
	wmove(p->stat,  1, 1); waddstr(p->stat,  "...");
	wmove(p->input, 0, 1); waddstr(p->input, "Score ?");
}

void	gameboard_readinput(struct _player *p, char *buf)
{
	mvwgetnstr(p->input, 0, 9, buf, 50);
	mvwaddstr(p->input,  0, 9, "                                                    ");
}

struct _player *       init_players(struct _game *game)
{
	struct _player		*playersptr;
	int			p,y,x;
	int			size;

	size = game->playersnbr * sizeof(struct _player);
	
	playersptr = malloc(size);
	memset(playersptr, 0, size);

	for ( p=0 ; p < game->playersnbr ; p++ )
	{
		x = p % 2;
		y = p / 2;

		playersptr[p].main  = subwin(game->main, 24, 66, y*24, x*66);
		
		playersptr[p].stat  = subwin(playersptr[p].main, 2,  64, (y*24) +  1, (x*66) +  1);
		playersptr[p].list  = subwin(playersptr[p].main, 15, 10, (y*24) +  5, (x*66) +  1);
		playersptr[p].total = subwin(playersptr[p].main, 15, 51, (y*24) +  5, (x*66) + 13);
		playersptr[p].input = subwin(playersptr[p].main, 1,  64, (y*24) + 22, (x*66) +  1);

		playersptr[p].score = game->startpoints;
		playersptr[p].turn  = 0;

		scrollok(playersptr[p].list, TRUE);

		printdebug("[%p] [%p] [%p] [%p] [%p]", playersptr[p].main, playersptr[p].stat, playersptr[p].list
						     , playersptr[p].total, playersptr[p].input);

		gameboard_draw(&playersptr[p]);
		gameboard_show(&playersptr[p]);

		mvwgetnstr(playersptr[p].stat, 0, 7, playersptr[p].name, sizeof(playersptr[0].name));
	}

	return	playersptr;
}

int	isnumber(char *b)
{
	register char	i, n = strlen(b);
	for ( i=0 ; i < n ; i++ ) if ( ! isdigit(b[i]) ) return 0;
	return (i != 0);
}

int	main(void)
{
	struct	_player		*players;
	char			 buf[256];
	int			 i, actual_player = 0, last_player = 0;

	#ifdef	_DEBUG
	if ( (debugfile = fopen("debug.log","wt")) == NULL )
	{
		puts("Can't create debug file in current directory !");
		exit(-1);
	}
	#endif

	game.main = initscr();

	main_draw(&game);
	main_show(&game);

	main_get_infos(&game);	/* Get basical informations about the game */

	main_clear(&game);
	main_show(&game);

	players = init_players(&game); /* Initialize & show all game boards */

	while ( 1 )
	{
		if ( actual_player == game.playersnbr ) actual_player = 0;

		gameboard_focus(&players[actual_player]);
		gameboard_readinput(&players[actual_player], buf);

		printdebug("Readed: %s", buf);

		if ( ! isnumber(buf) )
		{
			fdebug("Not a number !");

			switch ( toupper(buf[0]) )
			{
				case 'S': goto endgame;		/* Stop the game	*/
					  break;

				case 'N': actual_player++;	/* Skip Current Player	*/
					  continue;

				case 'B': gameboard_rollback(&players[last_player]);
					  gameboard_show(&players[last_player]);
					  actual_player = last_player;
					  continue;
				
				default:  continue;
			}
		}
		else
		{
			fdebug("This is a Number ...");
			
			i = atoi(buf);
			
			if ( i <= players[actual_player].score && i <= 180 )
			{
				gameboard_update(&players[actual_player], atoi(buf));
				gameboard_show(&players[actual_player]);
			}
			else
			{
				beep();
				continue;
			}
		}

		if ( players[actual_player].score == 0 )
		{
			sprintf(buf, "  %s WINS THE GAME !!!", players[actual_player].name);
			popup(buf);

			popupask("Restart the same game ?", buf, 1);

			if ( toupper(buf[0]) == 'N' ) break;
			
			for ( i=0 ; i < game.playersnbr ; i++ )
			{
				if ( i == actual_player ) players[i].won++;
				else			  players[i].lost++;
				
				players[i].lastpnts = 0;
				players[i].score    = game.startpoints,
				players[i].turn     = 0;
				
				game.gamenbr++;

				wclear(players[i].list);
				gameboard_draw(&players[i]);
			}
			
			touchwin(game.main);
			main_show(&game);
		}

		last_player = actual_player++;
	}

endgame:

	endwin();

	return 0;
}
