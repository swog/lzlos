#include <stdint.h>
#include <stddef.h>
#include "elf.h"
#include "libelf.h"

// Check magic numbers
bool elf_magic(size_t size, const void* base) {
	if (size < 4) {
		return false;
	}

	unsigned char* e_ident = (unsigned char*)base;

	return e_ident[EI_MAG0] == ELFMAG0 &&
		e_ident[EI_MAG1] == ELFMAG1 &&
		e_ident[EI_MAG2] == ELFMAG2 &&
		e_ident[EI_MAG3] == ELFMAG3;
}

// Return
// -1 - Size does not contain header.
// ET_NONE - No type?
// ET_REL - Relocatable object file
// ET_EXEC - Executable file
// ET_DYN - Shared object file
// ET_CORE - Core file
int elf_type(size_t size, const void* base) {
	if (size < sizeof(Elf64_Ehdr)) {
		return -1;
	}

	return ((const Elf64_Ehdr*)base)->e_type;
}

// EM_NONE - No machine?
// EM_SPARC - SPARC ?
// EM_386 - 80386
// EM_X86_64 - AMD x86_64
int elf_machine(size_t size, const void* base) {
	if (size < sizeof(Elf64_Ehdr)) {
		return -1;
	}

	return ((const Elf64_Ehdr*)base)->e_machine;
}

// Entry point
const void* elf_entry(size_t size, const void* base) {
	return (const void*)((const Elf64_Ehdr*)base)->e_entry;
}






// Program header
// Pass non-consts into constant function.
Elf64_Phdr* elf_program(size_t size, void* base, size_t index) {
	return (Elf64_Phdr*)elf_program(size, (const void*)base, index);
}

const Elf64_Phdr* elf_program(size_t size, const void* base, size_t index) {
	if (size < sizeof(Elf64_Ehdr)) {
		return NULL;
	}

	Elf64_Off phoff = ((Elf64_Ehdr*)base)->e_phoff;

	if (!phoff) {
		return NULL;
	}

	size_t tableoff = ((Elf64_Ehdr*)base)->e_shentsize*index;

	if (size < phoff+tableoff) {
		return NULL;
	}

	return (Elf64_Phdr*)(((char*)base)+phoff+tableoff);	
}

const Elf64_Shdr* elf_section(size_t size, const void* base, size_t index) {
	if (size < sizeof(Elf64_Ehdr)) {
		return NULL;
	}

	Elf64_Off shoff = ((Elf64_Ehdr*)base)->e_shoff;

	if (!shoff) {
		return NULL;
	}

	size_t tableoff = ((Elf64_Ehdr*)base)->e_shentsize*index;

	if (size < shoff+tableoff) {
		return NULL;
	}

	return (Elf64_Shdr*)(((char*)base)+shoff+tableoff);	
}	

// Section header by index
// Pass non-const into const function.
Elf64_Shdr* elf_section(size_t size, void* base, size_t index) {
	return (Elf64_Shdr*)elf_section(size, (const void*)base, index);
}



// Section header count
size_t elf_section_count(size_t size, const void* base) {
	if (size < sizeof(Elf64_Ehdr)) {
		return -1;
	}

	return ((const Elf64_Ehdr*)base)->e_shnum;
}

// Get section name table.
// Elf64_Shdr's sh_name is an offset into this table.
const char* elf_section_names(size_t size, const void* base) {
	size_t strndx = elf_sectionidx_stringtable(size, base);
	const Elf64_Shdr* strshdr = elf_section(size, base, strndx);

	if (!strshdr) {
		return NULL;
	}

	if (size < strshdr->sh_offset) {
		return NULL;
	}

	return (const char*)base+strshdr->sh_offset;
}

// Get section header name by section index
const char* elf_section_name(size_t size, const void* base, size_t index) {
	const char* names = elf_section_names(size, base);
	
	if (!names) {
		return NULL;
	}

	return names+elf_section(size, base, index)->sh_name;
}

size_t elf_section_size(size_t size, const void* base, size_t index) {
	if (size < sizeof(Elf64_Ehdr)) {
		return -1;
	}

	return elf_section(size, base, index)->sh_size;
}

// Section index of the string table.
size_t elf_sectionidx_stringtable(size_t size, const void* base) {
	if (size < sizeof(Elf64_Ehdr)) {
		return -1;
	}

	return ((const Elf64_Ehdr*)base)->e_shstrndx;
}

// Get index of symbol table
size_t elf_sectionidx_sym(size_t size, const void* base) {
	size_t c = elf_section_count(size, base);
	const Elf64_Shdr* hdr;

	for (size_t i = 0; i < c; i++) {
		hdr = elf_section(size, base, i);

		if (hdr->sh_type == SHT_SYMTAB) {
			return i;
		}
	}

	return -1;
}


// Open a handle to the ELF executable.
void* elf_open_ex(size_t size, void* base) {
	return NULL;
}

// Close handle to ELF executable.
void elf_close(void* handle) {
}







