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
static char *filename = ".history";

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

/**
 * @brief opens the history file
 * @param mode the mode to open it in
 */
static inline void open_history(char *mode)
{
    if (!(history = fopen(filename, mode)))
        printerr("Failed to open history file\n", __LINE__ - 1);
}

void update_history()
{
    open_history("a");
    fprintf(history, "%s\n!!!\n", buffer);
    fclose(history);
}

void clear_history()
{
    open_history("w");
    fclose(history);
}

int show_history()
{
    int ch;
    char line[board.width * 8];
    char hist_buffer[sizeof(line) * board.height];

    hist_buffer[0] = '\0';
    open_history("r");
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
    if (is_printable(board.height, board.width))
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

    if ((r = remove(filename)) == -1)
        fprintf(stderr, "%s:%d: Couldn't remove the \"%s\" file\n", __FILE__, __LINE__ - 1, filename);

    return r;
}

static void printerr(char *msg, int line)
{
    fprintf(stderr, "%s:%d: %s", __FILE__, line, msg);
    exit(EXIT_FAILURE);
}