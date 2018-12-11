#include "tag.h"
#include "parser.h"

/*
 * A wrapper funtion for detailed parsing
 *
 * buffer:	Entire file mapped in memory
 * length:	Expected length of the whole tag
 */
tag* parseFile(char *buffer, int length);

/*
 * Parse next tag
 *
 * buffer:	Entire tag mapped in memory
 * offset:	The offset where next tag starts
 * length:	Expected length of the whole tag
 */
tag* nextTag(char *buffer, int* offset, int length);

/*
 * Recursive print
 *
 * nbt:		The tag to print
 * f:		The FILE to output
 */
void printTag(tag* nbt, FILE* f);
