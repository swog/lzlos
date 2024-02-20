bits 64

section .text

EXIT_SUCCESS equ 0

extern main

global _start
_start:
	call main

	; Give rax main return value to _exit
	mov rdi, rax
	mov rax, 0x60
	int 0x80

	ret
