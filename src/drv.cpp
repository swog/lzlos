#include <stdlib.h>
#include <stdio.h>
#include "drv.h"
#include "kdefs.h"

size_t drv_size(lzlos_teb* teb, void **base) {
	Elf64_Ehdr* eh = (Elf64_Ehdr*)teb->mod->fbase;
	unsigned char* ph_addr;
	const Elf64_Phdr* ph;
	// Base virtual address of the executable
	size_t vbase = -1;
	size_t size = 0;
	size_t segment_end;

	for (size_t i = 0; i < eh->e_phnum; i++) {
		ph = elf_program(teb->mod->fsize, teb->mod->fbase, i);
		
		// base = min(base, ph_vaddr)
		if (ph->p_vaddr < vbase) {
			vbase = ph->p_vaddr;
		}
	}

	for (size_t i = 0; i < eh->e_phnum; i++) {
		ph = elf_program(teb->mod->fsize, teb->mod->fbase, i);
		// This program header's segment size
		segment_end = ph->p_vaddr - vbase + ph->p_memsz;
		
		if (segment_end > size) {
			size = segment_end;
		}
	}

	if (base) {
		*base = (void*)vbase;
	}

	return size;
}

typedef int (*drv_startfn)(int argc, char** argv);

int drv_open(lzlos_teb* teb) {
	Elf64_Ehdr* eh = (Elf64_Ehdr*)teb->mod->fbase;
	unsigned char* ph_addr;
	const Elf64_Phdr* ph;

	// Copy elf header to image
	memcpy(teb->mod->ibase, teb->mod->fbase, sizeof(Elf64_Ehdr));

	printf("Open: ImageBase(%p), PhOff(%u), PhEntSize(%u)\n", teb->mod->fbase, eh->e_phoff, eh->e_phentsize);

	void* text = NULL;
	void* ph_ptr;

	for (size_t i = 0; i < eh->e_phnum; i++) {
		ph = elf_program(teb->mod->fsize, teb->mod->fbase, i);
		ph_ptr = (char*)teb->mod->ibase + eh->e_phoff + eh->e_phentsize * i;
		memcpy(ph_ptr, ph, sizeof(*ph));	
	
		// Executable
		if (ph->p_flags & 1) {
			printf("1 executable\n");
			text = ph_ptr;
		}
	}

	printf("text: %p\n", text);

	drv_startfn start = (drv_startfn)text;

	return 0;
}
