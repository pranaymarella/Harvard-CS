#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void cipher(string plain, int k);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar k\n");
        return 1;
    }
    
    printf("plaintext: ");
    string plain = get_string();
    
    cipher(plain, atoi(argv[1]));
}

void cipher(string plain, int k)
{
    int i = 0;
    int key = k % 26;

    printf("ciphertext: ");
    while (plain[i] != '\0')
    {
        if (plain[i] >= 'A' && plain[i] <= 'Z')
        {
            if (plain[i] + key <= 'A' || plain[i] + key >= 'Z')
                printf("%c", ((key % 26) - (90 - plain[i]) - 1) + 65);
            else
                printf("%c", plain[i] + key);
        }
        else if (plain[i] >= 'a' && plain[i] <= 'z')
        {
            if (plain[i] + key <= 'a' || plain[i] + key >= 'z')
                printf("%c", ((key % 26) - (122 - plain[i]) - 1) + 97);
            else
                printf("%c", plain[i] + key);
        }
        else
            printf("%c", plain[i]);
        i++;
    }
    printf("\n");
}