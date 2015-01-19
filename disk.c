#include "boot.h"
#include "disk.h"
#include "string.h"

struct chs_addr {
	unsigned cylinder;
	unsigned head;
	unsigned sector;
};

static void lba_to_chs(const struct disk_drive *info, unsigned block,
		struct chs_addr *addr)
{
	const unsigned tmp = block / info->sectors;

	addr->sector = (block % info->sectors) + 1;
	addr->head = tmp % info->heads;
	addr->cylinder = tmp / info->heads;
}

int fill_disk_drive(unsigned driveno, struct disk_drive *drive)
{
	struct biosregs iregs, oregs;

	initregs(&iregs);
	iregs.ah = 8;
	iregs.dl = driveno;
	intcall(0x13, &iregs, &oregs);

	drive->driveno = driveno;
	drive->heads = oregs.dh + 1;
	drive->sectors = oregs.cl & 0x3f;

	/* TODO: check for errors */

	return 0;
}

int read_block(const struct disk_drive *drive, unsigned block, void *memory)
{
	struct biosregs iregs, oregs;
	unsigned char buffer[512];
	struct chs_addr chs;

	lba_to_chs(drive, block, &chs);
	initregs(&iregs);
	iregs.ah = 2;
	iregs.al = 1;
	iregs.ch = chs.cylinder & 0xFF;
	iregs.cl = chs.sector | ((chs.cylinder >> 2) & 0xC0);
	iregs.dh = chs.head;
	iregs.dl = drive->driveno;
	iregs.bx = (unsigned long)buffer;
	intcall(0x13, &iregs, &oregs);

	/* TODO: check for errors */

	memcpy(buffer, memory, 512);

	return 0;
}
