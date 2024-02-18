/**
 * @file main.c
 * @author Ivano Izzo
 */
#include "../include/difficulty.h"
#include "../include/board.h"
#include "../include/minesweeper.h"
#include <ncurses.h>

int main(void)
{
    int diff, choice;

    // ncurses
    initscr();
    raw();
    noecho();
    curs_set(0);
    nodelay(stdscr, FALSE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    keypad(stdscr, TRUE);
    use_default_colors();
    do
    {
        choice = 0;
        if ((diff = select_diff()))
        {
            create_board(diff);
            choice = game_loop();
            free_board();
        }
    } while (choice == 'n' || choice == 'N');
    endwin();

    return 0;
}
