    bits 64
    
    lea     rsi, [rel data]
    
    mov     rax, 0x0000000000000000
    movsx   eax, byte [rsi]    
    mov     rax, 0xFFFFFFFFFFFFFFFF
    movsx   eax, byte [rsi]
    
    mov     rax, 0x0000000000000000
    movzx   eax, byte [rsi]    
    mov     rax, 0xFFFFFFFFFFFFFFFF
    movzx   eax, byte [rsi]
    
    mov     rax, 0x0000000000000000
    movsx   eax, byte [rsi + 8]    
    mov     rax, 0xFFFFFFFFFFFFFFFF
    movsx   eax, byte [rsi + 8]
    
    mov     rax, 0x0000000000000000
    movzx   eax, byte [rsi + 8]    
    mov     rax, 0xFFFFFFFFFFFFFFFF
    movzx   eax, byte [rsi + 8]
    
    int3
    
data:
    dq  0x1111111111111111
    dq  0x8888888888888888