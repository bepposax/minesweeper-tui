/**
 * @file campo_minato.c
 * @brief simulates a minesweeper game
 *
 * @author Ivano Izzo
 */
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "campo_minato.h"

#define GOAL (HEIGHT * WIDTH - NMINES)

int moves = 0, reclaimed = 0, mines_left = NMINES;
bool game_over;

void game_loop(cell **board)
{
    int ch, row, col;


    place_mines(board);
    print_board(board);
    while (!game_over)
    {
        if ((ch = getch()) == KEY_MOUSE)
        {
            MEVENT event;
            if (getmouse(&event) == OK)
            {
                row = event.y - 1;
                col = (event.x);

                if (col % 2 == 0)
                    col /= 2;
                else
                    continue;

                if (row >= 0 && row < HEIGHT && col >= 0 && col < WIDTH)
                {
                    if (event.bstate & BUTTON1_CLICKED)
                    {
                        play(row, col, board);
                        print_board(board);
                    }
                    else if (event.bstate & BUTTON3_CLICKED && !board[row][col].discovered)
                    {
                        bool *flag = &(board[row][col].is_flagged);
                        (*flag = !*flag) ? mines_left-- : mines_left++;
                        print_board(board);
                    }
                }
            }
        }
        else if (ch == 'q')
            return;
    }
    print_board(board);
    while ((ch = getchar()) != 'q')
        ;
}

/**
 * @brief places the mines randomly on the board
 * @param board the game board
 */
void place_mines(cell **board)
{
    int mine_row, mine_col, mines = NMINES;
    srand(time(NULL));

    while (mines)
    {
        mine_row = rand() % HEIGHT;
        mine_col = rand() % WIDTH;
        cell *pos = &(board[mine_row][mine_col]);

        if (!pos->is_mine)
        {
            pos->is_mine = true;
            signal_mine(board, mine_row, mine_col);
            mines--;
        }
    }
}

/**
 * @brief updates the number of surrounding mines of cells touching this mine
 * @param board the game board
 * @param row the mine's row
 * @param col the mine's column
 */
void signal_mine(cell **board, int row, int col)
{
    for (int i = row - 1; i <= row + 1; i++)
        if (i >= 0 && i < HEIGHT)
            for (int j = col - 1; j <= col + 1; j++)
                if (j >= 0 && j < WIDTH)
                {
                    cell *pos = &(board[i][j]);
                    if (!pos->is_mine)
                        (pos->surrounding_mines)++;
                }
}

/**
 * @brief prints the current state of the board
 * @param board the game board
 */
void print_board(cell **board)
{
    clear();
    refresh();

    // stats top
    printf(H_GRN);
    int len = printf("■ %d/%d", reclaimed, GOAL);
    for (int i = 0; i < WIDTH * 2 - len - 3; i++)
        printf(" ");
    printf(B_H_RED "*" H_RED " %2d\n\r" RESET, mines_left);

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            cell *pos = &(board[i][j]);
            int num_mines = pos->surrounding_mines;

            if (pos->is_mine && pos->discovered && game_over)
                printf(BG_RED B_WHT "\b * " RESET);
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
                    printf(". ");
            else
            {
                if (pos->is_flagged)
                    printf(RED);
                printf("■ " RESET);
            }
        }
        if (game_over)
            print_results(i);
        printf("\n\r");
    }

    // stats bottom
    printf(B_H_CYN "# " H_CYN "%d\n\r" RESET, moves);

    refresh();
}

/**
 * @brief discover a cell and checks if more can be discovered
 * @param row the row of the starting cell
 * @param col the column of the starting cell
 * @param board the game board
 * @return 1 if the game is over;
 *         0 if the game is not over
 */
int play(int row, int col, cell **board)
{
    cell *pos = &(board[row][col]);

    if (pos->is_flagged)
        return 0;

    if (pos->discovered)
    {
        if (!pos->surrounding_mines)
            return 0;
        if (discoverable(row, col, board))
        {
            for (int i = row - 1; i <= row + 1; i++)
                if (i >= 0 && i < HEIGHT)
                    for (int j = col - 1; j <= col + 1; j++)
                        if (j >= 0 && j < WIDTH && !board[i][j].is_flagged)
                            discover(i, j, board);
        }
        else
            return 0;
    }
    else
    {
        moves++;
        pos->discovered = true;
        reclaimed++;
        discover(row, col, board);
        if (pos->is_mine)
        {
            game_over = true;
            return 1;
        }
        if (!(GOAL - reclaimed))
        {
            game_over = true;
            return 1;
        }
    }
    return 0;
}

/**
 * @brief discovers all the possible cells from a starting cell
 * @param row the row of the starting cell
 * @param col the column of the starting cell
 * @param board the game board
 * @return 1 if a mine is reached; 0 otherwise
 */
int discover(int row, int col, cell **board)
{
    cell *this = &(board[row][col]);

    // this may happen if a flag is misplaced
    if (this->is_mine)
    {
        this->discovered = true;
        game_over = true;
        return 1;
    }
    if (!this->discovered)
    {
        this->discovered = true;
        reclaimed++;
    }
    for (int i = row - 1; i <= row + 1; i++)
        if (i >= 0 && i < HEIGHT)
            for (int j = col - 1; j <= col + 1; j++)
                if (j >= 0 && j < WIDTH)
                {
                    cell *neighbor = &(board[i][j]);
                    if (!neighbor->discovered)
                    {
                        if (!neighbor->surrounding_mines && !neighbor->is_mine)
                            discover(i, j, board);
                        else if (!this->surrounding_mines)
                        {
                            neighbor->discovered = true;
                            reclaimed++;
                        }
                    }
                }
    return 0;
}

/**
 * @brief checks if the number of surrounding flags matches the surrounding mines
 * @param row the row of the starting cell
 * @param col the column of the starting cell
 * @param board the game board
 */
bool discoverable(int row, int col, cell **board)
{
    int nmines = 0;
    bool clear = true;

    for (int i = row - 1; i <= row + 1; i++)
        if (i >= 0 && i < HEIGHT)
            for (int j = col - 1; j <= col + 1; j++)
                if (j >= 0 && j < WIDTH)
                {
                    cell *neighbor = &(board[i][j]);
                    if (neighbor->is_flagged)
                    {
                        if (neighbor->is_mine)
                            nmines++;
                        else
                            return true;
                    } // incorrect flag placement (leads to a loss)
                    if (!neighbor->discovered && !neighbor->is_flagged)
                        clear = false;
                }
    if (nmines == board[row][col].surrounding_mines && !clear)
        return true;
    return false;
}

/**
 * @brief prints the final state of the board after a game is over, with some final stats
 */
void print_results(int l)
{
    switch (l)
    {
    case 0:
        printf(B_H_WHT "\t----- Game Over -----" RESET);
        break;
    case 1:
        printf("\tMoves: " H_CYN "%d" RESET, moves);
        break;
    case 2:
        printf("\tCells reclaimed: " H_GRN "%d/%d" RESET, reclaimed, GOAL);
        break;
    case 3:
        printf("\tRemaining cells: " H_YEL "%d" RESET, GOAL - reclaimed);
        break;
    case 4:
        printf("\tMines left: " H_RED "%d" RESET, mines_left);
        break;
    case 5:
        break;
    case 6:
        if (GOAL - reclaimed)
            printf("\tYou " B_H_RED "LOST" RESET " - Try again");
        else
            printf("\tYou " B_H_GRN "WON" RESET " - Well done!");
        break;
    case HEIGHT - 1:
        printf("\t      q to exit");
        break;
    }
}
