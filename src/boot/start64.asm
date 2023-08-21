bits 64

section .data
idt:
	times 50 dq 0
idtr:
	dw $ - idt - 1
	dq idt

section .text
extern kernel_main
extern kernel_int_stub
extern kernel_int_test
global int_test
global int_stub
global idt_load
global idt_call
global long_mode_start

	; ===========================================
	; Default
int_stub:
	iretq

int_test:
	;cli
	mov ax, 0x10
	mov gs, ax
	mov dword [gs:0xb8000], ') : '
	;hlt
	iret

idt_load:
	lidt [rdi]
	ret

idt_call:
	int 49
	ret

idt_init:
	mov rbx, idt
.loop:
	mov rax, int_test
	mov [rbx], ax
	mov word [rbx+2], 0x8
	mov word [rbx+4], 0x8e00
	shr rax, 16
	mov [rbx+6], ax
	shr rax, 16
	mov [rbx+8], rax
	add rbx, 16
	cmp rbx, idtr
	jne .loop

	lidt [idtr]
	;sti
	int 49
	ret

	; ===========================================

long_mode_start:
	mov ax, 0x10
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ds, ax

	call idt_init

	;call kernel_main

	jmp $