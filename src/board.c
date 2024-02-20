/**
 * @file board.c
 * @author Ivano Izzo
 */
#include "../include/board.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

int height, width, mines;
cell **board;

/**
 * @brief prints memory allocation error to stderr
 * @param line the line where the error occourred
 */
static void printerr(int line);

/**
 * @brief asks the user for custom height, width and number of mines
 */
static void customize();

void create_board(int diff)
{
    switch (diff)
    {
    case 1:
        height = 9;
        width = 9;
        mines = 10;
        break;
    case 2:
        height = 16;
        width = 16;
        mines = 40;
        break;
    case 3:
        height = 16;
        width = 30;
        mines = 99;
        break;
    case 4:
        customize();
        break;
    }
    if (!(board = (cell **)calloc(height, sizeof(cell *))))
        printerr(__LINE__ - 1);
    for (int i = 0; i < height; i++)
        if (!(board[i] = (cell *)calloc(width, sizeof(cell))))
            printerr(__LINE__ - 1);
}

static void customize()
{
    char input[2];

    echo();
    curs_set(2);
    attron(A_BOLD);
    attron(COLOR_PAIR(COLOR_CYAN));
    do
    {
        mvprintw(15, 12, " Height    ");
        mvgetnstr(15, 20, input, 2);
        height = atoi(input);
    } while (height < 1 || height > 99);
    do
    {
        mvprintw(15, 12, " Width     ");
        mvgetnstr(15, 20, input, 2);
        width = atoi(input);
    } while (width < 1 || width > 99);
    do
    {
        mvprintw(15, 12, " Mines     ");
        mvgetnstr(15, 20, input, 2);
        mines = atoi(input);
    } while (mines < 1 || mines > height * width);
    attroff(COLOR_PAIR(COLOR_CYAN));
    attroff(A_BOLD);
    curs_set(0);
    noecho();
}

static void printerr(int line)
{
    fprintf(stderr, "%s:%d: Error: Failed to allocate memory\n", __FILE__, line);
    exit(EXIT_FAILURE);
}

void free_board()
{
    for (int i = 0; i < height; i++)
    {
        free(board[i]);
        board[i] = NULL;
    }
    free(board);
    board = NULL;
}
