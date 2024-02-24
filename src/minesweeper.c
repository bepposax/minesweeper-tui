/**
 * @file minesweeper.c
 * @author Ivano Izzo
 */
#include "../include/minesweeper.h"
#include "../include/ANSI-colors.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAXLEN 100000

char out[MAXLEN]; // output as string
int os;           // offset
int goal, moves, uncovered_cells;
bool game_over, lost;

/**
 * @brief checks if the number of flags and mines surrounding this cell match
 * @param row the cell's row
 * @param col the cell's column
 */
static bool discoverable(int row, int col);

/**
 * @brief checks if the game is over
 * @param this the cell to check
 * @return true if the game is over
 *         false if the game is not over
 * @note the game is over if the cell contains a mine or if the number of uncovered cells
 *       is equal to the number of cells on the board minus the number of mines
 *       (i.e. the user has uncovered all the cells that do not contain a mine)
 */
static bool is_game_over(cell *this);

/**
 * @brief prints the results of the game
 * @param line the line to print
 * @return 0 if the line is the last one
 */
static int print_results(int);

int game_loop()
{
    int ch = 0, row, col;

    do
    {
        game_over = false;
        goal = height * width - mines;
        moves = 0;
        uncovered_cells = 0;

        if (ch != 'r')
            place_mines();
        print_board();
        while (!game_over)
        {
            if ((ch = tolower(getch())) == KEY_MOUSE)
            {
                MEVENT event;
                if (getmouse(&event) == OK)
                {
                    row = event.y - 2;
                    col = event.x - 2;
                    if (col % 2 == 0)
                        col /= 2;
                    else
                        continue;
                    if (row >= 0 && row < height && col >= 0 && col < width)
                    {
                        if (event.bstate & BUTTON1_CLICKED)
                            play(row, col);
                        else if (event.bstate & BUTTON2_CLICKED)
                            mark(row, col);
                        else if (event.bstate & BUTTON3_CLICKED)
                            flag(row, col);
                    }
                }
            }
            else if (ch == KEY_RESIZE)
                print_board();
            else if (ch == 'n' || ch == 'q')
                return ch;
            else if (ch == 'r')
            {
                moves = 0;
                uncovered_cells = 0;
                reset_board();
                print_board();
            }
        }
        // game over
        print_board();
        while ((ch = tolower(getch())) != 'q' && ch != 'n' && ch != 'r')
            if (ch == KEY_RESIZE)
                print_board();
        if (ch == 'r')
            reset_board();
    } while (ch == 'r');

    return ch;
}

void place_mines()
{
    int mine_row, mine_col, mines_left = mines;
    srand(time(NULL));

    while (mines_left)
    {
        mine_row = rand() % height;
        mine_col = rand() % width;
        cell *pos = &(board[mine_row][mine_col]);

        if (!pos->is_mine)
        {
            pos->is_mine = true;
            signal_mine(mine_row, mine_col);
            mines_left--;
        }
    }
}

void signal_mine(int row, int col)
{
    for (int i = row - 1; i <= row + 1; i++)
        if (i >= 0 && i < height)
            for (int j = col - 1; j <= col + 1; j++)
                if (j >= 0 && j < width)
                {
                    cell *pos = &(board[i][j]);
                    if (!pos->is_mine)
                        (pos->surrounding_mines)++;
                }
}

void print_board()
{
    os = 0;

    clear();
    refresh();

    // stats top
    os += sprintf(out + os, H_GRN);
    int len = sprintf(out + os, " ■ %d/%d", uncovered_cells, goal);
    os += len;
    for (int i = 0; i < width * 2 - len; i++)
        os += sprintf(out + os, " ");
    os += sprintf(out + os, B_H_RED "*" H_RED " %2d\n\r" RESET, mines);

    // border top
    os += sprintf(out + os, "╭");
    for (int i = 0; i <= width * 2; i++)
        os += sprintf(out + os, "─");
    os += sprintf(out + os, "╮\n\r");

    // game board
    for (int i = 0; i < height; i++)
    {
        os += sprintf(out + os, "│ ");
        for (int j = 0; j < width; j++)
        {
            cell *pos = &(board[i][j]);
            int num_mines;

            if (pos->is_mine && game_over)
            {
                if (pos->is_discovered)
                    os += sprintf(out + os, RED "\b▐" BG_RED B_WHT "*" RESET RED "▌" RESET);
                else
                    os += sprintf(out + os, B_RED "* " RESET);
            }
            else if (pos->is_discovered)
                if ((num_mines = pos->surrounding_mines))
                {
                    switch (num_mines)
                    {
                    case 1:
                        os += sprintf(out + os, H_BLU);
                        break;
                    case 2:
                        os += sprintf(out + os, H_GRN);
                        break;
                    case 3:
                        os += sprintf(out + os, H_YEL);
                        break;
                    case 4:
                        os += sprintf(out + os, H_MAG);
                        break;
                    default:
                        os += sprintf(out + os, H_CYN);
                        break;
                    }
                    os += sprintf(out + os, "%d " RESET, num_mines);
                }
                else
                    os += sprintf(out + os, H_BLK "· " RESET);
            else
            {
                if (pos->is_flagged)
                    os += sprintf(out + os, RED "⚑ " RESET);
                else if (pos->is_marked)
                    os += sprintf(out + os, H_YEL "? " RESET);
                else
                    os += sprintf(out + os, "■ ");
            }
        }
        os += sprintf(out + os, "│");

        // results right
        if (game_over && (width <= (getmaxx(stdscr) - 34) / 2 && height > 8))
        {
            os += sprintf(out + os, "    ");
            print_results(i);
        }
        os += sprintf(out + os, "\n\r");
    }

    // border bottom
    os += sprintf(out + os, "╰");
    for (int i = 0; i <= width * 2; i++)
        os += sprintf(out + os, "─");
    os += sprintf(out + os, "╯\n\r");

    // stats bottom
    os += sprintf(out + os, B_H_CYN " # " H_CYN "%d\n" RESET, moves);

    // results bottom
    if (game_over && (width > (getmaxx(stdscr) - 34) / 2 || height <= 8))
    {
        int i = 0, indent = width - 11;

        do
        {
            os += sprintf(out + os, "\n\r");
            for (int j = 0; j < indent; j++)
                os += sprintf(out + os, " ");
        } while (print_results(i++));
    }
    printf("%s", out);
}

