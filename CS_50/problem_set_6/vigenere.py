import cs50
import sys

# MAIN FUNCTION
def main():
    if (len(sys.argv) != 2 or not (sys.argv[1].isalpha())):
        print("Usage: python vigenere.py key")
        exit(1)
        
    # store key
    key = sys.argv[1].upper()
    
    # store user plaintext
    while True:
        print("plaintext: ", end='')
        plaintext = cs50.get_string()
        if (plaintext):
            break
        
    # Ouput. Vigenere function handles the conversion of userinput into cipher output
    print('ciphertext: ', end='')
    vigenere(plaintext, key)

# Converts user input and key into cipher output
def vigenere(plaintext, key):
    # meant to keep track of key's position
    c = 0
    n = len(key)
    
    # go through every element in user input
    for i in plaintext:
        # reset key position if it reached the end
        if (c == n):
            c = 0
        
        # how much to shift by
        shift = ord(key[c]) - ord('A')
        # if current letter is an alphabet
        if (i.isalpha()):
            # perform the shit and increment to next postion in key
            tmp = chr(ord(i) + shift)
            c += 1
            
            # if the shifted postion is an alphabet, simply print it
            if (tmp.isalpha()):
                print(tmp, end='')
            # if shifted position is not an alphabet, wrap around
            else:
                print(chr(ord(tmp) - 26), end='')
        # if current element is not an alphabet, simply print it
        else:
            print(i, end='')
    # end of user input, everything has been shifted
    print()

if __name__ == '__main__':
    main()