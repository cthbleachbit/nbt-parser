#ifndef __libnbtp_parser
#define __libnbtp_parser

#include <stdint.h>

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
 * Read in next payload w/ fixed length
 *
 * header:	Type of the payload
 * dest:	Pointer to preallocated region write the payload
 * buffer:	input buffer
 * offset:	input offset
 */
void nextFixedLenPayload(tag_header header, void *dest, const char *buffer, int *offset);

/*
 * Read in next payload w/ variable length except for compound tags and list
 * tags.  Returns 0 on success and -1 on failure.
 *
 * header:	Type of the payload
 * size:	Pointer where the number of payload unit will be written
 * 		i.e. String length, array sizes, list sizes
 * dest:	Pointer to preallocated region write the payload
 * buffer:	input buffer
 * offset:	input offset
 */
int nextVarLenPayload(tag_header header, int32_t *size, void **dest,
		const char *buffer, int *offset);

/*
 * Parse next string length + string combo
 */
char* nextString(const char* buffer, int* offset);

#endif
