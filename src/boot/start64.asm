bits 64

extern kernel_main
extern kernel_setup_paging
extern page_table_l4
extern idt_init
extern gdt64
extern stack_top
global long_mode_start
global set_cr3

section .text
	; ===========================================
	; void lock_acquire(char* lock);
	; void lock_release(char* lock);
	; void spinlock(char* lock);
	;
	; A free spinlock is 0
set_cr3:
	mov cr3, rdi
	ret

long_mode_start:
	mov ax, 0x10
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ds, ax

	mov ax, 0x18
	ltr ax

	jmp $

	call idt_init
	call kernel_setup_paging
	call kernel_main

	;int 49

	hlt

