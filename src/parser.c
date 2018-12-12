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

/*
 * As simple as printing `indent` many tabs before printing actual thing.
 */
void printIndent(FILE *f, int indent) {
	for (int i = 0; i < indent; i++) {
		fprintf(f, "        ");
	}
}

/*
 * Recursive print
 *
 * nbt:		The tag to print
 * f:		The FILE to output
 * indent:	Indentation of output
 * columns:	The row width of array outputs, 0 to use default 32.
 */
void printTag(tag *nbt, FILE *f, int indent, int columns) {
	tag_header header = nbt -> header;
	char *name = nbt -> name;
	int width = (columns <= 0) ? 32 : ((columns >= 320) ? 320 : (columns + 1) / 8 * 8);
	if (header == TAG_END) {
		// This should not happen
		fprintf(stderr, "Unexpected TAG_END while printing\n");
		abort();
	} else if (header == TAG_BYTE) {
		printIndent(f, indent);
		char *payload = (char*) nbt -> payload;
		fprintf(f, "Byte: name = \"%s\", value = %x\n", name, *payload);
	} else if (header == TAG_SHORT) {
		printIndent(f, indent);
		int16_t *payload = (int16_t*) nbt -> payload;
		fprintf(f, "Short: name = \"%s\", value = %i\n", name, *payload);
	} else if (header == TAG_INT) {
		printIndent(f, indent);
		int32_t *payload = (int32_t*) nbt -> payload;
		fprintf(f, "Int: name = \"%s\", value = %i\n", name, *payload);
	} else if (header == TAG_LONG) {
		printIndent(f, indent);
		int64_t *payload = (int64_t*) nbt -> payload;
		fprintf(f, "Long: name = \"%s\", value = %li\n", name, *payload);
	} else if (header == TAG_FLOAT) {
		printIndent(f, indent);
		float *payload = (float*) nbt -> payload;
		fprintf(f, "Float: name = \"%s\", value = %f\n", name, *payload);
	} else if (header == TAG_DOUBLE) {
		printIndent(f, indent);
		double *payload = (double*) nbt -> payload;
		fprintf(f, "Double: name = \"%s\", value = %f\n", name, *payload);
	} else if (header == TAG_BYTES) {
		char *payload = (char*) nbt -> payload;
		printIndent(f, indent);
		fprintf(f, "Bytes: name = \"%s\", size = %i\n", name, nbt -> size);
		int bytesPerLine = (width - indent * 8 - 16) / 9 * 4;
		for (int i = 0; i < nbt -> size / bytesPerLine; i++) {
			printIndent(f, indent + 1);
			for (int j = 0; j < bytesPerLine; j++) {
				if (j % 4 == 0) { fprintf(f, " "); }
				fprintf(f, "%02x", *((unsigned char*) payload + bytesPerLine * i +j));
			}
			fprintf(f, "\n");
		}
		printIndent(f, indent + 1);
		for (int i = nbt -> size / bytesPerLine * bytesPerLine; i < nbt -> size; i++) {
			if (i % 4 == 0) { fprintf(f, " "); }
			fprintf(f, "%02x", *(payload + i));
		}
		fprintf(f, "\n");
	// Lots of other stuff
	} else if (header == TAG_COMPOUND) {
		tag **subtags = (tag**) nbt -> payload;
		int size = nbt -> size;
		printIndent(f, indent);
		fprintf(f, "Compound: name = \"%s\", size = %i\n", nbt -> name , size);
		for (int i = 0; i < size; i++) {
			printTag(*(subtags + i), f, indent + 1, width);
		}
	} else if (header == TAG_STRING) {
		char *str = (char*) nbt -> payload;
		int size = nbt -> size;
		printIndent(f, indent);
		fprintf(f, "String: name = \"%s\", size = %i\n", nbt -> name , size);
		printIndent(f, indent + 1);
		fprintf(f, "\"%s\"\n", str);
	} else {
		fprintf(stderr, "Unrecognized tag header %x\n", header);
		abort();
	}
}
