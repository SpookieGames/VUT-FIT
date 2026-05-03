CC      = gcc
CFLAGS  = -std=gnu99 -Wall -Wextra -Werror -pedantic
LDFLAGS = -pthread -lrt

.PHONY: all clean

all: proj2

proj2: proj2.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f proj2 proj2.out
