    bits 64
    
    push    ax
    pop     ax
    push    rax
    pop     rax
    
    push    word 0x7F
    push    dword 0x7FFF
    push    qword 0x7FFFFFFF    
    
    push    word [rbx]
    pop     word [rbx]
    push    word [ebx]
    pop     word [ebx]
    push    qword [rbx]
    pop     qword [rbx]
    push    qword [ebx]
    pop     qword [ebx]
    
    enter   0x10, 0x20
    leave
    
    push    fs
    pop     fs
    push    gs
    pop     gs
    
    pushfw
    popfw
    pushfq
    popfq