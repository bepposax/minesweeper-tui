/**
 * @file minesweeper.c
 *
 * @author Ivano Izzo
 */
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "../include/minesweeper.h"

int HEIGHT, WIDTH, NMINES, GOAL;
int moves = 0, uncovered = 0, mines_left;
bool game_over;

int select_diff();
void print_diff_menu();
int discover(int, int, cell **);
bool discoverable(int, int, cell **);
bool is_game_over(cell *);
void print_results(int);

/**
 * @brief initial setup
 */
void init()
{
    switch (select_diff())
    {
    case 1:
        HEIGHT = 9;
        WIDTH = 9;
        NMINES = 10;
        break;
    case 2:
        HEIGHT = 16;
        WIDTH = 16;
        NMINES = 40;
        break;
    case 3:
        HEIGHT = 16;
        WIDTH = 30;
        NMINES = 99;
        break;
    default:
        return;
    }

    mines_left = NMINES;
    GOAL = (HEIGHT * WIDTH - NMINES);

    cell **board = (cell **)malloc(HEIGHT * sizeof(cell));
    if (!board)
        exit(EXIT_FAILURE);
    for (int i = 0; i < HEIGHT; i++)
    {
        board[i] = (cell *)malloc(WIDTH * sizeof(cell));
        if (!board[i])
            exit(EXIT_FAILURE);
    }

    game_loop(board);

    for (int i = 0; i < HEIGHT; i++)
        free(board[i]);
    free(board);
}

/**
 * @brief asks the user to select the game difficulty
 */
int select_diff()
{
    int ch;

    print_diff_menu();
    while ((ch = getch()))
    {
        if (ch == KEY_MOUSE)
        {
            MEVENT event;
            if (getmouse(&event) == OK && (event.bstate & BUTTON1_CLICKED))
                switch (event.y)
                {
                case 3:
                    return 1;
                case 5:
                    return 2;
                case 7:
                    return 3;
                default:
                    break;
                }
        }
        else if (ch == 'q')
            return 0;
        else if (ch == KEY_RESIZE)
            print_diff_menu();
    }
    return -1;
}

/**
 * @brief prints the difficulties to choose from
 */
void print_diff_menu()
{
    clear();
    refresh();

    printf("\n\t▆■■■■■■ DIFFICULTY ■■■■■■");
    printf("▆\n\b█\r\t█");
    printf("\n\b█" H_GRN " ■■■■■■ BEGINNER ■■■■■■ " RESET);
    printf("█\n\b█\r\t█");
    printf("\n\b█" H_YEL " ■■■■ INTERMEDIATE ■■■■ " RESET);
    printf("█\n\b█\r\t█");
    printf("\n\b█" H_RED " ■■■■■■■ EXPERT ■■■■■■■ " RESET);
    printf("█\n\b█\r\t█\n\b█");
    for (int i = 23; i >= 0; i--)
        printf("■");
    printf("█\n\r");

    refresh();
}

/**
 * @brief the game loop - it ends when the game is over or the user quits
 */
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
        else if (ch == KEY_RESIZE)
            print_board(board);
    }
    print_board(board);
    while ((ch = getch()) != 'q')
        if (ch == KEY_RESIZE)
            print_board(board);
}

/**
 * @brief places the mines randomly on the board
 *
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
 *
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
 *
 * @param board the game board
 */
void print_board(cell **board)
{
    clear();
    refresh();

    // stats top
    printf(H_GRN);
    int len = printf("■ %d/%d", uncovered, GOAL);
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
 *
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
            moves++;
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
        uncovered++;
        if (is_game_over(pos))
            return 1;
        discover(row, col, board);
    }
    return 0;
}

/**
 * @brief discovers all the possible cells from a starting cell
 *
 * @param row the row of the starting cell
 * @param col the column of the starting cell
 * @param board the game board
 * @return 1 if a mine is reached; 0 otherwise
 */
int discover(int row, int col, cell **board)
{
    cell *this = &(board[row][col]);

    if (!this->discovered)
    {
        this->discovered = true;
        uncovered++;
        if (is_game_over(this))
            return 1;
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
                            uncovered++;
                            if (is_game_over(neighbor))
                                return 1;
                        }
                    }
                }
    return 0;
}

/**
 * @brief checks if the number of surrounding flags matches the surrounding mines
 *
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
 * @brief checks if the game is over
 *
 * @param this a cell of the game board
 * @return true if the cell contains a mine or if all the cells have been uncovered
 */
bool is_game_over(cell *this)
{
    return (game_over = (this->is_mine || !(GOAL - uncovered)));
}

/**
 * @brief prints the final state of the board after a game is over, with some final stats
 *
 * @param line the line to print
 */
void print_results(int line)
{
    switch (line)
    {
    case 0:
        printf(B_H_WHT "\t----- Game Over -----" RESET);
        break;
    case 1:
        printf("\tMoves: " H_CYN "%d" RESET, moves);
        break;
    case 2:
        printf("\tCells uncovered: " H_GRN "%d/%d" RESET, uncovered, GOAL);
        break;
    case 3:
        printf("\tRemaining cells: " H_YEL "%d" RESET, GOAL - uncovered);
        break;
    case 4:
        printf("\tMines left: " H_RED "%d" RESET, mines_left);
        break;
    case 5:
        break;
    case 6:
        if (GOAL - uncovered)
            printf("\tYou " B_H_RED "LOST" RESET " - Try again");
        else
            printf("\tYou " B_H_GRN "WON" RESET " - Well done!");
        break;
    case 15:
        printf("\t      q to exit");
        break;
    }
}
