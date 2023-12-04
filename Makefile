AS := nasm
AS_FLAGS := -f elf64

CC := gcc
CC_FLAGS := -Wall -fno-stack-protector

LNK := ld

AS_FILES := $(shell find src/boot -maxdepth 1 -name *.asm)
AS_OBJS := $(patsubst src/%.asm, bin/%.o, $(AS_FILES))
C_FILES := $(shell find src -maxdepth 1 -name *.c)
C_OBJS := $(patsubst src/%.c, bin/%.o, $(C_FILES))

$(AS_OBJS): $(AS_FILES)
	mkdir -p $(dir $@) && \
	$(AS) $(AS_FLAGS) $(patsubst bin/%.o, src/%.asm, $@) -o $@

$(C_OBJS): $(C_FILES)
	mkdir -p $(dir $@) && \
	$(CC) -c $(CC_FLAGS) $(patsubst bin/%.o, src/%.c, $@) -o $@

.PHONY: build
build: $(C_OBJS) $(AS_OBJS)
	$(LNK) -n $(CC_FILES) -T linker.ld $(C_OBJS) $(AS_OBJS) -o bin/kernel.bin && \
	cp bin/kernel.bin iso/boot/kernel.bin && \
	grub-mkrescue iso -o lzlos.iso

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

