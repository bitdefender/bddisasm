    
    bits 64

    sub         rsp, 0x40
    mov         rax, 0x1122334455667788
    mov         qword [rsp], rax
    mov         rax, 0x99AABBCCDDEEFF00
    mov         qword [rsp + 0x08], rax
    mov         rax, 0x0123456789ABCDEF
    mov         qword [rsp + 0x10], rax
    mov         rax, 0xFEDCBA9876543210
    mov         qword [rsp + 0x18], rax

    vlddqu      ymm0, [rsp]
    vmovntdqa   ymm1, [rsp]
    vbroadcastss ymm2, [rsp + 4]
    vbroadcastsd ymm3, [rsp + 8]
    vbroadcastss xmm4, xmm0

    vmovntdq    [rsp + 0x20], ymm2
    vmovntps    [rsp], ymm3
    vmovntpd    [rsp + 0x20], ymm1

    add         rsp, 0x40
    retn
