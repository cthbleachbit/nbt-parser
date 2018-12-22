#ifndef __libnbtp_parser
#define __libnbtp_parser

#include <stdint.h>

/*
 * A wrapper for a file to pass
 *
 * buffer:	Entire file mapped in memory
 * info:	Points to info for parsing this time
 */
typedef struct parse_info {
	char *buffer;
	int offset;
	int expect;
	int error;
} parse_info;

/*
 * Internal use only: Predict segfaults by checking offset against expect
 *
 * If reading `next` many would seek past end of file, return 1.
 * Otherwise return 0.
 */
int __predict_errors(parse_info *info, int next);

/*
 * A wrapper funtion for detailed parsing
 *
 * buffer:	Entire file mapped in memory
 * expect:	Expected length of the whole tag
 */
tag* nbtp_parse_file(char *buffer, const int expect);

/*
 * Parse next tag
 *
 * info:	Points to info for parsing this time
 *
 * Shall return NULL if it sees an TAG_END.
 */
tag* nbtp_next_tag(parse_info *info);

/*
 * Read in next payload w/ fixed length
 *
 * header:	Type of the payload
 * dest:	Pointer to preallocated region write the payload
 * info:	Points to info for parsing this time
 */
void __next_fixed_len_payload(tag_header header, void *dest, parse_info *info);

/*
 * Read in next payload w/ variable length except for compound tags and list
 * tags.  Returns 0 on success and -1 on failure.
 *
 * header:	Type of the payload
 * size:	Pointer where the number of payload unit will be written
 * 		i.e. String length, array sizes, list sizes
 * dest:	Pointer to preallocated region write the payload
 * info:	Points to info for parsing this time
 */
int __next_var_len_payload(tag_header header, int32_t *size, void **dest,
		parse_info *info);

/*
 * Parse next string length + string combo
 */
char* nbtp_next_string(parse_info *info);

#endif
