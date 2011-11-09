/**
 * Defines wrappers for several common functions that perform error checking.
 *
 * Chris Blades
 * Februrary 10,, 2011
 */
#include <stdlib.h>
#include <stdio.h>

/**
 * Simple wrapper around malloc that prints and error message and exits if
 * malloc fails.
 *
 * PARAMETERS:
 *  size - the size of the memory segment to allocate
 *
 * RETURNS:
 * a pointer to the allocated memory
 */
void *smalloc(size_t size) {
    void *temp = NULL;
    temp = malloc(size);
    if (temp == NULL) {
        printf("Error allocating memory.\n");
        exit(1);
    }
    return temp;
}

/**
 * Opens a file and if there was a problem prints a message to stderr.
 *
 * PARAMETERS
 *  file - filename to open
 *  mode - how to open the file, eg rw, r, w, etc
 *
 * RETURN
 *  a file pointer to the opened file, or a file pointer to NULL if there was
 *  an error
 */
FILE *fopenAndCheck(char *file, char *mode) {
    FILE *fpointer;         // file pointer to opened file

    fpointer = fopen(file, mode);
    if (NULL == fpointer) {
        // add filename to error message, subtract from DEFAULT_STRING_SIZE
        // to account for chars already in errMsg
        perror("Error opening file.\n");
    }

    return fpointer;
}
