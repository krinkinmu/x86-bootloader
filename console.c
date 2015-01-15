#include "boot.h"

static void bios_set_video_mode(unsigned char mode)
{
	struct biosregs iregs;

	initregs(&iregs);
	iregs.ah = 0;
	iregs.al = mode;
	intcall(0x10, &iregs, 0);
}

void console_init(void)
{ bios_set_video_mode(3); }

static void bios_putchar(int ch)
{
	struct biosregs iregs;

	initregs(&iregs);
	iregs.bx = 0x0007;
	iregs.cx = 0x0001;
	iregs.ah = 0x0e;
	iregs.al = ch;
	intcall(0x10, &iregs, 0);
}

void putchar(int ch)
{
	if (ch == '\n')
		putchar('\r');
	bios_putchar(ch);
}

void puts_nn(const char *str)
{
	while (*str)
		putchar(*str++);
}

void puts(const char *str)
{
	puts_nn(str);
	putchar('\n');
}
