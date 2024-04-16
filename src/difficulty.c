/**
 * @file difficulty.c
 * @author Ivano Izzo
 */
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "difficulty.h"
#include "string-builder.h"
#include "symbols.h"

const int side = 16, menu_height = side + 4, menu_width = side * 2 + 3;

extern bool is_printable(int height, int width);

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

#ifdef TEST
    return -1;
#endif
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

int print_diff_menu()
{
    clear();
    if (!is_printable(menu_height, menu_width))
        return 1;

    // corner stats
    cmvprintw(COLOR_GREEN, 0, 1, "goal");
    cmvprintw(COLOR_RED, 0, side * 2 - 3, "mines");
    cmvprintw(COLOR_CYAN, side + 3, 1, "moves");
    cmvprintw(COLOR_YELLOW, side + 3, side * 2 - 2, "time");
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
    if (side * 2 + 13 < COLS)
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

    return 0;
}

static int cmvprintw(int color, int row, int col, const char *str)
{
    attron(COLOR_PAIR(color));
    int r = mvprintw(row, col, "%s", str);
    attroff(COLOR_PAIR(color));
    return r;
}