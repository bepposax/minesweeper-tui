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
#include <ncurses.h>

cell **board;
int height, width, mines;
extern int goal, moves, uncovered_cells;
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
 * @brief prints the stats of the game when it ends
 * @param line the line to print
 * @return 0 if the line is the last one
 */
static int print_stats(int);

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
        limit = prompt[0] == 'H' ? maxy - 6 : prompt[0] == 'W' ? maxx / 2 - 2
                                                               : height * width;
        mvprintw(16, 14, "max%4d ", limit);
        mvgetnstr(15, 20, input, 4);
        choice = atoi(input);
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

void print_board()
{
    clear();
    refresh();

    // stats top
    strappend(H_GRN);
    int len = strappend(" " CELL " %d/%d", uncovered_cells, goal);
    for (int i = 0; i < width * 2 - len; i++)
        strappend(" ");
    strappend(B_H_RED MINE H_RED " %2d\n\r" RESET, mines);

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
                    strappend(RED "\b" H_BLOCK_R BG_RED B_WHT MINE RESET RED H_BLOCK_L RESET);
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
                    strappend("■ ");
            }
        }
        strappend("│");

        // results right
        if (game_over && (width <= (getmaxx(stdscr) - 34) / 2 && height > 8))
        {
            strappend("    ");
            print_stats(i);
        }
        strappend("\n\r");
    }

    // border bottom
    strappend(ARC_2);
    for (int i = 0; i <= width * 2; i++)
        strappend(LINE_H);
    strappend(ARC_3 "\n\r");

    // stats bottom
    strappend(B_H_CYN " " MOVES " " H_CYN "%d\n" RESET, moves);

    // results bottom
    if (game_over && (width > (getmaxx(stdscr) - 34) / 2 || height <= 8))
    {
        int i = 0, indent = width - 11;

        do
        {
            strappend("\n\r");
            for (int j = 0; j < indent; j++)
                strappend(" ");
        } while (print_stats(i++));
    }
    printf("%s\r", buffer);
    offset = 0;
}

static int print_stats(int line)
{
    switch (line)
    {
    case 0:
        return strappend(B_H_WHT "------- Game Over -------" RESET);
    case 1:
        return strappend("Moves: " H_CYN "%18d" RESET, moves);
    case 2:
        char s[10];
        snprintf(s, 10, "%d/%d", uncovered_cells, goal);
        return strappend("Uncovered cells:" H_GRN "%9s" RESET, s);
    case 3:
        return strappend("Remaining cells: " H_YEL "%8d" RESET, goal - uncovered_cells);
    case 4:
        return strappend("Mines left: " H_RED "%13d" RESET, mines);
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