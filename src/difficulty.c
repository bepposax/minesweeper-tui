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
    return -1;
}

void print_diff_menu()
{
    char *menu[4] = {
        "D I F F I C U L T Y",
        "B E G I N N E R",
        "I N T E R M E D I A T E",
        "E X P E R T"};

    clear();
    refresh();

    // border top
    printf("\n\r╭");
    for (int i = 0; i <= 16 * 2; i++)
        printf("─");
    printf("╮\n\r");

    // board background
    for (int i = 0; i < 16; i++)
    {
        printf("│ ");
        for (int j = 0; j < 16; j++)
            printf("■ ");
        printf("│\n\r");
    }

    // border bottom
    printf("╰");
    for (int i = 0; i <= 16 * 2; i++)
        printf("─");
    printf("╯\n\r");

    // menu
    use_default_colors();
    attron(A_BOLD);
    mvprintw(5, 8, "%s", menu[0]);
    start_color();
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_RED, -1);
    attron(COLOR_PAIR(1));
    mvprintw(8, 10, "%s", menu[1]);
    attron(COLOR_PAIR(2));
    mvprintw(11, 6, "%s", menu[2]);
    attron(COLOR_PAIR(3));
    mvprintw(14, 12, "%s", menu[3]);
    attroff(COLOR_PAIR(3));
    attroff(A_BOLD);
    refresh();
    use_default_colors();
}
