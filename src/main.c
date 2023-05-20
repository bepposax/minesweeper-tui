/**
 * @file main.c
 *
 * @author Ivano Izzo
 */
#include "../include/minesweeper.h"

int main(void)
{
    initscr();
    raw();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    keypad(stdscr, TRUE);

    init(); // game starts

    endwin();
    return 0;
}
