bits 64

extern kmain
extern idt_init
global long_mode_start

section .text
	; ===========================================

long_mode_start:
	mov ax, 0x10
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ds, ax

	call idt_init
	call kmain

	int 49

	hlt

