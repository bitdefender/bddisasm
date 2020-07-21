
    bits 64

    xor     eax, eax
    jnz     _skip
    lea     rsi, [rel _skip]
    mov     byte [rsi], 0xCC
_skip:
    retn