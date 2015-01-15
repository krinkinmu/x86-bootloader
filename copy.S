	.code16
	.text

	.global memcpy
	.type memcpy, @function
memcpy:
	pushl	%ebp
	movl	%esp, %ebp

	pushl	%esi
	pushl	%edi
	pushl	%ecx

	movl	0x10(%ebp), %ecx
	movl	0x0c(%ebp), %esi
	movl	0x08(%ebp), %edi

	cld
/**
 * I want the following code use edi/esi instead di/si, but clang doesn't
 * support explicit addr32 prefix.
 *
 *	rep
 *	addr32
 *	movsb
 **/
	.byte 0xf3, 0x67, 0xa4

	popl	%ecx
	popl	%edi
	popl	%esi

	popl	%ebp
	retl
	.size memcpy, .-memcpy

	.global memset
	.type memset, @function
memset:
	pushl	%ebp
	movl	%esp, %ebp

	pushl	%eax
	pushl	%edi
	pushl	%ecx

	movl	0x10(%ebp), %ecx
	movl	0x0c(%ebp), %eax
	movl	0x08(%ebp), %edi

	cld
/**
 * The same thing again - prevent clang builded image fail
 *
 *	rep
 *	addr32
 *	stosb
 **/
	.byte 0xf3, 0x67, 0xaa

	popl	%ecx
	popl	%edi
	popl	%eax

	popl	%ebp
	retl
	.size memset, .-memset
