/**
 * @file difficulty.c
 * @author Ivano Izzo
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "difficulty.h"
#include "string-builder.h"
#include "symbols.h"

const int board_size = 16, mid_col = board_size + 1;
const char *diff[] = {"D I F F I C U L T Y",
                      "B E G I N N E R",
                      "I N T E R M E D I A T E",
                      "E X P E R T",
                      "C U S T O M"};

#define half_d0 (int)strlen(diff[0]) / 2
#define half_d1 (int)strlen(diff[1]) / 2
#define half_d2 (int)strlen(diff[2]) / 2
#define half_d3 (int)strlen(diff[3]) / 2
#define half_d4 (int)strlen(diff[4]) / 2

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
    int ch, offset;

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
            {
                offset = abs(mid_col - event.x);
                switch (event.y)
                {
                case 6:
                    if (offset <= half_d1)
                        return 1;
                    break;
                case 9:
                    if (offset <= half_d2)
                        return 2;
                    break;
                case 12:
                    if (offset <= half_d3)
                        return 3;
                    break;
                case 15:
                    if (offset <= half_d4)
                        return 4;
                    break;
                }
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
    const int board_h = board_size, board_w = board_size * 2,
              menu_h = board_h + 4, menu_w = board_w + 3;
    int optns_col = board_w + 6;

    clear();
    if (!is_printable(menu_h, menu_w))
        return 1;

    // corner stats
    cmvprintw(COLOR_GREEN, 0, 1, "goal");
    cmvprintw(COLOR_RED, 0, menu_w - 6, "mines");
    cmvprintw(COLOR_CYAN, menu_h - 1, 1, "moves");
    cmvprintw(COLOR_YELLOW, menu_h - 1, menu_w - 5, "time");
    move(1, 0);
    refresh();

    // board
    strappend(ARC_0);
    for (int i = 0; i <= board_w; i++)
        strappend(LINE_H);
    strappend(ARC_1 "\n\r");
    for (int i = 0; i < board_h; i++)
    {
        strappend(LINE_V " ");
        for (int j = 0; j < board_w; j += 2)
            strappend(CELL " ");
        strappend(LINE_V "\n\r");
    }
    strappend(ARC_2);
    for (int i = 0; i <= board_w; i++)
        strappend(LINE_H);
    strappend(ARC_3 "\n\r");
    printf("%s", buffer);
    offset = 0;

    // difficulties
    attron(A_BOLD);
    mvprintw(3, mid_col - half_d0, "%s", diff[0]);
    cmvprintw(COLOR_GREEN, 6, mid_col - half_d1, diff[1]);
    cmvprintw(COLOR_YELLOW, 9, mid_col - half_d2, diff[2]);
    cmvprintw(COLOR_RED, 12, mid_col - half_d3, diff[3]);
    cmvprintw(COLOR_CYAN, 15, mid_col - half_d4, diff[4]);
    attroff(A_BOLD);

    // side options
    if (optns_col + 7 < COLS)
    {
        attron(A_UNDERLINE);
        mvprintw(menu_h - 5, optns_col, "n");
        mvprintw(menu_h - 4, optns_col, "r");
        mvprintw(menu_h - 3, optns_col, "q");
        attroff(A_UNDERLINE);
        optns_col++;
        mvprintw(menu_h - 5, optns_col, "ew game");
        mvprintw(menu_h - 4, optns_col, "estart");
        mvprintw(menu_h - 3, optns_col, "uit");
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