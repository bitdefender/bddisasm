
    bits 32
    
    nop
    call    next
next:
    pop     eax
    sub     eax, next
    lea     eax, [eax + new_stack]
    dec     eax
    xchg    esp, eax
    int3
    
    align 4
    
new_stack:
    resd    64