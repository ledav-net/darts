
OBJS:=		darts.o
BUILD:=		darts

build:		$(BUILD)

install:	$(BUILD)
		@rm -f ~/bin/$(BUILD)
		@cp -v $(BUILD) ~/bin/$(BUILD)

debug:		darts.c
		gcc -ggdb $< -o $(BUILD) -lallproc -lncurses -D_DEBUG

darts:		$(OBJS)
		gcc $(OBJS) -o $(BUILD) -lncurses -lallproc

clean:
		rm -f *~ $(OBJS)

distclean:	clean
		rm -f $(BUILD)

$(OBJS):
		gcc -O -c $*.c
