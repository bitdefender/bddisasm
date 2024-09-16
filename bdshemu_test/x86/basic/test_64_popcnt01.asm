    bits 64

    global _start
    
    section .text
    
_start:
    nop
    mov         rcx, 0xFFFFFFFFFFFFFFFF
    mov         rax, 0x0000000000000000
    popcnt      cx, ax
    
    nop
    mov         rcx, 0xFFFFFFFFFFFFFFFF
    mov         rax, 0x0000000000000000
    popcnt      ecx, eax
    
    nop
    mov         rcx, 0xFFFFFFFFFFFFFFFF
    mov         rax, 0x0000000000000000
    popcnt      rcx, rax
    
    nop
    mov         rcx, 0xFFFFFFFFFFFFFFFF
    mov         rax, 0x8000000000000000
    popcnt      cx, ax
    
    nop
    mov         rcx, 0xFFFFFFFFFFFFFFFF
    mov         rax, 0x8000000000000000
    popcnt      ecx, eax
    
    nop
    mov         rcx, 0xFFFFFFFFFFFFFFFF
    mov         rax, 0x8000000000000000
    popcnt      rcx, rax
    
    nop
    mov         rcx, 0xFFFFFFFFFFFFFFFF
    mov         rax, 0xFFFFFFFFFFFFFFFF
    popcnt      cx, ax
    
    nop
    mov         rcx, 0xFFFFFFFFFFFFFFFF
    mov         rax, 0xFFFFFFFFFFFFFFFF
    popcnt      ecx, eax
    
    nop
    mov         rcx, 0xFFFFFFFFFFFFFFFF
    mov         rax, 0xFFFFFFFFFFFFFFFF
    popcnt      rcx, rax
    
    int3