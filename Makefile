CC = gcc 
LD = ld

CFLAGS = -ffreestanding -m32 -O2 -Wall -Wextra -Iinclude \
         -fno-stack-protector -fno-pic -fno-asynchronous-unwind-tables
ASFLAGS = -m32 -c
LDFLAGS = -m elf_i386 -T linker/linker.ld -nostdlib

BUILD = build

C_SOURCES = $(wildcard kernel/**/*.c)
ASM_SOURCES = $(wildcard boot/*.S)

OBJECTS = $(C_SOURCES:%.c=$(BUILD)/%.o) \
				 $(ASM_SOURCES:%.S=$(BUILD)/%.o)
all: $(BUILD)/kernel.elf

$(BUILD)/kernel.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

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
	qemu-system-x86_64 -cdrom $(ISO)

clean:
	rm -rf $(BUILD)
