#include "util.h"

#include <assert.h>
#include <stdio.h>

void* __nbtp_malloc(size_t size) {
	void *ret = malloc(size);
	assert(ret != NULL);
	return ret;
}

void* __nbtp_calloc(size_t nmemb, size_t bsize) {
	void *ret = calloc(nmemb, bsize);
	assert(ret != NULL);
	return ret;
}

void __nbtp_complain(char *message, int offset) {
	fprintf(stderr, "Error at offset %i:\n", offset);
	fprintf(stderr, "    %s\n", message);
}
