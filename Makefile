AS := nasm
AS_FLAGS := -f elf64

CC := gcc
CC_FLAGS := -nostdlib -nodefaultlibs -ffreestanding -Wall -fno-stack-protector

LD := ld

AR := ar

AS_FILES := $(shell find src/boot -maxdepth 1 -name *.asm)
AS_OBJS := $(patsubst src/%.asm, bin/%.o, $(AS_FILES))
C_FILES := $(shell find src -maxdepth 1 -name *.c)
C_OBJS := $(patsubst src/%.c, bin/%.o, $(C_FILES))

LIBC_FILES := $(shell find src/libgcc -name *.c)
LIBC_OBJS := $(patsubst src/%.c, bin/%.o, $(LIBC_FILES))

#LIBC must be added at the end of any list of objects
LIBC := -L./bin -lgcc
LD_FLAGS := -T linker.ld 

$(AS_OBJS): $(AS_FILES)
	mkdir -p $(dir $@) && \
	$(AS) $(AS_FLAGS) $(patsubst bin/%.o, src/%.asm, $@) -o $@

$(C_OBJS): $(C_FILES)
	mkdir -p $(dir $@) && \
	$(CC) -c $(CC_FLAGS) $(patsubst bin/%.o, src/%.c, $@) -o $@

$(LIBC_OBJS): $(LIBC_FILES)
	mkdir -p $(dir $@) && \
	$(CC) -c $(CC_FLAGS) -I./src $(patsubst bin/%.o, src/%.c, $@) -o $@

# Might need to add more LIBC assembly files
bin/libgcc/sys.o: src/libgcc/sys.asm
	mkdir -p $(dir $@) && \
	$(AS) $(AS_FLAGS) $< -o $@

.PHONY: build-libgcc
build-libgcc: $(LIBC_OBJS) bin/libgcc/sys.o	
	$(AR) -rcs bin/libgcc.a $(LIBC_OBJS) bin/libgcc/sys.o

.PHONY: build
build: $(C_OBJS) $(AS_OBJS)
	$(LD) -n $(LD_FLAGS) $(C_OBJS) $(AS_OBJS) $(LIBC) -o bin/kernel.bin && \
	cp bin/kernel.bin iso/boot/kernel.bin && \
	grub-mkrescue iso -o lzlos.iso

.PHONY: all
all: clean build-libgcc build

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

