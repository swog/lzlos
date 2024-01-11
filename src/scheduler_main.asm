bits 64

; LZLOS Thread Environment Block
;typedef struct _lzlos_teb {
;	0x0
;	size_t 	image_size;		// Process image size
;	0x8
;	void* 	image_base; 		// Process virtual address
;	0x10
;	void*	stack;			// Virtual address
;	0x18
;	void*	pagetable;		// Page table linear address
;	struct _lzlos_teb* prev;
;	struct _lzlos_teb* next;
;	unsigned char	flags;		// Busy, etc
;	unsigned char	priority;	// Higher priority
;	size_t		uptime;		// CPU time
;	char	name[TEB_NAMESIZE];	// Process name (path), paged
;} lzlos_teb;

; void scheduler_switch(lzlos_teb* teb)

extern scheduler_task

section .text

global scheduler_switch
scheduler_switch:
	; Save these to the current stack.
	push rbp	

	; Current task
	call scheduler_task

	; Save stack pointer
	mov qword [rax+0x10], rsp
	
	; Save pagetable
	mov rbx, cr3
	mov qword [rax+0x18], rbx
	
	; Load new stack pointer
	mov rsp, qword [rdi+0x10]

	; Load new page table into rcx
	mov rcx, qword [rdi+0x18]
	
	; Check if we need to switch
	cmp rcx, rbx	
	je .ld

	; Load new page table
	mov cr3, rcx

.ld:
	; Load these from the new stack
	pop rbp

	ret











