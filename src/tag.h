#ifndef __libnbtp_tag
#define __libnbtp_tag

#include <stdio.h>
#include <string.h>

// The address to set when an error is encountered
#define PARSE_ERR_PTR -1

typedef char tag_header;

// Not that size is the number of payload of appripriate type
// i.e. size = 2 means 2 integers, etc.
typedef struct tag {
    tag_header header;
    // If the tag is part of a list, the name will be NULL
    char *name;
    int size;
    // Only used when this tag is a list
    tag_header listType;
    /*
     * If the tag is
     * - compound
     *     payload points to an array of tag*
     * - list
     *     payload points to content of the list formed into tags of same type
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


/* Calculates the size of tags if written as nbt binary
 *
 * nbt:     Tag to be calculated
 */
int nbtp_bin_size(tag* nbt);

/*
 * Deallocate tags on heap
 */
void nbtp_free_tag(tag* nbt);

/*
 * Recursive print
 *
 * nbt:		The tag to print
 * f:		The FILE to output
 * indent:	Indentation of output
 * columns:	The row width of array outputs, 0 to use default 32.
 */
void nbtp_print_tag(tag *nbt, FILE *f, int indent, int columns);

/*
 * Write out a tag in binary
 *
 * nbt:		The tag to print
 * f:		The FILE to write to
 */
void nbtp_write_tag(tag *nbt, FILE *f);

#endif

// vim: set tabstop=4 expandtab :
