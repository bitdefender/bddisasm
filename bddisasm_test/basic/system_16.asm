    bits 16
    
    ; System instructions
    mov     cr0, eax
    mov     eax, cr0
    mov     ecx, cr2
    mov     cr2, ecx
    mov     cr4, esi
    mov     esi, cr4
    
    mov     dr0, eax
    mov     eax, dr0
    mov     dr1, esi
    mov     esi, dr1
    mov     dr7, ecx
    mov     ecx, dr7

    lidt    [bx]
    lgdt    [bx]
    sidt    [bx]
    sgdt    [bx]
    ltr     ax
    str     si
    lldt    [bx]
    sldt    [bx]
    
    rdmsr    
    wrmsr
    rdtsc
    rdtscp
    
    verw    ax
    verr    si
    
    lsl     eax, eax
    lar     esi, edi
    
    lmsw    cx
    smsw    sp
    
    invlpg  [eax]
    invpcid eax, [ebp]
        
    clac
    stac
    
    clts
    invd
    wbinvd
    
    db 0x0F, 0x01, 0xE8 ; serialize