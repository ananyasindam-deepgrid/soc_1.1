# Bootloader Makefile - builds a tiny bootloader that fits in 16KB TCM BRAM
# The bootloader reads main firmware from SD card and loads it into DDR

RISCV_PATH ?= /opt/riscv/
RISCV_NAME ?= riscv32-unknown-elf
MARCH = rv32im
MABI = ilp32

CC = $(RISCV_PATH)/bin/$(RISCV_NAME)-gcc
OBJCOPY = $(RISCV_PATH)/bin/$(RISCV_NAME)-objcopy
OBJDUMP = $(RISCV_PATH)/bin/$(RISCV_NAME)-objdump
SIZE = $(RISCV_PATH)/bin/$(RISCV_NAME)-size

CFLAGS = -march=$(MARCH) -mabi=$(MABI) -Os -Wall -fno-builtin -nostdlib
LDFLAGS = -march=$(MARCH) -mabi=$(MABI) -nostdlib -nostartfiles -T linker_boot.ld -lgcc

OBJDIR = build

SRCS = boot_crt.S boot_main.c
OBJS = $(addprefix $(OBJDIR)/,$(SRCS:.S=.o))
OBJS := $(OBJS:.c=.o)

all: $(OBJDIR)/bootloader.elf $(OBJDIR)/bootloader.hex $(OBJDIR)/bootloader.bin
	@echo "Bootloader built successfully."
	@$(SIZE) $(OBJDIR)/bootloader.elf

$(OBJDIR)/bootloader.elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/bootloader.hex: $(OBJDIR)/bootloader.elf
	$(OBJCOPY) -O ihex $< $@

$(OBJDIR)/bootloader.bin: $(OBJDIR)/bootloader.elf
	$(OBJCOPY) -O binary $< $@

$(OBJDIR)/%.o: %.S | $(OBJDIR)
	$(CC) -c $(CFLAGS) -D__ASSEMBLY__=1 -o $@ $<

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)

.PHONY: all clean
