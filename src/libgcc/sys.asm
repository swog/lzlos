bits 64
section .text

%macro sys 2
global %1
%1:
	mov rax, %2
	int 0x80
	ret
%endmacro

; Many MANY os system calls
; These include networking functions

sys fread, 		0
sys fwrite,		1
sys fopen, 		2
sys fclose, 	3
sys stat, 		4
sys fstat, 		5
sys lstat, 		6
sys poll, 		7
sys lseek, 		8
sys mmap, 		9
sys mprotect, 	10
sys munmap, 	11
sys brk,		12

sys ioctl, 		16

sys execve,		59
sys exit,		60







