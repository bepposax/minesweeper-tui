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

board_t board;

typedef struct results
{
    int height, width, row, col, margin;
} results_t;

results_t res = {
    .height = 8,
    .width = 26,
    .margin = 4
};

extern int goal, moves, uncovered_cells, mines_left;
extern bool game_over, lost;

extern int print_diff_menu();

/**
 * @brief calculates the column where the results should be printed, if printing them at the bottom
 * @return the number of the column where to start printing the results, so that they'll be centered
 * compared to the board 
 */
static inline int margin_l() { return res.width >= board.width ? 0 : (board.width - res.width) / 2; }

/**
 * @brief checks if there's space to print the results
 * @return true if there's space, false otherwise
 */
static bool printable_results(){
    // printable to the right?
    if (COLS >= (board.width + res.width + 3) && board.rows >= res.height)
    {
        res.row = (board.height - res.height) / 3;
        res.col = board.width + 3;
        return true;
    }
    // printable at the bottom?
    if (LINES >= (board.height + res.height + 3) && COLS >= (res.width + margin_l()))
    {
        res.row = board.height + 1;
        res.col = margin_l();
        return true;
    }
    return false;
}


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
        board.rows = board.cols = 9;
        board.mines = 10;
        break;
    case 2:
        board.rows = board.cols = 16;
        board.mines = 40;
        break;
    case 3:
        board.rows = 16;
        board.cols = 30;
        board.mines = 99;
        break;
    case 4:
        board.rows = customize("Height");
        board.cols = customize("Width ");
        board.mines = customize("Mines ");
        break;
    }
#else
    FILE *f;
    char s[COLS];
    board.rows = board.cols = board.mines = 0;

    if (!(f = fopen(".testing/board.txt", "r")))
        printerr("Can't open file", __LINE__ - 1);
    if (!(board.cols = strlen(fgets(s, COLS, f)) / 2))
        printerr("fgets error", __LINE__ - 1);
    board.rows++;
    while (fgets(s, COLS, f))
        board.rows++;
    fclose(f);
#endif
    board.height = board.rows + 4;
    board.width = board.cols * 2 + 3;

    if (!(board.cells = (cell_t **)calloc(board.rows, sizeof(cell_t *))))
        printerr("Failed to allocate memory", __LINE__ - 1);
    for (int i = 0; i < board.rows; i++)
        if (!(board.cells[i] = (cell_t *)calloc(board.cols, sizeof(cell_t))))
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
            limit = board.rows * board.cols;
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
    for (int i = 0; i < board.rows; ++i)
        for (int j = 0; j < board.cols; ++j)
            board.cells[i][j].state = UNDISCOVERED;
}

void free_board()
{
    for (int i = 0; i < board.rows; i++)
    {
        free(board.cells[i]);
        board.cells[i] = NULL;
    }
    free(board.cells);
    board.cells = NULL;
}

bool is_printable(int h, int w)
{
    bool printable = true;
    char *msg = "Resize window";
    int center_y = LINES / 2, center_x = COLS / 2;

    clear();
    refresh();
    if (h >= LINES)
    {
        mvprintw(0, center_x, UP);
        mvprintw(LINES - 1, center_x, DOWN);
        printable = false;
        timer_newwin();
    }
    if (w > COLS)
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
    if (!is_printable(board.height, board.width))
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
    strappend("%*.s", board.cols * 2 - len, "");
    strappend(B_H_RED MINE H_RED " %2d\n\r" RESET, mines_left);

    // border top
    strappend(ARC0);
    for (int i = 0; i <= board.cols * 2; i++)
        strappend(LINE_H);
    strappend(ARC1 "\n\r");

    // game board
    for (int i = 0; i < board.rows; i++)
    {
        strappend(LINE_V " ");
        for (int j = 0; j < board.cols; j++)
        {
            cell_t *pos = &(board.cells[i][j]);
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
    for (int i = 0; i <= board.cols * 2; i++)
        strappend(LINE_H);
    strappend(ARC3 "\n\r");

    // stat bottom
    strappend(B_H_CYN " " MOVES " " H_CYN "%d" RESET "\n\r", moves);

    if (game_over)
        print_results();
    printf("%s", buffer);
    update_history();

    // time bottom
    print_time();
}

void print_results()
{
    if (game_over && printable_results())
    {
        char *lines_h = LINE_H LINE_H LINE_H LINE_H LINE_H LINE_H LINE_H;
        char s[10];
        init_pair(8, COLOR_WHITE, COLOR_RED);
        init_pair(9, COLOR_WHITE, COLOR_GREEN);
        mvprintw(res.row, res.col, "%s YOU", lines_h);
        cmvprintw(lost ? 8 : 9, res.row, res.col + 12, "%s", lost ? " LOST " : " WON! ");
        mvprintw(res.row, res.col + 19, "%s", lines_h);
        mvprintw(res.row + 1, res.col, "Moves:");
        snprintf(s, 10, "%d/%d", uncovered_cells, goal);
        mvprintw(res.row + 2, res.col, "Uncovered cells:");
        mvprintw(res.row + 3, res.col, "Remaining cells:");
        mvprintw(res.row + 4, res.col, "Mines left:");
        attron(A_BOLD);
        cmvprintw(COLOR_CYAN, res.row + 1, res.col + 7, "%19d", moves);
        cmvprintw(COLOR_GREEN, res.row + 2, res.col + 17, "%9s", s);
        cmvprintw(COLOR_YELLOW, res.row + 3, res.col + 17, "%9d", goal - uncovered_cells);
        cmvprintw(COLOR_RED, res.row + 4, res.col + 12, "%14d", mines_left);
        attroff(A_BOLD);
        res.row += 6;
        attron(A_UNDERLINE);
        mvprintw(res.row, res.col + res.margin, "n");
        mvprintw(res.row, res.col + 19 - res.margin, "r");
        mvprintw(res.row + 1, res.col + res.margin, "h");
        mvprintw(res.row + 1, res.col + 19 - res.margin, "q");
        attroff(A_UNDERLINE);
        mvprintw(res.row, res.col + res.margin + 1, "ew-game");
        mvprintw(res.row++, res.col + 20 - res.margin, "estart");
        mvprintw(res.row, res.col + res.margin + 1, "istory");
        mvprintw(res.row, res.col + 20 - res.margin, "uit");
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
