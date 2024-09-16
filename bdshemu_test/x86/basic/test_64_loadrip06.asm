    
    bits 64

    call    _test
_test:
    xor     eax, eax    
    add     qword [rsp], rax
    int3