/**
 * @file main.c
 * @author Ivano Izzo
 */
#include <ncurses.h>
#include <locale.h>
#include "difficulty.h"
#include "board.h"
#include "minesweeper.h"

static void ncurses_init();

int main(void)
{
    int diff, choice;

    setlocale(LC_ALL, "");
    ncurses_init();
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

static void ncurses_init()
{
    initscr();
    raw();
    noecho();
    curs_set(0);
    nodelay(stdscr, FALSE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    keypad(stdscr, TRUE);
    use_default_colors();
    start_color();
    for (int i = 0; i < 8; ++i)
        init_pair(i, i, -1);
}
