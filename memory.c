#include "boot.h"
#include "printf.h"

#define SMAP             0x534D4150
#define RAM              1
#define RESERVED         2
#define ACPI_RECLAIMABLE 3
#define ACPI_NVS         4
#define BAD_MEMORY       5


int e820_detect(struct boot_params *params)
{
	struct biosregs iregs, oregs;
	struct e820entry buf;
	struct e820entry *entries = params->e820_mm;
	const int size = sizeof(params->e820_mm) / sizeof(buf);
	int count = 0;

	initregs(&iregs);
	iregs.ax = 0xe820;
	iregs.cx = sizeof(buf);
	iregs.edx = SMAP;
	iregs.di = (unsigned long)&buf;

	do {
		intcall(0x15, &iregs, &oregs);

		if (oregs.eflags & X86_CF_FLAG)
			break;

		if (oregs.eax != SMAP) {
			count = 0;
			break;
		}

		iregs.ebx = oregs.ebx;
		entries[count++] = buf;
	} while (oregs.ebx && count < size);

	params->e820_entries = count;
	return count;
}

static const char *e820_type(unsigned long type)
{
	switch (type) {
	case RAM: return "RAM";
	case RESERVED: return "reserved";
	case ACPI_RECLAIMABLE: return "ACPI reclaimable memory";
	case ACPI_NVS: return "ACPI NVS";

	case BAD_MEMORY:
	default: return "unknown";
	}
}

void e820_show(const struct boot_params *params)
{
	const struct e820entry *entries = params->e820_mm;
	int i;

	printf("Memory map (%u ranges):\n", params->e820_entries);
	for (i = 0; i < params->e820_entries; ++i) {
		printf("region 0x%x-0x%x %s\n",
			entries[i].addrl,
			entries[i].addrl + entries[i].sizel - 1,
			e820_type(entries[i].type));
	}
}
