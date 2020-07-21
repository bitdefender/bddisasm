    
    bits 64

    lea     rsi, [rel _xxx]
    mov     rdi, rsi
    lodsb
    xor     al, cl
    stosb
    
_xxx:    
    nop
    retn