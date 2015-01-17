#include "boot.h"
#include "console.h"

static struct boot_params params __attribute__((aligned(16)));

void setup(struct boot_header *header)
{
	console_init();
	puts("Booting...");
	params.boot_header = *header;

	e820_detect(&params);
	e820_show(&params);

	enable_a20();
}
