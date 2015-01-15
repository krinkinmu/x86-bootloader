#include "string.h"

void *memset(void *ptr, int value, unsigned size)
{
	char *buf = ptr;

	while (size--)
		*buf++ = value;

	return ptr;
}

/**
 * Used by clang implicitly
 **/
void *memcpy(void *dst, const void *src, unsigned size)
{
	char *to = dst;
	const char *from = src;

	while (size--)
		*to++ = *from++;

	return dst;
}
