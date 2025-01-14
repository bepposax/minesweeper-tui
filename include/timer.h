/**
 * @file timer.h
 * @author Ivano Izzo
 */
#ifndef TIMER_H
#define TIMER_H

extern bool timer_running;

/**
 * @brief creates the timer window
 */
void timer_newwin();

/**
 * @brief starts the timer thread
 */
void timer_start();

/**
 * @brief cancels the timer thread
 */
void timer_stop();

/**
 * @brief resets the time to 0:00
 */
void timer_reset();

void print_time();

#endif
