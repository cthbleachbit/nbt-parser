#ifndef __libnbtp_tag
#define __libnbtp_tag

#include <stdio.h>
#include <string.h>

typedef char tag_header;

// Not that size is the number of payload of appripriate type
// i.e. size = 2 means 2 integers, etc.
typedef struct tag {
    tag_header header;
    char *name;
    int size;
    /*
     * If the tag is
     * - compound
     *     payload points to an array of tag*
     * - list
     *     payload points to an array of appropriate payloads.
     * - int(s), long(s), byte(s)
     *     payload points to an array of ints, longs, or bytes.
     *     Note that this array can be of size 1.
     * - string
     *     payload is a char*.
     */
    void *payload;
} tag;

/*
 * Tag header byte to tell different tags apart
 */

#define TAG_END         0
#define TAG_BYTE        1
#define TAG_SHORT       2
#define TAG_INT         3
#define TAG_LONG        4
#define TAG_FLOAT       5
#define TAG_DOUBLE      6
#define TAG_BYTES       7
#define TAG_STRING      8
#define TAG_LIST        9
#define TAG_COMPOUND    10
#define TAG_INTS        11
#define TAG_LONGS       12

/* Return whether this type can have variable payload size */
int tag_variable_length(tag_header header);

/* Return the unit payload length */
int type_length(tag_header header);


/* Calculates the size of tags when written in nbt binary
 *
 * nbt:     Tag to be calculated
 */
int binary_size(tag* nbt);

/*
 * Deallocate tags on heap
 */
void freeTag(tag* nbt);

#endif

// vim: set tabstop=4 expandtab :
