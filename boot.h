#ifndef __BOOT_H__
#define __BOOT_H__

#define E820_MAX_MAP_SIZE 128
#define X86_CF_FLAG (1 << 0)

struct boot_header {
	unsigned long magic;
	unsigned long image_size;
	unsigned long image_addr;
	unsigned long boot_drive;
	unsigned long boot_image_size;
} __attribute__((packed));

struct e820entry {
	unsigned long addrl, addrh;
	unsigned long sizel, sizeh;
	unsigned long type;
	unsigned long unused;
} __attribute__((packed));

struct boot_params {
	struct boot_header boot_header;
	struct e820entry e820_mm[E820_MAX_MAP_SIZE];
	unsigned char e820_entries;
	unsigned char _padding[3];
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

static inline __attribute__((noreturn)) void die(void)
{
	while (1)
		asm("hlt");
}

static inline void cli(void)
{
	asm volatile("cli");
}

static inline void sti(void)
{
	asm volatile("sti");
}

static inline void io_delay(void)
{
	/* it is safe to use 0x80 port */
	asm volatile("outb %al, $0x80");
}

static inline void outb(unsigned char v, unsigned short port)
{
	asm volatile("movb %0, %%al; outb %%al, %1"
			:
			: "i"(v), "i"(port)
			: "al");
}

static inline unsigned char inb(unsigned short port)
{
	unsigned char v;

	asm volatile("inb %1, %%al; movb %%al, %0"
			: "=rm"(v)
			: "i"(port)
			: "al");
	return v;
}

static inline void outw(unsigned short v, unsigned short port)
{
	asm volatile("movw %0, %%ax; outw %%ax, %1"
			:
			: "i"(v), "i"(port)
			: "ax");
}

static inline unsigned short inw(unsigned short port)
{
	unsigned short v;

	asm volatile("inw %1, %%ax; movw %%ax, %0"
			: "=rm"(v)
			: "i"(port)
			: "ax");
	return v;
}

static inline void outl(unsigned long v, unsigned short port)
{
	asm volatile("movl %0, %%eax; outl %%eax, %1"
			:
			: "i"(v), "i"(port)
			: "eax");
}

static inline unsigned long inl(unsigned short port)
{
	unsigned long v;

	asm volatile("inl %1, %%eax; movl %%eax, %0"
			: "=rm"(v)
			: "i"(port)
			: "eax");
	return v;
}

static inline unsigned short get_ds(void)
{
	unsigned short seg;

	asm volatile("movw %%ds,%0" : "=rm"(seg));
	return seg;
}

static inline unsigned short get_es(void)
{
	unsigned short seg;

	asm volatile("movw %%es,%0" : "=rm"(seg));
	return seg;
}

static inline void set_fs(unsigned short seg)
{
	asm volatile("movw %0,%%fs" : : "rm"(seg));
}

static inline void set_gs(unsigned short seg)
{
	asm volatile("movw %0,%%gs" : : "rm"(seg));
}

static inline unsigned long readl_gs(unsigned short off)
{
	unsigned long v;

	asm volatile("movl %%gs:%1,%0" : "=q"(v) : "m"(off));
	return v;
}

static inline void writel_gs(unsigned short off, unsigned long v)
{
	asm volatile("movl %0,%%gs:%1" : : "qi"(v), "m"(off));
}

static inline unsigned long readl_fs(unsigned short off)
{
	unsigned long v;

	asm volatile("movl %%fs:%1,%0" : "=q"(v) : "m"(off));
	return v;
}

static inline void writel_fs(unsigned short off, unsigned long v)
{
	asm volatile("movl %0,%%fs:%1" : : "qi"(v), "m"(off));
}

void initregs(struct biosregs *ireg);
void intcall(int intno, const struct biosregs *ireg, struct biosregs *oreg);

int e820_detect(struct boot_params *params);
void e820_show(const struct boot_params *params);

int enable_a20(void);

#endif /*__BOOT_H__*/
