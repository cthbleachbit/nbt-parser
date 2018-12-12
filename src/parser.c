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
 * length:	Expected length of the whole tag
 */
tag* parseFile(const char *buffer, const int length) {
	int offset = 0;
	// Should only have one root tag
	tag* result = nextTag(buffer, &offset);
	if (offset != length) {
		fprintf(stderr, "Wrong tag size? Expected %i, got %i.\n", length, offset);
	}
	return result;
}

/*
 * Parse next tag
 *
 * buffer:	Entire tag mapped in memory
 * offset:	The offset where next tag starts
 *
 * Shall return NULL if it sees an TAG_END.
 */
tag* nextTag(const char *buffer, int *offset) {
	// Read tag type
	tag_header header = *(buffer + *offset);
	*offset = *offset + 1;
	tag *nbt;
	if (header == TAG_END) { return (tag*) NULL; }
	if (header == TAG_COMPOUND) {
		// Offloading the task to a function dedicated for compound.
		*offset -= 1;
		nbt = compoundDecomp(buffer, offset);
		return nbt;
	}
	// Grab name first
	char *name = nextString(buffer, offset);
	nbt = p_malloc(sizeof(tag));
	nbt -> header = header;
	nbt -> name = name;
	if (!tag_variable_length(header)) {
		// A tag with fixed payload size, i.e. 1
		nbt -> size = 1;
		nbt -> payload = p_malloc(type_length(header));
		void *payload = nbt -> payload;
		if (header == TAG_BYTE) {
			fprintf(stderr, "Found Byte %x\n", *(buffer + *offset));
			*((char*) payload) = *(buffer + *offset);
		} else if (header == TAG_SHORT) {
			uint16_t s = be16toh(*(buffer + *offset));
			fprintf(stderr, "Found Short %i\n", s);
			*((int16_t*) payload) = (int16_t) s;
		} else if (header == TAG_INT) {
			uint32_t s = be32toh(*(buffer + *offset));
			fprintf(stderr, "Found Int %i\n", s);
			*((int32_t*) payload) = (int32_t) s;
		} else if (header == TAG_LONG) {
			uint64_t s = be64toh(*(buffer + *offset));
			fprintf(stderr, "Found Long %li\n", s);
			*((int64_t*) payload) = (int64_t) s;
		} else if (header == TAG_FLOAT) {
			float s = (float) be32toh(*(buffer + *offset));
			fprintf(stderr, "Found Float %f\n", s);
			*((float*) payload) = s;
		} else if (header == TAG_DOUBLE) {
			double s = (double) be64toh(*(buffer + *offset));
			fprintf(stderr, "Found Double %f\n", s);
			*((double*) payload) = s;
		}
		*offset += type_length(header);
	} else {
		void *payload;
		if (header == TAG_BYTES) {
			// Read int size of payload in multiple of bytes
			int32_t length = (int32_t) be32toh(*((uint32_t*) (buffer + *offset)));
			*offset += 4;
			payload = p_malloc(length);
			memcpy(payload, buffer + *offset, length);
			*offset += length;
			nbt -> size = length;
		} else if (header == TAG_STRING) {
			payload = nextString(buffer, offset);
			nbt -> size = strlen((char *) payload);
		}
		nbt -> payload = payload;
	}
	return nbt;
}

/*
 * Parse next string length + string combo
 */
char* nextString(const char* buffer, int* offset) {
	// Read in 2 bytes
	int16_t length = (int16_t) be16toh(*((uint16_t*) (buffer + *offset)));
	*offset += 2;
	// Read in next length bytes
	char *str = p_calloc(1, length + 1);
	strncpy(str, buffer + *offset, length);
	*offset += length;
	return str;
}
