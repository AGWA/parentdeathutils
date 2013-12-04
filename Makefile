CC=gcc

CFLAGS=-Wall -pedantic -s -O2

LDFLAGS=

PROGRAMS = $(patsubst %.c,%,$(wildcard *.c))

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -lX11 -o $@ $<

.PHONY: all clean
