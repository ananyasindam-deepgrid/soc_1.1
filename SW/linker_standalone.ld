/*
 * Standalone linker script for dgrid firmware.
 * Used when firmware is loaded by the SD card bootloader.
 *
 * Key difference from linker.ld:
 *   - Entry point is at RAM (0x4000) instead of TCM (0x0000)
 *   - TCM is NOT used (bootloader owns TCM)
 *   - All sections live in DDR RAM
 *   - The bootloader copies this binary to 0x4000 and jumps there
 */

OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(crtStart)

MEMORY {
  RAM (rwx): ORIGIN = 0x00004000, LENGTH = 255m
}

_stack_size = 8000;
_heap_size = 200000000;

SECTIONS {

  ._vector ORIGIN(RAM): {
    *crt.o(.start_jump);
    */base/task.o(.text);
  } > RAM

  ._stack (NOLOAD):
  {
    . = ALIGN(16);
    PROVIDE (_stack_end = .);
    . = . + _stack_size;
    . = ALIGN(16);
    PROVIDE (_stack_start = .);
  } > RAM

  .bss (NOLOAD) : {
    . = ALIGN(4);
    _bss_start = .;
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
    _bss_end = .;
  } > RAM

  ._user_heap (NOLOAD):
  {
    . = ALIGN(16);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    PROVIDE ( _heap_start = .);
    . = . + _heap_size;
    . = ALIGN(8);
    PROVIDE ( _heap_end = .);
  } > RAM

  .data :
  {
    *(.rdata)
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.sdata .sdata.*)
    *(.gnu.linkonce.s.*)
    . = ALIGN(8);
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata .srodata.*)
  } > RAM

  .rodata :
  {
    *(.rdata)
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
  } > RAM

  .noinit (NOLOAD) : {
    . = ALIGN(4);
    *(.noinit .noinit.*)
    . = ALIGN(4);
  } > RAM

  .memory : {
    *(.text);
    end = .;
  } > RAM

  .ctors :
  {
    . = ALIGN(4);
    _ctors_start = .;
    KEEP(*(.init_array*))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    . = ALIGN(4);
    _ctors_end = .;
    PROVIDE ( END_OF_SW_IMAGE = . );
  } > RAM
}
