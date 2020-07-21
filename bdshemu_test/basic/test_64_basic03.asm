
    bits 64
    
    lea     rbp, [rel _xxx]
    mov     eax, dword [rbp - 4 + 0x100]
    mov     eax, dword [rbp - 3 + 0x100]
    retn
_xxx:    