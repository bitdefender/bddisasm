    bits 32
    
    mov       eax, 0x41414141
    mov       ebx, 0xbdbdbdbd
    push      eax
    push      ebx
    push      eax
    push      ebx
    push      eax
    push      ebx
    push      eax
    push      ebx
    retn
