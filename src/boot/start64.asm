bits 64

extern kernel_main
extern kernel_setup_paging

section .text
	; ===========================================
	; void lock_acquire(char* lock);
	; void lock_release(char* lock);
	; void spinlock(char* lock);
	;
	; A free spinlock is 0
global set_cr3
set_cr3:
	mov cr3, rdi
	ret

extern tss
extern idt_init
extern gdt64
global long_mode_start
long_mode_start:
	mov ax, 0x10
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ds, ax

	; Load task state register (broken)
	mov rax, gdt64+0x18
	mov rbx, tss
	mov word [rax+2], bx
	shr rbx, 16
	mov byte [rax+4], bl
	mov byte [rax+7], bh

	mov ax, 0x18
	;ltr ax

	call idt_init

	;sti

	call kernel_setup_paging
	call kernel_main

	;int 49

	hlt

