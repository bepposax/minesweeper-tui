/**
 * @file minesweeper.c
 * @author Ivano Izzo
 */
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "minesweeper.h"
#include "string-builder.h"
#include "history.h"
#include "timer.h"

int goal, uncovered_cells, mines_left, moves;
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
static inline bool is_game_over(cell_t *this)
{
    return (game_over = ((lost = this->is_mine) || (goal - uncovered_cells == 0)));
}

#ifdef TEST
/**
 * @brief sets the board's state as specified in .testing/board.txt
 */
static void init_test_board();

/**
 * @brief places a mine on the board
 * @param row the board's row
 * @param col the board's column
 */
static void place_mine(int row, int col);
#endif

int game_loop()
{
    int ch = 0, row, col;

    timer_reset();
    do
    {
        goal = height * width - (mines_left = mines);
        uncovered_cells = 0;
        moves = 0;
        game_over = false;

        strfree();
        clear_history();
#ifdef TEST
        init_test_board();
#else
        if (ch != 'r')
            place_mines();
#endif
        print_board(false);
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
                        // cell pressed "animation"
                        if ((event.bstate & (BUTTON1_PRESSED | BUTTON2_PRESSED | BUTTON3_PRESSED)) &&
                            board[row][col].state == UNDISCOVERED)
                        {
                            mvprintw(event.y, event.x, "\u25FE");
                            if ((ch = tolower(getch())) == KEY_MOUSE)
                            {
                                MEVENT releasevent;
                                if (getmouse(&releasevent) == OK)
                                {
                                    if ((releasevent.y == event.y) && (releasevent.x == event.x))
                                    {
                                        if ((releasevent.bstate & BUTTON1_RELEASED) &&
                                            (event.bstate & BUTTON1_PRESSED))
                                            play(row, col);
                                        else if ((releasevent.bstate & BUTTON2_RELEASED) &&
                                                 (event.bstate & BUTTON2_PRESSED))
                                            mark(row, col);
                                        else if ((releasevent.bstate & BUTTON3_RELEASED) &&
                                                 (event.bstate & BUTTON3_PRESSED))
                                            flag(row, col);
                                    }
                                    else
                                    {
                                        print_board(true);
                                        continue;
                                    }
                                }
                            }
                            else
                            {
                                print_board(true);
                                continue;
                            }
                        }
                        else if (event.bstate & BUTTON1_CLICKED)
                            play(row, col);
                        else if (event.bstate & BUTTON2_CLICKED)
                            mark(row, col);
                        else if (event.bstate & BUTTON3_CLICKED)
                            flag(row, col);
                        if (!timer_running)
                            timer_start();
                    }
                }
            }
            if (ch == KEY_RESIZE)
                print_board(true);
            else if (ch == 'n' || ch == 'q')
            {
                strfree();
                remove_history();
                timer_stop();
                offset = 0;

                return ch;
            }
            else if (ch == 'r')
            {
                uncovered_cells = moves = 0;
                mines_left = mines;
                reset_board();
                strfree();
                clear_history();
                timer_stop();
                timer_reset();
#ifdef TEST
                init_test_board();
#endif
                print_board(false);
            }
        }
        // game over
        print_board(false);
        timer_stop();
        while ((ch = tolower(getch())) != 'q' && ch != 'n' && ch != 'r')
        {
            if (ch == 'h' && ((ch = show_history()) == 'q' || ch == 'n' || ch == 'r'))
                break;
            if (ch == KEY_RESIZE)
                print_board(true);
        }
        if (ch == 'r')
        {
            reset_board();
            strfree();
            clear_history();
            timer_stop();
            timer_reset();
        }
    } while (ch == 'r');
    timer_stop();
    strfree();
    remove_history();
    offset = 0;

    return ch;
}

static void place_mines()
{
    int mine_row, mine_col, mines_to_place = mines;
    cell_t *pos;

    srand(time(NULL));
    while (mines_to_place)
    {
        mine_row = rand() % height;
        mine_col = rand() % width;
        if (!(pos = &(board[mine_row][mine_col]))->is_mine)
        {
            pos->is_mine = true;
            signal_mine(mine_row, mine_col);
            mines_to_place--;
        }
    }
}

