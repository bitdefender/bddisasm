    
    bits 64

    sub         rsp, 0x40
    mov         rax, 0x1122334455667788
    mov         qword [rsp], rax
    mov         rax, 0x99AABBCCDDEEFF00
    mov         qword [rsp + 0x08], rax

    pause
    serialize

    lddqu       xmm0, [rsp]
    movntdqa    xmm2, [rsp]
    movntdq     [rsp + 0x10], xmm0
    movntps     [rsp + 0x20], xmm2
    movntpd     [rsp + 0x30], xmm0

    movdq2q     mm0, xmm0
    movq2dq     xmm2, mm0
    movntq      [rsp + 0x20], mm0
    movnti      [rsp + 0x28], rax
    emms

    add         rsp, 0x40
    retn
