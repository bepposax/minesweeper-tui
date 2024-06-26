CC = gcc
CFLAGS = -I include/ -Wall -Wextra -std=c99 -O3
LDFLAGS = -lncursesw
TARGET = minesweeper
SRCS = src/*.c

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

test:	$(SRCS)
	$(CC) $(SRCS) -o $(TARGET) -I include/ $(LDFLAGS) -D TEST