/**
 * @file timer.h
 * @author Ivano Izzo
 */
#ifndef TIMER_H
#define TIMER_H

extern bool timer_running;

/**
 * @brief initializes the timer at 0:00
 */
void timer_init();

void timer_start();

void timer_stop();

/**
 * @brief resets the timer to 0:00
 */
void timer_reset();

/**
 * @brief resets the timer but keeps its state
 */
void timer_win_reset();

void print_time();

#endif
