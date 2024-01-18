#include <stdint.h>
#include "elf.h"

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

	return ((Elf64_Ehdr*)base)->e_type;
}

// EM_NONE - No machine?
// EM_SPARC - SPARC ?
// EM_386 - 80386
// EM_X86_64 - AMD x86_64
int elf_machine(size_t size, const void* base) {
	if (size < sizeof(Elf64_Ehdr)) {
		return -1;
	}

	return ((Elf64_Ehdr*)base)->e_machine;
}

// Entry point
void* elf_entry(size_t size, const void* base) {
	return (void*)((Elf64_Ehdr*)base)->e_entry;
}

Elf64_Phdr* elf_phdr(size_t size, const void* base) {
	if (size < sizeof(Elf64_Ehdr)) {
		return NULL;
	}

	Elf64_Off phoff = ((Elf64_Ehdr*)base)->e_phoff;

	if (!phoff) {
		return NULL;
	}

	return (Elf64_Phdr*)(((char*)base)+phoff);
}

Elf64_Shdr* elf_shdr(size_t size, const void* base) {
	if (size < sizeof(Elf64_Ehdr)) {
		return NULL;
	}

	Elf64_Off shoff = ((Elf64_Ehdr*)base)->e_shoff;

	if (!shoff) {
		return NULL;
	}

	return (Elf64_Shdr*)(((char*)base)+shoff);	
}











