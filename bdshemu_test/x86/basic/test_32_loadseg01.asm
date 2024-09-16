    
    bits 32
    
    mov         eax, 0xBDBDBDBD
    push        eax
    pop         ecx

    push cs
    pop  ds
    nop
    
    int3