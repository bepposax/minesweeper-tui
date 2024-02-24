/**
 * @file string_builder.h
 * @author Ivano Izzo
 */
#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

extern char *buffer;
extern long unsigned int offset;

/**
 * @brief appends formatted strings to the output buffer
 * @param fmt the formatted strings
 * @return the number of appended characters
 * @see vsprintf
 */
int strappend(const char *fmt, ...);

/**
 * @brief frees the allocated string
 */
void strfree();

#endif