bits 64

extern kernel_main
extern page_table_l4

section .text
	; Rdi-16 bit port
	; Rsi-8 bit output
global io_outb
io_outb:
	push rdx
	push rax	

	mov rdx, rdi
	mov rax, rsi

	out dx, al	

	pop rax
	pop rdx

	ret

	; Rdi-16 bit port
	; Rax-8 bit input
global io_inb:
io_inb:
	push rdx

	xor rax, rax
	mov rdx, rdi
	
	in al, dx

	pop rdx	

	ret

; This enables the interrupt flag
; The interrupt flag enables maskable interrupts
global set_interrupts
set_interrupts:
	sti
	ret

global set_cr3
set_cr3:
	mov cr3, rdi
	ret

global set_cr8
set_cr8:
	mov cr8, rdi
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

	; Setup interrupt descriptor table
	call idt_init

	call kernel_main

	jmp $

