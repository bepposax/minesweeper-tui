#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>

static int y, x;
static pthread_t thread_id;
WINDOW *w;

static void *print_timer()
{
    w = newwin(1, 7, y, x);
    start_color();
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, -1);

    int seconds = 0;
    while (1)
    {
        attron(COLOR_PAIR(COLOR_MAGENTA));
        wprintw(w, "%02d:%02d\b\b\b\b\b", seconds / 60, seconds % 60);
        refresh();
        wrefresh(w);
        attroff(COLOR_PAIR(COLOR_MAGENTA));
        seconds++;
        sleep(1);
    }
    return NULL;
}

void timer_start(int row, int col)
{
    y = row, x = col;

    if (pthread_create(&thread_id, NULL, &print_timer, NULL))
    {
        printf("Failed to create thread.\n");
        exit(1);
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
}

void timer_kill()
{
    timer_stop();
    delwin(w);
}