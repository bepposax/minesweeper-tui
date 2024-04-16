/**
 * @file timer.h
 * @author Ivano Izzo
 */
#ifndef TIMER_H
#define TIMER_H

extern bool timer_running;

void timer_init();
void timer_start();
void timer_stop();
void timer_reset();
void timer_win_reset();
void print_time();

#endif