#ifdef TEST
static void place_mine(int row, int col)
{
    bool *is_mine = &(board[row][col].is_mine);

    if (!(*is_mine))
    {
        *is_mine = true;
        signal_mine(row, col);
        mines++;
    }
}

static void init_test_board()
{
    FILE *f;

    if (!(f = fopen(".testing/board.txt", "r")))
    {
        fprintf(stderr, "%s:%d: Error: Can't open file\n", __FILE__, __LINE__ - 2);
        exit(EXIT_FAILURE);
    }
    for (int row = 0; row < height; ++row)
        for (int col = 0; col < width; ++col)
            switch (fgetc(f))
            {
            case '*':
                place_mine(row, col);
                break;
            case '.':
                board[row][col].state = DISCOVERED;
                uncovered_cells++;
                break;
            case '#':
                board[row][col].state = UNDISCOVERED;
                break;
            case 'f':
                flag(row, col);
                break;
            case 'm':
                mark(row, col);
                break;
            case 'F':
                place_mine(row, col);
                flag(row, col);
                break;
            case 'M':
                place_mine(row, col);
                mark(row, col);
                break;
            case ' ':
            case '\n':
                col--;
                break;
            }
    fclose(f);
    goal = height * width - (mines_left = mines);
}
#endif

static void signal_mine(int row, int col)
{
    cell_t *pos;

    for (int i = row - 1; i <= row + 1; i++)
        if (i >= 0 && i < height)
            for (int j = col - 1; j <= col + 1; j++)
                if (j >= 0 && j < width)
                    if (!(pos = &(board[i][j]))->is_mine)
                        (pos->surrounding_mines)++;
}

static void flag(int row, int col)
{
    cell_state_t *state = &board[row][col].state;

    if (*state != DISCOVERED)
    {
        *state = *state == FLAGGED ? UNDISCOVERED : FLAGGED;
        *state == FLAGGED ? mines_left-- : mines_left++;
        print_board(false);
    }
}

static void mark(int row, int col)
{
    cell_state_t *state = &board[row][col].state;

    if (*state != DISCOVERED)
    {
        if (*state == FLAGGED)
            mines_left++;
        *state = *state == MARKED ? UNDISCOVERED : MARKED;
        print_board(false);
    }
}

static int play(int row, int col)
{
    cell_t *pos = &(board[row][col]);

    if (pos->state == FLAGGED || pos->state == MARKED)
        return 0;
    if (pos->state == DISCOVERED)
    {
        if (!pos->surrounding_mines)
            return 0;
        if (discoverable(row, col))
        {
            moves++;
            for (int i = row - 1; i <= row + 1; i++)
                if (i >= 0 && i < height)
                    for (int j = col - 1; j <= col + 1; j++)
                        if (j >= 0 && j < width && board[i][j].state != FLAGGED)
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
        pos->state = DISCOVERED;
        if (!pos->is_mine)
            uncovered_cells++;
        if (is_game_over(pos))
            return 1;
        discover(row, col);
    }
    print_board(false);
    return 0;
}

static int discover(int row, int col)
{
    cell_t *this = &(board[row][col]);
    cell_t *neighbor;

    if (this->state != DISCOVERED)
    {
        if (this->state == FLAGGED)
            mines_left++;
        this->state = DISCOVERED;
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
                    if ((neighbor = &(board[i][j]))->state != DISCOVERED)
                    {
                        if (!neighbor->surrounding_mines && !neighbor->is_mine)
                            discover(i, j);
                        else if (!this->surrounding_mines)
                        {
                            neighbor->state = DISCOVERED;
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
    cell_t *neighbor;

    for (int i = row - 1; i <= row + 1; i++)
        if (i >= 0 && i < height)
            for (int j = col - 1; j <= col + 1; j++)
                if (j >= 0 && j < width)
                {
                    if ((neighbor = &(board[i][j]))->state == FLAGGED)
                        flags++;
                    else if (!undiscovered_neighbors && neighbor->state != DISCOVERED)
                        undiscovered_neighbors = true;
                }
    return (flags == board[row][col].surrounding_mines) && undiscovered_neighbors;
}
