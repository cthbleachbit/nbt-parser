#include "tag.h"
#include "compound.h"
#include "util.h"
#include "parser.h"

#include <assert.h>
#include <stdint.h>

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
 * Handle compound paylaods, i.e. list of tags
 *
 * size will be updated to reflect number of tags in this compound
 */
tag **compoundPayload(int *size, const char *buffer, int *offset) {
	// Construct a proper linked list head w/o messing with heap
	linked_tag head;
	head.prev = NULL;
	head.nbt = NULL;
	head.next = NULL;
	linked_tag *tail = &head;

	// Push the button
	*size = 0;
	do {
		tag *nbt = nextTag(buffer, offset);
		if (nbt == NULL) { break; }
		tail = chainTag(tail, nbt);
		*size += 1;
	} while ( 1 );

	// Flatten down the linked tags
	tag **tagArray;
	if (*size > 0) {
		tagArray = chainToArray(&head, *size);
	} else {
		tagArray = NULL;
	}
	freeChain(&head);
	return tagArray;
}

/*
 * Decompose compound, returned as a complete new tag with name and subtags
 */
tag *compoundDecomp(const char *buffer, int *offset) {

	tag *compound;
	// Bailing out if we are not dealing with a compound
	tag_header header = *(buffer + *offset);
	assert(header == TAG_COMPOUND);
	*offset = *offset + 1;
	// Grab its name
	char *name = nextString(buffer, offset);

	int size = 0;
	tag **tagArray = compoundPayload(&size, buffer, offset);

	// Construct the compound tag
	compound = p_malloc(sizeof(tag));
	compound -> header = header;
	compound -> name = name;
	compound -> size = size;
	compound -> payload = tagArray;
	return compound;
}

/*
 * Handle list paylaods, i.e. list of tag payloads
 *
 * psize:	Will contain number of items
 * listType:	Will contain type of each payload
 */
tag **listPayload(int *psize, tag_header *listType, const char *buffer, int *offset) {
	// Get payload type and size
	tag_header header = *(buffer + *offset);
	*offset += 1;
	int32_t size = (int32_t) be32toh(*((uint32_t *) (buffer + *offset)));
	*offset += 4;
	// Setting these info
	*psize = size;
	*listType = header;

	if (size == 0) {
		return NULL;
	} else if (size < 0) {
		fprintf(stderr, "List has negative size %i at offset %i\n", size, *offset);
		abort();
	}

	// Some normal tags. Since we know the size ahead...
	tag **tagArray = p_malloc(size * sizeof(tag*));
	if (header == TAG_END) {
		fprintf(stderr, "List has subtype TAG_END?!\n");
		abort();
	}
	if (header == TAG_COMPOUND) {
		// Offloading the task to a function dedicated for compound.
		for (int i = 0; i < size; i++){
			int nSize = 0;
			tag **nTagArray = compoundPayload(&nSize, buffer, offset);

			// Construct the compound tag
			tag *nbt = p_malloc(sizeof(tag));
			nbt -> header = TAG_COMPOUND;
			nbt -> name = NULL;
			nbt -> size = nSize;
			nbt -> payload = nTagArray;
			*(tagArray + i) = nbt;
		}
		return tagArray;
	}
	if (header == TAG_LIST) {
		// Offloading this task to a function dedicated for lists
		for (int i = 0; i < size; i++){
			int nSize;
			tag_header nHeader;
			void *payload = listPayload(&nSize, &nHeader, buffer, offset);
			// New tag
			tag *nbt = p_malloc(sizeof(tag));
			nbt -> header = TAG_LIST;
			nbt -> name = NULL;
			nbt -> size = nSize;
			nbt -> listType = nHeader;
			nbt -> payload = payload;
			*(tagArray + i) = nbt;
		}
		return tagArray;
	}


	if (!tag_variable_length(header)) {
		// All payloads are singleton
		for (int i = 0; i < size; i++){
			tag *nbt = p_calloc(1, sizeof(tag));
			nbt -> size = 1;
			nbt -> header = header;
			nbt -> payload = p_malloc(type_length(header));
			void *payload = nbt -> payload;
			nextFixedLenPayload(header, payload, buffer, offset);

			// Fill in array
			*(tagArray + i) = nbt;
		}
	} else if (header < 13) {
		// Tags with variable payload sizes
		for (int i = 0; i < size; i++){
			void *payload;
			int32_t payloadSize;
			nextVarLenPayload(header, &payloadSize, &payload, buffer, offset);

			tag *nbt = p_calloc(1, sizeof(tag));
			nbt -> size = payloadSize;
			nbt -> header = header;
			nbt -> payload = payload;
			// Fill in array
			*(tagArray + i) = nbt;
		}
	} else {
		fprintf(stderr, "Unrecognized tag %02x at offset %i\n", header, *offset);
		abort();
	}
	return tagArray;
}

/*
 * Decompose list, returned as a complete new list tag
 *
 * Here a list tag's payload is represented as an array of full tags without
 * names. In an actual nbt file, only payload is present.
 *
 */
tag *listDecomp(const char *buffer, int *offset) {
	// Bailing out if we are not dealing with a list
	tag_header header = *(buffer + *offset);
	assert(header == TAG_LIST);
	*offset = *offset + 1;
	// Grab its name
	char *name = nextString(buffer, offset);

	int size;
	tag_header listType;
	tag **tagArray = listPayload(&size, &listType, buffer, offset);

	// Construct the list tag
	tag *list;
	list = p_malloc(sizeof(tag));
	list -> header = TAG_LIST;
	list -> name = name;
	list -> size = size;
	list -> listType = listType;
	list -> payload = tagArray;
	return list;
}
