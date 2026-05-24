.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager6handleEhm

.global _ZN16InterruptManager6ignoreEv

.macro HandleException num
.global _ZN16InterruptManager16exception\num\()Ev
_ZN16InterruptManager16exception\num\()Ev:
    movb $\num, (number)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager11request\num\()Ev
_ZN16InterruptManager11request\num\()Ev:
    movb $\num + IRQ_BASE, (number)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:

    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (number)
    call _ZN16InterruptManager6handleEhm
#    addl $5, %esp
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

_ZN16InterruptManager6ignoreEv:

    iret

.data
    number: .byte 0
