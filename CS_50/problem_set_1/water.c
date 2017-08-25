#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int minutes;

    printf("Minutes: ");
    do
        minutes = get_int();
    while (!minutes);
    printf("Bottles: %i\n", minutes * 12);
}
