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
tag* parseFile(char *buffer, const int expect) {
	parse_info *info = p_malloc(sizeof(parse_info));
	info -> buffer = buffer;
	info -> offset = 0;
	info -> expect = expect;
	// Should only have one root tag
	tag* result = nextTag(info);
	if (info -> offset != expect) {
		fprintf(stderr, "Wrong tag size? Expected %i, got %i.\n",
				expect, info -> offset);
	}
	free(info);
	return result;
}

/*
 * Read in next payload w/ fixed length
 *
 * header:	Type of the payload
 * dest:	Pointer to preallocated region write the payload
 * buffer:	input buffer
 * offset:	input offset
 * expect:	Expected length of the whole file
 */
void nextFixedLenPayload(tag_header header, void *dest, parse_info *info) {
	char *buffer = info -> buffer;
	int *offset = &(info -> offset);
	void *payload = dest;
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
		float s = (float) be32toh(*((uint32_t *) (buffer + *offset)));
//		fprintf(stderr, "Found Float %f\n", s);
		*((float*) payload) = s;
	} else if (header == TAG_DOUBLE) {
		double s = (double) be64toh(*((uint64_t *) (buffer + *offset)));
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
 * buffer:	input buffer
 * offset:	input offset
 * expect:	Expected length of the whole file
 */
int nextVarLenPayload(tag_header header, int32_t *size, void **dest,
		parse_info *info) {
	char *buffer = info -> buffer;
	int *offset = &(info -> offset);
	void **payload = dest;
	if (header == TAG_BYTES) {
		// Read int size of payload in multiple of bytes
		int32_t length = be32toh(*((uint32_t*) (buffer + *offset)));
		*offset += 4;
		*payload = p_malloc(length);
		memcpy(*payload, buffer + *offset, length);
		*offset += length;
		*size = length;
	} else if (header == TAG_STRING) {
		*payload = nextString(info);
		*size = strlen((char *) *payload);
	} else if (header == TAG_INTS || header == TAG_LONGS) {
		// Read int size of payload in multiple of bytes
		int32_t length = be32toh(*((uint32_t*) (buffer + *offset)));
		*offset += 4;
		int typeLen = type_length(header);
		*payload = p_malloc(length * typeLen);
		tag_header singleHdr = header - 8;
		// Handle integers and longs by offloading to fixed length
		for (int i = 0; i < length; i++) {
			// Read i-th integer or long
			nextFixedLenPayload(singleHdr, *payload + i * typeLen,
					info);
		}
		*size = length;
	} else {
		// Strange tags?
		return -1;
	}
	return 0;
}

/*
 * Parse next tag
 *
 * buffer:	Entire tag mapped in memory
 * offset:	The offset where next tag starts
 * expect:	Expected length of the whole file
 *
 * Shall return NULL if it sees an TAG_END.
 */
tag* nextTag(parse_info *info) {
	char *buffer = info -> buffer;
	int *offset = &(info -> offset);
	// Read tag type
	tag_header header = *(buffer + *offset);
	*offset = *offset + 1;
	tag *nbt;
	if (header == TAG_END) { return (tag*) NULL; }
	if (header == TAG_COMPOUND) {
		// Offloading the task to a function dedicated for compound.
		*offset -= 1;
		nbt = compoundDecomp(info);
		return nbt;
	}
	if (header == TAG_LIST) {
		// Offloading this task to a function dedicated for lists
		*offset -= 1;
		nbt = listDecomp(info);
		return nbt;
	}

	// Grab name first
	char *name = nextString(info);
	nbt = p_malloc(sizeof(tag));
	nbt -> header = header;
	nbt -> name = name;
	if (!tag_variable_length(header)) {
		// A tag with fixed payload size, i.e. 1
		nbt -> size = 1;
		nbt -> payload = p_malloc(type_length(header));
		void *payload = nbt -> payload;
		nextFixedLenPayload(header, payload, info);
	} else if (header < 13) {
		// Tags with variable payload sizes
		void *payload;
		int32_t size;
		nextVarLenPayload(header, &size, &payload, info);
		nbt -> payload = payload;
		nbt -> size = size;
	} else {
		fprintf(stderr, "Unrecognized tag %02x at offset %i\n", header, *offset);
		abort();
	}
	return nbt;
}

/*
 * Parse next string length + string combo
 */
char* nextString(parse_info *info) {
	char *buffer = info -> buffer;
	int *offset = &(info -> offset);
	// Read in 2 bytes
	int16_t size = (int16_t) be16toh(*((uint16_t*) (buffer + *offset)));
	*offset += 2;
	// Read in next length bytes
	char *str = p_calloc(1, size + 1);
	strncpy(str, buffer + *offset, size);
	*offset += size;
	return str;
}
