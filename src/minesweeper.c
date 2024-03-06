/**
 * @file minesweeper.c
 * @author Ivano Izzo
 */
#include "../include/minesweeper.h"
#include "../include/string_builder.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int goal, moves, uncovered_cells;
bool game_over, lost;

/**
 * @brief places the mines randomly on the board
 */
static void place_mines();

/**
 * @brief signals a mine to the cells surrounding it
 * @param row the mine's row
 * @param col the mine's column
 */
static void signal_mine(int row, int col);

/**
 * @brief plays a move in a cell
 * @param row the cell's row
 * @param col the cell's column
 * @return 1 if the game is over
 *         0 if the game is not over
 */
static int play(int row, int col);

/**
 * @brief flags or unflags an undiscovered cell
 * @param row the cell's row
 * @param col the cell's column
 */
static void flag(int row, int col);

/**
 * @brief marks or unmarks an undiscovered cell
 * @param row the cell's row
 * @param col the cell's column
 */
static void mark(int row, int col);

/**
 * @brief discovers the cells surrounding a cell
 * @param row the cell's row
 * @param col the cell's column
 * @return 1 if the game is over
 *         0 if the game is not over
 * @see discoverable
 */
static int discover(int row, int col);

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

static void place_mines()
{
    int mine_row, mine_col, mines_left = mines;
    cell *pos;

    srand(time(NULL));
    while (mines_left)
    {
        mine_row = rand() % height;
        mine_col = rand() % width;
        if (!(pos = &(board[mine_row][mine_col]))->is_mine)
        {
            pos->is_mine = true;
            signal_mine(mine_row, mine_col);
            mines_left--;
        }
    }
}

static void signal_mine(int row, int col)
{
    cell *pos;

    for (int i = row - 1; i <= row + 1; i++)
        if (i >= 0 && i < height)
            for (int j = col - 1; j <= col + 1; j++)
                if (j >= 0 && j < width)
                    if (!(pos = &(board[i][j]))->is_mine)
                        (pos->surrounding_mines)++;
}

static void flag(int row, int col)
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

static void mark(int row, int col)
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

static int play(int row, int col)
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

static int discover(int row, int col)
{
    cell *this = &(board[row][col]);
    cell *neighbor;

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
                    if (!(neighbor = &(board[i][j]))->is_discovered)
                    {
                        if (!neighbor->surrounding_mines && !neighbor->is_mine)
                            discover(i, j);
                        else if (!this->surrounding_mines)
                        {
                            neighbor->is_discovered = true;
                            neighbor->is_marked &= 0;
                            neighbor->is_flagged &= 0;
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
    bool undiscovered_neighbors = false;
    cell *neighbor;

    for (int i = row - 1; i <= row + 1; i++)
        if (i >= 0 && i < height)
            for (int j = col - 1; j <= col + 1; j++)
                if (j >= 0 && j < width)
                {
                    if ((neighbor = &(board[i][j]))->is_flagged)
                        flags++;
                    else if (!neighbor->is_discovered)
                        undiscovered_neighbors = true;
                }
    return flags == board[row][col].surrounding_mines && undiscovered_neighbors;
}

static bool is_game_over(cell *this)
{
    return (game_over = ((lost = this->is_mine) || !(goal - uncovered_cells)));
}
