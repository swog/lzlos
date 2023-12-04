AS := nasm
AS_FLAGS := -f elf64

CC := gcc
CC_FLAGS := -nolibc -Wall -fno-stack-protector

LNK := ld

AR := ar

AS_FILES := $(shell find src/boot -maxdepth 1 -name *.asm)
AS_OBJS := $(patsubst src/%.asm, bin/%.o, $(AS_FILES))
C_FILES := $(shell find src -maxdepth 1 -name *.c)
C_OBJS := $(patsubst src/%.c, bin/%.o, $(C_FILES))

LIBC_FILES := $(shell find src/libc -name *.c)
LIBC_OBJS := $(patsubst src/%.c, bin/%.o, $(LIBC_FILES))

KLNK_FLAGS := -T linker.ld -nostdlib -znodefaultlib -Lbin -lc

$(AS_OBJS): $(AS_FILES)
	mkdir -p $(dir $@) && \
	$(AS) $(AS_FLAGS) $(patsubst bin/%.o, src/%.asm, $@) -o $@

$(C_OBJS): $(C_FILES)
	mkdir -p $(dir $@) && \
	$(CC) -c $(CC_FLAGS) $(patsubst bin/%.o, src/%.c, $@) -o $@

$(LIBC_OBJS): $(LIBC_FILES)
	mkdir -p $(dir $@) && \
	$(CC) -c -fPIC -I src $(CC_FILES) $(patsubst bin/%.o, src/%.c, $@) -o $@

.PHONY: build-libc
build-libc: $(LIBC_OBJS)
	$(CC) -shared $(LIBC_OBJS) -o bin/libc.so

.PHONY: build
build: $(C_OBJS) $(AS_OBJS)
	$(LNK) -n $(KLNK_FLAGS) $(CC_FILES) $(C_OBJS) $(AS_OBJS) -o bin/kernel.bin && \
	cp bin/kernel.bin iso/boot/kernel.bin && \
	grub-mkrescue iso -o lzlos.iso

.PHONY: all
all: clean build-libc build

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

