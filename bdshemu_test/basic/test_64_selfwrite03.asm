    
    bits 64

    lea     rsp, [rel _xxx + 8]
    mov     rax, 0x9090909090909090
    push    rax    
    
_xxx:    
    dq 0    
    retn