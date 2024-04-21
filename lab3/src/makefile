CC = gcc
CFLAGS = -std=c11 -pedantic -W -Wall -Wextra -Werror

all: parent child

parent: main.c
	$(CC) $(CFLAGS) main.c list.c -o parent

child: child.c
	$(CC) $(CFLAGS) child.c -o child

clean:
	rm -f parent child
