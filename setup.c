#include "boot.h"
#include "console.h"

static struct boot_params params __attribute__((aligned(16)));

void setup(struct boot_header *header)
{
	console_init();
	puts("Booting...");

	if (!e820_detect(&params)) {
		puts("Failed to detect memory... Dying!");
		die();
	}
	e820_show(&params);

	while (1);
}
