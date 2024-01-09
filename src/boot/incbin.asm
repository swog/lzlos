bits 64

section .data

global _lzlos_sys_base
global _lzlos_sys_end

_lzlos_sys_base:
	incbin "linker.ld"
_lzlos_sys_end:
