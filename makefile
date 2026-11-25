CC = gcc
CFLAGS = -I include/ -Wall -Wextra -std=c99 -O3

ifeq ($(OS),Windows_NT)
    EXE      = .exe
    LDFLAGS  = -lpdcurses_vt
	CFLAGS  += -I/ucrt64/include/pdcurses/ -IC:/msys64/ucrt64/include/pdcurses/ -DPDC_WIDE -DPDC_FORCE_UTF8 -DPDC_NCMOUSE
else
    EXE =
	LDFLAGS = -lncursesw
endif
ifeq (musl,$(findstring musl, $(shell $(CC) -dumpmachine)))
$(TARGET): LDFLAGS += -static
endif

TARGET = minesweeper$(EXE)
TEST = $(TARGET)-test$(EXE)
SRCS = $(wildcard src/*.c)
HDRS = $(wildcard include/*.h)

.PHONY: all clean run test

all: $(TARGET)


$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS)

$(TEST): $(SRCS) $(HDRS)
	$(CC) $(SRCS) -o $@ -I include/ -std=c99 $(LDFLAGS) -D TEST

clean:
	rm -f $(TARGET) $(TEST)

run: $(TARGET)
	./$(TARGET)

test: $(TEST)
	./$(TEST)
