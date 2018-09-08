#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define REPETITIONS 100
#define rows 22
#define cols 80

void rand_board(int board[rows][cols])
{
    int seed = time(0);
    srand(seed);
    for(int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < cols; ++k)
            board[i][k] = rand()%2;
    }
}

void temp_print(int board[rows][cols])
{
    for (int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < cols; ++k)
            printf(" %d", board[i][k]);
        printf("\n");
    }
    printf("\n");
}

void curses_print(int board[rows][cols])
{
    for (int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < cols; ++k)
        {
            wmove(stdscr, i+1 , k+1);
            //mvwaddch(stdscr,i+1, k+1, board[i][k]);
            if (board[i][k] == 1)
                addch('O');
                //printw("%d", board[i][k]);
            else
                addch(' ');
        }
    }
}

void copy_board(int board[rows][cols], int copy[rows][cols])
{
    for (int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < cols; ++k)
            copy[i][k] = board[i][k];
    }
}

int inbounds(int r, int c)
{
    if ( (r >= 0 && r <= rows-1) && (c >= 0 && c <= cols-1) )
        return 1;
    return -1;
}

int living_adjacent(int r, int c, int board[rows][cols])
{
    int count = 0;
    for (int i = -1; i <= 1; ++i)
    {
        for (int k = -1; k <= 1; ++k)
        {
            if ( inbounds(r+i, c+k)==1 && (i != 0 || k != 0) )
            {
                //printf("    adding: (%d, %d) = %d\n", i, k, board[r+i][c+k]);
                count += board[r+i][c+k];
            }
        }
    }
    return count;
}

void update_copy(int count, int i, int k, int copy[rows][cols])
{
    if (count < 2 || count >3)
        copy[i][k] = 0;
    else if (count == 3)
        copy[i][k] = 1;
}

void play_gol(int board[rows][cols])
{
    int copy[rows][cols];
    copy_board(board, copy);
    for (int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < cols; ++k)
        {
            //printf("(%d,%d)\n", i, k);
            int count = living_adjacent(i, k, board);
            update_copy(count, i, k, copy);
            //printf("        count: %d\n", count);
        }
    }
    copy_board(copy, board);
}

void make_blinker(int blink[rows][cols])
{
    for (int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < cols; ++k)
        {
            if (cols/(float)2.0f == k)
                blink[i][k] = 1;
            else
                blink[i][k] = 0;
        }
    }
}

int main(int argc, char **argv)
{
    initscr();
    start_color();
    init_pair(1,COLOR_WHITE, COLOR_MAGENTA);
    wbkgd(stdscr, COLOR_PAIR(1));
    printw("%s", "GAME OF LIFE:");
    int gol_board[rows][cols];
    rand_board(gol_board);
    //make_blinker(gol_board);
    for (int i = 0; i < REPETITIONS; ++i)
    {
        play_gol(gol_board);
        curses_print(gol_board);
        refresh();
        unsigned int usecs = 1000000;
        usleep(usecs);
    }
    getch();
    endwin();
    return 0;
}