int play(int row, int col)
{
    cell *pos = &(board[row][col]);

    if (pos->is_flagged || pos->is_marked)
        return 0;
    if (pos->is_discovered)
    {
        if (!pos->surrounding_mines)
            return 0;
        if (discoverable(row, col))
        {
            moves++;
            for (int i = row - 1; i <= row + 1; i++)
                if (i >= 0 && i < height)
                    for (int j = col - 1; j <= col + 1; j++)
                        if (j >= 0 && j < width && !board[i][j].is_flagged)
                        {
                            discover(i, j);
                            if (is_game_over(&board[i][j]))
                                return 1;
                        }
        }
        else
            return 0;
    }
    else
    {
        moves++;
        pos->is_discovered = true;
        if (!pos->is_mine)
            uncovered_cells++;
        if (is_game_over(pos))
            return 1;
        discover(row, col);
    }
    print_board();
    return 0;
}

void flag(int row, int col)
{
    cell *this = &board[row][col];

    if (!this->is_discovered)
    {
        if (this->is_marked)
            mark(row, col);
        (this->is_flagged = !this->is_flagged) ? mines-- : mines++;
        print_board();
    }
}

void mark(int row, int col)
{
    cell *this = &board[row][col];

    if (!this->is_discovered)
    {
        if (this->is_flagged)
            flag(row, col);
        this->is_marked = !this->is_marked;
        print_board();
    }
}

int discover(int row, int col)
{
    cell *this = &(board[row][col]);

    if (!this->is_discovered)
    {
        this->is_discovered = true;
        if (!this->is_mine)
            uncovered_cells++;
        if (is_game_over(this))
            return 1;
    }
    for (int i = row - 1; i <= row + 1; i++)
        if (i >= 0 && i < height)
            for (int j = col - 1; j <= col + 1; j++)
                if (j >= 0 && j < width)
                {
                    cell *neighbor = &(board[i][j]);
                    if (!neighbor->is_discovered)
                    {
                        if (!neighbor->surrounding_mines && !neighbor->is_mine)
                            discover(i, j);
                        else if (!this->surrounding_mines)
                        {
                            neighbor->is_discovered = true;
                            uncovered_cells++;
                            if (is_game_over(neighbor))
                                return 1;
                        }
                    }
                }
    return 0;
}

static bool discoverable(int row, int col)
{
    int nmines = 0;
    bool clear = true;

    for (int i = row - 1; i <= row + 1; i++)
        if (i >= 0 && i < height)
            for (int j = col - 1; j <= col + 1; j++)
                if (j >= 0 && j < width)
                {
                    cell *neighbor = &(board[i][j]);
                    if (neighbor->is_flagged)
                    {
                        if (neighbor->is_mine)
                            nmines++;
                        else
                            return true;
                    }
                    // incorrect flag placement (leads to a loss)
                    else if (!neighbor->is_discovered)
                        clear = false;
                }
    if (nmines == board[row][col].surrounding_mines && !clear)
        return true;
    return false;
}

static bool is_game_over(cell *this)
{
    return (game_over = ((lost = this->is_mine) || !(goal - uncovered_cells)));
}

int print_results(int line)
{
    switch (line)
    {
    case 0:
        return (os += sprintf(out + os, B_H_WHT "------- Game Over -------" RESET));
    case 1:
        return (os += sprintf(out + os, "Moves: " H_CYN "%18d" RESET, moves));
    case 2:
        char s[10];
        snprintf(s, 10, "%d/%d", uncovered_cells, goal);
        return (os += sprintf(out + os, "Uncovered cells:" H_GRN "%9s" RESET, s));
    case 3:
        return (os += sprintf(out + os, "Remaining cells: " H_YEL "%8d" RESET, goal - uncovered_cells));
    case 4:
        return (os += sprintf(out + os, "Mines left: " H_RED "%13d" RESET, mines));
    case 6:
        if (lost)
            return (os += sprintf(out + os, "You " BG_RED B_H_WHT " LOST " RESET "%15s", "Try again"));
        else
            return (os += sprintf(out + os, "You " BG_GRN B_H_YEL " WON " RESET "%16s", "Well done!"));
    case 8:
        return (os += sprintf(out + os, "" U_WHT "n" RESET "ew-game   " U_WHT "r" RESET "estart   " U_WHT "q" RESET "uit"));
    case 9:
        return 0;
    default:
        return 1;
    }
}
