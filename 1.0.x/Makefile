
OBJS:=		darts.o
BUILD:=		darts

build:		darts

debug:		darts.c
		gcc -ggdb $< -o $(BUILD) -lncurses -D _DEBUG

darts:		$(OBJS)
		gcc $(OBJS) -o $(BUILD) -lncurses -lallproc

clean:
		rm -f *~ $(OBJS)

distclean:	clean
		rm -f $(BUILD)

$(OBJS):
		gcc -O -c $*.c
