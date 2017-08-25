#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

void find_initials(string s);

int main(void)
{
    // gets string input from user
    string s = get_string();
    
    //if there is a valid input, then trys to find initials
    if (s)
        find_initials(s);

    return (0);
}

void find_initials(string s)
{
    //n is for the index of s and count is to keep track of end of words
    int n = 0;
    int count = 0;

    while (s[n] != '\0')
    {
        //checks to see if the letter is the first one encountered within the word
        if ((toupper(s[n]) >= 'A' && toupper(s[n]) <='Z') && (count == 0))
        {
            printf("%c", toupper(s[n]));
            count++;
        }
        //resets count if word ended
        if (s[n] == ' ')
            count = 0;
        n++;
    }
    printf("\n");
}