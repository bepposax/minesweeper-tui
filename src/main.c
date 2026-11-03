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
static void cleanup(int signo);

int main(void)
{
    int diff, choice;

    for (int i = 1; i <= 3; ++i)
        signal(i, cleanup); // SIGHUP; SIGNINT; SIGQUIT
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
    cleanup(0);

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

static void cleanup(int signo)
{
    if (signo) remove_history();
    endwin();
    printf("\033[0 q"); // reset cursor default style
    fflush(stdout);
    if (signo) _exit(signo);
}