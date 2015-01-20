	.text
	.code16
	.global enter_pm
enter_pm:
	popl	%ecx

	movl	%cr0, %eax
	orb	$0x01, %al
	movl	%edx, %cr0
	jmp	1f
1:

	movw	$0x0010, %ax


	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %ss
	movw	%ax, %fs
	movw	%ax, %gs

	ljmpl	$0x0008, $pm_entry

	.code32
pm_entry:
	/* stub */
1:
	jmp	1b
	jmp	*%ecx
