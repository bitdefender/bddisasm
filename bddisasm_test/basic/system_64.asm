    bits 64
    
    ; System instructions
    mov     cr0, rax
    mov     rax, cr0
    mov     rcx, cr2
    mov     cr2, rcx
    mov     cr4, r15
    mov     r15, cr4
    mov     cr8, r8
    mov     r8, cr8
    
    mov     dr0, rax
    mov     rax, dr0
    mov     dr1, r15
    mov     r15, dr1
    mov     dr7, rcx
    mov     rcx, dr7

    lidt    [rbx]
    lgdt    [rbx]
    sidt    [rbx]
    sgdt    [rbx]
    ltr     ax
    str     r15w
    lldt    [rbx]
    sldt    [rbx]
    
    rdmsr    
    wrmsr
    rdtsc
    rdtscp
    
    verw    ax
    verr    r11w    
    
    lsl     eax, eax
    lar     r14d, r13d
    
    lmsw    cx
    smsw    sp
    
    invlpg  [rax]
    invpcid rax, [rbp]
    
    rdpkru
    wrpkru
    
    clac
    stac
    
    clts
    invd
    wbinvd
    
    db 0x0F, 0x01, 0xE8 ; serialize
    
    db 0xF3, 0x0F, 0x3A, 0xF0, 0xC0, 0xBD   ; hreset