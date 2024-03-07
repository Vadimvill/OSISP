CC = gcc
CFLAGS = -std=c11 -pedantic -W -Wall -Wextra -Werror

TARGET = dirwalk

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) $< -o $@

debug: $(TARGET))
	gdb ./$(TARGET)

clean:
	rm -f $(TARGET)

