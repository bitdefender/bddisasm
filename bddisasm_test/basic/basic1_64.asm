    bits 64
    
    ; Arithemtic.
    add     al, bl
    add     al, sil
    add     al, [rbx]
    add     [ebx], al
    add     al, [ebx]
    add     [ebx], al
    add     al, 0x10
    add     ax, bx
    add     ax, r11w
    add     ax, [rbx]
    add     [ebx], ax
    add     ax, [ebx]
    add     [ebx], ax
    add     ax, 0x1000
    add     eax, ebx
    add     eax, r11d
    add     eax, [rbx]
    add     [rbx], eax    
    add     eax, [ebx]
    add     [ebx], eax
    add     eax, 0x10000000
    add     rax, rbx
    add     rax, r11
    add     rax, [rbx]
    add     [rbx], rax
    add     rax, [ebx]
    add     [ebx], rax
    add     rax, 0x10000000
    
    sub     al, bl
    sub     al, sil
    sub     al, [rbx]
    sub     [ebx], al
    sub     al, [ebx]
    sub     [ebx], al
    sub     al, 0x10
    sub     ax, bx
    sub     ax, r11w
    sub     ax, [rbx]
    sub     [ebx], ax
    sub     ax, [ebx]
    sub     [ebx], ax
    sub     ax, 0x1000
    sub     eax, ebx
    sub     eax, r11d
    sub     eax, [rbx]
    sub     [rbx], eax    
    sub     eax, [ebx]
    sub     [ebx], eax
    sub     eax, 0x10000000
    sub     rax, rbx
    sub     rax, r11
    sub     rax, [rbx]
    sub     [rbx], rax
    sub     rax, [ebx]
    sub     [ebx], rax
    sub     rax, 0x10000000
    
    adc     al, bl
    adc     al, sil
    adc     al, [rbx]
    adc     [ebx], al
    adc     al, [ebx]
    adc     [ebx], al
    adc     al, 0x10
    adc     ax, bx
    adc     ax, r11w
    adc     ax, [rbx]
    adc     [ebx], ax
    adc     ax, [ebx]
    adc     [ebx], ax
    adc     ax, 0x1000
    adc     eax, ebx
    adc     eax, r11d
    adc     eax, [rbx]
    adc     [rbx], eax    
    adc     eax, [ebx]
    adc     [ebx], eax
    adc     eax, 0x10000000
    adc     rax, rbx
    adc     rax, r11
    adc     rax, [rbx]
    adc     [rbx], rax
    adc     rax, [ebx]
    adc     [ebx], rax
    adc     rax, 0x10000000
    
    sbb     al, bl
    sbb     al, sil
    sbb     al, [rbx]
    sbb     [ebx], al
    sbb     al, [ebx]
    sbb     [ebx], al
    sbb     al, 0x10
    sbb     ax, bx
    sbb     ax, r11w
    sbb     ax, [rbx]
    sbb     [ebx], ax
    sbb     ax, [ebx]
    sbb     [ebx], ax
    sbb     ax, 0x1000
    sbb     eax, ebx
    sbb     eax, r11d
    sbb     eax, [rbx]
    sbb     [rbx], eax    
    sbb     eax, [ebx]
    sbb     [ebx], eax
    sbb     eax, 0x10000000
    sbb     rax, rbx
    sbb     rax, r11
    sbb     rax, [rbx]
    sbb     [rbx], rax
    sbb     rax, [ebx]
    sbb     [ebx], rax
    sbb     rax, 0x10000000
    
    and     al, bl
    and     al, sil
    and     al, [rbx]
    and     [ebx], al
    and     al, [ebx]
    and     [ebx], al
    and     al, 0x10
    and     ax, bx
    and     ax, r11w
    and     ax, [rbx]
    and     [ebx], ax
    and     ax, [ebx]
    and     [ebx], ax
    and     ax, 0x1000
    and     eax, ebx
    and     eax, r11d
    and     eax, [rbx]
    and     [rbx], eax    
    and     eax, [ebx]
    and     [ebx], eax
    and     eax, 0x10000000
    and     rax, rbx
    and     rax, r11
    and     rax, [rbx]
    and     [rbx], rax
    and     rax, [ebx]
    and     [ebx], rax
    and     rax, 0x10000000
    
    or      al, bl
    or      al, sil
    or      al, [rbx]
    or      [ebx], al
    or      al, [ebx]
    or      [ebx], al
    or      al, 0x10
    or      ax, bx
    or      ax, r11w
    or      ax, [rbx]
    or      [ebx], ax
    or      ax, [ebx]
    or      [ebx], ax
    or      ax, 0x1000
    or      eax, ebx
    or      eax, r11d
    or      eax, [rbx]
    or      [rbx], eax    
    or      eax, [ebx]
    or      [ebx], eax
    or      eax, 0x10000000
    or      rax, rbx
    or      rax, r11
    or      rax, [rbx]
    or      [rbx], rax
    or      rax, [ebx]
    or      [ebx], rax
    or      rax, 0x10000000
    
    xor     al, bl
    xor     al, sil
    xor     al, [rbx]
    xor     [ebx], al
    xor     al, [ebx]
    xor     [ebx], al
    xor     al, 0x10
    xor     ax, bx
    xor     ax, r11w
    xor     ax, [rbx]
    xor     [ebx], ax
    xor     ax, [ebx]
    xor     [ebx], ax
    xor     ax, 0x1000
    xor     eax, ebx
    xor     eax, r11d
    xor     eax, [rbx]
    xor     [rbx], eax    
    xor     eax, [ebx]
    xor     [ebx], eax
    xor     eax, 0x10000000
    xor     rax, rbx
    xor     rax, r11
    xor     rax, [rbx]
    xor     [rbx], rax
    xor     rax, [ebx]
    xor     [ebx], rax
    xor     rax, 0x10000000
    
    cmp     al, bl
    cmp     al, sil
    cmp     al, [rbx]
    cmp     [ebx], al
    cmp     al, [ebx]
    cmp     [ebx], al
    cmp     al, 0x10
    cmp     ax, bx
    cmp     ax, r11w
    cmp     ax, [rbx]
    cmp     [ebx], ax
    cmp     ax, [ebx]
    cmp     [ebx], ax
    cmp     ax, 0x1000
    cmp     eax, ebx
    cmp     eax, r11d
    cmp     eax, [rbx]
    cmp     [rbx], eax    
    cmp     eax, [ebx]
    cmp     [ebx], eax
    cmp     eax, 0x10000000
    cmp     rax, rbx
    cmp     rax, r11
    cmp     rax, [rbx]
    cmp     [rbx], rax
    cmp     rax, [ebx]
    cmp     [ebx], rax
    cmp     rax, 0x10000000
    
    test    eax, 1
    test    rax, 1
    test    eax, r14d
    test    rax, r14
    test    byte [rbx], 1
    test    word [rbx], 1
    test    dword [rbx], 1
    test    qword [rbx], 1
    test    byte [rbx], al
    test    word [rbx], ax
    test    dword [rbx], eax
    test    qword [rbx], rax
    

    ; Data transfer.
    mov     al, byte 0xBD
    mov     ax, word 0xBDBD
    mov     eax, dword 0xBDBDBDBD
    mov     rax, qword 0xBDBDBDBDBDBDBDBD
    mov     al, [qword 0xBDBDBDBDBDBDBDBD]
    mov     ax, [qword 0xBDBDBDBDBDBDBDBD]
    mov     eax, [qword 0xBDBDBDBDBDBDBDBD]
    mov     rax, [qword 0xBDBDBDBDBDBDBDBD]
    mov     [qword 0xBDBDBDBDBDBDBDBD], al
    mov     [qword 0xBDBDBDBDBDBDBDBD], ax
    mov     [qword 0xBDBDBDBDBDBDBDBD], eax
    mov     [qword 0xBDBDBDBDBDBDBDBD], rax
    
    xlatb
    xchg    al, cl
    xchg    ax, cx
    xchg    eax, ecx
    xchg    rax, rcx
    xchg    rax, [rbx]
    xchg    rax, [ebx]
    
    ; Unary opcodes.
    inc     al
    inc     r15b
    inc     ax
    inc     r15w
    inc     eax
    inc     r15d
    inc     rax
    inc     r15
    inc     byte [rbx]
    inc     word [rbx]
    inc     dword [rbx]
    inc     qword [rbx]
    
    dec     al
    dec     r15b
    dec     ax
    dec     r15w
    dec     eax
    dec     r15d
    dec     rax
    dec     r15
    dec     byte [rbx]
    dec     word [rbx]
    dec     dword [rbx]
    dec     qword [rbx]
    
    not     al
    not     r15b
    not     ax
    not     r15w
    not     eax
    not     r15d
    not     rax
    not     r15
    not     byte [rbx]
    not     word [rbx]
    not     dword [rbx]
    not     qword [rbx]
    
    neg     al
    neg     r15b
    neg     ax
    neg     r15w
    neg     eax
    neg     r15d
    neg     rax
    neg     r15
    neg     byte [rbx]
    neg     word [rbx]
    neg     dword [rbx]
    neg     qword [rbx]
    
    div     al
    div     r15b
    div     ax
    div     r15w
    div     eax
    div     r15d
    div     rax
    div     r15
    div     byte [rbx]
    div     word [rbx]
    div     dword [rbx]
    div     qword [rbx]
    
    mul     al
    mul     r15b
    mul     ax
    mul     r15w
    mul     eax
    mul     r15d
    mul     rax
    mul     r15
    mul     byte [rbx]
    mul     word [rbx]
    mul     dword [rbx]
    mul     qword [rbx]
    
    idiv    al
    idiv    r15b
    idiv    ax
    idiv    r15w
    idiv    eax
    idiv    r15d
    idiv    rax
    idiv    r15
    idiv    byte [rbx]
    idiv    word [rbx]
    idiv    dword [rbx]
    idiv    qword [rbx]
    
    imul    al
    imul    r15b
    imul    ax
    imul    r15w
    imul    eax
    imul    r15d
    imul    rax
    imul    r15
    imul    byte [rbx]
    imul    word [rbx]
    imul    dword [rbx]
    imul    qword [rbx]
    
     ; Shift/rotate
    rol     al, 1
    rol     al, 100
    rol     al, cl
    rol     byte [rbx], 1
    rol     byte [rbx], cl
    rol     ax, 1
    rol     ax, 100
    rol     ax, cl
    rol     word [rbx], 1
    rol     word [rbx], 100
    rol     word [rbx], cl
    rol     dword [rbx], 1
    rol     dword [rbx], 100
    rol     dword [rbx], cl
    rol     qword [rbx], 1
    rol     qword [rbx], 100
    rol     qword [rbx], cl
    
    ror     al, 1
    ror     al, 100
    ror     al, cl
    ror     byte [rbx], 1
    ror     byte [rbx], cl
    ror     ax, 1
    ror     ax, 100
    ror     ax, cl
    ror     word [rbx], 1
    ror     word [rbx], 100
    ror     word [rbx], cl
    ror     dword [rbx], 1
    ror     dword [rbx], 100
    ror     dword [rbx], cl
    ror     qword [rbx], 1
    ror     qword [rbx], 100
    ror     qword [rbx], cl
    
    rcl     al, 1
    rcl     al, 100
    rcl     al, cl
    rcl     byte [rbx], 1
    rcl     byte [rbx], cl
    rcl     ax, 1
    rcl     ax, 100
    rcl     ax, cl
    rcl     word [rbx], 1
    rcl     word [rbx], 100
    rcl     word [rbx], cl
    rcl     dword [rbx], 1
    rcl     dword [rbx], 100
    rcl     dword [rbx], cl
    rcl     qword [rbx], 1
    rcl     qword [rbx], 100
    rcl     qword [rbx], cl
    
    rcr     al, 1
    rcr     al, 100
    rcr     al, cl
    rcr     byte [rbx], 1
    rcr     byte [rbx], cl
    rcr     ax, 1
    rcr     ax, 100
    rcr     ax, cl
    rcr     word [rbx], 1
    rcr     word [rbx], 100
    rcr     word [rbx], cl
    rcr     dword [rbx], 1
    rcr     dword [rbx], 100
    rcr     dword [rbx], cl
    rcr     qword [rbx], 1
    rcr     qword [rbx], 100
    rcr     qword [rbx], cl
    
    shl     al, 1
    shl     al, 100
    shl     al, cl
    shl     byte [rbx], 1
    shl     byte [rbx], cl
    shl     ax, 1
    shl     ax, 100
    shl     ax, cl
    shl     word [rbx], 1
    shl     word [rbx], 100
    shl     word [rbx], cl
    shl     dword [rbx], 1
    shl     dword [rbx], 100
    shl     dword [rbx], cl
    shl     qword [rbx], 1
    shl     qword [rbx], 100
    shl     qword [rbx], cl
    
    shr     al, 1
    shr     al, 100
    shr     al, cl
    shr     byte [rbx], 1
    shr     byte [rbx], cl
    shr     ax, 1
    shr     ax, 100
    shr     ax, cl
    shr     word [rbx], 1
    shr     word [rbx], 100
    shr     word [rbx], cl
    shr     dword [rbx], 1
    shr     dword [rbx], 100
    shr     dword [rbx], cl
    shr     qword [rbx], 1
    shr     qword [rbx], 100
    shr     qword [rbx], cl
    
    sar     al, 1
    sar     al, 100
    sar     al, cl
    sar     byte [rbx], 1
    sar     byte [rbx], cl
    sar     ax, 1
    sar     ax, 100
    sar     ax, cl
    sar     word [rbx], 1
    sar     word [rbx], 100
    sar     word [rbx], cl
    sar     dword [rbx], 1
    sar     dword [rbx], 100
    sar     dword [rbx], cl
    sar     qword [rbx], 1
    sar     qword [rbx], 100
    sar     qword [rbx], cl
    
    ; imul with 3 operands
    imul    ax, cx, 100
    imul    ax, [rbx], 100
    imul    eax, ecx, 100
    imul    eax, [rbx], 100
    imul    rax, rcx, 100
    imul    rax, [rbx], 100
    
    ; String
    movsb
    movsw
    movsd
    movsq
    stosb
    stosw
    stosd
    stosq
    lodsb
    lodsw
    lodsd
    lodsq
    scasb
    scasw
    scasd
    scasq
    cmpsb
    cmpsb
    cmpsw
    cmpsd
    cmpsq
    
    ; I/O
    insb
    insw
    insd
    outsb
    outsw
    outsd
    
    in      al, 0x10
    in      ax, 0x10
    in      al, dx
    in      ax, dx
    out     0x10, al
    out     0x10, ax
    out     dx, al
    out     dx, ax
   
   
    ; Prefixes.
    lock add qword [rdi + 0x1000], 1
    lock xacquire add dword [rax], 1
    lock xrelease sub qword [rsi + rdi * 8 - 0x1000], 2
    xrelease mov byte [rbp + 0x1000], spl
    
    rep stosb
    rep lodsb
    repnz scasw
    repz cmpsq