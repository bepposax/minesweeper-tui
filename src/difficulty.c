/**
 * @file difficulty.c
 * @author Ivano Izzo
 */
#include "../include/difficulty.h"
#include <stdio.h>
#include <ncurses.h>

/**
 * @brief mvprintw with color
 * @param color the color to use
 * @param row the row to print
 * @param col the column to print
 * @param str the string to print
 * @return the return value of mvprintw
 * @see mvprintw
 */
int cmvprintw(int color, int row, int col, const char *str);

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

    // corner stats
    cmvprintw(COLOR_PAIR(COLOR_GREEN), 0, 1, "goal");
    cmvprintw(COLOR_PAIR(COLOR_RED), 0, width * 2 - 3, "mines");
    cmvprintw(COLOR_PAIR(COLOR_CYAN), width + 3, 1, "moves");
    move(1, 0);
    refresh();

    // board
    printf("╭");
    for (int i = 0; i <= width * 2; i++)
        printf("─");
    printf("╮\n\r");
    for (int i = 0; i < width; i++)
    {
        printf("│ ");
        for (int j = 0; j < width; j++)
            printf("■ ");
        printf("│\n\r");
    }
    printf("╰");
    for (int i = 0; i <= width * 2; i++)
        printf("─");
    printf("╯\n");

    // difficulties
    attron(A_BOLD);
    mvprintw(5, 8, "D I F F I C U L T Y");
    cmvprintw(COLOR_PAIR(COLOR_GREEN), 8, 10, "B E G I N N E R");
    cmvprintw(COLOR_PAIR(COLOR_YELLOW), 11, 6, "I N T E R M E D I A T E");
    cmvprintw(COLOR_PAIR(COLOR_RED), 14, 12, "E X P E R T");
    attroff(A_BOLD);

    // side options
    attron(A_UNDERLINE);
    mvprintw(width, width * 2 + 6, "n");
    mvprintw(width + 1, width * 2 + 6, "q");
    attroff(A_UNDERLINE);
    mvprintw(width, width * 2 + 7, "ew game");
    mvprintw(width + 1, width * 2 + 7, "uit");

    refresh();
}

int cmvprintw(int color, int row, int col, const char *str)
{
    attron(color);
    int r = mvprintw(row, col, "%s", str);
    attroff(color);
    return r;
}