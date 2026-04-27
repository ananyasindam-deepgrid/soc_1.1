/* Bootloader linker script - everything in TCM (16KB BRAM) */

OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(crtStart)

MEMORY {
  TCM (rwx): ORIGIN = 0x00000000, LENGTH = 16k
}

_stack_size = 2048;

SECTIONS {

  ._vector ORIGIN(TCM): {
    *boot_crt.o(.start_jump);
  } > TCM

  .text : {
    *(.text .text.*);
  } > TCM

  .rodata : {
    *(.rodata .rodata.*);
    *(.srodata .srodata.*);
  } > TCM

  .data : {
    . = ALIGN(4);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.data .data.*);
    *(.sdata .sdata.*);
  } > TCM

  .bss (NOLOAD) : {
    . = ALIGN(4);
    _bss_start = .;
    *(.bss .bss.*);
    *(.sbss .sbss.*);
    *(COMMON);
    . = ALIGN(4);
    _bss_end = .;
  } > TCM

  ._stack (NOLOAD) : {
    . = ALIGN(16);
    PROVIDE (_stack_end = .);
    . = . + _stack_size;
    . = ALIGN(16);
    PROVIDE (_stack_start = .);
  } > TCM
}
