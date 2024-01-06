bits 64
section .text

IRQ_SYSCALL equ 0x80

global sys_read:
sys_read:
	xor rax, rax
	int IRQ_SYSCALL
	ret

global sys_write:
sys_write:
	mov rax, 1
	int IRQ_SYSCALL
	ret

global sys_open:
sys_open:
	mov rax, 2
	int IRQ_SYSCALL
	ret

global sys_close:
sys_close:
	mov rax, 3
	int IRQ_SYSCALL
	ret
