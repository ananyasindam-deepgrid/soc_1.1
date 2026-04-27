#----------------------------------------------------------------------------
# Copyright [2026] [Deepgrid Semi Pvt Ltd]
#
# Author: Deepgrid Semi Pvt Ltd
#------------------------------------------------------------------------------

    .file "setjmp.s"
    .option nopic
    .attribute arch,"rv32i2p0_m2p0"
    .attribute unaligned_access, 0
    .attribute stack_align, 16
    .text
    .align 2

    .globl setjmp
    .type setjmp, @function

setjmp:
                 sw    ra,0(a0)
                 sw    s0,4(a0)
                 sw    s1,8(a0)
                 sw    s2,12(a0)
                 sw    s3,16(a0)
                 sw    s4,20(a0)
                 sw    s5,24(a0)
                 sw    s6,28(a0)
                 sw    s7,32(a0)
                 sw    s8,36(a0)
                 sw    s9,40(a0)
                 sw    s10,44(a0)
                 sw    s11,48(a0)
                 sw    sp,52(a0)
                 li    a0,0
                 ret
    .size setjmp, .-setjmp
    .align 2


    .globl longjmp
    .type longjmp, @function

longjmp:
                lw        ra,0(a0)
                lw        s0,4(a0)
                lw        s1,8(a0)
                lw        s2,12(a0)
                lw        s3,16(a0)
                lw        s4,20(a0)
                lw        s5,24(a0)
                lw        s6,28(a0)
                lw        s7,32(a0)
                lw        s8,36(a0)
                lw        s9,40(a0)
                lw        s10,44(a0)
                lw        s11,48(a0)
                lw        sp,52(a0)
                seqz      a0,a1
                add       a0,a0,a1
                ret
    .size longjmp, .-longjmp

    .ident "GCC: (GNU) 10.2.0"
