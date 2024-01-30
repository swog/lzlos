ENTRY(start)

/* Align kernel sections of kernel so that we can  */
SECTIONS {
	/* GNU Grub starts kernel at 1M */
	. = 1M;

	/* Boot section after boot is useless I believe */
	.boot : {
		KEEP(*(.boot))
	}

	_ehdr = .;
	.ehdr : {
		KEEP(*(.ehdr))
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

