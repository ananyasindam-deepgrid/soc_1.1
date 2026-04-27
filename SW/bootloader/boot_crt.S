# Bootloader startup - lives entirely in TCM (16KB)
# Loads main firmware from SD card into DDR, then jumps to it

.global crtStart
.global boot_main

.section .start_jump,"ax",@progbits
crtStart:
  lui x2,       %hi(boot_init)
  addi x2, x2,  %lo(boot_init)
  jalr x1,x2
  nop

.section .text

boot_init:
  .option push
  .option norelax
  la gp, __global_pointer$
  .option pop
  la sp, _stack_start

  # Clear BSS
  la a0, _bss_start
  la a1, _bss_end
bss_loop:
  beq a0,a1,bss_done
  sw zero,0(a0)
  add a0,a0,4
  j bss_loop
bss_done:

  call boot_main

  # If boot_main returns (firmware loaded), jump to firmware entry at 0x4000
  li a0, 0x00004000
  jalr x0, a0, 0

hang:
  j hang
