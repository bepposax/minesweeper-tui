/**
 * @file difficulty.h
 * @author Ivano Izzo
 */
#pragma once

#include <ncurses.h>

/**
 * @brief the difficulties selection
 * @return the chosen difficulty
 */
int select_diff();

/**
 * @brief prints the difficulties to choose from
 */
void print_diff_menu();