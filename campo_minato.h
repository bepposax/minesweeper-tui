/**
 * @file campo_minato.h
 *
 * @author Ivano Izzo
 */
#ifndef CAMPO_MINATO_H
#define CAMPO_MINATO_H

#include <stdlib.h>
#include <stdbool.h>
#include "lib/ANSI-colors.h"

#define HEIGHT 15
#define WIDTH 15
#define NMINES 35

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

extern cell **board;

void game_loop(cell **);
void place_mines(cell **);
void signal_mine(cell **, int, int);
void print_board(cell **);
int play(int, int, cell **);
int discover(int, int, cell **);
bool discoverable(int, int, cell **);
void print_results();

#endif