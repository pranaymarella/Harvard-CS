/**
 * Recovers JPEG images from input file given, if any exist
 */

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCKSIZE 512

int main(int argc, char *argv[])
{
    // makes sure to accept only 1 command line argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // remembering filename
    char *raw_input = argv[1];
    
    FILE *inptr = fopen(raw_input, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", raw_input);
        return 2;
    }
    
    // keeps count of files found
    int count = 0;
    
    // BYTE is an 8 bit unsigned value, and buffer will contain no more than 512 bytes at a time
    typedef uint8_t  BYTE;
    BYTE buffer[BLOCKSIZE];
    
    // initialize an empty file
    FILE *output = NULL;
    
    // reading 1 block of 512 bytes long
    while (fread(buffer, 512, 1, inptr))
    {
        // check first 4 bytes of this block, to see if we reached a new image
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
            {
                // we found the beginning of a JPEG image

                // close previous file, if one is open
                if (output != NULL)
                    fclose(output);
                
                // char array to store the filename    
                char *filename = malloc(sizeof(char) * 4);
                
                // create new file
                sprintf(filename, "%03i.jpg", count);
                count++;

                // New image, therefore writing to new file
                output = fopen(filename, "w");
                
                // freeup allocated memory
                free(filename);
            }
        
        // if not beginning of new image, check to see if currently on image
        if (output != NULL)
            // Currently on image, keep writing to file from buffer
            fwrite(buffer, BLOCKSIZE, 1, output);
    }
    
    // Reached End of File, close final image file
    if (output != NULL)
        fclose(output);
    
    // Reached End of File, close raw input file
    fclose(inptr);
    
    return (0);
}
