; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#Header-layout

section .boot

header_start:
	magic 		dd 0xe85250d6
	arch 		dd 0
	length		dd header_end - header_start

	; Must have a uint32 sum of zero with above fields
	checksum	dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

	; multiboot_header_tag
	; uint16 type
	dw 0
	; uint16 flags
	dw 0
	; uint32 size (2w+1dw)
	dd 8
header_end: