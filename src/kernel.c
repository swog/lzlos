
#include "vga.h"
#include "ktypes.h"

typedef struct kisrcall_s {
	u64 r15;
    u64 r14;
    u64 r13;
    u64 r12;
    u64 r11;
    u64 r10;
    u64 r9;
    u64 r8;
    u64 rsi;
    u64 rdi;
    u64 rbp;
    u64 rdx;
    u64 rcx;
    u64 rbx;
    u64 rax;
    u64 isr_number;
    u64 err_code;
    u64 rip;
    u64 cs; 
    u64 rflags; 
    u64 rsp;
    u64 ss;
} kisrcall_t;

static const char* exception_names[] = {
	"Divide by zero",
	"Debug",
	"NMI",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invaild Opcode",
	"Device Not Available", 
	"Double fault", 
	"Co-processor Segment Overrun",
	"Invaild TSS",
	"Segment not present",
	"Stack-Segment Fault",
	"GPF",
	"Page Fault",
	"Reserved",
	"x87 Floating Point Exception",
	"allignement check",
	"Machine check",
	"SIMD floating-point exception",
	"Virtualization Excpetion",
	"Deadlock",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Security Exception",
	"Reserved",
	"Triple Fault",
	"FPU error"
};

extern u32 *page_table_l4;
extern u32 *page_table_l3;
extern u32 *page_table_l2;
extern u32 *page_table_l1;

void kernel_setup_paging() {
	u32 addr;

	addr = (u32)page_table_l3;
	page_table_l4[0] = addr | 0b11;

	addr = (u32)page_table_l2;
	page_table_l3[0] = addr | 0b11;
	
	addr = (u32)page_table_l1;
	page_table_l2[0] = addr | 0b11;

	addr = 0;
	page_table_l1[0] = addr | 0b10000011;
}

void kernel_isrhandler(kisrcall_t *info) {
	if (info->isr_number < 32) {
		vga_printf("Fatal exception: %s\n", exception_names[info->isr_number]);
		
		asm("hlt");
	}

	vga_printf("isr: %p\n", info->isr_number);
}

void kernel_main() {
	vga_puts("Hello, world\n");
}
