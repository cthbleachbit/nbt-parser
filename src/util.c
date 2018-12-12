#include "util.h"

#include <assert.h>

void* p_malloc(size_t size) {
	void *ret = malloc(size);
	assert(ret != NULL);
	return ret;
}

void* p_calloc(size_t nmemb, size_t bsize) {
	void *ret = calloc(nmemb, bsize);
	assert(ret != NULL);
	return ret;
}
