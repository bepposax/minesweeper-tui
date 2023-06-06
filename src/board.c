/**
 * @file board.c
 * @author Ivano Izzo
 */
#include "../include/board.h"
#include <stdlib.h>
#include <stdio.h>

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
    if (!(board = (cell **)calloc(height, sizeof(cell *))))
    {
        fprintf(stderr, "%s:%d: Error: Failed to allocate memory\n", __FILE__, __LINE__ - 2);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < height; i++)
        if (!(board[i] = (cell *)calloc(width, sizeof(cell))))
        {
            fprintf(stderr, "%s:%d: Error: Failed to allocate memory\n", __FILE__, __LINE__ - 2);
            exit(EXIT_FAILURE);
        }
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