/**
 * @file history.c
 * @author Ivano Izzo
 */
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include "history.h"
#include "string-builder.h"
#include "board.h"

static FILE *history;

/**
 * @brief prints memory allocation error to stderr
 * @param msg the message to print
 * @param line the line where the error occourred
 */
static void printerr(char *msg, int line);

void add_to_history()
{
    if (!(history = fopen("history", "a")))
        printerr("Failed to open history file\n", __LINE__ - 1);
    fprintf(history, "%s\n!!!\n", buffer);
    fclose(history);
}

void clear_history()
{
    if (!(history = fopen("history", "w")))
        printerr("Failed to open history file\n", __LINE__ - 1);
    fclose(history);
}

int show_history()
{
    int ch;
    char line[board_w * 8];
    char hist_buffer[sizeof(line) * board_h];

    hist_buffer[0] = '\0';
    if (!(history = fopen("history", "r")))
        printerr("Failed to open history file\n", __LINE__ - 1);
    while (fgets(line, sizeof(line), history))
    {
        if (strcmp(line, "!!!\n") != 0)
            strcat(hist_buffer, line);
        else
        {
            if (is_printable(board_h, board_w))
            {
                clear();
                refresh();
                printf("%s", hist_buffer);
                while ((ch = tolower(getch())) != 'h')
                {
                    if (ch == 'q' || ch == 'n' || ch == 'r')
                    {
                        hist_buffer[0] = '\0';
                        return ch;
                    }
                    if (ch == KEY_RESIZE)
                        if (is_printable(board_h, board_w))
                        {
                            clear();
                            refresh();
                            printf("%s", hist_buffer);
                        }
                }
            }
            hist_buffer[0] = '\0';
        }
    }
    fclose(history);

    return 'h';
}

static void printerr(char *msg, int line)
{
    fprintf(stderr, "%s:%d: %s", __FILE__, line, msg);
    exit(EXIT_FAILURE);
}