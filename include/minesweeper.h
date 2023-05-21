/**
 * @file minesweeper.h
 * @author Ivano Izzo
 */
#pragma once

#include <ncurses.h>

extern int height, width, mines, goal, moves, uncovered_cells;
extern bool game_over;

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
 * @brief places the mines randomly on the board
 */
void place_mines();

/**
 * @brief updates the number of surrounding mines of cells touching this mine
 * @param row the mine's row
 * @param col the mine's column
 */
void signal_mine(int, int);

/**
 * @brief the game loop - it ends when the game is over or the user quits
 */
void game_loop();

/**
 * @brief discover a cell and checks if more can be discovered
 * @param row the row of the starting cell
 * @param col the column of the starting cell
 * @return 1 if the game is over;
 *         0 if the game is not over
 */
int play(int, int);

/**
 * @brief places or removes a flag on an undiscovered cell
 * @param row the row of the starting cell
 * @param col the column of the starting cell
 */
void flag(int, int);

/**
 * @brief discovers all the possible cells from a starting cell
 * @param row the row of the starting cell
 * @param col the column of the starting cell
 * @return 1 if a mine is reached; 0 otherwise
 */
int discover(int, int);

/**
 * @brief prints the current state of the board
 */
void print_board();
