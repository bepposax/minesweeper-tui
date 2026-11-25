/**
 * @file history.c
 * @author Ivano Izzo
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <ctype.h>
#include "history.h"
#include <errno.h>
#include "string-builder.h"
#include "board.h"
#include "symbols.h"

static FILE *history;
static char filename[] = ".history.XXXXXX";
static int pages = 0;

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
 * @return an unique history file
 */
static void create_history()
{
    int fd;

    strcpy(filename, ".history.XXXXXX");
    if ((fd = mkstemp(filename)) == -1)
    {
        fprintf(stderr, "mkstemp failed: %s\n", strerror(errno));
        printerr("Failed to create history file\n", __LINE__);
    }
    if ((close(fd)) == -1)
        printerr("Failed to close history file descriptor\n", __LINE__);
}
    
/**
 * @brief opens the history file
 * @param mode the mode to open it in
 */
static void open_history(const char *mode)
{
    if (!history)
        create_history();
    if (!(history = fopen(filename, mode)))
        printerr("Failed to open history file\n", __LINE__);

}

void update_history()
{
    open_history("a");
    fprintf(history, "%s\n!!!\n", buffer);
    fclose(history);
    history = NULL;
    pages++;
}

void clear_history()
{
    open_history("w");
    fclose(history);
    history = NULL;

}

int show_history()
{
    int ch = 0, page = 0;
    char line[board.width * 8];
    char hist_buffer[sizeof(line) * board.height];
    long cursor[pages + 1];

    hist_buffer[0] = '\0';
    open_history("r");
    cursor[0] = 0;

    while (fgets(line, sizeof(line), history))
    {
        if (strcmp(line, "!!!\n") != 0)
            strcat(hist_buffer, line);
        else
        {
            cursor[++page] = ftell(history);
            print_history(hist_buffer);

            while ((ch = getch()))
            {
                if (ch >= 0 && ch <= 255)
                    ch = tolower(ch);
                if (ch == 'q' || ch == 'n' || ch == 'r')
                {
                    hist_buffer[0] = '\0';
                    fclose(history);
                    history = NULL;
                    remove_history();
                    return ch;
                }
                if (ch == 'h' || ch == KEY_RIGHT)
                {
                    if (page == pages)
                    {
                        mvprintw(board.height - 1, board.width / 2 + 2, "  ");
                        continue;
                    }
                    else
                        break;
                }
                else if (ch == KEY_LEFT)
                {
                    if (page >= 2)
                    {
                        page -= 2;
                        fseek(history, cursor[page], SEEK_SET);
                        break;
                    }
                    else
                    {
                        mvprintw(board.height - 1, board.width / 2 - 3, "  ");
                        continue;
                    }
                }
                else if (ch == KEY_RESIZE)
                    print_history(hist_buffer);
            }
            hist_buffer[0] = '\0';
        }
    }
    fclose(history);
    history = NULL;
    remove_history();

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
        attron(A_BOLD);
        mvprintw(board.height - 1, board.width / 2 - 3, "%s%s %s %s%s", LEFT_H, LINE_H, LINE_V, LINE_H, RIGHT_H);
        attroff(A_BOLD);
    }
}

int remove_history()
{
    int r;

    if ((r = remove(filename)) == -1)
        fprintf(stderr, "%s:%d: Couldn't remove the \"%s\" file\n", __FILE__, __LINE__ - 1, filename);
    pages = 0;
    return r;
}

static void printerr(char *msg, int line)
{
    fprintf(stderr, "%s:%d: %s", __FILE__, line, msg);
    exit(EXIT_FAILURE);
}