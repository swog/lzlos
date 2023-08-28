ENTRY(start)

/* Align kernel sections of kernel so that we can  */
SECTIONS {
	. = 0xb8000;
	.vga : {
		*(.vga)
	}

	/* GNU Grub starts kernel at 1M */
	. = 1M;

	/* Boot section after boot is useless I believe */
	.boot : {
		KEEP(*(.boot))
	}

	.text : {
		*(.text)
	}

	/* Stack & page tables */
	.bss : {
		*(.bss)
	}

	.data : {
		*(.data)
	}

	.rodata : {
		*(.rodata)
	}
}

