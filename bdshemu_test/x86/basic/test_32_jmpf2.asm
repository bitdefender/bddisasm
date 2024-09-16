
    bits 32
    
    call    $+5
    pop     ebp
    sub     ebp, 5
    
    lea     eax, [ebp + dest]
    mov     dword [ebp + t_offset], eax
    
    jmp     far [ebp + t_offset]
    
dest:
    int3
    
t_offset  dd    0x0
t_segment dw    0x33
    
