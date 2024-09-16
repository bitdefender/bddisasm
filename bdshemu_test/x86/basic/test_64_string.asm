    
    bits 64

    lea     rsi, [rel _xxx]
    lea     rdi, [rel _yyy]
    
    movsb
    movsb
    movsb
    movsb
    movsd
    
    sub     rsi, 8
    sub     rdi, 8
    mov     ecx, 4
    rep movsq
    
    lea     rsi, [rel _xxx]
    lea     rdi, [rel _yyy]
    xor     ecx, ecx
    repz cmpsb
    
    mov     ecx, 4
    repz cmpsb
    
    mov     ecx, 4
    xor     al, al
    repnz scasb
    
    lea     rdi, [rel _zzz]
    mov     ecx, 0x100
    xor     al, al
    repnz scasb
    mov     eax, 0x100
    sub     eax, ecx
    dec     eax
    
    retn
_xxx:
    dq 1, 2, 3, 4
_yyy:
    dq 5, 6, 7, 8
_zzz:
    db 'ana are mere',0