/*
 * Takes a ppm header as input and outputs the same imaged scaled into four 
 * smaller images with different color filters for each subimage.
 *
 * USAGE:
 *  a.out <in.ppm >out.ppm
 *
 * Chris Blades
 * 11 March, 2011
 */
#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "safe.h"


#define TRUE  1
#define FALSE 0
#define RED   0
#define GREEN 1
#define BLUE  2
#define NONE 3
#define PPM_COLOR_VERSION 6
#define COLOR_MAGIC_NUM "P6"
#define HEADER_ERROR 1
#define VERSION_ERROR 2
#define CORRUPT_ERROR 4


void scale(unsigned char *, unsigned char *, int, int, int, int, int);

/**
 * Reads in header data and picture data from standard in and calls functions
 * to scale and perform color filters.
 *
 * PARAMETERS:
 *  argc - the number of command line arguments, including the command
 *  argv - array of command line arguments, including the command
 *
 *  RETURN:
 *   Success if conversion was performed successfully, otherwise for usage, 
 *   corruption, or invalid format errors.
 */
int main(int argc, char **argv) {
    int            size       = 0;
    unsigned char *in         = NULL;
    unsigned char *out        = NULL;
    ppm_header    *header     = NULL;
    
    header = (ppm_header *)malloc(sizeof(ppm_header));
    readHeader(stdin, header);
   
    if (header == NULL) {
        fprintf(stderr, "Invalid header, exiting..\n");
        exit(HEADER_ERROR);
    } else if (header->version != PPM_COLOR_VERSION) {
        fprintf(stderr, "Header version must be P6, exiting.\n");
        exit(VERSION_ERROR);
    }
    size = header->width * header->height;
    
    in = (unsigned char *)smalloc(3 * size);

    int numRead = fread(in, sizeof(unsigned char), size * 3, stdin);

    if (numRead != size * 3) {
        fprintf(stderr, "Corrupt input file, quiting.\n");
        exit(CORRUPT_ERROR);
    }

    out = (unsigned char *)smalloc(size * 3);

    // call scale for each quadrant and adjust out accordingly

    // quadrant 1 (top, left)
    scale(in, out, header->width, header->height, FALSE, FALSE, NONE);

    // quadrant 2 (top, right)
    scale(in, out + (3 * (header->width / 2)), 
          header->width, header->height, TRUE, FALSE, RED);

    // quadrant 3 (bottom, left)
    scale(in, out + (3 * (header->width * (header->height / 2))), 
          header->width, header->height, FALSE, TRUE, GREEN);

    // quadrant 4 (bottom, right)
    scale(in, 
     out + (3 * ((header->width / 2) + (header->width * (header->height / 2)))),
     header->width, header->height, TRUE, TRUE, BLUE);

    fprintf(stdout, "%s %d %d %d\n", COLOR_MAGIC_NUM, header->width,        
                                     header->height, header->maxValue);
    fwrite(out, 1, size * 3, stdout);

    free(in);
    free(out);
    free(header);
    return EXIT_SUCCESS;
}

/**
 * Scales a picture and writes it to memory.  Also does color filtering.
 *
 * PARAMETERS:
 *  source        - memory containing picture data in ppm P6 format
 *  dest          - where to write scaled picture to
 *  width         - the width of source
 *  height        - the height of source
 *  overflowRight - if the width doesn't evenly divide, add extra pixel to dest
 *  overflowLeft  - if the height doesn't evenly divide, add extra pixel to dest
 *  conversion    - what color filter should be applied
 */
void scale(unsigned char *source, unsigned char*dest,int width, int height, 
                        int overflowRight, int overflowBottom, int conversion) {
    int sWidth = 0;         // scaled width
    int sHeight = 0;        // scaled height
    unsigned char *pixelSrc;
    unsigned char *pixelDst;
    int i;
    int j;
    
    sWidth = width / 2;

    // add extra pixel if overflow is desired and needed
    sWidth = (width % 2 != 0 && overflowRight) ? sWidth + 1 : sWidth;

    sHeight = height / 2;
    
    // add extra pixel if overflow is desired and needed
    sHeight = (height % 2 != 0 && overflowBottom) ? sHeight + 1 : sHeight;

    //
    // traverse source and copy every other pixel to dest.  Both source and
    // dest are in row major order.
    //
    for (i = 0; i < sHeight; i++) {
        for (j = 0; j < sWidth; j++) {
            // ppm pixels are stored in row major order, so find offset with
            // offset = row * numCols + column
            // and multiply offset by 3 b/c of pixels
            pixelSrc = source + (((i * 2) * width + (j * 2)) * 3);
            pixelDst = dest + (((i * width) + j) * 3);
            
            // do color filter and write to dest
            switch(conversion) {
                case NONE:
                    *pixelDst = *pixelSrc;
                    *(pixelDst + 1) = *(pixelSrc + 1);
                    *(pixelDst + 2) = *(pixelSrc + 2);
                    break;
                case RED:
                    *pixelDst = *pixelSrc;
                    *(pixelDst + 1) = 0;
                    *(pixelDst + 2) = 0;
                    break;
                case GREEN:
                    *pixelDst = 0;
                    *(pixelDst + 1) = *(pixelSrc + 1);
                    *(pixelDst + 2) = 0;
                    break;
                case BLUE:
                    *pixelDst = 0;
                    *(pixelDst + 1) = 0;
                    *(pixelDst + 2) = *(pixelSrc + 2);
                    break;
            }
                    
        }
    }
}
