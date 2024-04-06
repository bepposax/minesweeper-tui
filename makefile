CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lncursesw
TARGET = minesweeper
SRCS = src/*.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)