#ifndef __DISK_H__
#define __DISK_H__

struct disk_drive {
	unsigned driveno;
	unsigned heads;
	unsigned sectors;
};

int fill_disk_drive(unsigned driveno, struct disk_drive *drive);
int read_block(const struct disk_drive *drive, unsigned block, void *memory);

#endif /*__DISK_H__*/
