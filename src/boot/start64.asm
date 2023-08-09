
section .text
extern kernel_main
extern gdt64
global long_mode_start
bits 64

long_mode_start:
	; Load null into all data segment registers
	mov ax, 0
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ds, ax

	call kernel_main
	
	cli
	hlt