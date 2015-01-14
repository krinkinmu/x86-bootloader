#include "boot.h"
#include "console.h"
#include "utils.h"

void setup(struct boot_params *params)
{
	console_init();
	puts("Booting...");
	while (1);
}
