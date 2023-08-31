bits 32

; Character high byte of word
; Background is hi 4 bits, fg lo 4 bits
; Clear vga memory & print info if needed
section .vga
vga:
	times 25*80*2+vga-$ db 0

section .text
extern long_mode_start
extern page_table_l4
global start

start:
	mov esp, stack_top

	call check_multiboot
	call check_cpuid
	call check_long_mode

	call setup_paging
	call enable_paging

	lgdt [gdt64r]

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

	; Setup 2MB paging for kernel setup
setup_paging:
	; PML4
	mov eax, page_table_l3
	or eax, 0b11
	mov dword [page_table_l4], eax

	; Directory Ptr
	mov eax, page_table_l2
	or eax, 0b11
	mov dword [page_table_l3], eax

	; Directory
	mov eax, 0
	or eax, 0b10000011
	mov dword [page_table_l2], eax

	ret

	; ===========================================
	; Set CR3, CR4.PAE=1, IA32_EFER.LME=1, CR0.PAE=1
	; CR0.PG=1,CR4.PAE=1,IA32_EFER.LME=1,CR4.LA57=0 Level 4
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
page_table_l1:
	resb 4096
stack_bottom:
	resb 4096*4
stack_top:

section .rodata
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
gdt64r:
	dw $ - gdt64 - 1
	dq gdt64