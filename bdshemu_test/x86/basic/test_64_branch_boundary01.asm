   
    bits 64

    call    $+4
    db 0xC0
    pop     rbp
    nop
    int3
