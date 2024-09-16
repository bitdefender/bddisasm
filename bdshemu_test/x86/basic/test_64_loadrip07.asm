    
    bits 64

    call    _test
_test:
    xor     eax, eax    
    add     qword [rsp], rax
    test    qword [rsp], rax
    mov     rax, [rsp + rax]
    int3