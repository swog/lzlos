bits 64

extern kmain
extern kidtstubs
extern kpremain
global long_mode_start

section .data
idt:
	times 256 dq 0
idtr:
	dw $ - idt - 1
	dq idt

section .text
	; ===========================================
	; RBX-IDT destination
	; RAX-Address
	; CX-Increment

idt_init:
	mov rbx, idt
	mov rcx, kidtstubs
.loop:
	mov rax, qword [rcx]
	
	mov word [rbx], ax
	mov word [rbx+2], 0x8
	mov word [rbx+4], 0x8e00
	shr rax, 16
	mov word [rbx+6], ax
	shr rax, 16
	mov qword [rbx+8], rax
	
	add rbx, 16
	add rcx, 16
	
	cmp rbx, idtr
	jne .loop

	lidt [idtr]
	;sti
	ret

	; ===========================================

long_mode_start:
	mov ax, 0x10
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ds, ax

	call kpremain
	call idt_init
	call kmain

	int 49

	jmp $