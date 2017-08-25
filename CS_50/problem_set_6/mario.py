import cs50

def main():
    # make sure user gives a positive integer no greater than 23
    while True:
        print("Height: ", end='')
        h = cs50.get_int()
        if (h >= 0 and h <= 23):
            break
    
    # draws the pramid
    draw_pyramid(h)

# draws the pyramid based on the h (height) given
def draw_pyramid(h):
    # go through each step of pyramid
    for i in range(h):
        # print spaces for left pyramid
        for j in range(h - i - 1):
            print(' ', end='')
        
        # print hashes for left pyramid 
        for k in range(i + 1):
            print('#', end='')
        
        # gap between left and right pyramid
        print(' ', end='')
        
        # print hashes for right pyramid
        for l in range(i+1):
            print('#', end='')
        
        # this line is done, move onto next line
        print()

if __name__ == "__main__":
    main()