.POSIX:

CFLAGS   = -Wall -Wextra -g
GTKFLAGS = $$(pkg-config --cflags --libs gtk+-2.0)
CC       = gcc

TAGS     = etags

all: pulse

clean:
	rm -f *~ *.o TAGS pulse

tags: *.c *.h
	$(TAGS) *.[ch]

fail.o: fail.c

system.o: system.c

cpu.o: cpu.c cpu.h types.h constants.h

interface.o: interface.c interface.h types.h constants.h events.o
	$(CC) interface.c -o interface.o -c $(CFLAGS) $(GTKFLAGS)

events.o: events.c gpointer-structs.h cpu.h interface.h types.h macros.h
	$(CC) events.c -o events.o -c $(CFLAGS) $(GTKFLAGS)

pulse: main.c types.h interface.h cpu.h gpointer-structs.h config.h cpu.o interface.o fail.o system.o events.o
	$(CC) main.c cpu.o interface.o fail.o system.o events.o -o pulse $(CFLAGS) $(GTKFLAGS)
