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

typedef struct diff
{
    const char *name;
    const int color;
    int row;
} diff_t;

diff_t diffs[] = {
    {.name = "D I F F I C U L T Y",
     .color = COLOR_WHITE},
    {.name = "B E G I N N E R",
     .color = COLOR_GREEN},
    {.name = "I N T E R M E D I A T E",
     .color = COLOR_YELLOW},
    {.name = "E X P E R T",
     .color = COLOR_RED},
    {.name = "C U S T O M",
     .color = COLOR_CYAN}};

#define NUM_DIFFS (int)sizeof(diffs) / (int)sizeof(diffs[0])

const int side = 18, mid_col = side + 1,
          board_h = side, board_w = side * 2,
          menu_h = board_h + 4, menu_w = board_w + 3,
          vertical_offset = board_h / (NUM_DIFFS);
int semilen[NUM_DIFFS];
int selected = 0;
mmask_t oldmask;

extern bool is_printable(int height, int width);

/**
 * @brief populate difficulties with their row
 */
static void set_diffs_rows()
{
    for (int i = 0; i < NUM_DIFFS; ++i)
        diffs[i].row = vertical_offset * (i + 1) + 1 + (i != 0);
}

/**
 * @brief prints a border over a difficulty name when mouse hovering
 * @param d the difficulty number
 */
static void print_border(int d);

/**
 * @brief removes a border around a difficulty name when not hovering it
 * @param d the difficulty number
 */
static void clear_border(int d);

/**
 * @brief initializes semilen[] with half the lengths of the strings in diffs[]
 */
static inline void init_semilenghts()
{
    for (int i = 0; i < NUM_DIFFS; ++i)
        semilen[i] = (int)strlen(diffs[i].name) / 2;
}

static inline int abs(int x) { return x < 0 ? -x : x; }

/**
 * @brief mvprintw with color
 * @param color the color to use
 * @param row the row to print
 * @param col the column to print
 * @param str the string to print
 * @return the return value of mvprintw
 * @see mvprintw
 */
static int cmvprintw(int color, int row, int col, const char *str, ...);

int select_diff()
{
    int ch, x_offset, y_offset, hovering = 0, bordered = 0;

#ifdef TEST
    return -1;
#endif
    selected = 0;
    init_semilenghts();
    set_diffs_rows();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, &oldmask);
    printf("\033[?1003h\n"); // terminal reports mouse events
    print_diff_menu();
    while ((ch = getch()))
    {
        if (ch == KEY_MOUSE)
        {
            MEVENT event;
            if (getmouse(&event) == OK)
            {
                hovering = abs(diffs[0].row - event.y) / vertical_offset;
                y_offset = abs(diffs[hovering].row - event.y);
                x_offset = abs(mid_col - event.x);
                if (hovering < NUM_DIFFS && event.y >= diffs[1].row - 2 && 
                    x_offset <= semilen[hovering] + 2 && y_offset <= 1) 
                {
                    if (hovering != bordered)
                    {
                        if (bordered)
                            clear_border(bordered);
                        print_border((bordered = hovering));
                    }
                    if (event.bstate & BUTTON1_PRESSED)
                        selected = hovering;
                }
                else if (bordered)
                {
                    clear_border(bordered);
                    bordered = 0;
                }
            }
        }
        else if (ch == 'q' || ch == 'Q')
            break;
        else if (ch == KEY_RESIZE)
            print_diff_menu();
        if (selected)
            break;
    }
    printf("\033[?1003ln"); // disable terminal reporting
    mousemask(oldmask, NULL);

    return selected;
}

int print_diff_menu()
{
    int optns_col = menu_w + 3;

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
    strappend(ARC0);
    for (int i = 0; i <= board_w; i++)
        strappend(LINE_H);
    strappend(ARC1 "\n\r");
    for (int i = 0; i < board_h; i++)
    {
        strappend(LINE_V " ");
        for (int j = 0; j < board_w; j += 2)
            strappend(CELL " ");
        strappend(LINE_V "\n\r");
    }
    strappend(ARC2);
    for (int i = 0; i <= board_w; i++)
        strappend(LINE_H);
    strappend(ARC3 "\n\r");
    printf("%s", buffer);
    offset = 0;

    // difficulties
    attron(WA_BOLD);
    for (int i = 0; i < NUM_DIFFS; ++i)
    {
        attron(COLOR_PAIR(diffs[i].color));
        mvprintw(diffs[i].row, mid_col - semilen[i], "%s", diffs[i].name);
        attroff(COLOR_PAIR(diffs[i].color));
    }
    attroff(WA_BOLD);
    print_border(0);
    if (selected == 4)
        print_border(4);

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

static void print_border(int d)
{
    attron(WA_BOLD | COLOR_PAIR(diffs[d].color));
    mvprintw(diffs[d].row - 1, mid_col - semilen[d] - 2, "%s", ARC0);
    for (int j = 0; j < (int)strlen(diffs[d].name) + 2; ++j)
        printw("%s", LINE_H);
    printw("%s", ARC1);
    mvprintw(diffs[d].row, mid_col - semilen[d] - 2, "%s", LINE_V);
    mvprintw(diffs[d].row, mid_col + semilen[d] + 2, "%s", LINE_V);
    mvprintw(diffs[d].row + 1, mid_col - semilen[d] - 2, "%s", ARC2);
    for (int j = 0; j < (int)strlen(diffs[d].name) + 2; ++j)
        printw("%s", LINE_H);
    printw("%s", ARC3);
    attroff(WA_BOLD | COLOR_PAIR(diffs[d].color));
    refresh();
}

static void clear_border(int d)
{
    attron(WA_BOLD);
    move(diffs[d].row - 1, mid_col - semilen[d] - 2);
    for (int j = 0; j < (int)strlen(diffs[d].name) + 4; j += 2)
        printw("%s ", CELL);
    mvprintw(diffs[d].row, mid_col - semilen[d] - 2, "%s", CELL);
    mvprintw(diffs[d].row, mid_col + semilen[d] + 2, "%s", CELL);
    move(diffs[d].row + 1, mid_col - semilen[d] - 2);
    for (int j = 0; j < (int)strlen(diffs[d].name) + 4; j += 2)
        printw("%s ", CELL);
    attroff(WA_BOLD);
    refresh();
}

static int cmvprintw(int color, int row, int col, const char *str, ...)
{
    attron(COLOR_PAIR(color));
    move(row, col);
    va_list args;
    va_start(args, str);
    int r = vw_printw(stdscr, str, args);
    va_end(args);
    attroff(COLOR_PAIR(color));

    return r;
}