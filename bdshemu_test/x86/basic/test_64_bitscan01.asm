    bits 64

    global _start
    
    section .text
    
_start:
    nop
    xor     eax, eax
    bsf     ax, ax
    
    nop
    xor     eax, eax
    bsf     eax, eax
    
    nop
    xor     eax, eax
    bsf     rax, rax
    
    nop
    xor     eax, eax
    bsr     ax, ax
    
    nop
    xor     eax, eax
    bsr     eax, eax
    
    nop
    xor     eax, eax
    bsr     rax, rax
    
    nop
    mov     eax, 0x00010000
    bsf     ax, ax
    
    nop
    mov     eax, 0x00010000
    bsf     eax, eax
    
    nop
    mov     eax, 0x00010000
    bsf     rax, rax
    
    nop
    mov     eax, 0x00010000
    bsr     ax, ax
    
    nop
    mov     eax, 0x00010000
    bsr     eax, eax
    
    nop
    mov     eax, 0x00010000
    bsr     rax, rax
    
    nop
    mov     rax, 0xF000000000000000
    bsf     ax, ax
    
    nop
    mov     rax, 0xF000000000000000
    bsf     eax, eax
    
    nop
    mov     rax, 0xF000000000000000
    bsf     rax, rax
    
    nop
    mov     rax, 0xF000000000000000
    bsr     ax, ax
    
    nop
    mov     rax, 0xF000000000000000
    bsr     eax, eax
    
    nop
    mov     rax, 0xF000000000000000
    bsr     rax, rax
    
    int3