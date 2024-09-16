    bits 64

    global _start
    
    section .text
    
_start:
    nop
    clc
    mov     al, 0xAA
    shr     al, 0
    
    nop
    clc
    mov     al, 0xAA
    shr     al, 1
    
    nop
    clc
    mov     al, 0xAA
    shr     al, 7
    
    nop
    clc
    mov     al, 0xAA
    shr     al, 8
    
    nop
    clc
    mov     al, 0xAA
    shr     al, 30
    
    
    
    nop
    clc
    mov     al, 0x77
    shr     al, 0
    
    nop
    clc
    mov     al, 0x77
    shr     al, 1
    
    nop
    clc
    mov     al, 0x77
    shr     al, 7
    
    nop
    clc
    mov     al, 0x77
    shr     al, 8
    
    nop
    clc
    mov     al, 0x77
    shr     al, 30
    
    
    
    nop
    clc
    mov     al, 0xFF
    shr     al, 0
    
    nop
    clc
    mov     al, 0xFF
    shr     al, 1
    
    nop
    clc
    mov     al, 0xFF
    shr     al, 7
    
    nop
    clc
    mov     al, 0xFF
    shr     al, 8
    
    nop
    clc
    mov     al, 0xFF
    shr     al, 30
    
    int3