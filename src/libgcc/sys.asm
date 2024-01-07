bits 64
section .text

global sys_read:
sys_read:
	xor rax, rax
	syscall
	ret

global sys_write:
sys_write:
	mov rax, 1
	syscall
	ret

global sys_open:
sys_open:
	mov rax, 2
	syscall	
	ret

global sys_close:
sys_close:
	mov rax, 3
	syscall
	ret
