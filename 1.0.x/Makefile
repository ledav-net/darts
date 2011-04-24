
OBJS:=		darts.o
BUILD:=		darts

build:		$(BUILD)

install:	$(BUILD)
		@rm -f ~/bin/$(BUILD)
		@cp -v $(BUILD) ~/bin/$(BUILD)

debug:		$(BUILD).c
		gcc -ggdb $< -o $(BUILD) -lallproc -lncurses -D_DEBUG

$(BUILD):	$(OBJS)
		gcc $(OBJS) -o $(BUILD) -lncurses -lallproc

clean:
		rm -f *~ $(OBJS)

distclean:	clean
		rm -f $(BUILD) debug.log

.c.o:
		gcc -O -c $*.c
