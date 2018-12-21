#include "tag.h"

#include <stdlib.h>

/*
 * Unit payload sizes for nbt in bytes
 *
 *  0 = Not applicable
 */
const int tag_payload_size[13] = {0, 1, 2, 4, 8, 4, 8, 1, 0, 0, 0, 4, 8};

/* Return the unit payload length */
int type_length(tag_header header) {
    if (header > 12) {
        fprintf(stderr, "Looking up tag type %d?!", header);
        return 0;
    }
    return tag_payload_size[(int) header];
}

/* Calculates the size of tags when written in nbt binary
 *
 * nbt:     Tag to be calculated
 */
int nbt_binary_size(tag* nbt) {
    int size = 0;
    if (nbt -> name != NULL) {
        // This a fully formed tag if name is not null, otherwise this tag is
        // part of a list and should not have its own name and header, etc.
        size = 3; // Counting header, name length encoding
        size += strlen(nbt -> name); // Actual name of this tag
    }
    if (nbt -> header == TAG_COMPOUND || nbt -> header == TAG_LIST) {
        tag **payload = (tag **) nbt -> payload;
        for (int i = 0; i < nbt -> size; i++) {
            size += nbt_binary_size(payload[i]);
        }
        if (nbt -> header == TAG_COMPOUND) {
            // Compound tags has extra 1 byte at the end
            size++;
        } else {
            // List tags has 4 bytes of length + 1 byte of payload type
            size += 5;
        }
    } else if (tag_variable_length(nbt -> header)) {
        // Arrays of numbers or a single string
        if (nbt -> header == TAG_STRING) {
            size += 2 + strlen((char *)nbt -> payload);
        } else {
            // All others has 4 bytes to encode array size
            size += nbt -> size * type_length(nbt -> header);
            size += 4;
        }
    } else {
        // Singleton tags
        size += type_length(nbt -> header);
    }
    return size;
}

/* Return whether this type can have variable payload size */
int tag_variable_length(tag_header header) {
    return (header > 6);
}

/*
 * Deallocate tags on heap
 */
void freeTag(tag* nbt) {
    // A broken tag is skipped
    if (nbt == (tag *) PARSE_ERR_PTR) {
        return;
    }
    tag_header header = nbt -> header;
    if (header == TAG_COMPOUND || header == TAG_LIST) {
        tag **subtags = (tag**) nbt -> payload;
        int size = nbt -> size;
        for (int i = 0; i < size; i++) {
            freeTag(*(subtags + i));
        }
        free(subtags);
    } else {
        free(nbt -> payload);
    }
    free(nbt -> name);
    free(nbt);
}

/*
 * As simple as printing `indent` many tabs before printing actual thing.
 */
void printIndent(FILE *f, int indent) {
    for (int i = 0; i < indent; i++) {
        fprintf(f, "    ");
    }
}

/*
 * Recursive print
 *
 * nbt:        The tag to print
 * f:        The FILE to output
 * indent:    Indentation of output
 * columns:    The row width of array outputs, 0 to use default 32.
 */
void printTag(tag *nbt, FILE *f, int indent, int columns) {
    tag_header header = nbt -> header;
    char *name = nbt -> name;
    if (name == NULL) { name = "LIST ITEM"; }
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
        fprintf(f, "Compound: name = \"%s\", size = %i\n", name , size);
        for (int i = 0; i < size; i++) {
            printTag(*(subtags + i), f, indent + 1, width);
        }
    } else if (header == TAG_LIST) {
        tag **subtags = (tag**) nbt -> payload;
        int size = nbt -> size;
        printIndent(f, indent);
        fprintf(f, "List: name = \"%s\", size = %i\n", name , size);
        for (int i = 0; i < size; i++) {
            printTag(*(subtags + i), f, indent + 1, width);
        }
    } else if (header == TAG_STRING) {
        char *str = (char*) nbt -> payload;
        int size = nbt -> size;
        printIndent(f, indent);
        fprintf(f, "String: name = \"%s\", size = %i\n", name , size);
        printIndent(f, indent + 1);
        fprintf(f, "\"%s\"\n", str);
    } else if (header == TAG_INTS) {
        int32_t *payload = (int32_t*) nbt -> payload;
        int size = nbt -> size;
        printIndent(f, indent);
        fprintf(f, "Ints: name = \"%s\", size = %i\n", name, size);
        for (int i = 0; i < size; i++) {
            printIndent(f, indent + 1);
            fprintf(f, "%i\n", *(payload + i));
        }
    } else if (header == TAG_LONGS) {
        int64_t *payload = (int64_t*) nbt -> payload;
        int size = nbt -> size;
        printIndent(f, indent);
        fprintf(f, "Longs: name = \"%s\", size = %i\n", name, size);
        for (int i = 0; i < size; i++) {
            printIndent(f, indent + 1);
            fprintf(f, "%li\n", *(payload + i));
        }
    } else {
        fprintf(stderr, "Unrecognized tag header %x\n", header);
        abort();
    }
}

// vim: set tabstop=4 expandtab :
