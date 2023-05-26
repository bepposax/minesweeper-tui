/**
 * @file board.h
 * @author Ivano Izzo
 */
#pragma once

#include <stdbool.h>

extern int height, width, mines;

/**
 * @brief represents a single cell of the board
 * @param is_mine true if the cell contains a mine
 * @param surrounding_mines counts mines surrounding the cell
 * @param discovered true if the cell has been visited
 */
typedef struct cell
{
    bool is_mine;
    int surrounding_mines;
    bool discovered;
    bool is_flagged;
} cell;

/**
 * @brief represents the game board
 */
extern cell **board;

/**
 * @brief allocates the game board, based on the chosen difficulty
 * @param diff the chosen difficulty
 */
void create_board(int);

/**
 * @brief deallocates the game board once the game is over
 */
void free_board();