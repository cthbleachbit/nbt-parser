#include "tag.h"
#include "compound.h"
#include "parser.h"
#include "util.h"

#include <endian.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/*
 * A wrapper funtion for detailed parsing
 *
 * buffer:	Entire file mapped in memory
 * expect:	Expected length of the whole tag
 */
tag* nbtp_parse_file(char *buffer, const int expect) {
	parse_info *info = __nbtp_malloc(sizeof(parse_info));
	info -> buffer = buffer;
	info -> offset = 0;
	info -> expect = expect;
	// Should only have one root tag
	tag* result = nbtp_next_tag(info);
	if (info -> offset != expect) {
		fprintf(stderr, "Wrong tag size? Expected %i, got %i.\n",
				expect, info -> offset);
	}
	free(info);
	return result;
}

/*
 * Predict segfaults by checking offset against expect
 *
 * If reading `next` many would cause seek past end of file,
 *     set the error flag and return 1.
 * Otherwise return 0.
 */
int __predict_errors(parse_info *info, int next) {
	int ret = (info -> offset + next) > (info -> expect);
	info -> error = ret;
	return ret;
}

/*
 * Read in next payload w/ fixed length
 *
 * header:	Type of the payload
 * dest:	Pointer to preallocated region to write the payload
 * info:	Points to info for parsing this time
 */
void __next_fixed_len_payload(tag_header header, void *dest, parse_info *info) {
	char *buffer = info -> buffer;
	int *offset = &(info -> offset);
	void *payload = dest;

	if (__predict_errors(info, type_length(header))) {
		__nbtp_complain("Seeking past end of file while reading fixed length payload", *offset);
		return;
	}

	if (header == TAG_BYTE) {
//		fprintf(stderr, "Found Byte %x\n", *(buffer + *offset));
		*((char*) payload) = *(buffer + *offset);
	} else if (header == TAG_SHORT) {
		uint16_t s = be16toh(*((uint16_t *) (buffer + *offset)));
//		fprintf(stderr, "Found Short %i\n", s);
		*((int16_t*) payload) = (int16_t) s;
	} else if (header == TAG_INT) {
		uint32_t s = be32toh(*((uint32_t *) (buffer + *offset)));
//		fprintf(stderr, "Found Int %i\n", s);
		*((int32_t*) payload) = (int32_t) s;
	} else if (header == TAG_LONG) {
		uint64_t s = be64toh(*((uint64_t *) (buffer + *offset)));
//		fprintf(stderr, "Found Long %li\n", s);
		*((int64_t*) payload) = (int64_t) s;
	} else if (header == TAG_FLOAT) {
		float s = *((float *) (buffer + *offset));
//		fprintf(stderr, "Found Float %f\n", s);
		*((float*) payload) = s;
	} else if (header == TAG_DOUBLE) {
		double s = *((double *) (buffer + *offset));
//		fprintf(stderr, "Found Double %f\n", s);
		*((double*) payload) = s;
	}
	*offset += type_length(header);
}

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
		parse_info *info) {
	char *buffer = info -> buffer;
	int *offset = &(info -> offset);
	void **payload = dest;
	if (header == TAG_BYTES) {
		// Read int size of payload in multiple of bytes
		if (__predict_errors(info, 4)) {
			__nbtp_complain("Seeking past end of file when reading byte array length", *offset);
			return PARSE_ERR_PTR;
		}
		int32_t length = be32toh(*((uint32_t*) (buffer + *offset)));
		if (__predict_errors(info, length)) {
			__nbtp_complain("Seeking past end of file when reading byte array", *offset);
			return PARSE_ERR_PTR;
		}
		*offset += 4;
		*payload = __nbtp_malloc(length);
		memcpy(*payload, buffer + *offset, length);
		*offset += length;
		*size = length;
	} else if (header == TAG_STRING) {
		*payload = nbtp_next_string(info);
		*size = strlen((char *) *payload);
	} else if (header == TAG_INTS || header == TAG_LONGS) {
		// Read int size of payload in multiple of bytes
		if (__predict_errors(info, 4)) {
			__nbtp_complain("Seeking past end of file when reading number array length", *offset);
			return PARSE_ERR_PTR;
		}
		int32_t length = be32toh(*((uint32_t*) (buffer + *offset)));
		*offset += 4;
		int typeLen = type_length(header);
		if (__predict_errors(info, length * typeLen)) {
			__nbtp_complain("Seeking past end of file when reading number array", *offset);
			return PARSE_ERR_PTR;
		}
		*payload = __nbtp_malloc(length * typeLen);
		tag_header singleHdr = header - 8;
		// Handle integers and longs by offloading to fixed length
		for (int i = 0; i < length; i++) {
			// Read i-th integer or long
			__next_fixed_len_payload(singleHdr, *payload + i * typeLen,
					info);
		}
		*size = length;
	} else {
		__nbtp_complain("Unrecognized tag header", *offset);
		// Strange tags? Shouldn't happen.
		abort();
	}
	return 0;
}

