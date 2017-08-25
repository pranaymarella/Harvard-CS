#include <cs50.h>
#include <stdio.h>

bool check_visa(long long numbers);
bool check_amex(long long numbers);
bool check_master_card(long long numbers);
bool valid_card(long long numbers);

int card_sum_one(long long numbers);
int card_sum_two(long long numbers);
int check_length(long long numbers);
int number_sum(int numbers);



int main(void)
{
    long long numbers;
    //   Gets User Input (Long Long because of credit card number) 
    do
    {
        printf("Number: ");
        numbers = get_long_long();
    }
    while (!numbers);
    
    // Makes Checks to return what type of card number the users input
    if (valid_card(numbers))
    {
        if (check_length(numbers) == 15 && check_amex(numbers))
            printf("AMEX\n");
        else if (check_length(numbers) == 16)
        {
            if (check_visa(numbers))
                printf("VISA\n");
            else if (check_master_card(numbers))
                printf("MASTERCARD\n");
            else
                printf("INVALID\n");
        }
        else if (check_length(numbers) == 13 && check_visa(numbers))
            printf("VISA\n");
        else
            printf("INVALID\n");
    }
    else
        printf("INVALID\n");
}

// Checks for Visa Cards
bool check_visa(long long numbers)
{
    if (numbers)
    {
        do
            numbers = numbers / 10;
        while (numbers > 10);
        
        if (numbers == 4)
            return true;
        else
            return false;
    }
    else
        return false;
}

// Checks for Master Cards
bool check_master_card(long long numbers)
{
    if (numbers)
    {
        do
            numbers = numbers / 10;
        while (numbers > 100);
        
        if (numbers == 51 || numbers == 52 || numbers == 53 || numbers == 54 || numbers == 55)
            return true;
        else
            return false;
    }
    else
        return false;
}

// Checks for American Express Cards
bool check_amex(long long numbers)
{
    if (numbers)
    {
        do
            numbers = numbers / 10;
        while (numbers > 100);
        
        if (numbers == 34 || numbers == 37)
            return true;
        else
            return false;
    }
    else
        return false;
}

// Checks for Length of Card
int check_length(long long numbers)
{
    int number;
    int count;
    
    number = 0;
    count = 0;
    
    if (numbers)
    {
        do
        {
            number = numbers % 10;
            numbers = numbers / 10;
            count++;
        }
        while (numbers > 0);
        
        return count;
    }
    else
        return 0;
}

// Multiplies every other digit in Credit Card Number and adds each individual digit ex. 1 + 8 + ... instead of 18 + ...
int card_sum_one(long long numbers)
{
    int number = 0;
    int sum = 0;
    
    if (numbers)
    {
        do
        {
            number = ((numbers % 100) / 10)*2;
            numbers = numbers / 100;
            sum += number_sum(number);
        }
        while (numbers > 0);
        
        return sum;
    }
    else
        return 0;
}

// Adds together all the leftover numbers from Credit Card Number
int card_sum_two(long long numbers)
{
    int number = 0;
    int sum = 0;
    
    if (numbers)
    {
        do
        {
            number = ((numbers % 100)%10);
            numbers = numbers / 100;
            sum += number;
        }
        while (numbers > 0);
        
        return sum;
    }
    else
        return 0;
}

// takes multi digit numbers and splits them and adds each individual digit together
int number_sum(int numbers)
{
    int number = 0;
    int sum = 0;
    
    if (numbers)
    {
        do
        {
            number = numbers % 10;
            numbers = numbers / 10;
            sum += number;
        }
        while (numbers > 0);
        
        return sum;
    }
    else
        return 0;
}

// Checks to see if the card number passed in can be a Credit Card Number
bool valid_card(long long numbers)
{
    if (numbers)
    {
        if ((card_sum_one(numbers) + card_sum_two(numbers))%10 == 0)
            return true;
        else
            return false;
    }
    else
        return false;
}