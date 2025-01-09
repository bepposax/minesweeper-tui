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

const int board_size = 16, mid_col = board_size + 1;
const char title[] = "D I F F I C U L T Y";
const char *diff[] = {"B E G I N N E R",
                      "I N T E R M E D I A T E",
                      "E X P E R T",
                      "C U S T O M"};

#define t_h (int)strlen(title) / 2
#define d0_h (int)strlen(diff[0]) / 2
#define d1_h (int)strlen(diff[1]) / 2
#define d2_h (int)strlen(diff[2]) / 2
#define d3_h (int)strlen(diff[3]) / 2

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
                    if (event.x >= mid_col - d0_h && event.x <= mid_col + d0_h)
                        return 1;
                    break;
                case 9:
                    if (event.x >= mid_col - d1_h && event.x <= mid_col + d1_h)
                        return 2;
                    break;
                case 12:
                    if (event.x >= mid_col - d2_h && event.x <= mid_col + d2_h)
                        return 3;
                    break;
                case 15:
                    if (event.x >= mid_col - d3_h && event.x <= mid_col + d3_h)
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
    mvprintw(3, mid_col - t_h, title);
    cmvprintw(COLOR_GREEN, 6, mid_col - d0_h, diff[0]);
    cmvprintw(COLOR_YELLOW, 9, mid_col - d1_h, diff[1]);
    cmvprintw(COLOR_RED, 12, mid_col - d2_h, diff[2]);
    cmvprintw(COLOR_CYAN, 15, mid_col - d3_h, diff[3]);
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