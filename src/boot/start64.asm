bits 64

extern kernel_main
extern kernel_setup_paging
extern page_table_l4
extern idt_init
global long_mode_start

section .text
	; ===========================================
	; void lock_acquire(char* lock);
	; void lock_release(char* lock);
	; void spinlock(char* lock);
	;
	; A free spinlock is 0

long_mode_start:
	mov ax, 0x10
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ds, ax

	call idt_init
	mov rdi, page_table_l4
	call kernel_setup_paging
	call kernel_main

	;int 49

	hlt

