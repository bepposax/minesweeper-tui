/**
 * @file minesweeper.c
 * @author Ivano Izzo
 */
#include "../include/minesweeper.h"
#include "../include/ANSI-colors.h"
#include "../include/string_builder.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

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
            {
                strfree();
                return ch;
            }
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
    strfree();

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
                            if (neighbor->is_marked)
                                neighbor->is_marked = false;
                            else if (neighbor->is_flagged)
                                neighbor->is_flagged = false;
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
    int flags = 0;

    for (int i = row - 1; i <= row + 1; i++)
        if (i >= 0 && i < height)
            for (int j = col - 1; j <= col + 1; j++)
                if (j >= 0 && j < width && i != j)
                    if (board[i][j].is_flagged)
                        flags++;
    return (flags == board[row][col].surrounding_mines) ? true : false;
}

static bool is_game_over(cell *this)
{
    return (game_over = ((lost = this->is_mine) || !(goal - uncovered_cells)));
}

void print_board()
{
    clear();
    refresh();

    // stats top
    strappend(H_GRN);
    int len = strappend(" ■ %d/%d", uncovered_cells, goal);
    for (int i = 0; i < width * 2 - len; i++)
        strappend(" ");
    strappend(B_H_RED "*" H_RED " %2d\n\r" RESET, mines);

    // border top
    strappend("╭");
    for (int i = 0; i <= width * 2; i++)
        strappend("─");
    strappend("╮\n\r");

    // game board
    for (int i = 0; i < height; i++)
    {
        strappend("│ ");
        for (int j = 0; j < width; j++)
        {
            cell *pos = &(board[i][j]);
            int num_mines;

            if (pos->is_mine && game_over)
            {
                if (pos->is_discovered)
                    strappend(RED "\b▐" BG_RED B_WHT "*" RESET RED "▌" RESET);
                else
                    strappend(B_RED "* " RESET);
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
                    strappend(H_BLK "· " RESET);
            else
            {
                if (pos->is_flagged)
                    strappend(RED "⚑ " RESET);
                else if (pos->is_marked)
                    strappend(H_YEL "? " RESET);
                else
                    strappend("■ ");
            }
        }
        strappend("│");

        // results right
        if (game_over && (width <= (getmaxx(stdscr) - 34) / 2 && height > 8))
        {
            strappend("    ");
            print_results(i);
        }
        strappend("\n\r");
    }

    // border bottom
    strappend("╰");
    for (int i = 0; i <= width * 2; i++)
        strappend("─");
    strappend("╯\n\r");

    // stats bottom
    strappend(B_H_CYN " # " H_CYN "%d\n" RESET, moves);

    // results bottom
    if (game_over && (width > (getmaxx(stdscr) - 34) / 2 || height <= 8))
    {
        int i = 0, indent = width - 11;

        do
        {
            strappend("\n\r");
            for (int j = 0; j < indent; j++)
                strappend(" ");
        } while (print_results(i++));
    }
    printf("%s\r", buffer);
    offset = 0;
}

int print_results(int line)
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
