
    bits 64

    mov     eax, 0xFFFFFFFF
    add     eax, 1          ; Will set CF
    jc      _skip
    lea     rsi, [rel _skip]
    mov     byte [rsi], 0xCC
_skip:
    retn