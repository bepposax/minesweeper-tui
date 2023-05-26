/**
 * @file minesweeper.h
 * @author Ivano Izzo
 */
#pragma once
#include "../include/board.h"

extern int goal, moves, uncovered_cells;
extern bool game_over;

/**
 * @brief places the mines randomly on the board
 */
void place_mines();

/**
 * @brief updates the surrounding_mines attribute of each cell touching this mine
 * @param row the mine's row
 * @param col the mine's column
 */
void signal_mine(int, int);

/**
 * @brief the game loop - it ends when the game is over or the user quits
 * @return 'q' to quit the game or 'n' to start a new game
 */
int game_loop();

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
