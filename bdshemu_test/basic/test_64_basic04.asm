
    bits 64

_xxx:    
    lea     rsi, [rel _xxx]
    mov     rcx, 0xFFFFFFFF
    rep lodsb
 