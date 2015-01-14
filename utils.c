void ultoa(char *buf, unsigned long d, int base)
{
	char *begin = buf;
	char *end = buf;

	do {
		unsigned rem = d % base;

		*end++ = (rem < 10) ? rem + '0' : rem + 'a' - 10;
	} while (d /= base);

	*end-- = 0;
	while (begin < end) {
		char tmp = *begin;

		*begin++ = *end;
		*end-- = tmp;
	}
}

void ltoa(char *buf, long d, int base)
{
	unsigned long n = d;

	if (base == 10 && d < 0) {
		*buf++ = '-';
		n = (unsigned long)(-d);
	}
	ultoa(buf, n, base);
}
