#ifndef __libnbtp_parser
#define __libnbtp_parser

#include <stdint.h>

/*
 * A wrapper for a file to pass
 *
 * buffer:	Entire file mapped in memory
 * offset:	The place to record parse progress
 * expect:	Expected length of the whole tag
 */
typedef struct parse_info {
	char *buffer;
	int offset;
	int expect;
} parse_info;

/*
 * A wrapper funtion for detailed parsing
 *
 * buffer:	Entire file mapped in memory
 * expect:	Expected length of the whole tag
 */
tag* parseFile(char *buffer, const int expect);

/*
 * Parse next tag
 *
 * buffer:	Entire tag mapped in memory
 * offset:	The offset where next tag starts
 * expect:	Expected length of the whole file
 *
 * Shall return NULL if it sees an TAG_END.
 */
tag* nextTag(parse_info *info);

/*
 * Read in next payload w/ fixed length
 *
 * header:	Type of the payload
 * dest:	Pointer to preallocated region write the payload
 * buffer:	input buffer
 * offset:	input offset
 * expect:	Expected length of the whole file
 */
void nextFixedLenPayload(tag_header header, void *dest, parse_info *info);

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
 * expect:	Expected length of the whole file
 */
int nextVarLenPayload(tag_header header, int32_t *size, void **dest,
		parse_info *info);

/*
 * Parse next string length + string combo
 */
char* nextString(parse_info *info);

#endif
