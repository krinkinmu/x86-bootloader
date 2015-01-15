#include "boot.h"

#define SMAP 0x534D4150

static int e820_detect(struct e820entry *entries, unsigned size)
{
	struct biosregs iregs, oregs;
	struct e820entry buf;
	int count = 0;

	initregs(&iregs);
	iregs.ax = 0xe820;
	iregs.cx = sizeof(buf);
	iregs.edx = SMAP;
	iregs.di = (unsigned short)&buf;

	do {
		intcall(0x15, &iregs, &oregs);

		if (oregs.eflags & X86_CF_FLAG)
			break;

		if (oregs.eax != SMAP)
			return 0;

		iregs.ebx = oregs.ebx;
		entries[count] = buf;
		count++;
	} while (iregs.ebx && count < size);

	return count;
}

int detect_memory(struct boot_params *params)
{
	params->e820_entries = e820_detect(params->e820_mm, E820_MAX_MAP_SIZE);
	return params->e820_entries;
}
