/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    // reads first argument and converts to integer. Else, returns 1 as error
    int scale = atoi(argv[1]);
    if (scale < 1 || scale > 100)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Original Width and Height of input file image
    // int original_width = bi.biWidth;
    int original_height = bi.biHeight;
    
    // Original padding of input file image
    int original_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // new width and height, scaled up to users input
    bi.biWidth *= scale;
    bi.biHeight *= scale;
    
    // new padding, needed to calculate new size of image and overall file size
    int output_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // New image size and total filze size
    bi.biSizeImage = ((bi.biWidth * sizeof(RGBTRIPLE)) + output_padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    

    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(original_height); i < biHeight; i++)
    {
        // allocate memory into an array of RGBTRIPLE's 
        RGBTRIPLE *row = malloc(sizeof(RGBTRIPLE) * (bi.biWidth + output_padding));
        
        // iterate over pixels
        for (int n = 0; n < bi.biWidth; n++)
        {
            // for each pixel, write to array n times
            for (int k  = 0; k < scale; k++)
            {
                fread(&row[n], sizeof(RGBTRIPLE), 1, inptr);
                fseek(inptr, -3, SEEK_CUR);
                n = n + 1;
            }
            fseek(inptr, 3, SEEK_CUR);
            n = n - 1;
        }
        
        // for n times
        for (int k = 0; k < scale; k++)
        {
            // write array to outfile
            for (int l = 0; l < bi.biWidth; l++)
                fwrite(&row[l], sizeof(RGBTRIPLE), 1, outptr);
            
            // write outfile padding
            for (int m = 0; m < output_padding; m++)
                fputc(0x00, outptr);
        }
        
        // skip over infile padding
        fseek(inptr, original_padding, SEEK_CUR);
        
        // you are done with this round of iteration, free up the space you allocated
        free(row);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
