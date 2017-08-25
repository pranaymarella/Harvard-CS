/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// blank tile location
int k;
int l;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void swap(int i, int j, int k, int l);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int count = (d*d) - 1;

    // TODO
    clear();
    
    // assigns values to board in row x column sequence
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = count;
            count--;
        }
    }
    
    // if board has an odd number of tiles, switches the tiles 1 and 2
    if (d % 2 == 0)
    {
        board[d - 1][d - 2] = 2;
        board[d - 1][d - 3] = 1;
    }
    
    //blank tile location
    k = d - 1;
    l = d - 1;
    
    usleep(20000);
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    // Navigates through the board in row x column sequence and prints the values
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == 0)
                printf(" __ ");
            else
                printf(" %2i ", board[i][j]);
        }
        printf("\n");
    }
    usleep(20000);
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    // Loops through the row x columns to find where tile is
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // Once the tile is found
            if (board[i][j] == tile)
            {
                // If tile is bordering the empty space on horizontal axis
                if (i - k == 0 && (j - l == 1 || j - l == -1))
                {
                    swap(i, j, k, l);
                    return true;
                }
                // if tile is bordering the empty space on the vertical axis
                else if (j - l == 0 && (i - k == 1 || i - k == -1))
                {
                    swap(i, j, k, l);
                    return true;
                }
                else
                    break;
            }
        }
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int count = 0;
    // TODO
    // go through the board
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // if we reach the end, the game is won
            if (i == d - 1 && j == d - 1)
                return true;
            // the number we are on now must be 1 greater than the number that came before
            else if ((board[i][j] - count) == 1)
                count++;
            // otherwise break the loop, game is not yet won
            else
                break;
        }
    }
    return false;
}

/**
 * Swaps the values of numbers if they are bordering the null space
 */
void swap(int i, int j, int m, int n)
{
    // temporary placeholder
    int temp = 0;
    
    // swap values
    temp = board[m][n];
    board[m][n] = board[i][j];
    board[i][j] = temp;
    
    // update the location of the blank
    k = i;
    l = j;
}
