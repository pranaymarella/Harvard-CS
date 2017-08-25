import cs50

def validate(number):
    # n_one is a string of the given number starting from second to last digit
    n_one = str(number)[-2::-2]
    # n_two is the leftover numbers. If even, must start from 1'st element in string. If odd, must start from 0'th element.
    if (len(str(number)) % 2 == 0):
        n_two = str(number)[1::2]
    else:
        n_two = str(number)[0::2]
    # keep track of n_one's sum
    sum_one = 0
    # keep track of n_two's sum
    sum_two = 0
    
    # multiply every other digit by two, and add those product's together
    for i in n_one:
        for j in str(int(i) * 2):
            sum_one += int(j)
    
    # get sum of the digits that weren't multiplied by 2
    for i in n_two:
        sum_two += int(i)
    
    # returns True if the total's last digit is 0 (total modulo 10 is equal to 0)
    if ((sum_one + sum_two) % 10 == 0):
        return True
    else:
        return False

def check_card(number):
    # possible first two digits of Amercian Express cards
    amex = ['34', '37']
    # possible first two digits of Master cards
    master = ['51', '52', '53', '54', '55']
    
    # get first two digits
    n = str(number)[:2]
    # get total length of number
    size = len(str(number))
    
    # only if the number given is valid (using Luhn's algorithm)
    if (validate(number)):
        # if number is of size 15 and has first two digits of AMEX card's
        if (n in amex and size == 15):
            print('AMEX')
        # else if n is of sizse 16 and has first two digits of Master card's
        elif (n in master and size == 16):
            print('MASTERCARD')
        # else if the first digit is a 4 and number is of size 13 or 16, it's a visa card
        elif (n[0] == '4' and (size == 13 or size == 16)):
            print('VISA')
        # if none of the above criteria meet, then card is not a valid card
        else:
            print('INVALID')
    # card number is invalid
    else:
        print('INVALID')

# MAIN FUNCTION
def main():
    # keep prompting user to give us a number, only contine if it is a number
    while True:
        print('Number: ', end='')
        number = cs50.get_int()
        if (number):
            break
    
    # check the number to see if it is valid and what type of card it is
    check_card(number)

if __name__ == "__main__":
    main()