    
    bits 64

    sub     rsp, 0x40
    mov     rax, 0x1122334455667788
    mov     qword [rsp], rax
    mov     rax, 0x99AABBCCDDEEFF00
    mov     qword [rsp + 0x08], rax
    mov     rax, 0x0123456789ABCDEF
    mov     qword [rsp + 0x10], rax
    mov     rax, 0xFEDCBA9876543210
    mov     qword [rsp + 0x18], rax
    mov     rax, 0x5A5A5A5A5A5A5A5A
    mov     qword [rsp + 0x20], rax
    mov     qword [rsp + 0x28], rax
    mov     qword [rsp + 0x30], rax
    mov     qword [rsp + 0x38], rax

    vmovups ymm0, [rsp]
    vmovaps ymm1, [rsp + 0x20]
    vmovupd ymm2, ymm0
    vmovapd xmm3, xmm1

    vxorps  ymm4, ymm0, ymm1
    vxorpd  xmm5, xmm2, [rsp + 0x20]

    vmovups [rsp], ymm4

    add     rsp, 0x40
    retn
