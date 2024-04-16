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

static void *timer_run();

extern int board_height, board_width;
static pthread_t thread_id;
static WINDOW *w = NULL;
int min, sec;
bool timer_running;

void timer_init()
{
    w = newwin(1, 8, board_height - 1, board_width - 8);
    min = sec = 0;
    start_color();
    init_pair(COLOR_BLACK, COLOR_BLACK, -1);
    wattron(w, COLOR_PAIR(COLOR_BLACK));
}

void timer_start()
{
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
    if (board_height < LINES && board_width <= COLS)
    {
        wattron(w, A_BOLD);
        wprintw(w, TIMER);
        wattroff(w, A_BOLD);
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
    delwin(w);
    timer_init();
}