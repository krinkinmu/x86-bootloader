#include "console.h"
#include "utils.h"

#include <stdarg.h>

void printf(const char *fmt, ...)
{
	char buffer[20];
	int c;

	va_list vararg_lst;
	va_start(vararg_lst, fmt);

	while ((c = *fmt++) != 0) {
		if (c != '%') {
			putchar(c);
			continue;
		}

		c = *fmt++;
		switch (c) {
		case 'u':
		case 'x': {
			unsigned value = va_arg(vararg_lst, unsigned);
			ultoa(buffer, value, (c == 'x' ? 16 : 10));
			puts_nn(buffer);
			break;
		}
		case 'd': {
			int value = va_arg(vararg_lst, int);
			ltoa(buffer, value, 10);
			puts_nn(buffer);
			break;
		}
		case 's':
			puts_nn(va_arg(vararg_lst, char const *));
			break;
		case 'c':
			putchar(va_arg(vararg_lst, int));
			break;
		default:
			putchar(c);
			break;
		}
	}

	va_end(vararg_lst);
}
