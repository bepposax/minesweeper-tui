/**
 * @file board.c
 * @author Ivano Izzo
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <stdarg.h>
#include "board.h"
#include "ANSI-colors.h"
#include "string-builder.h"
#include "history.h"
#include "symbols.h"
#include "timer.h"

cell_t **board;
int height, width, board_h, board_w, mines;
const int results_width = 29, results_height = 8;
extern int goal, moves, uncovered_cells, mines_left;
extern bool game_over, lost;

extern int print_diff_menu();

/**
 * @brief prints memory allocation error to stderr
 * @param line the line where the error occourred
 */
static void printerr(char *msg, int line);

/**
 * @brief asks the user for custom input
 * @param prompt the user prompt
 * @return the user's input
 */
static int customize(char *prompt);

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

void create_board(int diff)
{
#ifndef TEST
    switch (diff)
    {
    case 1:
        height = width = 9;
        mines = 10;
        break;
    case 2:
        height = width = 16;
        mines = 40;
        break;
    case 3:
        height = 16;
        width = 30;
        mines = 99;
        break;
    case 4:
        height = customize("Height");
        width = customize("Width ");
        mines = customize("Mines ");
        break;
    }
#else
    FILE *f;
    char s[COLS];
    height = width = mines = 0;

    if (!(f = fopen(".testing/board.txt", "r")))
        printerr("Can't open file", __LINE__ - 1);
    if (!(width = strlen(fgets(s, COLS, f)) / 2))
        printerr("fgets error", __LINE__ - 1);
    height++;
    while (fgets(s, COLS, f))
        height++;
    fclose(f);
#endif
    board_h = height + 4;
    board_w = width * 2 + 3;

    if (!(board = (cell_t **)calloc(height, sizeof(cell_t *))))
        printerr("Failed to allocate memory", __LINE__ - 1);
    for (int i = 0; i < height; i++)
        if (!(board[i] = (cell_t *)calloc(width, sizeof(cell_t))))
            printerr("Failed to allocate memory", __LINE__ - 1);
}

static int customize(char *prompt)
{
    char input[4];
    int limit, choice, ch;

    do
    {
        while (print_diff_menu())
            while ((ch = getch()) != KEY_RESIZE)
                ;
        attron(A_BOLD | COLOR_PAIR(COLOR_CYAN));
        mvprintw(15, 12, " %s --  ", prompt);
        switch (prompt[0])
        {
        case 'H':
            limit = LINES - 5;
            break;
        case 'W':
            limit = COLS / 2 - 2;
            break;
        default:
            limit = height * width;
        }
        mvprintw(16, 14, "max%4d ", limit);
        mvprintw(17, 14, "or 100%% ");
        echo();
        curs_set(2);
        mvgetnstr(15, 20, input, 4);
        noecho();
        curs_set(0);
        attroff(A_BOLD | COLOR_PAIR(COLOR_CYAN));
        choice = atoi(input);
        if (choice < 3 && (prompt[0] == 'H' || prompt[0] == 'W'))
            choice = -1;
        if (input[strlen(input) - 1] == '%')
            choice = choice > 100 ? -1 : limit * choice / 100;
    } while (choice < 1 || choice > limit);

    return choice;
}

static void printerr(char *msg, int line)
{
    fprintf(stderr, "%s:%d: Error: %s\n", __FILE__, line, msg);
    exit(EXIT_FAILURE);
}

void reset_board()
{
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            board[i][j].state = UNDISCOVERED;
}

void free_board()
{
    for (int i = 0; i < height; i++)
    {
        free(board[i]);
        board[i] = NULL;
    }
    free(board);
    board = NULL;
}

bool is_printable(int board_h, int board_w)
{
    bool printable = true;
    char *msg = "Resize window";
    int center_y = LINES / 2, center_x = COLS / 2;

    clear();
    refresh();
    if (board_h >= LINES)
    {
        mvprintw(0, center_x, UP);
        mvprintw(LINES - 1, center_x, DOWN);
        printable = false;
        timer_newwin();
    }
    if (board_w > COLS)
    {
        mvprintw(center_y, 1, LEFT);
        mvprintw(center_y, COLS - 2, RIGHT);
        printable = false;
        timer_newwin();
    }
    if (!printable)
        mvprintw(center_y, center_x - strlen(msg) / 2, "%s", msg);

    return printable;
}

