/**
 * @file campo_minato.c
 * @brief simulates a minesweeper game
 *
 * @author Ivano Izzo
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "lib/ANSI-colors.h"

#define NMINES 35
#define HEIGHT 15
#define WIDTH 15
#define GOAL (HEIGHT * WIDTH - NMINES)

int moves = 1, reclaimed = 0, mines_left = NMINES;
bool game_over;

/**
 * @brief represents a single cell of the board
 * @param is_mine true if the cell contains a mine
 * @param surrounding_mines counts mines surrounding the cell
 * @param discovered true if the cell has been visited
 */
typedef struct cell
{
    bool is_mine;
    int surrounding_mines;
    bool discovered;
    bool is_flagged;
} cell;

void place_mines(cell **);
void signal_mine(cell **, int, int);
void print_board(cell **);
int play(int, int, cell **);
int discover(int, int, cell **);
bool discoverable(int, int, cell **);
void print_results();

int main(void)
{
    cell **board = (cell **)malloc(HEIGHT * sizeof(cell));
    if (!board)
        exit(EXIT_FAILURE);
    for (int i = 0; i < HEIGHT; i++)
    {
        board[i] = (cell *)malloc(WIDTH * sizeof(cell));
        if (!board[i])
            exit(EXIT_FAILURE);
    }

    place_mines(board);

    char x;
    int row, col;

    // Game Loop
    while (!game_over)
    {
        do
        {
            system("clear");
            print_board(board);
            printf("   Move " H_CYN "%d" RESET " -> ", moves);
            if (!scanf(" %d", &row))
                row = -1;
            else
            {
                scanf(" %c", &x);
                col = tolower(x) - 'a';
            }
            while (getchar() != '\n')
                ;
        } while (row < 0 || row >= HEIGHT || col < 0 || col >= WIDTH);

        if (isupper(x))
        {
            bool *flag = &(board[row][col].is_flagged);
            (*flag = !*flag) ? mines_left-- : mines_left++;
        }
        else
            play(row, col, board);
    }

    system("clear");
    print_board(board);
    print_results();

    for (int i = 0; i < HEIGHT; i++)
        free(board[i]);
    free(board);

    exit(EXIT_SUCCESS);
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
    char *rules[] = {
        "\tHOW TO PLAY",
        "",
        "Insert the coordinates <row> <col>",
        "- to play: [0-9][a-z]",
        "- to flag: [0-9][A-Z]",
        "- space is optional",
        "- you can play in a cell again",
        "  if all its mines are flagged"};

    // stats top
    printf(H_GRN);
    int len = printf("   ■ %d/%d", reclaimed, GOAL);
    for (int i = 0; i < WIDTH * 2 - len; i++)
        printf(" ");
    printf(B_H_RED "*" H_RED " %2d\n" RESET, mines_left);

    // letters top
    printf("   ");
    for (int j = 0; j < WIDTH; j++)
        printf(BLK "%c " RESET, j + 'A');
    puts("");

    for (int i = 0; i < HEIGHT; i++)
    {
        // numbers left
        printf(BLK "%2d " RESET, i);

        for (int j = 0; j < WIDTH; j++)
        {
            cell *pos = &(board[i][j]);
            int num_mines = pos->surrounding_mines;

            if (pos->is_mine && pos->discovered && game_over)
                printf(BG_RED B_WHT "\b %c " RESET, '*');
            else if (pos->discovered || game_over)
                if (pos->is_mine)
                    printf(B_RED "%c " RESET, '*');
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
                    printf(BLK ". " RESET);
            else
            {
                if (pos->is_flagged)
                    printf(RED);
                printf("%s" RESET " ", "■");
            }
        }
        // numbers right
        printf(BLK "%d " RESET, i);
        // instructions
        if (i < sizeof(rules) / sizeof(rules[0]))
            printf(BLK "\t%s" RESET, rules[i]);
        puts("");
    }
    // letters bottom
    printf("   ");
    for (int j = 0; j < WIDTH; j++)
        printf(BLK "%c " RESET, j + 'A');
    printf("\n\n");
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

    pos->discovered = true;

    if (pos->is_mine)
    {
        game_over = true;
        return 1;
    }

    reclaimed++;
    discover(row, col, board);

    int remaining_cells = GOAL - reclaimed;

    if (!remaining_cells)
    {
        game_over = true;
        return 1;
    }
    moves++;
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
                        if (neighbor->is_mine)
                            nmines++;
                        else
                            return true; // incorrect flag placement (leads to a loss)
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
void print_results()
{
    int remaining_cells = GOAL - reclaimed;

    printf(B_H_WHT "   -- Game Over --\n" RESET);
    printf("   Moves: " H_CYN "%d" RESET "\n", moves);
    printf("   Cells reclaimed: " GRN "%d/%d" RESET "\n", reclaimed, GOAL);
    printf("   Remaining cells: " YEL "%d" RESET "\n", remaining_cells);
    printf("   Mines left: " RED "%d" RESET "\n\n", mines_left);
    if (remaining_cells)
        printf("   You " U_RED "LOST" RESET " - Try again\n\n");
    else
        printf("   You " U_GRN "WON" RESET " - Well done!\n\n");
}