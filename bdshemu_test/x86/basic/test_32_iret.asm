
    bits 32

    pushfd
    push    0x33
    call    $+5
    add     dword [esp], 5
    iretd
    int3