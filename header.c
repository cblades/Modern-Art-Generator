/*
 * header.c
 * Chris Blades
 * February 4, 2011
 *
 * Parses a PPM header and prints out version number, width, height, and maximum
 * value for color components.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include "header.h"

#define DEFAULT_STRING_SIZE 256
#define DELIMITERS          " \t\n\r\v\f"
#define NUM_PPM_NUMS        3
#define TRUE                1
#define FALSE               0

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
void readHeader(FILE *in, ppm_header *header) {
    // whether magic num has been compiled already or not, necessary because
    // magicPattern is static
    static int      regexCompiled               = FALSE;
    
    // compiled regex pattern to match version magic number
    static regex_t  magicPattern;
    
    int             numsFound                   = 0;  // number of nums found
    int             currNum;                          // current number found
    regmatch_t      matches;                          // match of magicPattern
    char           *token;                            // current token in line
    char            line[DEFAULT_STRING_SIZE];        // current line
    int             inComment;                        // whether current token 
                                                      // is in a comment
                                                        
    int             foundVersion;                     // if version num has been
                                                      // found already
    
    // if magic num regex hasn't already been compiled, compile it
    if (!regexCompiled) {
        regcomp(&magicPattern, "[pP][123456]", REG_EXTENDED);
    }

    // if input file couldn't be opened, return
    if (in == NULL) {
        return;
    }
    
    foundVersion = FALSE;

    // read from file, line by line, and parse each line for version, comments,
    // or numbers
    while (fgets(line, DEFAULT_STRING_SIZE, in) != NULL 
												&& numsFound < NUM_PPM_NUMS) {
        inComment = FALSE;
        token = strtok(line, DELIMITERS);
        if (token != NULL) {
            do {
                // if this token starts a comment, set flag...
                if (token[0] == '#') {
                    inComment = TRUE;
                }
                // only check for version or numbers if not in a comment
                if (!inComment) {
                    // check for version magic num
                    if (regexec(&magicPattern, token, 1, &matches, 0) == 0) {
                        // print version number...
                        if (!foundVersion) {
                            sscanf(token, "%*c%d", &header->version);
                            foundVersion = TRUE;
                        // ...unless version num has already been found, in 
                        // which case print error message and return
                        } else {
                            printf("\nHeader contains incorrrect formating: "
                                   "specifies version more than once.\n"
                                   "Program will quit parsing header.\n");
                            return;
                        }
                    
                    // check for number
                    } else if (sscanf(token, "%d", &currNum) > 0) {
                        if (numsFound < NUM_PPM_NUMS) {
                            switch (numsFound) {
                                case 0:
                                    header->width = currNum;
                                    break;
                                case 1:
                                    header->height = currNum;
                                    break;
                                case 2:
                                    header->maxValue = currNum;
									break;
                            }
                            numsFound++;
                        } else {
                            printf("\nHeader has extra data.\n");
                        }

                    // else, header is formatted incorrectly, print error       
                    } else {
                        printf("Header contains incorrect formatting "
                                "on line: \"%s\".\n"
                                "Program will quit parsing header.\n",
                                line);
                        return;
                    }
                }
            } while (numsFound < NUM_PPM_NUMS && 
								(token = strtok(NULL, DELIMITERS)) != NULL);
            
            if (numsFound >= NUM_PPM_NUMS) {
                break;
            }
        }
    }
}
