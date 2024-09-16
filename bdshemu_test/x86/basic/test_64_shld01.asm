    bits 64

    global _start
    
    section .text
    
_start:
    ; 16-bit shift.
    mov     eax, 0x0000FFFF
    mov     ebx, 0x0000BDBD
    shld    ax, bx, 8
    
    ; 32-bit shift.
    mov     eax, 0x0000FFFF
    mov     ebx, 0xBDBD0000
    shld    eax, ebx, 16
    
    ; 64-bit shift.
    mov     rax, 0x0000FFFF0000FFFF
    mov     rbx, 0xBDBD00000000FFFF
    shld    rax, rbx, 32
    
    ; 64-bit shift with count larger than 64, masked.
    mov     rax, 0x0000FFFF0000FFFF
    mov     rbx, 0xBDBD00000000FFFF
    shld    rax, rbx, 65
    
    int3