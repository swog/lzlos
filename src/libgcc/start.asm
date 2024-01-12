bits 64

section .text

EXIT_SUCCESS equ 0

extern main

global _start
_start:
	call main

	; Call exit(0)
	mov rax, 0x60
	mov rdi, EXIT_SUCCESS
	int 0x80

	ret
