; ===========================================
; 32 bit paging
;
; This will still work when we enter 64 bit mode.
; ===========================================

bits 32
section .data

	; ===========================================
	; Intel pg. 3117
section .bss
align 4096
global page_table_l4
page_table_l4:
	resb 4096
page_table_l3:
	resb 4096
page_table_l2:
	resb 4096
page_table_l1:
	resb 4096

section .text

	; ===========================================
	; Setup 2MB paging for kernel setup
global setup_paging
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
	mov eax, 0b10000011
	mov dword [page_table_l2], eax

	; Kernel heap base
	or eax, 0x200000
	mov dword [page_table_l2+8], eax

	ret

	; ===========================================
	; Set CR3, CR4.PAE=1, IA32_EFER.LME=1, CR0.PAE=1
	; CR0.PG=1,CR4.PAE=1,IA32_EFER.LME=1,CR4.LA57=0 Level 4
global enable_paging
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
	; .LME long mode enable & no-exec
	or eax, (1<<8);|(1<<11)
	wrmsr

	; Enable paging flag 1<<31 in cr0
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	ret

