/**
 * @file main.c
 * @author Ivano Izzo
 */
#include <ncurses.h>
#include <locale.h>
#include <signal.h>
#include <unistd.h>
#include "difficulty.h"
#include "board.h"
#include "minesweeper.h"
#include "history.h"

static void ncurses_init();

static void sighup_handler(int signo)
{
    remove_history();
    _exit(signo);
}

int main(void)
{
    int diff, choice;

    setlocale(LC_ALL, "");
    ncurses_init();
    signal(SIGHUP, sighup_handler);
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
    mouseinterval(1);
    keypad(stdscr, TRUE);
    use_default_colors();
    start_color();
    for (int i = 0; i < 8; ++i)
        init_pair(i, i, -1);
}
