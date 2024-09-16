    
    bits 32

    mov     esi, 0x7FFE0000
    mov     eax, dword [esi + 0x308]
    int3