    bits 64
    
    mov     rax, 1
    mov     rcx, 2
    sub     rax, rcx    ; cy
    mov     rax, 2
    mov     rcx, 1
    sub     rax, rcx    ; nc
    mov     rax, 0xffffffffffffffff
    mov     rcx, 1
    sub     rax, rcx    ; nc
    mov     rax, 1
    mov     rcx, 0xffffffffffffffff
    sub     rax, rcx    ; cy
    mov     rax, 0x7fffffffffffffff
    mov     rcx, 0x8000000000000000
    sub     rax, rcx    ; cy
    mov     rax, 0x8000000000000000
    mov     rcx, 0x7fffffffffffffff
    sub     rax, rcx    ; nc
    
    mov     rax, 1
    mov     rcx, 2
    add     rcx, rcx    ; nc

    mov     rax, 0xffffffffffffffff
    mov     rcx, 2
    add     rax, rcx    ; cy
    
    retn