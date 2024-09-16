    bits 64

    global _start
    
    section .text
    
_start:
    nop
    clc
    mov     al, 0xAA
    ror     al, 0
    
    nop
    clc
    mov     al, 0xAA
    ror     al, 1
    
    nop
    clc
    mov     al, 0xAA
    ror     al, 7
    
    nop
    clc
    mov     al, 0xAA
    ror     al, 8
    
    nop
    clc
    mov     al, 0xAA
    ror     al, 30
    
    
    
    nop
    clc
    mov     al, 0x77
    ror     al, 0
    
    nop
    clc
    mov     al, 0x77
    ror     al, 1
    
    nop
    clc
    mov     al, 0x77
    ror     al, 7
    
    nop
    clc
    mov     al, 0x77
    ror     al, 8
    
    nop
    clc
    mov     al, 0x77
    ror     al, 30
    
    
    
    nop
    clc
    mov     al, 0xFF
    ror     al, 0
    
    nop
    clc
    mov     al, 0xFF
    ror     al, 1
    
    nop
    clc
    mov     al, 0xFF
    ror     al, 7
    
    nop
    clc
    mov     al, 0xFF
    ror     al, 8
    
    nop
    clc
    mov     al, 0xFF
    ror     al, 30
    
    int3