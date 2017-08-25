/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // There can only be 1 or 2 command line arguments passed into file
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // Converts the 1st command line argument from string to integer.
    // Converts up to the first non integer it encounters or end of line.
    // Returns 0 otherwise. This is the first command line argument
    // Which is how many numbers we need to generate
    int n = atoi(argv[1]);

    // srand48 must be called as an initializer function before drand48
    // srand48 takes a seed value, either one provided to us by the user
    // or it creates a seed using the system time, seconds since a certain date
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    else
    {
        srand48((long) time(NULL));
    }

    // Creates a pseduo-random number for each number from 0 to 
    // number of random numbers we need to generate
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
