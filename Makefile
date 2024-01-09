#Assembler
AS := nasm
AS_FLAGS := -f elf64

#Compiler
CC := gcc
CC_FLAGS := -nostdlib -nodefaultlibs -ffreestanding -Wall -fno-stack-protector

#This is the gnu linker
LD := ld
#This makes static and dynamic elf libraries
AR := ar
#This allows us to create object files out of binaries
OBJCOPY := objcopy

AS_FILES := $(shell find src/boot -maxdepth 1 -name *.asm)
AS_OBJS := $(patsubst src/%.asm, bin/%.o, $(AS_FILES))

C_FILES := $(shell find src -maxdepth 1 -name *.c)
C_OBJS := $(patsubst src/%.c, bin/%.o, $(C_FILES))

LIBC_FILES := $(shell find src/libgcc -name *.c)
LIBC_OBJS := $(patsubst src/%.c, bin/%.o, $(LIBC_FILES))

#Included binary files into objects
INCBIN := $(shell find src/incbin -name *.bin)
INCBIN_OBJS := $(patsubst src/incbin/%.bin, bin/incbin/%.o, $(INCBIN))

#LIBC must be added at the end of any list of objects
LIBC := -L./bin -lgcc
LD_FLAGS := -T linker.ld 

#Assemble assembly files
$(AS_OBJS): $(AS_FILES)
	mkdir -p $(dir $@) && \
	$(AS) $(AS_FLAGS) $(patsubst bin/%.o, src/%.asm, $@) -o $@

#Compile c files
$(C_OBJS): $(C_FILES)
	mkdir -p $(dir $@) && \
	$(CC) -c $(CC_FLAGS) $(patsubst bin/%.o, src/%.c, $@) -o $@

#Compile libc c files
$(LIBC_OBJS): $(LIBC_FILES)
	mkdir -p $(dir $@) && \
	$(CC) -c $(CC_FLAGS) -I./src $(patsubst bin/%.o, src/%.c, $@) -o $@

# Might need to add more LIBC assembly files
# Assembles libc assembly file
bin/libgcc/sys.o: src/libgcc/sys.asm
	mkdir -p $(dir $@) && \
	$(AS) $(AS_FLAGS) $< -o $@

.PHONY: build-libgcc
build-libgcc: $(LIBC_OBJS) bin/libgcc/sys.o	
	mkdir -p bin/libgcc && \
	$(AR) -rcs bin/libgcc.a $(LIBC_OBJS) bin/libgcc/sys.o

.PHONY: build-incbin
build-incbin: $(INCBIN)
	mkdir -p bin/incbin && \
	$(OBJCOPY) -B i386:x86-64 -I binary -O elf64-x86-64 $(INCBIN) $(patsubst src/incbin/%.bin, bin/incbin/%.o, $(INCBIN))

.PHONY: build
build: $(C_OBJS) $(AS_OBJS)
	$(LD) -n $(LD_FLAGS) $(C_OBJS) $(AS_OBJS) $(LIBC) $(INCBIN_OBJS)  -o bin/kernel.bin && \
	cp bin/kernel.bin iso/boot/kernel.bin && \
	grub-mkrescue iso -o lzlos.iso

.PHONY: all
all: clean build-libgcc build-incbin build

.PHONY: clean
clean:
	rm -f -R bin && \
	rm -f lzlos.iso

.PHONY: run
run:
	qemu-system-x86_64 -cdrom lzlos.iso

.PHONY: test
test:
	gcc -Wall -std=gnu99 -D DEBUG src/mmap.c -o bin/test/a.out && \
	./bin/test/a.out

