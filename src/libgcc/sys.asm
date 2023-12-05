bits 64

section .text

global sys_read:
sys_read:
	xor rax, rax
	int 0x80
	ret

global sys_write:
sys_write:
	mov rax, 1
	int 0x80
	ret

global sys_open:
sys_open:
	mov rax, 2
	int 0x80
	ret

global sys_close:
sys_close:
	mov rax, 3
	int 0x80
	ret
