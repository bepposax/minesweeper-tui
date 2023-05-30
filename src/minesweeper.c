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

int goal, moves, uncovered_cells;
bool game_over;

/**
 * @brief checks if the number of flags and mines surrounding this cell match
 * @param row the row of the cell
 * @param col the column of the cell
 */
bool discoverable(int, int);

/**
 * @brief checks if the game is over
 * @param this the cell to check
 * @return true if the game is over
 *        false if the game is not over
 * @note the game is over if the cell contains a mine or if the number of uncovered cells
 *      is equal to the number of cells on the board minus the number of mines
 *     (i.e. the user has uncovered all the cells that do not contain a mine)
 */
bool is_game_over(cell *);

/**
 * @brief prints the results of the game
 * @param line the line to print
 * @return 0 if the line is the last one
 */
int print_results(int);

/**
 * @brief resets the game stats
 */
void reset_stats();

int game_loop()
{
    int ch, row, col;

    goal = height * width - mines;
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
                    else if (event.bstate & BUTTON3_CLICKED)
                        flag(row, col);
                }
            }
        }
        else if (ch == 'n' || ch == 'q')
        {
            reset_stats();
            return ch;
        }
        else if (ch == KEY_RESIZE)
            print_board();
    }
    print_board();
    do
    {
        if ((ch = getch()) == KEY_RESIZE)
            print_board();
        else if ((ch = tolower(ch)) == 'n')
            reset_stats();
    } while (ch != 'q' && ch != 'n');

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
    clear();
    refresh();

    // stats top
    printf(H_GRN);
    int len = printf(" ■ %d/%d", uncovered_cells, goal);
    for (int i = 0; i < width * 2 - len; i++)
        printf(" ");
    printf(B_H_RED "*" H_RED " %2d\n\r" RESET, mines);

    // border top
    printf("╭");
    for (int i = 0; i <= width * 2; i++)
        printf("─");
    printf("╮\n\r");

    // game board
    for (int i = 0; i < height; i++)
    {
        printf("│ ");
        for (int j = 0; j < width; j++)
        {
            cell *pos = &(board[i][j]);
            int num_mines = pos->surrounding_mines;

            if (pos->is_mine && pos->discovered && game_over)
                printf(RED "\b▐" BG_RED B_WHT "*" RESET RED "▌" RESET);
            else if (pos->discovered || game_over)
                if (pos->is_mine)
                    printf(B_RED "* " RESET);
                else if (num_mines)
                {
                    switch (num_mines)
                    {
                    case 1:
                        printf(H_BLU);
                        break;
                    case 2:
                        printf(H_GRN);
                        break;
                    case 3:
                        printf(H_YEL);
                        break;
                    case 4:
                        printf(H_MAG);
                        break;
                    default:
                        printf(H_CYN);
                        break;
                    }
                    printf("%d " RESET, num_mines);
                }
                else
                    printf(". " RESET);
            else
            {
                if (pos->is_flagged)
                    printf(RED);
                printf("■ " RESET);
            }
        }
        printf("│");
        if (game_over && width <= 16)
            print_results(i);
        printf("\n\r");
    }

    // border bottom
    printf("╰");
    for (int i = 0; i <= width * 2; i++)
        printf("─");
    printf("╯\n\r");

    // stats bottom
    printf(B_H_CYN " # " H_CYN "%d\n" RESET, moves);

    if (game_over && width > 16)
    {
        int i = 0;
        printf("\r\t");
        while (print_results(i++))
            printf("\n\r\t");
    }
    printf("\n");
    refresh();
}

int play(int row, int col)
{
    cell *pos = &(board[row][col]);

    if (pos->is_flagged)
        return 0;
    if (pos->discovered)
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
                            discover(i, j);
        }
        else
            return 0;
    }
    else
    {
        moves++;
        pos->discovered = true;
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
    if (!board[row][col].discovered)
    {
        bool *flag = &(board[row][col].is_flagged);
        (*flag = !*flag) ? mines-- : mines++;
        print_board();
    }
}

int discover(int row, int col)
{
    cell *this = &(board[row][col]);

    if (!this->discovered)
    {
        this->discovered = true;
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
                    if (!neighbor->discovered)
                    {
                        if (!neighbor->surrounding_mines && !neighbor->is_mine)
                            discover(i, j);
                        else if (!this->surrounding_mines)
                        {
                            neighbor->discovered = true;
                            uncovered_cells++;
                            if (is_game_over(neighbor))
                                return 1;
                        }
                    }
                }
    return 0;
}

bool discoverable(int row, int col)
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
                    if (!neighbor->discovered && !neighbor->is_flagged)
                        clear = false;
                }
    if (nmines == board[row][col].surrounding_mines && !clear)
        return true;
    return false;
}

bool is_game_over(cell *this)
{
    return (game_over = (this->is_mine || !(goal - uncovered_cells)));
}

int print_results(int line)
{
    switch (line)
    {
    case 0:
        return printf(B_H_WHT "\t------ Game Over ------" RESET);
    case 1:
        return printf("\tMoves: " H_CYN "%d" RESET, moves);
    case 2:
        return printf("\tUncovered cells: " H_GRN "%d/%d" RESET, uncovered_cells, goal);
    case 3:
        return printf("\tRemaining cells: " H_YEL "%d" RESET, goal - uncovered_cells);
    case 4:
        return printf("\tMines left: " H_RED "%d" RESET, mines);
    case 6:
        if (goal - uncovered_cells)
            return printf("\tYou " BG_RED B_H_WHT " LOST " RESET " - Try again");
        else
            return printf("\tYou " BG_GRN B_H_YEL " WON " RESET " - Well done!");
    case 8:
        return printf("\t" U_WHT "n" RESET "ew-game           " U_WHT "q" RESET "uit");
    case 9:
        return 0;
    default:
        return 1;
    }
}

void reset_stats()
{
    moves = 0;
    uncovered_cells = 0;
    game_over = false;
}