void print_board(bool resizing)
{
    clear();
    refresh();
    if (!is_printable(board_h, board_w))
        return;
    if (resizing && buffer)
    {
        printf("%s", buffer);
        print_time();
        print_results();
        return;
    }
    else
        offset = 0;

    // stats top
    strappend(H_GRN);
    int len = strappend(" " CELL " %d/%d", uncovered_cells, goal);
    strappend("%*.s", width * 2 - len, "");
    strappend(B_H_RED MINE H_RED " %2d\n\r" RESET, mines_left);

    // border top
    strappend(ARC0);
    for (int i = 0; i <= width * 2; i++)
        strappend(LINE_H);
    strappend(ARC1 "\n\r");

    // game board
    for (int i = 0; i < height; i++)
    {
        strappend(LINE_V " ");
        for (int j = 0; j < width; j++)
        {
            cell_t *pos = &(board[i][j]);
            int num_mines;

            if (pos->is_mine && game_over)
            {
                if (pos->state == DISCOVERED)
                    strappend(RED_ "\b" HBLOCK_R BG_RED_ B_WHT MINE RESET RED_ HBLOCK_L RESET);
                else
                    strappend(B_RED MINE " " RESET);
            }
            else if (pos->state == DISCOVERED)
                if ((num_mines = pos->surrounding_mines))
                {
                    switch (num_mines)
                    {
                    case 1:
                        strappend(H_BLU);
                        break;
                    case 2:
                        strappend(H_GRN);
                        break;
                    case 3:
                        strappend(H_YEL);
                        break;
                    case 4:
                        strappend(H_MAG);
                        break;
                    default:
                        strappend(H_CYN);
                    }
                    strappend("%d " RESET, num_mines);
                }
                else
                    strappend(H_BLK MDOT " " RESET);
            else
            {
                if (pos->state == FLAGGED)
                    strappend(RED FLAG " " RESET);
                else if (pos->state == MARKED)
                    strappend(H_YEL MARK " " RESET);
                else
                    strappend(CELL " ");
            }
        }
        strappend(LINE_V "\n\r");
    }

    // border bottom
    strappend(ARC2);
    for (int i = 0; i <= width * 2; i++)
        strappend(LINE_H);
    strappend(ARC3 "\n\r");

    // stat bottom
    strappend(B_H_CYN " " MOVES " " H_CYN "%d" RESET "\n\r", moves);

    if (game_over)
        print_results();
    printf("%s", buffer);
    add_to_history();

    // time bottom
    print_time();
}

void print_results()
{
    int margin_left = results_width >= board_w ? 0 : (board_w - results_width) / 2,
        margin,
        row = 0, col = 0;
    bool printable_results_r = COLS - board_w >= results_width && height >= results_height,
         printable_results_b = LINES - board_h >= results_height + 3 && COLS >= results_width + margin_left,
         printable = false;

    if (game_over)
    {
        if (printable_results_r)
        {
            row = (board_h - results_height) / 3;
            col = board_w + 3;
            printable = true;
        }
        else if (printable_results_b)
        {
            row = board_h + 1;
            col = margin_left;
            printable = true;
        }
        if (printable)
        {
            char *lines_h = LINE_H LINE_H LINE_H LINE_H LINE_H LINE_H LINE_H;
            char s[10];
            init_pair(8, COLOR_WHITE, COLOR_RED);
            init_pair(9, COLOR_WHITE, COLOR_GREEN);

            mvprintw(row, col, "%s YOU", lines_h);
            cmvprintw(lost ? 8 : 9, row, col + 12, "%s", lost ? " LOST " : " WON! ");
            mvprintw(row, col + 19, "%s", lines_h);
            mvprintw(row + 1, col, "Moves:");
            snprintf(s, 10, "%d/%d", uncovered_cells, goal);
            mvprintw(row + 2, col, "Uncovered cells:");
            mvprintw(row + 3, col, "Remaining cells:");
            mvprintw(row + 4, col, "Mines left:");
            attron(A_BOLD);
            cmvprintw(COLOR_CYAN, row + 1, col + 7, "%19d", moves);
            cmvprintw(COLOR_GREEN, row + 2, col + 17, "%9s", s);
            cmvprintw(COLOR_YELLOW, row + 3, col + 17, "%9d", goal - uncovered_cells);
            cmvprintw(COLOR_RED, row + 4, col + 12, "%14d", mines_left);
            attroff(A_BOLD);
            row += 6;
            margin = 4;
            attron(A_UNDERLINE);
            mvprintw(row, col + margin, "n");
            mvprintw(row, col + 19 - margin, "r");
            mvprintw(row + 1, col + margin, "h");
            mvprintw(row + 1, col + 19 - margin, "q");
            attroff(A_UNDERLINE);
            mvprintw(row, col + margin + 1, "ew-game");
            mvprintw(row++, col + 20 - margin, "estart");
            mvprintw(row, col + margin + 1, "istory");
            mvprintw(row, col + 20 - margin, "uit");
        }
    }
}

static int cmvprintw(int color, int row, int col, const char *str, ...)
{
    va_list args;
    va_start(args, str);
    attron(COLOR_PAIR(color));
    move(row, col);
    int r = vw_printw(stdscr, str, args);
    attroff(COLOR_PAIR(color));
    va_end(args);

    return r;
}
