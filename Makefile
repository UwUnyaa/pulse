.POSIX:

CFLAGS     = -Wall -Wextra -g
GTKFLAGS   = $$(pkg-config --cflags --libs gtk+-2.0)
CAIROFLAGS = $$(pkg-config --cflags --libs pangocairo)
CC         = gcc

TAGS       = etags

all: pulse

clean:
	rm -f *~ *.o TAGS pulse badge/badgedata.h utils/dumpfile

tags: *.c *.h
	$(TAGS) *.[ch]

utils/dumpfile: utils/dumpfile.c

fail.o: fail.c

system.o: system.c

cpu.o: cpu.c cpu-types.h types.h constants.h

vendor.o: vendor.c vendor-constants.h macros.h

interface.o: interface.c interface-types.h types.h constants.h events.o
	$(CC) interface.c -o interface.o -c $(CFLAGS) $(GTKFLAGS)

events.o: events.c gpointer-structs.h cpu-types.h interface-types.h types.h macros.h
	$(CC) events.c -o events.o -c $(CFLAGS) $(GTKFLAGS)

badge/badgedata.h: utils/dumpfile badge/badge.png
	utils/dumpfile badge/badge.png badge/badgedata.h

badge.o: badge.c badge-constants.h constants.h macros.h badge/badgedata.h
	$(CC) badge.c -o badge.o -c $(CFLAGS) $(GTKFLAGS) $(CAIROFLAGS)

main.o: main.c types.h interface-types.h cpu-types.h gpointer-structs.h config.h
	$(CC) main.c -o main.o -c $(CFLAGS) $(GTKFLAGS)

pulse: main.o events.o interface.o cpu.o system.o fail.o vendor.o badge.o
	$(CC) main.o events.o interface.o cpu.o system.o fail.o vendor.o badge.o -o pulse $(CFLAGS) $(GTKFLAGS)
