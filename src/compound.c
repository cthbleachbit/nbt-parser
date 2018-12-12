#include "tag.h"
#include "compound.h"
#include "util.h"
#include "parser.h"

#include <assert.h>

/*
 * A homebrew linked list to add tags in the compound on the fly without need
 * to worry about array sizes.
 */
typedef struct linked_tag {
	struct linked_tag *prev;
	tag *nbt;
	struct linked_tag *next; 
} linked_tag;

/*
 * Add nbt after linkedNBT. Returns the newly linked tag.
 */
linked_tag *chainTag(linked_tag *linkedNBT, tag *nbt) {
	linked_tag *origNext = linkedNBT -> next;
	// Calloc to wipe out any possible leaks on next
	linked_tag *newLinkedNBT = p_calloc(1, sizeof(linked_tag));
	newLinkedNBT -> nbt = nbt;
	// Linking new to next, works even when it's the last one
	newLinkedNBT -> next = origNext;
	if (origNext != NULL) {
		origNext -> prev = newLinkedNBT;
	}
	// Linking new and this
	linkedNBT -> next = newLinkedNBT;
	newLinkedNBT -> prev = linkedNBT;
	return newLinkedNBT;
}

/*
 * Convert the linked tags to a linear array
 *
 * This allows all compounds to be stored in a rigid tag struct
 */
tag **chainToArray(linked_tag *head, int size) {
	linked_tag *next = head;
	tag **tagArray = p_malloc(size * sizeof(tag*));
	for (int i = 0; i < size; i++) {
		next = next -> next;
		*(tagArray + i) = next -> nbt;
	}
	return tagArray;
}

/*
 * Tearing down the linked list to freeup memory
 */
void freeChain(linked_tag *head) {
	linked_tag *this = head -> next;
	linked_tag *next;
	while (this != NULL) {
		next = this -> next;
		free(this);
		this = next;
	}
}

/*
 * Decompose compound, returned as a complete new tag
 */
tag *compoundDecomp(const char *buffer, int *offset) {
	// Construct a proper linked list head w/o messing with heap
	linked_tag head;
	head.prev = NULL;
	head.nbt = NULL;
	head.next = NULL;
	linked_tag *tail = &head;
	tag *compound;
	// Bailing out if we are not dealing with a compound
	tag_header header = *(buffer + *offset);
	assert(header == TAG_COMPOUND);
	*offset = *offset + 1;
	// Grab its name
	char *name = nextString(buffer, offset);

	tag *nbt;
	int size = 0;
	// Push the button
	do {
		nbt = nextTag(buffer, offset);
		if (nbt == NULL) { break; }
		tail = chainTag(tail, nbt);
		size ++;
	} while ( 1 );

	// Flatten down the linked tags
	tag **tagArray;
	if (size > 0) {
		tagArray = chainToArray(&head, size);
	} else {
		tagArray = NULL;
	}
	freeChain(&head);

	// Construct the compound tag
	compound = p_malloc(sizeof(tag));
	compound -> header = header;
	compound -> name = name;
	compound -> size = size;
	compound -> payload = tagArray;
	return compound;
}
