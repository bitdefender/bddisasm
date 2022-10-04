    bits 32
    
    call    dword [eax]
    call    eax
    db 0x64
    call    dword [eax]
    db 0x64
    call    eax
    db 0x3E
    call    dword [eax]
    db 0x3E
    call    eax
    db 0x3E, 0x2E
    call    dword [eax]
    db 0x3E, 0x2E
    call    eax
    db 0x2E, 0x3E
    call    dword [eax]
    db 0x2E, 0x3E
    call    eax
    db 0x3E, 0x64
    call    dword [eax]
    db 0x3E, 0x64
    call    eax
    db 0x64, 0x3E
    call    dword [eax]
    db 0x64, 0x3E
    call    eax