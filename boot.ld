OUTPUT_ARCH(i8086)

SECTIONS {
	. = 0x7C00;
	.header : { *(.header) }

	. = 0x7DFE;
	.signature : { SHORT(0xAA55) }

	.text : { *(.text) }
	.rodata : { *(.rodata*) }
	.bss : { *(.bss) }

	.debug_aranges 0 : { *(.debug_aranges) }
	.debug_info 0 : { *(.debug_info) }
	.debug_abbrev 0 : { *(.debug_abbrev) }
	.debug_line 0 : { *(.debug_line) }
	.debug_str 0 : { *(.debug_str) }
}
