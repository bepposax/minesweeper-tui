/**
 * @file board.h
 * @author Ivano Izzo
 */
#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

extern int height, width, board_height, board_width, mines;

/**
 * @brief a cell of the game board
 * @param surrounding_mines the number of mines surrounding the cell
 * @param is_mine true if the cell contains a mine
 * @param is_discovered true if the cell has been discovered
 * @param is_flagged true if the cell has been flagged
 */
typedef struct cell
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
 * @param height the board's height
 * @param width the board's width
 * @param maxy the window's height
 * @param maxx the window's width
 * @return 1 if the board is bigger than the window; 0 otherwise
 */
bool is_printable(int height, int width, int maxy, int maxx);

#endif