CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = draw
SRCS = $(TARGET).c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $@ $(CFLAGS)

run:	$(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)