#ifndef MEMUTIL_H
#define MEMUTIL_H


#include <common/types.h>

void *memset(void *dest, int val, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memcpy(void *dest, const void* src, size_t n);

#endif