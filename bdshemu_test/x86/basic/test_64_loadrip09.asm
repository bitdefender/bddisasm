    
    bits 64

    call    _test
_test:
    mov     rsi, rsp
    lodsq
    int3