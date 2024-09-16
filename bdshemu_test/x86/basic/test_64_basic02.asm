
    bits 64
    
_xxx:    
    lea     rbp, [rel _xxx]
    mov     eax, dword [rbp]
    ; This must fail with SHEMU_ABORT_GLA_OUTSIDE
    mov     eax, dword [rbp - 3]
    retn