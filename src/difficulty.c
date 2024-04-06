/**
 * @file difficulty.c
 * @author Ivano Izzo
 */
#include "../include/difficulty.h"
#include "../include/string_builder.h"
#include "../include/symbols.h"
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

int side = 16;

/**
 * @brief mvprintw with color
 * @param color the color to use
 * @param row the row to print
 * @param col the column to print
 * @param str the string to print
 * @return the return value of mvprintw
 * @see mvprintw
 */
static int cmvprintw(int color, int row, int col, const char *str);

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
                case 6:
                    if (event.x >= 9 && event.x <= 25)
                        return 1;
                    break;
                case 9:
                    if (event.x >= 5 && event.x <= 29)
                        return 2;
                    break;
                case 12:
                    if (event.x >= 11 && event.x <= 23)
                        return 3;
                    break;
                case 15:
                    if (event.x >= 11 && event.x <= 23)
                        return 4;
                    break;
                }
        }
        else if (ch == 'q' || ch == 'Q')
            break;
        else if (ch == KEY_RESIZE)
            print_diff_menu();
    }
    return 0;
}

void print_diff_menu()
{
    clear();

    if (side * 2 + 3 >= getmaxx(stdscr) || side + 4 >= getmaxy(stdscr))
    {
        char *msg = "Resize window";
        mvprintw(getmaxy(stdscr) / 2, getmaxx(stdscr) / 2 - strlen(msg) / 2, "%s", msg);
        return;
    }
    start_color();
    init_pair(COLOR_GREEN, COLOR_GREEN, -1);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, -1);
    init_pair(COLOR_RED, COLOR_RED, -1);
    init_pair(COLOR_CYAN, COLOR_CYAN, -1);

    // corner stats
    cmvprintw(COLOR_GREEN, 0, 1, "goal");
    cmvprintw(COLOR_RED, 0, side * 2 - 3, "mines");
    cmvprintw(COLOR_CYAN, side + 3, 1, "moves");
    move(1, 0);
    refresh();

    // board
    strappend(ARC_0);
    for (int i = 0; i <= side * 2; i++)
        strappend(LINE_H);
    strappend(ARC_1 "\n\r");
    for (int i = 0; i < side; i++)
    {
        strappend(LINE_V " ");
        for (int j = 0; j < side; j++)
            strappend(CELL " ");
        strappend(LINE_V "\n\r");
    }
    strappend(ARC_2);
    for (int i = 0; i <= side * 2; i++)
        strappend(LINE_H);
    strappend(ARC_3 "\n\r");
    printf("%s", buffer);
    offset = 0;

    // difficulties
    attron(A_BOLD);
    mvprintw(3, 8, "D I F F I C U L T Y");
    cmvprintw(COLOR_GREEN, 6, 10, "B E G I N N E R");
    cmvprintw(COLOR_YELLOW, 9, 6, "I N T E R M E D I A T E");
    cmvprintw(COLOR_RED, 12, 12, "E X P E R T");
    cmvprintw(COLOR_CYAN, 15, 12, "C U S T O M");
    attroff(A_BOLD);

    // side options
    if (side * 2 + 13 < getmaxx(stdscr))
    {
        attron(A_UNDERLINE);
        mvprintw(side - 1, side * 2 + 6, "n");
        mvprintw(side + 0, side * 2 + 6, "r");
        mvprintw(side + 1, side * 2 + 6, "q");
        attroff(A_UNDERLINE);
        mvprintw(side - 1, side * 2 + 7, "ew game");
        mvprintw(side + 0, side * 2 + 7, "estart");
        mvprintw(side + 1, side * 2 + 7, "uit");
    }
}

static int cmvprintw(int color, int row, int col, const char *str)
{
    attron(COLOR_PAIR(color));
    int r = mvprintw(row, col, "%s", str);
    attroff(COLOR_PAIR(color));
    return r;
}