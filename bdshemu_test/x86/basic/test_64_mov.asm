    
    bits 64

    mov     ah, 0xBD
    mov     al, 0x12
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     cx, ax
    mov     ecx, eax
    mov     rbx, rax
    
    mov     ax, 0xFFFF
    movzx   ecx, ax
    movsx   ebx, ax

    mov     eax, 0x12345678
    mov     ebx, 0x09ABCDEF
    xchg    eax, ebx

    mov     qword [rsp], -1
    xchg    rax, qword [rsp]
    
    retn