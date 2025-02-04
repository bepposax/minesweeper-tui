/**
 * @file string-builder.c
 * @author Ivano Izzo
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "string-builder.h"
#include "board.h"

char *buffer;
long unsigned int offset;
static size_t allocated;
extern int board_size;

/**
 * @brief prints memory allocation error to stderr
 * @param msg the message to print
 * @param line the line where the error occourred
 */
static void printerr(char *msg, int line);

/**
 * @brief allocates the string
 */
static void strinit();

int strappend(const char *fmt, ...)
{
    int res;

    if (!buffer)
        strinit();
    va_list args;
    va_start(args, fmt);
    if (offset + strlen(fmt) > allocated)
        if (!(buffer = (char *)realloc(buffer, allocated *= 2)))
            printerr("Failed to allocate memory\n", __LINE__ - 1);
    res = vsprintf(buffer + offset, fmt, args);
    va_end(args);
    offset += res;

    return res;
}

void strfree()
{
    if (buffer)
    {
        free(buffer);
        buffer = NULL;
    }
}

static void strinit()
{
    int multiplier = 20;

    allocated = ((board.rows && board.cols) ? board.rows * board.cols : board_size * board_size) * multiplier;
    if (!(buffer = (char *)malloc(allocated)))
        printerr("Failed to allocate memory\n", __LINE__ - 1);
}

static void printerr(char *msg, int line)
{
    fprintf(stderr, "%s:%d: %s", __FILE__, line, msg);
    exit(EXIT_FAILURE);
}
