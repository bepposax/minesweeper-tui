CC = gcc
CFLAGS = -Wall -Wextra 
TARGET = campo_minato
SRCS = main.c campo_minato.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS) campo_minato.h
	$(CC) $(CFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)