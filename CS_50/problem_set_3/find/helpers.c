/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

int binary_search(int value, int values[], int high, int low);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    // size of array cannot be negative, return false
    if (n < 0)
        return false;
    
    // if binary search returns 0 (true), we found our value, return true. Otherwise, false.
    if (binary_search(value, values, n - 1, 0) == 0)
        return true;
    else
        return false;
}

// This is a Binary Search Algorithm, runs at O(log n) time
int binary_search(int value, int values[], int high, int low)
{
    // As long as we did not converge into one value in array, use recursion
    if (high >= low)
    {
        //if the middle number is our target value, return 0 for true.
        if (values[(high + low) / 2] == value)
            return 0;
        //if middle number of array is less than target value, stop looking at left half or array and increase middle number
        else if (values[(high + low) / 2] < value)
            return binary_search(value, values, high, ((high + low) / 2) + 1);
        //if middle number of array is greater than target value, stop looking at right half of array and decrease middle number
        else if (values[(high + low) / 2] > value)
            return binary_search(value, values, ((high+low) / 2) - 1 , low);
        //if none of the above, then we cannot find our target value in the array. Return 1 for false.
        else
            return 1;
    }
    // high is > low, cannot be possible, return 1 for false.
    else
        return 1;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // we will be using this value as placeholder when making swaps
    int temp = 0;
    
    // initial for loop for maximum amount of steps we have to go through the array
    for (int i = 0; i < n; i++)
    {
        // for each step we go through the array once
        for (int j = 0; j < n - 1; j++)
        {
            //any value that is greater than the value in front of it should be swapped
            if (values[j] > values[j + 1])
            {
                temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
            }
        }
    }
    //finished bubble sorting, return.
    return;
}
