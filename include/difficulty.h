/**
 * @file difficulty.h
 * @author Ivano Izzo
 */
#ifndef DIFFICULTY_H
#define DIFFICULTY_H

/**
 * @brief asks the user to choose a difficulty
 * @return  1 if the user chose beginner;
 *          2 if the user chose intermediate;
 *          3 if the user chose expert;
 *          0 if the user quit the game
 */
int select_diff();

/**
 * @brief prints the difficulty menu
 */
void print_diff_menu();

#endif