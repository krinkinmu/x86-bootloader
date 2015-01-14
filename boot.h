#ifndef __BOOT_H__
#define __BOOT_H__

void die(void) __attribute__((noreturn));

struct __attribute__ ((__packed__)) boot_params {
	unsigned long magic;
	unsigned long image_size;
	unsigned long image_addr;
	unsigned char boot_drive;
	unsigned char boot_image_size;
};

#endif /*__BOOT_H__*/
