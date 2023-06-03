/**
 * @file difficulty.c
 * @author Ivano Izzo
 */
#include "../include/difficulty.h"
#include <stdio.h>
#include <ncurses.h>

int select_diff()
{
    int ch;

    print_diff_menu();
    while ((ch = getch()))
    {
        if (ch == KEY_MOUSE)
        {
            MEVENT event;
            if (getmouse(&event) == OK && (event.bstate & BUTTON1_CLICKED))
                switch (event.y)
                {
                case 8:
                    return 1;
                case 11:
                    return 2;
                case 14:
                    return 3;
                }
        }
        else if (ch == 'q' || ch == 'Q')
            return 0;
        else if (ch == KEY_RESIZE)
            print_diff_menu();
    }
    return 0;
}

void print_diff_menu()
{
    int width = 16;

    clear();
    start_color();
    init_pair(COLOR_GREEN, COLOR_GREEN, -1);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, -1);
    init_pair(COLOR_RED, COLOR_RED, -1);
    init_pair(COLOR_CYAN, COLOR_CYAN, -1);

    // stats
    attron(COLOR_PAIR(COLOR_GREEN));
    mvprintw(0, 1, "goal");
    attron(COLOR_PAIR(COLOR_RED));
    mvprintw(0, width * 2 - 3, "mines");
    attron(COLOR_PAIR(COLOR_CYAN));
    mvprintw(width + 3, 1, "moves");
    attroff(COLOR_PAIR(COLOR_CYAN));
    move(1, 0);
    refresh();

    // border top
    printf("╭");
    for (int i = 0; i <= width * 2; i++)
        printf("─");
    printf("╮\n\r");
    // board
    for (int i = 0; i < width; i++)
    {
        printf("│ ");
        for (int j = 0; j < width; j++)
            printf("■ ");
        printf("│\n\r");
    }
    // border bottom
    printf("╰");
    for (int i = 0; i <= width * 2; i++)
        printf("─");
    printf("╯\n");

    // difficulties
    attron(A_BOLD);
    mvprintw(5, 8, "D I F F I C U L T Y");
    attron(COLOR_PAIR(COLOR_GREEN));
    mvprintw(8, 10, "B E G I N N E R");
    attron(COLOR_PAIR(COLOR_YELLOW));
    mvprintw(11, 6, "I N T E R M E D I A T E");
    attron(COLOR_PAIR(COLOR_RED));
    mvprintw(14, 12, "E X P E R T");
    attroff(COLOR_PAIR(COLOR_RED));
    attroff(A_BOLD);

    // option keys
    mvprintw(width, width * 2 + 4, "\t ew game");
    mvprintw(width, width * 2 + 4, "\t");
    attron(A_UNDERLINE);
    printw("n");
    attroff(A_UNDERLINE);
    mvprintw(width + 1, width * 2 + 4, "\t uit");
    mvprintw(width + 1, width * 2 + 4, "\t");
    attron(A_UNDERLINE);
    printw("q");
    attroff(A_UNDERLINE);

    refresh();
}
