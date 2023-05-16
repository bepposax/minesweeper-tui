CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lncurses
TARGET = campo_minato
SRCS = main.c campo_minato.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS) campo_minato.h
	$(CC) $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS)  

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)