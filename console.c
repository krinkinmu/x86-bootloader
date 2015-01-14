static void bios_set_video_mode(unsigned char mode)
{
	asm volatile ("xorw %%ax, %%ax\n"
		      "movb %0,   %%al\n"
		      "int  $0x10"
		      : : "q"(mode) : "%ax");
}

void console_init(void)
{ bios_set_video_mode(3); }

static void bios_putchar(int ch)
{
	asm volatile ("movb $0x0e, %%ah\n"
		      "movb %0,    %%al\n"
		      "int  $0x10"
		      : : "q"((unsigned char)ch) : "%ax");
}

void putchar(int ch)
{
	if (ch == '\n')
		putchar('\r');
	bios_putchar(ch);
}

void puts(char const *str)
{
	while (*str)
		putchar(*str++);
	putchar('\n');
}
