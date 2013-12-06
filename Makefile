CC ?= gcc

CFLAGS ?= -Wall -pedantic -s -O2

PROGRAMS = $(patsubst %.c,%,$(wildcard *.c))

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean
