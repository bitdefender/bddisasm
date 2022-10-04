    bits 32
    
    ; These should operate on CR8, as per AMD hack to make CR8 accessible outside long mode.
    db 0xF0
    mov     eax, cr0
    db 0xF0
    mov     cr0, eax
    