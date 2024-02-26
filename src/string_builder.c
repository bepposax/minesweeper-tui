/**
 * @file string_builder.c
 * @author Ivano Izzo
 */
#include "../include/string_builder.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

char *buffer;
size_t offset, allocated;

/**
 * @brief prints memory allocation error to stderr
 * @param line the line where the error occourred
 */
static void printerr(int line);

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
            printerr(__LINE__ - 1);
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
    allocated = height * width * 10;
    if (!(buffer = (char *)malloc(allocated)))
        printerr(__LINE__ - 1);
}

static void printerr(int line)
{
    fprintf(stderr, "%s:%d: Error: Failed to allocate memory\n", __FILE__, line);
    exit(EXIT_FAILURE);
}