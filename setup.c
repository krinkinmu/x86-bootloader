#include "boot.h"
#include "disk.h"
#include "gdt.h"
#include "console.h"
#include "pm.h"

static struct boot_params params __attribute__((aligned(16)));

static void load_kernel(struct boot_header *header)
{
	struct disk_drive drive;
	char *load_addr = (char *)header->image_addr;
	unsigned block = header->boot_image_size;
	unsigned blocks = (header->image_size + 511) / 512;

	fill_disk_drive(header->boot_drive, &drive);
	while (blocks--) {
		read_block(&drive, block++, load_addr);
		load_addr += 512;
	}
}

void setup(struct boot_header *header)
{
	console_init();
	puts("Booting...");
	params.boot_header = *header;
	e820_detect(&params);
	e820_show(&params);
	enable_a20();
	load_kernel(&params.boot_header);
	setup_boot_gdt();
	enter_pm(params.boot_header.image_addr, &params);
}
