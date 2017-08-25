#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool check_key(string k);
void cipher(string plain, string k);

int main(int argc, string argv[])
{
    //makes sure only one arugment is given, and that argument is a string of letters
    if (argc != 2 || (check_key(argv[1]) == false))
    {
        printf("Usage: ./vigenere k\n");
        return (1);
    }
    
    printf("plaintext: ");
    string plain = get_string();
    
    //Takes plaintext (user input) and the key passed in as parameter and does cipher
    cipher(plain, argv[1]);
    return (0);
}

bool check_key(string k)
{
    //checks every element within key string array to make sure it is an alphabet
    for (int i = 0, n = strlen(k); i < n; i++)
    {
        if (!(toupper(k[i]) >= 'A' && toupper(k[i]) <= 'Z'))
            return false;
    }
    return true;
}

void cipher(string p, string k)
{
    //initializing counting variables to be used within loop
    int i = 0;
    int j = 0;
    int n = strlen(k);
    
    printf("ciphertext: ");
    while (p[i] != '\0')
    {
        //if key's jth element reaches its length, it will wrap around/reset 
        if (j == n)
            j = 0;
        //if plaintext (p)'s i'th element is an upper case alphabet
        if (p[i] >= 'A' && p[i] <= 'Z')
        {
            //if addtion overflows, wraparound. Else, simply add value of key
            if (p[i] + (toupper(k[j]) - 65) < 'A' ||
                p[i] + (toupper(k[j]) - 65) > 'Z')
                printf("%c", (p[i] + (toupper(k[j]) - 65)) - 26);
            else
                printf("%c", p[i] + (toupper(k[j]) - 65));
            j++;
        }
        else if (p[i] >= 'a' && p[i] <= 'z')
        {
            //if addtion overflows, wraparound. Else, simply add value of key
            if (p[i] + (tolower(k[j]) - 97) < 'a' ||
                p[i] + (tolower(k[j]) - 97) > 'z')
                printf("%c", (p[i] + (tolower(k[j]) - 97)) - 26);
            else
                printf("%c", p[i] + (tolower(k[j]) - 97));
            j++;
        }
        //if not an alphabet, then print the character as is
        else
            printf("%c", p[i]);
        //increments i to keep loop going and avoid infinite looping
        i++;
    }
    printf("\n");
}
