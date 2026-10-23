/**
 * @file history.c
 * @author Ivano Izzo
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include "history.h"
#include "string-builder.h"
#include "board.h"
#include "symbols.h"

static FILE *history;
static char filename[] = ".history.XXXXXX";

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
static FILE* create_history()
{
    FILE* f;
    int fd;
    
    if (!(f = fdopen((fd = mkstemp(filename)), "rx"))) {
        close(fd);
        printerr("Failed to create history file\n", __LINE__ - 1);
    }
    return f;
}

/**
 * @brief opens the history file
 * @param mode the mode to open it in
 */
static void open_history(char *mode)
{
    if (!history)
    history = create_history();
    history = fopen(filename, mode);
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

int show_history(int moves) 
{
    moves += 2;
    int ch = 0, move = 0;
    char line[board.width * 8];
    char hist_buffer[sizeof(line) * board.height];
    long cursor[moves];
    
    hist_buffer[0] = '\0';
    open_history("r");
    memset(cursor, -1, sizeof(cursor));
    cursor[0] = 0;

    while (fgets(line, sizeof(line), history)) 
    {
        if (strcmp(line, "!!!\n") != 0)
            strcat(hist_buffer, line);
        else 
        {
            cursor[++move] = ftell(history);
            print_history(hist_buffer);

            while ((ch = getch())) 
            {
                if (ch >= 0 && ch <= 255)
                    ch = tolower(ch);
                if (ch == 'q' || ch == 'n' || ch == 'r')
                {
                    hist_buffer[0] = '\0';
                    return ch;
                }
                if (ch == 'h' || ch == KEY_RIGHT)
                {
                    if (move == moves - 1)
                    {
                        move--;
                        fseek(history, cursor[move], SEEK_SET);
                    }
                    break;
                }
                else if (ch == KEY_LEFT)
                {
                    move = move >= 2 ? (move - 2) : 0;
                    fseek(history, cursor[move], SEEK_SET);
                    break;
                }
                else if (ch == KEY_RESIZE)
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
        attron(A_BOLD);
        mvprintw(board.height - 1, board.width / 2 - 2, "%s %s %s", LEFT_H, LINE_V, RIGHT_H);
        attroff(A_BOLD);
        move(0, 0);
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