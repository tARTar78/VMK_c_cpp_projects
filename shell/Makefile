CC = gcc
CFLAGS = -g -Wall -Wextra -Wpedantic -std=c11
SOURCES = list.c tree_art.c exec_art.c t5_t3.c
TARGET = shell_art

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: clean

