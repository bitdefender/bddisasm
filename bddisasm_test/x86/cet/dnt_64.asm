    bits 64
    
    call    qword [rax]
    call    rax
    db 0x64
    call    qword [rax]
    db 0x64
    call    rax
    db 0x3E
    call    qword [rax]
    db 0x3E
    call    rax
    db 0x3E, 0x2E
    call    qword [rax]
    db 0x3E, 0x2E
    call    rax
    db 0x2E, 0x3E
    call    qword [rax]
    db 0x2E, 0x3E
    call    rax
    db 0x3E, 0x64
    call    qword [rax]
    db 0x3E, 0x64
    call    rax
    db 0x64, 0x3E
    call    qword [rax]
    db 0x64, 0x3E
    call    rax