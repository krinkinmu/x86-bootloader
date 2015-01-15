#include "string.h"

void *memset(void *ptr, int value, unsigned size)
{
	char *buf = ptr;

	while (size--)
		*buf++ = value;

	return ptr;
}
