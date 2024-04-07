/**
 * @file board.c
 * @author Ivano Izzo
 */
#include "../include/board.h"
#include "../include/difficulty.h"
#include "../include/ANSI-colors.h"
#include "../include/string_builder.h"
#include "../include/symbols.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

cell **board;
int height, width, mines;
extern int goal, moves, uncovered_cells, mines_left;
extern bool game_over, lost;

/**
 * @brief prints memory allocation error to stderr
 * @param line the line where the error occourred
 */
static void printerr(int line);

/**
 * @brief asks the user for custom input
 * @param prompt the user prompt
 * @return the user's input
 */
static int customize(char *prompt);

/**
 * @brief informs the user if the window needs to be resized to print the board
 * @param height the board's height
 * @param width the board's width
 * @param maxy the window's current height
 * @param maxx the window's current width
 * @return 1 if the board is bigger than the window; 0 otherwise
 */
static int is_bigger(int height, int width, int maxy, int maxx);

/**
 * @brief prints the stats of the game when it ends
 * @param line the line to print
 * @return 0 if the line is the last one
 */
static int print_results(int);

void create_board(int diff)
{
    switch (diff)
    {
    case 1:
        height = 9;
        width = 9;
        mines = 10;
        break;
    case 2:
        height = 16;
        width = 16;
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
    if (!(board = (cell **)calloc(height, sizeof(cell *))))
        printerr(__LINE__ - 1);
    for (int i = 0; i < height; i++)
        if (!(board[i] = (cell *)calloc(width, sizeof(cell))))
            printerr(__LINE__ - 1);
}

static int customize(char *prompt)
{
    char input[4];
    int maxx, maxy, limit, choice;

    echo();
    curs_set(2);
    do
    {
        print_diff_menu();
        attron(A_BOLD);
        attron(COLOR_PAIR(COLOR_CYAN));
        getmaxyx(stdscr, maxy, maxx);
        mvprintw(15, 12, " %s --  ", prompt);
        limit = prompt[0] == 'H' ? maxy - 4 : prompt[0] == 'W' ? maxx / 2 - 2
                                                               : height * width;
        mvprintw(16, 14, "max%4d ", limit);
        mvprintw(17, 14, "or 100%% ");
        mvgetnstr(15, 20, input, 4);
        choice = atoi(input);
        if (input[strlen(input) - 1] == '%')
            choice = choice > 100 ? -1 : limit * choice / 100;
    } while (choice < 1 || choice > limit);
    if (prompt[0] == 'M')
    {
        curs_set(0);
        noecho();
        clear();
    }

    return choice;
}

static void printerr(int line)
{
    fprintf(stderr, "%s:%d: Error: Failed to allocate memory\n", __FILE__, line);
    exit(EXIT_FAILURE);
}

void reset_board()
{
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
        {
            board[i][j].is_discovered &= 0;
            board[i][j].is_flagged &= 0;
            board[i][j].is_marked &= 0;
        }
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

static int is_bigger(int height, int width, int maxy, int maxx)
{
    int resize = 0;
    char *msg = "Resize window";

    if (height > maxy)
    {
        mvprintw(0, maxx / 2, UP);
        mvprintw(maxy - 1, maxx / 2, DOWN);
        resize = 1;
    }
    if (width > maxx)
    {
        mvprintw(maxy / 2, 0, LEFT);
        mvprintw(maxy / 2, maxx - 1, RIGHT);
        resize = 1;
    }
    if (resize)
        mvprintw(maxy / 2, maxx / 2 - strlen(msg) / 2, "%s", msg);

    return resize;
}

void print_board()
{
    clear();
    refresh();

    int maxy = getmaxy(stdscr), maxx = getmaxx(stdscr);
    static int results_width = 27, results_height = 9;
    int board_width = width * 2 + 3, board_height = height + 4;
    int margin_right = maxx - board_width;
    int margin_bottom = maxy - board_height;
    int margin_left = results_width >= board_width ? 0 : (board_width - results_width) / 2 + 1;
    bool right = margin_right >= results_width && height >= results_height;
    bool bottom = margin_bottom >= results_height + 2 && maxx >= results_width + margin_left;

    if (is_bigger(board_height, board_width, maxy, maxx))
        return;

    // stats top
    strappend(H_GRN);
    int len = strappend(" " CELL " %d/%d", uncovered_cells, goal);
    for (int i = 0; i < width * 2 - len; i++)
        strappend(" ");
    strappend(B_H_RED MINE H_RED " %2d\n\r" RESET, mines_left);

    // border top
    strappend(ARC_0);
    for (int i = 0; i <= width * 2; i++)
        strappend(LINE_H);
    strappend(ARC_1 "\n\r");

    // game board
    for (int i = 0; i < height; i++)
    {
        strappend(LINE_V " ");
        for (int j = 0; j < width; j++)
        {
            cell *pos = &(board[i][j]);
            int num_mines;

            if (pos->is_mine && game_over)
            {
                if (pos->is_discovered)
                    strappend(RED_ "\b" H_BLOCK_R BG_RED_ B_WHT MINE RESET RED_ H_BLOCK_L RESET);
                else
                    strappend(B_RED MINE " " RESET);
            }
            else if (pos->is_discovered)
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
                        break;
                    }
                    strappend("%d " RESET, num_mines);
                }
                else
                    strappend(H_BLK M_DOT " " RESET);
            else
            {
                if (pos->is_flagged)
                    strappend(RED FLAG " " RESET);
                else if (pos->is_marked)
                    strappend(H_YEL MARK " " RESET);
                else
                    strappend(CELL " ");
            }
        }
        strappend(LINE_V);

        // results right
        if (game_over && right)
        {
            strappend("  ");
            print_results(i);
        }
        strappend("\n\r");
    }

    // border bottom
    strappend(ARC_2);
    for (int i = 0; i <= width * 2; i++)
        strappend(LINE_H);
    strappend(ARC_3 "\n\r");

    // stat bottom
    strappend(B_H_CYN " " MOVES " " H_CYN "%d" RESET "\n", moves);

    // results bottom
    if (game_over && !right && bottom)
    {
        int i = 0;
        do
            strappend("\n\r%*.s", margin_left, "");
        while (print_results(i++));
    }
    printf("%s\r", buffer);
    offset = 0;
}

static int print_results(int line)
{
    switch (line)
    {
    case 0:
        char *line = LINE_H LINE_H LINE_H LINE_H LINE_H LINE_H LINE_H;
        return strappend(B_H_WHT "%s Game Over %s" RESET, line, line);
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
    case 6:
        if (lost)
            return strappend("You " BG_RED B_H_WHT " LOST " RESET "%15s", "Try again");
        else
            return strappend("You " BG_GRN B_H_YEL " WON " RESET "%16s", "Well done!");
    case 8:
        return strappend(U_WHT "n" RESET "ew-game   " U_WHT "r" RESET "estart   " U_WHT "q" RESET "uit");
    case 9:
        return 0;
    default:
        return 1;
    }
}