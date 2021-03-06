


OBJS:=		darts.o common.o
BUILD:=		darts
INSTALLDIR:=	/usr/local/bin

build:		$(BUILD)

install:	$(BUILD)
		@rm -f $(INSTALLDIR)/$(BUILD)
		@cp -v $(BUILD) $(INSTALLDIR)/$(BUILD)

debug:		$(BUILD).c
		gcc -ggdb $< -o $(BUILD) -lncurses -D_DEBUG

$(BUILD):	$(OBJS)
		gcc $(OBJS) -o $(BUILD) -lncurses

clean:
		rm -f *~ $(OBJS)

distclean:	clean
		rm -f $(BUILD) debug.log

.c.o:
		gcc -O -c $*.c
