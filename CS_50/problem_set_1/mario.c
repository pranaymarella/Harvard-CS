#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    
    // ask user to give a height between 0 and 23
    // if user gives bad input, prompts them again
    do
    {
        printf("Height: ");
        height = get_int();
    }
    while (height < 0 || height > 23);
    
    for (int i = 0; i < height; i++)
    {
        // print spaces for left pyramid
        for (int j = 0; j < (height - i - 1); j++)
        {
            printf(" ");
        }
        // print hashes for left pyramid
        for (int k = i; k >= 0; k--)
        {
            printf("#");
        }
        //print gap
        printf("  ");
        // print hashes for right pyramid
        for (int l = i; l >= 0; l--)
        {
            printf("#");
        }
        // print new line
        printf("\n");
    }
}
