    bits 64

    global _start
    
    section .text
    
_start:
    ; Undefined shift, count larger than operand size.
    mov     eax, 0x0000FFFF
    mov     ebx, 0x0000BDBD
    shld    ax, bx, 31
    
    int3