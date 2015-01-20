struct gdt_ptr {
	unsigned short size;
	unsigned long addr;
} __attribute__((packed));

#define GDT_ENTRY(base, limit, flags)		\
	(((limit) & 0x0000ffffull)        |	\
	(((limit) & 0x000f0000ull) << 32) |	\
	(((base)  & 0x00ffffffull) << 16) |	\
	(((base)  & 0xff000000ull) << 32) |	\
	(((flags) & 0x0000f0ffull) << 40))

static const unsigned long long boot_gdt[] __attribute__((aligned(16))) = {
	GDT_ENTRY(0, 0, 0),            /* null entry */
	GDT_ENTRY(0, 0xfffff, 0xc09a), /* code segment */
	GDT_ENTRY(0, 0xfffff, 0xc092), /* data segment */
};

static struct gdt_ptr ptr;

void setup_boot_gdt(void)
{
	ptr.size = sizeof(boot_gdt) - 1;
	ptr.addr = (unsigned long)&boot_gdt;

	asm volatile("lgdtl %0" : : "m"(ptr));
}
