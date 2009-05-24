
OBJS=		darts.o

build:		darts

debug:		darts.c
		gcc darts.c -o darts -lncurses -D _DEBUG

darts:		$(OBJS)
		gcc $(OBJS) -o darts -lncurses -lallproc

clean:
		rm -f *~ $(OBJS)

$(OBJS):
		gcc -O3 -c $*.c
