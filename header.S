	.code16
	.section ".header", "ax"
	.global boot_start

boot_start:
	ljmp	$0x0000, $real_start

boot_params:
	.long	0x13131313	/* magic number */
kernel_size:
	.long	0x00000000	/* kernel image size */
load_address:
	.long	0x00000000	/* kernel load addr */
drive_number:
	.byte	0x00		/* boot drive number */
setup_sectors:
	.byte	0x06		/* bootloader size in 512 byte blocks */

real_start:
	xorl	%eax, %eax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss
	movl	%eax, %esp

	movb	%dl, drive_number
	movb	setup_sectors, %al
	dec	%al

	/*
	 * dl - drive number
	 * dh - head, set it to 0
	 */
	xorb	%dh, %dh
	/*
	 * ah - function number, 2 in this case
	 */
	movb	$0x02, %ah
	/*
	 * cl - (sector | (cylinder >> 2) & 0xC0), set it to 0
	 * ch - cylinder & 0xff, set it to 0
	 */
	movw	$0x0002, %cx
	/*
	 * bx - offset to load, all data will be loaded to es:bx, so
	 *      set it to 0x7C00 + 0x0200 = 0x7E00, right after bootsector
	 */
	movw	$0x7E00, %bx
	int	$0x13

	pushl	$boot_params
	calll	setup

	.global die
	.type die, @function
die:
	hlt
	jmp	die
	.size die, .-die
