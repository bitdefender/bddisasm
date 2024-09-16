    
    bits 64

    mov     rax, 0xFFFFFFFFFFFFFFF0
    mov     rax, qword [rax + 0x7FFFFFFF]
    retn