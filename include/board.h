/**
 * @file board.h
 * @author Ivano Izzo
 */
#pragma once

#include <stdbool.h>

extern int height, width, mines;

/**
 * @brief a cell of the game board
 * @param is_mine true if the cell contains a mine
 * @param surrounding_mines the number of mines surrounding the cell
 * @param discovered true if the cell has been discovered
 * @param is_flagged true if the cell has been flagged
 */
typedef struct cell
{
    bool is_mine;
    int surrounding_mines;
    bool discovered;
    bool is_flagged;
} cell;

/**
 * @brief the game board
 * @see cell
 */
extern cell **board;

/**
 * @brief creates the game board
 * @param diff the difficulty chosen by the user
 */
void create_board(int);

/**
 * @brief frees the game board
 */
void free_board();