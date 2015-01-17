#include "boot.h"
#include "printf.h"

static int a20_enabled(void)
{
	/**
 	 * We can use any address above 1Mb that doesn't point to this function
	 * stack, we use first bootloader sector (0x7c00 and 0x107c00).
	 *
	 * With clang it is possible to use 32 bit address in RM, but gcc has
	 * lack of native RM support and truncates all 16+ bit immediates in
	 * memory access instructions to 16 bit values, so we have to use
	 * functions like set_{fs|gs}, readl_{fs|gs} and writel_{fs|gs}.
	 **/
	const unsigned short low_off = 0x7c00;
	const unsigned short high_off = 0x7c10; /*0xffff0 + 0x7c10 = 0x107c00*/

	unsigned long saved, control;
	int repeats = 32;
	int rc = 0;

	cli();
	set_fs(0x0000);
	set_gs(0xffff);
	saved = control = readl_fs(low_off);
	while (--repeats) {
		rc = (readl_gs(high_off) != control);
		if (rc)
			break;

		writel_fs(low_off, ++control);
		io_delay();
	}
	writel_fs(low_off, saved);
	sti();

	return rc;
}

static void enable_a20_bios(void)
{
	struct biosregs iregs;

	initregs(&iregs);
	iregs.ax = 0x2401;
	intcall(0x15, &iregs, 0);
}

int enable_a20(void)
{
	if (a20_enabled())
		return 0;

	enable_a20_bios();
	if (a20_enabled())
		return 0;

	return -1;
}
