#ifndef HEADR_H
#define HEADER_H

// structre to containe the required parts of a ppm header
typedef struct ppm_header {
    int version;
    int width;
    int height;
    int maxValue;
} ppm_header; 

/**
 * Parses a PPM header and prints out version magic number and 3 numbers.  
 * PPM headers are in the format:
 *     <magic number>
 *     <whitespace>
 *     <number>
 *     <whitespace>
 *     <number>
 *     <whitespace>
 *     <number>
 * where <magic num> is in the format [pP][0-9].
 *
 * Comments are also allowed.  Comments begin with '#' and continue until the
 * end of the line.
 *
 * PARAMETERS
 *  in      - file pointer to header
 *  header  - struct to store header info in
 */
void readHeader(FILE *in, ppm_header *);
#endif
