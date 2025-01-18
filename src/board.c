/**
 * @file board.c
 * @author Ivano Izzo
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "board.h"
#include "ANSI-colors.h"
#include "string-builder.h"
#include "history.h"
#include "symbols.h"
#include "timer.h"

cell_t **board;
int height, width, board_h, board_w, mines;
const int results_width = 27, results_height = 9;
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
 * @brief prints the stats of the game when it ends
 * @param line the line to print
 * @return 0 if the line is the last one
 */
static int print_results(int);

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

void print_board()
{
    int margin_left = results_width >= board_w ? 0 : (board_w - results_width) / 2 + 1;
    bool printable_results_r = COLS - board_w >= results_width && height >= results_height;
    bool printable_results_b = LINES - board_h >= results_height + 2 && COLS >= results_width + margin_left;

    clear();
    refresh();
    if (!is_printable(board_h, board_w))
        return;

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
        strappend(LINE_V);

        // results right
        if (game_over && printable_results_r)
        {
            strappend("  ");
            print_results(i);
        }
        strappend("\n\r");
    }

    // border bottom
    strappend(ARC2);
    for (int i = 0; i <= width * 2; i++)
        strappend(LINE_H);
    strappend(ARC3 "\n\r");

    // stat bottom
    strappend(B_H_CYN " " MOVES " " H_CYN "%d" RESET "\n", moves);

    // results bottom
    if (game_over && !printable_results_r && printable_results_b)
    {
        int i = 0;
        do
            strappend("\n\r%*.s", margin_left, "");
        while (print_results(i++));
    }
    strappend("\r");
    printf("%s", buffer);
    add_to_history();
    offset = 0;

    // time bottom
    print_time();
}

static int print_results(int line)
{
    switch (line)
    {
    case 0:
        char *lines_h = LINE_H LINE_H LINE_H LINE_H LINE_H LINE_H LINE_H;
        return strappend(B_H_WHT "%s Game Over %s" RESET, lines_h, lines_h);
    case 1:
        return strappend("Moves:" H_CYN "%19d" RESET, moves);
    case 2:
        char s[10];
        snprintf(s, 10, "%d/%d", uncovered_cells, goal);
        return strappend("Uncovered cells:" H_GRN "%9s" RESET, s);
    case 3:
        return strappend("Remaining cells:" H_YEL "%9d" RESET, goal - uncovered_cells);
    case 4:
        return strappend("Mines left:" H_RED "%14d" RESET, mines_left);
    case 5:
        if (lost)
            return strappend("You " BG_RED B_H_WHT " LOST " RESET "%15s", "Try again");
        else
            return strappend("You " BG_GRN B_H_YEL " WON " RESET "%16s", "Well done!");
    case 7:
        return strappend(U_WHT "n" RESET "ew-game%28s", U_WHT "r" RESET "estart");
    case 8:
        return strappend(U_WHT "h" RESET "istory%29s", U_WHT "q" RESET "uit");
    case 9:
        return 0;
    default:
        return 1;
    }
}