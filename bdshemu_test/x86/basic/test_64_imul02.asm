    bits 64

    global _start
    
    section .text
    
_start:
    nop
    
    mov     rax, -1
    mov     rdx, -2
    imul    rdx
    
    mov     rax, -1
    mov     rdx, 2
    imul    rdx
    
    mov     rax, 1
    mov     rdx, 2
    imul    rdx
    
    mov     rax, 0x7fffffffffffffff
    mov     rdx, 2
    imul    rdx    
    
    mov     rax, 0x7fffffffffffffff
    mov     rdx, 4
    imul    rdx    
    
    
    int3