/*
 * Parse next tag
 *
 * info:	Points to info for parsing this time
 *
 * Shall return NULL if it sees an TAG_END.
 */
tag* nbtp_next_tag(parse_info *info) {
	char *buffer = info -> buffer;
	int *offset = &(info -> offset);

	if (__predict_errors(info, 1)) {
		__nbtp_complain("Read past end of file.", *offset);
		return (tag*) PARSE_ERR_PTR;
	}
	tag_header header = *(buffer + *offset);
	*offset = *offset + 1;

	tag *nbt;
	if (header == TAG_END) { return (tag*) NULL; }
	if (header == TAG_COMPOUND) {
		// Offloading the task to a function dedicated for compound.
		*offset -= 1;
		nbt = nbtp_compound_decomp(info);
		return nbt;
	}
	if (header == TAG_LIST) {
		// Offloading this task to a function dedicated for lists
		*offset -= 1;
		nbt = nbtp_list_decomp(info);
		return nbt;
	}

	// Grab name first
	char *name = nbtp_next_string(info);
	if (name == (char *) PARSE_ERR_PTR) {
		return (tag*) PARSE_ERR_PTR;
	}
	nbt = __nbtp_malloc(sizeof(tag));
	nbt -> header = header;
	nbt -> name = name;
	if (!tag_variable_length(header)) {
		// A tag with fixed payload size, i.e. 1
		nbt -> size = 1;
		nbt -> payload = __nbtp_malloc(type_length(header));
		void *payload = nbt -> payload;
		__next_fixed_len_payload(header, payload, info);
		if (info -> error) {
			return (tag*) PARSE_ERR_PTR;
		}
	} else if (header < 13) {
		// Tags with variable payload sizes
		void *payload;
		int32_t size;
		__next_var_len_payload(header, &size, &payload, info);
		nbt -> payload = payload;
		nbt -> size = size;
		if (info -> error) {
			return (tag*) PARSE_ERR_PTR;
		}
	} else {
		__nbtp_complain("Unrecognized tag header", *offset);
		info -> error = 1;
		return (tag*) PARSE_ERR_PTR;
	}
	return nbt;
}

/*
 * Parse next string length + string combo
 */
char* nbtp_next_string(parse_info *info) {
	char *buffer = info -> buffer;
	int *offset = &(info -> offset);
	if (__predict_errors(info, 2)) {
		__nbtp_complain("Seeking past end of file when reading string length", *offset);
		return (char*) PARSE_ERR_PTR;
	}
	// Read in 2 bytes
	int16_t size = (int16_t) be16toh(*((uint16_t*) (buffer + *offset)));
	*offset += 2;
	if (__predict_errors(info, size)) {
		__nbtp_complain("Seeking past end of file when reading string", *offset);
		return (char*) PARSE_ERR_PTR;
	}
	// Read in next length bytes
	char *str = __nbtp_calloc(1, size + 1);
	strncpy(str, buffer + *offset, size);
	*offset += size;
	return str;
}
