/**
 * @file main.c
 * @author Ivano Izzo
 */
#include <stdlib.h>
#include "../include/difficulty.h"
#include "../include/minesweeper.h"

int height, width, mines;

int main(void)
{
    // ncurses
    initscr();
    raw();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    keypad(stdscr, TRUE);

    // difficulty selection
    switch (select_diff())
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
    default:
        endwin();
        return 0;
    }

    // board allocation
    board = (cell **)calloc(height, sizeof(cell));
    if (!board)
        exit(EXIT_FAILURE);
    for (int i = 0; i < height; i++)
    {
        board[i] = (cell *)calloc(width, sizeof(cell));
        if (!board[i])
            exit(EXIT_FAILURE);
    }

    int ch = game_loop();

    // board deallocation
    for (int i = 0; i < height; i++)
        free(board[i]);
    free(board);

    endwin();

    if (ch == 'n')
        main();

    return 0;
}
