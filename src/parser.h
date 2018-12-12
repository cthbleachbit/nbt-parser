#ifndef __libnbtp_parser
#define __libnbtp_parser

/*
 * A wrapper funtion for detailed parsing
 *
 * buffer:	Entire file mapped in memory
 * length:	Expected length of the whole tag
 */
tag* parseFile(const char *buffer, const int length);

/*
 * Parse next tag
 *
 * buffer:	Entire tag mapped in memory
 * offset:	The offset where next tag starts
 *
 * Shall return NULL if it sees an TAG_END.
 */
tag* nextTag(const char *buffer, int *offset);

/*
 * Parse next string length + string combo
 */
char* nextString(const char* buffer, int* offset);

/*
 * Recursive print
 *
 * nbt:		The tag to print
 * f:		The FILE to output
 * indent:	Indentation of output
 */
void printTag(tag* nbt, FILE* f, int indent);

#endif
