CC=gcc
CFLAG= -std=c99 -Wall
PFLAG= -pg
MEMFLAG= -m32
DEBUGFLAG= -g

all: orderbook orderbook_profile
profile: orderbook_profile
debug: orderbook_debug

orderbook: orderbook.c orderbook.h
	$(CC) $(CFLAG) orderbook.h orderbook.c -o orderbook

orderbook_profile: orderbook.c orderbook.h
	$(CC) $(CFLAG) $(PFLAG) orderbook.h orderbook.c -o orderbook_gprof
	$(CC) $(CFLAG) $(MEMFLAG) orderbook.h orderbook.c -o orderbook_mem

orderbook_debug: orderbook.c orderbook.h
	$(CC) $(CFLAG) $(DEBUGFLAG) orderbook.h orderbook.c -o orderbook_debug

clean:
	rm -f orderbook orderbook_gprof orderbook_mem orderbook_debug