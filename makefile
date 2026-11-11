CC = gcc
CFLAGS = -I include/ -Wall -Wextra -std=c99 -O3
LDFLAGS = -lncursesw
TARGET = minesweeper
TEST = $(TARGET)-test
SRCS = $(wildcard src/*.c)
HDRS = $(wildcard include/*.h)

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS)

$(TEST): $(SRCS) $(HDRS)
	$(CC) $(SRCS) -o $(TEST) -I include/ -std=c99 $(LDFLAGS) -D TEST

clean:
	rm -f $(TARGET) $(TEST)

run: $(TARGET)
	./$(TARGET)

test: $(TEST)
	./$(TEST)