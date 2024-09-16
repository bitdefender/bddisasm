    bits 64

    global _start
    
    section .text
    
_start:
    nop
    mov     eax, 0x11111111
    mov     ecx, 0x22222222
    mov     edx, 0x33333333
    cmpxchg edx, ecx
    
    nop
    mov     eax, 0x11111111
    mov     ecx, 0x22222222
    mov     edx, 0x00000000
    cmpxchg edx, ecx
    
    nop
    mov     eax, 0x11111111
    mov     ecx, 0x22222222
    mov     edx, 0x11111111
    cmpxchg edx, ecx
    
    int3