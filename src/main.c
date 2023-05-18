/**
 * @file main.c
 *
 * @author Ivano Izzo
 */
#include "../include/minesweeper.h"

int main(void)
{
    cell **board = (cell **)malloc(HEIGHT * sizeof(cell));
    if (!board)
        exit(EXIT_FAILURE);
    for (int i = 0; i < HEIGHT; i++)
    {
        board[i] = (cell *)malloc(WIDTH * sizeof(cell));
        if (!board[i])
            exit(EXIT_FAILURE);
    }

    initscr();
    raw();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    keypad(stdscr, TRUE);

    game_loop(board);

    for (int i = 0; i < HEIGHT; i++)
        free(board[i]);
    free(board);

    endwin();
    return 0;
}
