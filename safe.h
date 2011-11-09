/**
 * Defines wrappers for several common functions that perform error checking.
 *
 * Chris Blades
 * Februrary 10,, 2011
 */
#ifndef SAFE_H
#define SAFE_H
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
void *smalloc(size_t);

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
FILE *fopenAndCheck(char *, char *);
#endif
