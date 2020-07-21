    bits 64
    
    mov     rax, 0xBDBDBDBDBDBDBDBD
    xor     ax, ax
    div     ax
    retn    