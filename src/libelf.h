#ifndef _LIBELF_H
#define _LIBELF_H

//https://wiki.osdev.org/ELF_Tutorial
enum {
	SHT_NULL	= 0,   // Null section
	SHT_PROGBITS	= 1,   // Program information
	SHT_SYMTAB	= 2,   // Symbol table
	SHT_STRTAB	= 3,   // String table
	SHT_RELA	= 4,   // Relocation (w/ addend)
	SHT_NOBITS	= 8,   // Not present in file
	SHT_REL		= 9,   // Relocation (no addend)
};

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
const void* elf_entry(size_t size, const void* base);





// Program header table
Elf64_Phdr* elf_program(size_t size, void* base, size_t index);
// Section header by index
Elf64_Shdr* elf_section(size_t size, void* base, size_t index);


// Section header count
size_t elf_section_count(size_t size, const void* base);
// Get section name table.
// Elf64_Shdr's sh_name is an offset into this table.
const char* elf_section_names(size_t size, const void* base);
// Get section header name by section index
const char* elf_section_name(size_t size, const void* base, size_t index);


// Section index of the string table, into the section table
// Fast, O(1)
size_t elf_sectionidx_stringtable(size_t size, const void* base);
// Get index of symbol table
// Warning: Low speed O(all sections) as of Jan 29, 24
// -1 is returned when not found.
size_t elf_sectionidx_sym(size_t size, const void* base);


// Symbol table size
size_t elf_sym_count(size_t size, const void* base);


// Open a handle to the ELF executable.
void* elf_open_ex(size_t size, void* base);
// Close handle to ELF executable
void elf_close(void* handle);

#endif















