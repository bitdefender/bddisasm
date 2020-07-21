    
    bits 64

    xor     eax, eax
    mov     eax, 0xFFFFFFFF
    add     eax, 1
    
    xor     eax, eax
    mov     eax, 0xFFFFFFFF
    add     eax, 1
    adc     eax, 0

    xor     eax, eax
    sub     eax, 1
    sbb     eax, 0
    
    xor     eax, eax
    mov     eax, 0xBDBDBDBD
    rol     eax, 16
    
    xor     eax, eax
    mov     eax, 0xBDBDBDBD
    ror     eax, 24
    
    xor     eax, eax
    mov     eax, 0xBDBDBDBD
    rcl     eax, 24
    
    xor     eax, eax
    mov     eax, 0xBDBDBDBD
    rcr     eax, 24
    
    xor     eax, eax
    mov     eax, 0xBDBDBDBD
    shl     eax, 24
    
    xor     eax, eax
    mov     eax, 0xBDBDBDBD
    shr     eax, 24
    
    retn