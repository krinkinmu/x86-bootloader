#ifndef __BOOT_H__
#define __BOOT_H__

#define E820_MAX_MAP_SIZE 128
#define X86_CF_FLAG (1 << 0)

void die(void) __attribute__((noreturn));

struct boot_header {
	unsigned long magic;
	unsigned long image_size;
	unsigned long image_addr;
	unsigned char boot_drive;
	unsigned char boot_image_size;
} __attribute__((packed));

struct e820entry {
	unsigned long addrl, addrh;
	unsigned long sizel, sizeh;
	unsigned long type;
	unsigned long unused;
} __attribute__((packed));

struct boot_params {
	struct e820entry e820_mm[E820_MAX_MAP_SIZE];
	unsigned char e820_entries;
} __attribute__((packed));

struct biosregs {
	union {
		struct {
			unsigned long edi;
			unsigned long esi;
			unsigned long ebp;
			unsigned long _esp; /* unused */
			unsigned long ebx;
			unsigned long edx;
			unsigned long ecx;
			unsigned long eax;
			unsigned long _fsgs; /* unused */
			unsigned long _dses; /* unsued */
			unsigned long eflags;
		};
		struct {
			unsigned short di, _hdi;
			unsigned short si, _hsi;
			unsigned short bp, _hbp;
			unsigned short _sp, _hsp;
			unsigned short bx, _hbx;
			unsigned short dx, _hdx;
			unsigned short cx, _hcx;
			unsigned short ax, _hax;
			unsigned short gs, fs;
			unsigned short es, ds;
			unsigned short flags, hflags;
		};
		struct {
			unsigned char dil, dih, _edi2, _edi3;
			unsigned char sil, sih, _esi2, _esi3;
			unsigned char bpl, bph, _ebp2, _ebp3;
			unsigned char _spl, _sph, _esp2, _esp3;
			unsigned char bl, bh, _ebx2, _ebx3;
			unsigned char dl, dh, _edx2, _edx3;
			unsigned char cl, ch, _ecx2, _ecx3;
			unsigned char al, ah, _eax2, _eax3;
			unsigned char _gsl, _gsh, _fsl, _fsh;
			unsigned char _esl, _esh, _dsl, _dsh;
			unsigned char _fl, _fh, _ef2, _ef3;
		};
	};
};

static inline unsigned short get_ds(void)
{
	unsigned short seg;
	asm("movw %%ds,%0" : "=rm"(seg));
	return seg;
}

static inline unsigned short get_fs(void)
{
	unsigned short seg;
	asm("movw %%fs,%0" : "=rm"(seg));
	return seg;
}

static inline unsigned short get_gs(void)
{
	unsigned short seg;
	asm("movw %%gs,%0" : "=rm"(seg));
	return seg;
}

static inline unsigned short get_es(void)
{
	unsigned short seg;
	asm("movw %%es,%0" : "=rm"(seg));
	return seg;
}

void initregs(struct biosregs *ireg);
void intcall(unsigned char intno, const struct biosregs *ireg,
		struct biosregs *oreg);

int e820_detect(struct boot_params *params);
void e820_show(const struct boot_params *params);

#endif /*__BOOT_H__*/
