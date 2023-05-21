/**
 * @file difficulty.c
 * @author Ivano Izzo
 */
#include <stdio.h>
#include "../include/difficulty.h"
#include "../include/ANSI-colors.h"

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

void print_diff_menu()
{
    clear();
    refresh();

    printf("\n\t┏━━━━━━ DIFFICULTY ━━━━━━");
    printf("┓\n\b┃\r\t┃");
    printf("\n\b┃" H_GRN " ────── BEGINNER ────── " RESET);
    printf("┃\n\b┃\r\t┃");
    printf("\n\b┃" H_YEL " ──── INTERMEDIATE ──── " RESET);
    printf("┃\n\b┃\r\t┃");
    printf("\n\b┃" H_RED " ─────── EXPERT ─────── " RESET);
    printf("┃\n\b┃\r\t┃\n\b┗");
    for (int i = 23; i >= 0; i--)
        printf("━");
    printf("┛\n\r");

    refresh();
}