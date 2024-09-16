
    bits 64
    
    nop
    lea     rax, [rel new_stack]
    dec     rax
    xchg    rsp, rax
    int3
    
    align 8
    
new_stack:
    resq    64