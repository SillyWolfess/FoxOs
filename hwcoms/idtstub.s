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
    pushl $0
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:

    # save registers
#    pusha
#    pushl %ds
#    pushl %es
#    pushl %fs
#    pushl %gs

    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    # call c++ handler
    pushl %esp
    push (number)
    call _ZN16InterruptManager6handleEhm
#    addl $5, %esp
    movl %eax, %esp

    # restore registers
 #   popl %gs
 #   popl %fs
 #   popl %es
 #   popl %ds
 #   popa

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp

    add $4, %esp

_ZN16InterruptManager6ignoreEv:

    iret

.data
    number: .byte 0
