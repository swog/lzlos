#ifndef _LIBELF_H
#define _LIBELF_H

// Check magic numbers
bool elf_magic(size_t size, const void* base);

// -1 - Size does not contain header.
// ET_NONE - No type?
// ET_REL - Relocatable object file
// ET_EXEC - Executable file
// ET_DYN - Shared object file
// ET_CORE - Core file
int elf_type(size_t size, const void* base);

// -1 - Size does not contain header.
// EM_NONE - No machine?
// EM_SPARC - SPARC ?
// EM_386 - 80386
// EM_X86_64 - AMD x86_64
int elf_machine(size_t size, const void* base);

// Entry point
void* elf_entry(size_t size, const void* base);

// Program header table
Elf64_Phdr* elf_phdr(size_t size, const void* base);
// Section header table
Elf64_Shdr* elf_shdr(size_t size, const void* base);



#endif
