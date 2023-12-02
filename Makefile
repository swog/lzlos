as_files := $(shell find src -name *.asm)
as_objfiles := $(patsubst src/%.asm, bin/%.o, $(as_files))
c_files := $(shell find src -name *.c)
c_objfiles := $(patsubst src/%.c, bin/%.o, $(c_files))
objfiles := $(as_objfiles) $(c_objfiles)

$(as_objfiles): bin/%.o : src/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst bin/%.o, src/%.asm, $@) -o $@ && \
	touch $(patsubst bin/%.o, src/%.asm, $@)

$(c_objfiles): bin/%.o : src/%.c
	mkdir -p $(dir $@) && \
	gcc -c -Wall -std=gnu99 -fno-stack-protector $(patsubst bin/%.o, src/%.c, $@) -o $@ && \
	touch $(patsubst bin/%.i, src/%.c, $@)

.PHONY: build
build: $(objfiles)
	ld -n -o bin/kernel.bin -T linker.ld $(objfiles) && \
	cp bin/kernel.bin iso/boot/kernel.bin && \
	grub-mkrescue -o lzlos.iso iso

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

