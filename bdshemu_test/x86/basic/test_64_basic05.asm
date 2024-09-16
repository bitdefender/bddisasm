    
    bits 64

_xxx:    
    lea     rbp, [rel _xxx]
    sub     rbp, 1
    jmp     rbp
 