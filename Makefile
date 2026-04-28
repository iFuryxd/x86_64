CC = gcc
LD = ld

CFLAGS = -std=gnu11 -ffreestanding -m64 -O2 -Wall -Wextra -Iinclude \
         -fno-stack-protector -fno-pic -fno-asynchronous-unwind-tables \
         -mno-red-zone -mcmodel=kernel
ASFLAGS = -m64 -c
LDFLAGS = -m elf_x86_64 -T linker/linker.ld -nostdlib -z max-page-size=0x1000

BUILD = build

C_SOURCES := \
	kernel/kernel64.c \
	kernel/arch/x86_64/vga.c \
	kernel/arch/x86_64/serial.c \
	kernel/arch/x86_64/cpuid.c \
	kernel/lib/serial_debug.c \
	kernel/handle/error/validate_mbi.c \
	kernel/handle/error/pmmerr.c \
	kernel/lib/util.c \
	kernel/lib/print.c \
	kernel/lib/memutil.c \
	kernel/lib/string.c \
	kernel/mm/parse_mbi.c \
	kernel/mm/pmm.c

CHECK_C_SOURCES := $(sort $(C_SOURCES) $(shell find kernel -name '*.c'))
CHECK_OBJECTS := $(patsubst %.c,$(BUILD)/check/%.o,$(CHECK_C_SOURCES))

ASM_SOURCES := \
	boot/multiboot2_header.S \
	boot/entry_point.S

OBJECTS := $(patsubst %.c,$(BUILD)/%.o,$(C_SOURCES)) \
           $(patsubst %.S,$(BUILD)/%.o,$(ASM_SOURCES))
all: $(BUILD)/kernel.elf

debug: CFLAGS += -DKERNEL_DEBUG
debug: clean all

check-all: $(CHECK_OBJECTS)

$(BUILD)/check/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.elf: $(OBJECTS)
	mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.S
	mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) $< -o $@

ISO = $(BUILD)/x86_64.iso
$(ISO): $(BUILD)/kernel.elf
	mkdir -p $(BUILD)/iso/boot/grub
	cp $(BUILD)/kernel.elf $(BUILD)/iso/boot/kernel.elf
	cp grub/grub.cfg $(BUILD)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(BUILD)/iso

run: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO) -serial stdio

clean:
	rm -rf $(BUILD)
