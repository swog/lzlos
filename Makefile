# Assembler
AS := nasm
AS_FLAGS := -f elf64

# Compiler
CC := g++
CC_FLAGS := -nostdlib -fno-stack-protector

# This is the gnu linker
LD := ld
# This makes static and dynamic elf libraries
AR := ar
# This allows us to create object files out of binaries
OBJCOPY := objcopy

# Kernel boot assembly files
BOOT_FILES := $(shell find src/boot -maxdepth 1 -name *.asm)
BOOT_OBJS := $(patsubst src/%.asm, bin/%.o, $(BOOT_FILES))

# Kernel core source files
C_FILES := $(shell find src -maxdepth 1 -name *.cpp)
C_OBJS := $(patsubst src/%.cpp, bin/%.o, $(C_FILES))
AS_FILES := $(shell find src -maxdepth 1 -name *.asm)
AS_OBJS := $(patsubst src/%.asm, bin/%.o, $(AS_FILES))

# LIBC source files
# These need to be built first before linking.
LIBC_FILES := $(shell find src/libgcc -name *.cpp)
LIBC_OBJS := $(patsubst src/%.cpp, bin/%.o, $(LIBC_FILES))
LIBC_FILES_AS := $(shell find src/libgcc -name *.asm)
LIBC_OBJS_AS := $(patsubst src/%.asm, bin/%.o, $(LIBC_FILES_AS))

# LIBC must be added at the end of any list of objects
LIBC := -L./bin -lgcc
LD_FLAGS := -T linker.ld 

# Included binary files into objects
INCBIN := $(shell find src/incbin -name *.bin)
INCBIN_OBJS := $(patsubst src/incbin/%.bin, bin/incbin/%.o, $(INCBIN))

# All subdirectories in /drivers
DRIVERS := $(shell find src/drivers/ -mindepth 1 -type d)
DRIVERS_OBJS := $(patsubst src/drivers/%, bin/drivers/%, $(DRIVERS))

$(DRIVERS_OBJS): $(DRIVERS)
	mkdir -p $@ && \
	$(CC) -c $(shell find $? -name *.cpp) -o $(patsubst src/%.cpp, bin/%.o, $(shell find $? -name *.cpp)) && \
	$(LD) $(LIBC) $(CC_FLAGS) -shared $(patsubst src/%.cpp, bin/%.o, $(shell find $? -name *.cpp)) -o $(patsubst bin/drivers/%, src/incbin/%.bin, $@)

.PHONY: drivers
drivers: build-libgcc $(DRIVERS_OBJS)	

# Assemble assembly files
$(BOOT_OBJS) $(LIBC_OBJS_AS): $(BOOT_FILES) $(LIBC_FILES_AS)
	mkdir -p $(dir $@) && \
	$(AS) -felf64 $(patsubst bin/%.o, src/%.asm, $@) -o $@

# Compile c kernel files
$(C_OBJS): $(C_FILES)
	mkdir -p $(dir $@) && \
	$(CC) -c $(CC_FLAGS) $(patsubst bin/%.o, src/%.cpp, $@) -o $@

$(AS_OBJS): $(AS_FILES)
	$(AS) -felf64 $(patsubst bin/%.o, src/%.asm, $@) -o $@

# Compile libc c files
$(LIBC_OBJS): $(LIBC_FILES)
	mkdir -p $(dir $@) && \
	$(CC) -c $(CC_FLAGS) -I./src $(patsubst bin/%.o, src/%.cpp, $@) -o $@

# Link all libc prerequisite object files into a static library
.PHONY: build-libgcc
build-libgcc: $(LIBC_OBJS) $(LIBC_OBJS_AS)
	$(AR) -rcs bin/libgcc.a $?

# This won't run if the prerequisites aren't updated.
# Target is object files
# Prerequisite is incbin binary
$(INCBIN_OBJS): $(INCBIN) drivers
	mkdir -p $(dir $@) && \
	$(OBJCOPY) -B i386:x86-64 -I binary -O elf64-x86-64 $(patsubst bin/incbin/%.o, src/incbin/%.bin, $@) $@

# Incbin binaries are prerequisites
.PHONY: build-incbin
build-incbin: $(INCBIN_OBJS)

.PHONY: build
build: $(C_OBJS) $(BOOT_OBJS) $(INCBIN_OBJS) $(AS_OBJS)
	$(LD) -n $(LD_FLAGS) $(C_OBJS) $(BOOT_OBJS) $(LIBC) $(AS_OBJS) $(INCBIN_OBJS) -o bin/kernel.bin && \
	cp bin/kernel.bin iso/boot/kernel.bin && \
	grub-mkrescue iso -o lzlos.iso

.PHONY: all
all: build-libgcc drivers build-incbin build

.PHONY: clean
clean:
	rm -f -R bin && \
	rm -f lzlos.iso

.PHONY: run
run:
	qemu-system-x86_64 -cdrom lzlos.iso

