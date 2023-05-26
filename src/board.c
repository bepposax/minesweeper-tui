/**
 * @file board.c
 * @author Ivano Izzo
 */
#include "../include/board.h"
#include <stdlib.h>

int height, width, mines;
cell **board;

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
    }

    board = (cell **)calloc(height, sizeof(cell));
    if (!board)
        exit(EXIT_FAILURE);
    for (int i = 0; i < height; i++)
    {
        board[i] = (cell *)calloc(width, sizeof(cell));
        if (!board[i])
            exit(EXIT_FAILURE);
    }
}

void free_board()
{
    for (int i = 0; i < height; i++)
        free(board[i]);
    free(board);
}