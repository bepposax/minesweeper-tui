/**
 * @file timer.c
 * @author Ivano Izzo
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>
#include "timer.h"
#include "symbols.h"

/**
 * @brief creates the timer window
 */
static void timer_newwin();

/**
 * @brief runs the time
 */
static void *timer_run();

extern int board_h, board_w;
static pthread_t thread_id;
static WINDOW *w = NULL;
int min, sec;
bool timer_running;

static void timer_newwin()
{
    delwin(w);
    w = newwin(1, 8, board_h - 1, board_w - 8);
}

void timer_start()
{
    min = sec = 0;
    if (pthread_create(&thread_id, NULL, &timer_run, NULL))
    {
        printf("Failed to create thread.\n");
        exit(1);
    }
    timer_running = true;
}

static void *timer_run()
{
    int seconds = 1;

    while (1)
    {
        sleep(1);
        min = seconds / 60;
        sec = seconds % 60;
        print_time();
        seconds++;
    }
    return NULL;
}

void print_time()
{
    if (board_h < LINES && board_w <= COLS)
    {
        wattron(w, A_BOLD | COLOR_PAIR(COLOR_YELLOW));
        wprintw(w, TIMER);
        wattroff(w, A_BOLD | COLOR_PAIR(COLOR_YELLOW));
        wprintw(w, " %02d:%02d\b\b\b\b\b\b\b", min, sec);
        wrefresh(w);
    }
}

void timer_stop()
{
    if (thread_id && pthread_cancel(thread_id))
    {
        printf("Failed to cancel thread.\n");
        exit(1);
    }
    thread_id = 0;
    timer_running = false;
}

void timer_reset()
{
    min = 0;
    sec = 0;
    timer_newwin();
}
