/**
 * @file minesweeper.h
 *
 * @author Ivano Izzo
 */
#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "ANSI-colors.h"

extern int height, width, mines;

/**
 * @brief represents a single cell of the board
 *
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

void init();
void place_mines();
void signal_mine(int, int);
void game_loop();
int play(int, int);
void flag(int, int);
void print_board();