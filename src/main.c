/**
 * @file main.c
 * @author Ivano Izzo
 */
#include <locale.h>
#include <ncurses.h>
#include "difficulty.h"
#include "board.h"
#include "minesweeper.h"

int main(void)
{
    int diff, choice;

    setlocale(LC_ALL, "");
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
    } while (choice == 'n');
    endwin();
    curs_set(2);

    return 0;
}
