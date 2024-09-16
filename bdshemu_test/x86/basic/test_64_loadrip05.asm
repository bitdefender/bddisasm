    
    bits 64

    call    _test
_test:
    cmp     qword [rsp], 0
    int3