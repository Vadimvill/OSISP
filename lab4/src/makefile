CC=gcc
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -pedantic -Werror -ggdb

all: prog

prog: main.c func.c stack.c ring.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf prog
