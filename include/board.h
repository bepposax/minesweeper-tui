/**
 * @file board.h
 * @author Ivano Izzo
 */
#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

extern int height, width, board_h, board_w, mines;

/**
 * @brief a cell of the game board
 * @param surrounding_mines the number of mines surrounding the cell
 * @param is_mine true if the cell contains a mine
 * @param is_discovered true if the cell has been discovered
 * @param is_flagged true if the cell has been flagged
 */
typedef struct cell_t
{
    int surrounding_mines;
    bool is_mine, is_discovered, is_flagged, is_marked;
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
void create_board(int diff);

/**
 * @brief resets the board to its initial state
 */
void reset_board();

/**
 * @brief frees the game board
 */
void free_board();

/**
 * @brief prints the game board
 */
void print_board();

/**
 * @brief informs the user if the window needs to be resized to print the board
 * @param board_h the board's height
 * @param board_w the board's width
 * @return true if the board is smaller than the window; false otherwise
 */
bool is_printable(int board_h, int board_w);

#endif