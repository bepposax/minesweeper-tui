/**
 * @file minesweeper.h
 *
 * @author Ivano Izzo
 */
#pragma once

#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "include/ANSI-colors.h"

#define HEIGHT 15
#define WIDTH 15
#define NMINES 40

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

extern cell **board;

void place_mines(cell **);
void signal_mine(cell **, int, int);
void game_loop(cell **);
int play(int, int, cell **);
void print_board(cell **);
#endif