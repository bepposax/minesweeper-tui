#include <stdio.h>
#include <stdlib.h>

void clear_board(const int R, const int C)
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
            fwrite(". ", 1, 2, f);
        fwrite("\n", 1, 1, f);
    }
    fclose(f);
}

int main(int argc, char **argv)
{
    int rows, cols;

    if (argc == 3)
    {
        rows = atoi(argv[1]);
        cols = atoi(argv[2]);
    }
    else
    {
        printf("rows: ");
        scanf("%d", &rows);
        printf("cols: ");
        scanf("%d", &cols);
    }
    clear_board(rows, cols);

    return 0;
}
