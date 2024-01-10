bits 64

; void scheduler_switch(lzlos_teb* teb)

section .text
scheduler_switch:
	push rbx ; Callee saved
	push rdi ; New TEB
	push rbp ; Stack

	pop rbp
	pop rdi
	pop rbx

	ret
