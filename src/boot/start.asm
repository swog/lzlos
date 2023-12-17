bits 32
section .text

extern setup_paging
extern enable_paging
extern long_mode_start
global start
start:
	mov esp, stack_top
	
	; Save the physical address of the multiboot2 structure
	push ebx

	call clear

	call check_multiboot
	call check_cpuid
	call check_long_mode

	; Enable paging in 32 bit mode
	call setup_paging
	call enable_paging

	lgdt [gdt64r]

	; Can't pop 32 bit ebx in 64 bit
	pop ebx

	; Enter long mode	
	jmp gdt64.code_segment:long_mode_start

clear:
	push eax

	mov ecx, 0xb8000
	mov eax, 30*80
	
.loop:
	mov word [ecx+eax], 0

	sub eax, 2
	jnz .loop

	mov word [ecx], 0

	pop eax
	ret

	; ===========================================
	; GNU Grub should set EAX to 0x36d76289 magic number
check_multiboot:
	mov ecx, err_no_multiboot
	cmp eax, 0x36d76289
	jne error
	ret

	; ===========================================
	; EAX is not available after check_multiboot.
check_cpuid:
	pushfd
	pop eax

	mov ebx, eax	
	xor eax, 1 << 21

	push eax
	popfd

	pushfd
	pop eax

	push ebx
	popfd

	mov ecx, err_no_cpuid
	xor eax, ebx
	jz error

	ret

	; ===========================================

check_long_mode:
	mov eax, 0x80000000
	cpuid
	mov ecx, err_no_long_mode
	cmp eax, 0x80000001
	jb error

	mov eax, 0x80000001
	cpuid
	mov ecx, err_no_long_mode
	test edx, (1<<29)
	jz error

	ret

	; ECX - error string
	; AX - 
print:
	mov ebx, 0xb8000
	xor ah, ah
	or ah, 2
.loop:
	mov al, byte [ecx]
	test al, al
	jz .end
	
	mov word [ebx], ax
	add ebx, 2

	inc ecx
	jmp .loop
.end:
	ret

	; ===========================================
	; ECX should contain pointer to error string
error:
	call print
	cli
	hlt

align 4096
stack_bottom:
	resb 4096*4
stack_top:

section .rodata
err_no_cpuid:
	db "ERR: The system does not support CPUID! Haulting.", 0
err_no_multiboot:
	db "ERR: The bootloader used to boot this medium is not GNU Grub! Haulting.", 0
err_no_long_mode:
	db "ERR: Your system does not support 64 bit long mode! Haulting.", 0

section .data
global gdt64
gdt64:
	; Always starts with zero
	dq 0 ; zero entry
.code_segment: equ $ - gdt64
	dw 0
	dw 0
	db 0
	db (1<<3)|(1<<4)|(1<<7) ; Executable, code segment, present
	db (1<<5) ; Long mode
	db 0
.data_segment: equ $ - gdt64
	dw 0
	dw 0
	db 0
	db (1<<1)|(1<<4)|(1<<7) ; Writable, data segment, present
	db (1<<5) ; Long mode
	db 0
	; 0x18
.tss_segment: equ $ - gdt64
	dw tss.end - tss
	dw 0
	db 0
	db 0x9 | (1<<7) ; Available TSS type, present
	db 0
	db 0
gdt64r:
	dw $ - gdt64 - 1
	dq gdt64

global tss
tss:
	dd 0 ; Reserved
	dq stack_top ; Rsp0
	dq stack_top ; Rsp1
	dq stack_top ; Rsp2
	dq 0 ; Reserved
	dq stack_top ; Ist1
	dq stack_top ; Ist2
	dq stack_top ; Ist3
	dq stack_top ; Ist4
	dq stack_top ; Ist5
	dq stack_top ; Ist6
	dq stack_top ; Ist7
	dq 0 ; Reserved
	dd 0 ; IOPB
.end:
