
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
	"Divide Error",
	"Debug Exception",
	"Nonmaskable external interrupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invaild Opcode",
	"Device Not Available", 
	"Double Fault", 
	"Co-processor Segment Overrun",
	"Invaild TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection",
	"Page Fault",
	"Reserved",
	"x87 Floating Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Excpetion",
	"Control Protection Exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	
	// Kernel-defined exceptions
};

void set_cr3(void *pagetable);

static ALIGNED(4096) u64 pml4[256];
static ALIGNED(4096) u64 directory_ptr[256];
static ALIGNED(4096) u64 directory[256];
static ALIGNED(4096) u64 table[512];

void kernel_setup_paging() {
	pml4[0] = (u64)directory_ptr | 0b11;
	pml4[255] = (u64)directory_ptr | 0b11;

	directory_ptr[0] = (u64)directory | 0b11;
	directory_ptr[255] = (u64)directory | 0b11;
	directory[0] = (u64)table | 0b11;

	u64 physical = 0x100000;

	// Map upper 0x100_000 kernel start
	for (int i = 256; i < 512; i++) {
		table[i] = physical | 0b11;
		physical += 0x1000;
	}

	// Map 0xb8000 VGA
	table[0xb8] = 0xb8000 | 0b11;

	set_cr3(pml4);

	*(u16*)0x7f80000b8000 = ' a';
	asm("hlt");
}

void kernel_isrhandler(kisrcall_t *info) {
	if (info->isr_number < sizeof(exception_names)/sizeof(exception_names[0])) {
		vga_printf("Fatal exception: %s\n", exception_names[info->isr_number]);

		asm("hlt");
	}

	vga_printf("isr: %p\n", info->isr_number);
}

void kernel_main() {
	vga_printf("%p\n", kernel_main);
	vga_puts("Hello, world\n");
}
