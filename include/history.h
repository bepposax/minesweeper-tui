/**
 * @file history.h
 * @author Ivano Izzo
 */
#ifndef HISTORY_H
#define HISTORY_H

/**
 * @brief appends the string to a file
 */
void update_history();

/**
 * @brief clears the saved history
 */
void clear_history();

/**
 * @brief prints the game history move by move, every time 'h' is pressed
 * @return the user's input: 'q' to quit, 'r' to restart, 'n' for a new game
 */
int show_history();

/**
 * @brief removes the history file before exiting the program
 * @return 0 if successfull, -1 otherwise
 * @see remove()
 */
int remove_history();

#endif