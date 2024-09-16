    
    bits 32
    
    mov         eax, 0xBDBDBDBD
    push        eax
    pop         ecx

    push        0xBD
    pop         ds
    nop
    
    int3