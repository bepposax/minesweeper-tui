/**
 * @file draw.c
 * @author Ivano Izzo
 */
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief writes an empty or full (of mines) board in `board.txt`
 * @param R the board's rows
 * @param C the board's columns
 * @param fill 0 if the board should be empty; filled with mines otherwise
 */
void draw(const int R, const int C, int fill)
{
    FILE *f;

    if (!(f = fopen("board.txt", "w")))
    {
        fprintf(stderr, "%s:%d: Can't open file\n", __FILE__, __LINE__ - 2);
        exit(EXIT_FAILURE);
    }
    for (int r = 0; r < R; ++r)
    {
        for (int c = 0; c < C; ++c)
            fwrite(fill ? "* " : ". ", 1, 2, f);
        fwrite("\n", 1, 1, f);
    }
    fclose(f);
}

int main(int argc, char **argv)
{
    int rows, cols, fill;

    if (argc >= 3)
    {
        rows = atoi(argv[1]);
        cols = atoi(argv[2]);
        if (argc >= 4)
            fill = atoi(argv[3]);
    }
    else
    {
        printf("rows: ");
        scanf("%d", &rows);
        printf("cols: ");
        scanf("%d", &cols);
        printf("fill: ");
        scanf("%d", &fill);
    }
    draw(rows, cols, fill);

    return 0;
}
