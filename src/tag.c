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
int binary_size(tag* nbt) {
    int size = 5; // Counting header, name length encoding
    size += strlen(nbt -> name); // Actual name of this tag
    // TODO: Payload size
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
    tag_header header = nbt -> header;
    if (header == TAG_COMPOUND) {
        tag **subtags = (tag**) nbt -> payload;
        int size = nbt -> size;
        for (int i = 0; i < size; i++) {
            freeTag(*(subtags + i));
        }
        free(subtags);
    } else if (header == TAG_LIST) {
        // TODO
    } else {
        free(nbt -> payload);
    }
    free(nbt -> name);
    free(nbt);
}

// vim: set tabstop=4 expandtab :
