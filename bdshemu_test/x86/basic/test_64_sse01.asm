    
    bits 64

    sub     rsp, 0x40
    mov     rax, 0x1122334455667788
    mov     qword [rsp], rax
    mov     rax, 0x99AABBCCDDEEFF00
    mov     qword [rsp + 0x08], rax
    mov     rax, 0x0F0F0F0F0F0F0F0F
    mov     qword [rsp + 0x10], rax
    mov     qword [rsp + 0x18], rax

    movups  xmm0, [rsp]
    movaps  xmm1, [rsp + 0x10]
    movupd  xmm2, xmm0
    movapd  xmm3, xmm1

    xorps   xmm0, xmm1
    xorpd   xmm2, [rsp + 0x10]
    xorps   xmm3, xmm3

    movaps  [rsp + 0x20], xmm0
    movupd  [rsp + 0x30], xmm2

    add     rsp, 0x40
    retn
