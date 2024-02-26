/**
 * @file string_builder.h
 * @author Ivano Izzo
 */
#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

extern char *buffer;
extern long unsigned int offset;
extern int height, width;

/**
 * @brief appends a formatted string to the buffer
 * @param fmt the formatted string
 * @return the number of appended characters
 * @see vsprintf
 */
int strappend(const char *fmt, ...);

/**
 * @brief frees the allocated string
 */
void strfree();

#endif