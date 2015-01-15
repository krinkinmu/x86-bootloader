#include "boot.h"
#include "console.h"

static struct boot_params params;

void setup(struct boot_header *header)
{
	console_init();
	puts("Booting...");

	if (!detect_memory(&params)) {
		puts("Failed to detect memory... Dying!");
		die();
	}

	while (1);
}
