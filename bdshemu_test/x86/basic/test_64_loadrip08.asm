    
    bits 64

    call    _test
    int3
_test:
    jmp     qword [rsp]