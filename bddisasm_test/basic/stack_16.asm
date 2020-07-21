    bits 16
    
    push    ax
    pop     ax
    push    eax
    pop     eax
    
    push    word 0x7F
    push    dword 0x7FFF
    
    push    word [bx]
    pop     word [bx]
    push    word [ebx]
    pop     word [ebx]
    push    dword [bx]
    pop     dword [bx]
    push    dword [ebx]
    pop     dword [ebx]
    
    enter   0x10, 0x20
    leave
    
    push    es
    pop     es
    push    ds
    pop     ds
    push    ss
    pop     ss
    push    fs
    pop     fs
    push    gs
    pop     gs
    
    pusha
    popa
    pushfw
    popfw
    pushfd
    popfd