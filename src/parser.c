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
			fprintf(stderr, "IFound Short %i\n", s);
			*((int16_t*) payload) = (int16_t) s;
		} else if (header == TAG_INT) {
			uint32_t s = be32toh(*(buffer + *offset));
			fprintf(stderr, "Found Int %i\n", s);
			*((int32_t*) payload) = (int32_t) s;
		} else if (header == TAG_LONG) {
			uint64_t s = be64toh(*(buffer + *offset));
			fprintf(stderr, "Found Long %li\n", s);
			*((int64_t*) payload) = (int64_t) s;
		} else {
			// Float or double then
			memcpy(nbt -> payload, buffer + *offset, type_length(header));
		}
		*offset += type_length(header);
	} else {
		if (header == TAG_BYTES) {
			// TODO
		}
	}
	return nbt;
}

/*
 * Parse next string length + string combo
 */
char* nextString(const char* buffer, int* offset) {
	// Read in 4 bytes
	int length = be32toh(*((uint32_t*) (buffer + *offset)));
	*offset += 4;
	// Read in next length bytes
	char *str = p_calloc(1, length + 1);
	strncpy(str, buffer + *offset, length);
	*offset += length;
	return str;
}

/*
 * As simple as printing `indent` many tabs before printing actual thing.
 */
void printIndent(FILE *f, int indent) {
	for (int i = 0; i < indent; i++) {
		fprintf(f, "\t");
	}
}

/*
 * Recursive print
 *
 * nbt:		The tag to print
 * f:		The FILE to output
 * indent:	Indentation of output
 */
void printTag(tag *nbt, FILE *f, int indent) {
	tag_header header = nbt -> header;
	if (header == TAG_END) {
		// This should not happen
		fprintf(stderr, "Unexpected TAG_END while printing\n");
		abort();
	} else if (header == TAG_BYTE) {
		printIndent(f, indent);
		char *payload = (char*) nbt -> payload;
		fprintf(f, "Byte\t%x\n", *payload);
	} else if (header == TAG_SHORT) {
		printIndent(f, indent);
		int16_t *payload = (int16_t*) nbt -> payload;
		fprintf(f, "Short\t%i\n", *payload);
	} else if (header == TAG_INT) {
		printIndent(f, indent);
		int32_t *payload = (int32_t*) nbt -> payload;
		fprintf(f, "Int\t%i\n", *payload);
	} else if (header == TAG_LONG) {
		printIndent(f, indent);
		int64_t *payload = (int64_t*) nbt -> payload;
		fprintf(f, "Long\t%li\n", *payload);
	} else if (header == TAG_FLOAT) {
		printIndent(f, indent);
		float *payload = (float*) nbt -> payload;
		fprintf(f, "Float\t%f\n", *payload);
	} else if (header == TAG_DOUBLE) {
		printIndent(f, indent);
		double *payload = (double*) nbt -> payload;
		fprintf(f, "Short\t%f\n", *payload);
	} else if (header == TAG_BYTES) {
		char *payload = (char*) nbt -> payload;
		printIndent(f, indent);
		fprintf(f, "Bytes\t%x\n", *payload);
	// Lots of other stuff
	} else if (header == TAG_COMPOUND) {
		tag **subtags = (tag**) nbt -> payload;
		int size = nbt -> size;
		printIndent(f, indent);
		fprintf(f, "Compound: name = \"%s\", size = %i\n", nbt -> name , size);
		for (int i = 0; i < size; i++) {
			printTag(*subtags + i, f, indent + 1);
		}
	} else {
		fprintf(stderr, "Unrecognized tag header %x\n", header);
		abort();
	}
}
