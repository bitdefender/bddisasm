    
    bits 64

    call    _test
_test:
    xor     ebp, ebp
    add     rbp, qword [rsp]
    int3