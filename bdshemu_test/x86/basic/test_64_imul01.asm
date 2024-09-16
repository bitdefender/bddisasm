    bits 64

    global _start
    
    section .text
    
_start:
    nop
    mov     eax, 0x01000000
    mov     edx, 0xffffff60
    imul    edx
    
    nop
    mov     eax, 0x01000000
    mov     edx, 0xffffff60
    imul    eax, edx
    
    nop
    mov     edx, 0xffffff60
    imul    eax, edx, 0x01000000
    
    
    int3