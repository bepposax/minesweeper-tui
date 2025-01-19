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
static char *hist_file = ".history";

/**
 * @brief prints memory allocation error to stderr
 * @param msg the message to print
 * @param line the line where the error occourred
 */
static void printerr(char *msg, int line);

/**
 * @brief prints the last game's history
 * @param buffer the string to print
 */
static void print_history(char *buffer);

void add_to_history()
{
    if (!(history = fopen(hist_file, "a")))
        printerr("Failed to open history file\n", __LINE__ - 1);
    fprintf(history, "%s\n!!!\n", buffer);
    fclose(history);
}

void clear_history()
{
    if (!(history = fopen(hist_file, "w")))
        printerr("Failed to open history file\n", __LINE__ - 1);
    fclose(history);
}

int show_history()
{
    int ch;
    char line[board_w * 8];
    char hist_buffer[sizeof(line) * board_h];

    hist_buffer[0] = '\0';
    if (!(history = fopen(hist_file, "r")))
        printerr("Failed to open history file\n", __LINE__ - 1);
    while (fgets(line, sizeof(line), history))
    {
        if (strcmp(line, "!!!\n") != 0)
            strcat(hist_buffer, line);
        else
        {
            print_history(hist_buffer);
            while ((ch = tolower(getch())) != 'h')
            {
                if (ch == 'q' || ch == 'n' || ch == 'r')
                {
                    hist_buffer[0] = '\0';
                    return ch;
                }
                if (ch == KEY_RESIZE)
                    print_history(hist_buffer);
            }
            hist_buffer[0] = '\0';
        }
    }
    fclose(history);

    return ch;
}

static void print_history(char *buffer)
{
    if (is_printable(board_h, board_w))
    {
        clear();
        refresh();
        printf("%s", buffer);
        print_results();
    }
}

int remove_history()
{
    int r;

    if ((r = remove(hist_file)) == -1)
        fprintf(stderr, "%s:%d: Couldn't delete the \".history\" file\n", __FILE__, __LINE__ - 1);

    return r;
}

static void printerr(char *msg, int line)
{
    fprintf(stderr, "%s:%d: %s", __FILE__, line, msg);
    exit(EXIT_FAILURE);
}