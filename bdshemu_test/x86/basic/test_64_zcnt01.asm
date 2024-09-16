    bits 64

    global _start
    
    section .text
    
_start:
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000000000000
    lzcnt   cx, ax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000000000000
    lzcnt   ecx, eax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000000000000
    lzcnt   rcx, rax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000080000000
    lzcnt   cx, ax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000080000000
    lzcnt   ecx, eax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000080000000
    lzcnt   rcx, rax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000000000001
    lzcnt   cx, ax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000000000001
    lzcnt   ecx, eax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000000000001
    lzcnt   rcx, rax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0xFFFFFFFFFFFFFFFF
    lzcnt   cx, ax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0xFFFFFFFFFFFFFFFF
    lzcnt   ecx, eax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0xFFFFFFFFFFFFFFFF
    lzcnt   rcx, rax
    
    
    
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000000000000
    tzcnt   cx, ax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000000000000
    tzcnt   ecx, eax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000000000000
    tzcnt   rcx, rax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000080000000
    tzcnt   cx, ax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000080000000
    tzcnt   ecx, eax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000080000000
    tzcnt   rcx, rax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000000000001
    tzcnt   cx, ax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000000000001
    tzcnt   ecx, eax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0x0000000000000001
    tzcnt   rcx, rax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0xFFFFFFFFFFFFFFFF
    tzcnt   cx, ax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0xFFFFFFFFFFFFFFFF
    tzcnt   ecx, eax
    
    nop
    mov     rcx, 0xFFFFFFFFFFFFFFFF
    mov     rax, 0xFFFFFFFFFFFFFFFF
    tzcnt   rcx, rax
    
    int3