#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // makes sure whodunit takes exactly 2 arguments
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile");
        return 1;
    }
    
    // storing filenames
    char *infile = argv[1];
    char *outfile = argv[2];
    
    // open infile, return 2 for error if file not found
    FILE *input_file = fopen(infile, "r");
    if (input_file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    // open outfile, return 3 for error if file not found
    FILE *output_file = fopen(outfile, "w");
    if (output_file == NULL)
    {
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }
    
    // read infile's BITMAPFILEHEADER. infile was opened as input_file
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, input_file);
    
    // read infile's BITMAPINFOHEADER, infile was opened as input_file
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, input_file);
    
    // make sure that infile, opened as input_file, is a 24-bit uncompressed BMP 4.0. Else, return 4 for error
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(input_file);
        fclose(output_file);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, output_file);
    
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, output_file);
    
    // check if we need to add padding
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // iterate over infile's lines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for (int j = 0; j < bi.biWidth; j++)
        {
                // temporary storage
            RGBTRIPLE triple;
            
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, input_file);
            
            if (triple.rgbtRed == 0xff)
            {
                triple.rgbtRed = 0x00;
            }
            
            fwrite(&triple, sizeof(RGBTRIPLE), 1, output_file);
        }
        
        // skip over padding, if there is any padding
        fseek(input_file, padding, SEEK_CUR);
        
        // add back the padding
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, output_file);
        }
    }
    
    // close infile
    fclose(input_file);
    
    // close outfile
    fclose(output_file);
    
    // sucess
    return (0);
}