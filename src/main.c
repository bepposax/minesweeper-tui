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
    // ncurses
    initscr();
    raw();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    keypad(stdscr, TRUE);
    use_default_colors();

    int diff = select_diff();

    if (diff)
    {
        create_board(diff);
        int ch = game_loop();
        free_board();

        if (ch == 'n')
        {
            endwin();
            main();
        }
    }

    endwin();
    return 0;
}
