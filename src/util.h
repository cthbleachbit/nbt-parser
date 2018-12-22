#ifndef __libnbtp_util
#define __libnbtp_util

#include <stdlib.h>

void* __nbtp_malloc(size_t size);

void* __nbtp_calloc(size_t nmemb, size_t bsize);

void __nbtp_complain(char *message, int offset);

#endif
