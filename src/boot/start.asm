
; Character high byte of word
; Background is hi 4 bits, fg lo 4 bits
; Clear vga memory & print info if needed
section .vga
vga_start:
	times 25*80*2+vga_start-$ db 0

section .text
global start
extern long_mode_start
bits 32

start:
	mov esp, stack_top

	call check_multiboot
	call check_cpuid
	call check_long_mode

	call setup_page_tables
	call enable_paging

	lidt [idt64.pointer]

	sti

	lgdt [gdt64.pointer]
	jmp gdt64.code_segment:long_mode_start

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
	ret

	; ===========================================
	; Set cr3, CR4.PAE=1, IA32_EFER.LME=1, CR0.PAE=1
enable_paging:
	mov eax, page_table_l4
	mov cr3, eax

	; Enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; IA32_EFER extended features
	mov ecx, 0xC0000080
	rdmsr
	; .LME long mode enable
	or eax, 1 << 8
	wrmsr

	; Enable paging flag 1<<31 in cr0
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	ret

	; ===========================================

setup_page_tables:
	mov eax, page_table_l3
	or eax, 0b11 ; present, writable
	mov [page_table_l4], eax

	mov eax, page_table_l2
	or eax, 0b11 ; present, writable
	mov [page_table_l3], eax

	mov ecx, 0 ; counter

.loop:
	; multiply eax with ecx counter
	mov eax, 0x200000 ; 2 mb
	mul ecx
	; add flags
	or eax, 0b10000011 ; present, writable, huge page
	
	mov [page_table_l2 + ecx * 8], eax

	inc ecx
	cmp ecx, 1
	jne .loop

	; Set second 2mb page to read only data (.text & .ro)
	; This in conjunction with the linker will set make .text & .ro sections for the kernel

	; 2 mb pages, set second page to read only
	mov eax, 0x200000
	or eax, 0b10000101 ; read only, present, supervisor

	mov [page_table_l2 + 8], eax

	ret

	; ===========================================
	; Divide by zero
section .isrs
interrupt_dbz:
	pushad
	cld

	mov ecx, err_divide_by_zero
	jmp error

	popad
	iret

	; ===========================================
	; ECX should contain pointer to error string
error:
	; EBX is vga iterator
	mov ebx, 0xb8000

	; Set background to 0
	; Set foreground to green
	xor ah, ah
	or ah, 2
.loop:
	; EAX is current character
	mov al, [ecx]
	test al, al
	jz .end

	; Set character (hi byte)
	mov byte [ebx], al
	inc ebx
	; Set color (lo byte)
	mov byte [ebx], ah
	inc ebx

	; Increment 1 ch
	inc ecx

	jmp .loop

.end:
	cli
	hlt

	; ===========================================
	; pg. 3117
section .bss
align 4096
page_table_l4:
	resb 4096
page_table_l3:
	resb 4096
page_table_l2:
	resb 4096
stack_bottom:
	resb 4096*4
stack_top:

section .rodata
err_divide_by_zero:
	db "ERR: Divide by zero", 0
err_no_cpuid:
	db "ERR: The system does not support CPUID", 0
err_no_multiboot:
	db "ERR: Not GNU grub multiboot", 0
err_no_long_mode:
	db "ERR: No long mode supported", 0

gdt64:
	; Always starts with zero
	dq 0 ; zero entry
.code_segment: equ $ - gdt64
	; Global data table descriptor
	; Kernel code segment
	; Access byte flags:
	; 	1<<43 - executable bit
	; 	1<<44 - descriptor type bit (code or data segment); 
	;	1<<47 - present bit
	%define ACCESS_EXEC (1<<43)
	%define ACCESS_CODEDATA (1<<44)
	%define ACCESS_PRESENT (1<<47)

	; Nibble flags:
	;	1<<55 - granularity (the scale of units in the lower word specifying the size. 0 specifies bytes)
	;	1<<53 - long mode
	%define FLAGS_LONG (1<<53)
	
	dq ACCESS_EXEC | ACCESS_CODEDATA | ACCESS_PRESENT | FLAGS_LONG ; code segment
	dq ACCESS_CODEDATA | ACCESS_PRESENT | FLAGS_LONG ; data segment second page
.pointer:
	dw $ - gdt64 - 1
	dq gdt64

idt64:
	%define IDT_PRESENT (1<<15)
	%define IDT_DPL_R3 3
	%define IDT_DPL_R0 0

	; offset, flags, segment
	%macro IDT_ENTRY 3
		dd 0 ; reserved
		dd ((%1 & 0xffff_ffff_0000_0000) >> 32)
		dw ((%1 & 0xffff_0000) >> 16)
		dw %2
		dw %3
		dw (%1 & 0xffff)
	%endmacro

	IDT_ENTRY 0x100000, IDT_PRESENT, 0
.pointer:
	dw $ - idt64 - 1
	dq idt64