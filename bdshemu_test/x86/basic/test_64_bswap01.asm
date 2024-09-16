    bits 64

    global _start
    
    section .text
    
_start:
    nop
    mov         rax, 0x1122334455667788
    db 0x66
    bswap       eax        
    
    nop
    mov         rax, 0x1122334455667788
    bswap       eax
    
    nop
    mov         rax, 0x1122334455667788
    bswap       rax
    
    int3