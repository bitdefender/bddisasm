
    bits 32
    
    push    0x33
    call    $+5
    add     dword [esp], 5
    retf
    int3