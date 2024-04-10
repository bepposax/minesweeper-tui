/**
 * @file minesweeper.h
 * @author Ivano Izzo
 */
#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "board.h"

extern int goal, moves, uncovered_cells;
extern bool game_over;

/**
 * @brief the game loop - it ends when the game is over or the user quits
 * @return 'q' to quit the game or 'n' to start a new game
 */
int game_loop();

#endif