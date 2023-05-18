CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lncurses
TARGET = minesweeper
SRCS = main.c minesweeper.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS) minesweeper.h
	$(CC) $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS)  

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)