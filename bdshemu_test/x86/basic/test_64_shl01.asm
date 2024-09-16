    bits 64

    global _start
    
    section .text
    
_start:
    nop
    clc
    mov     al, 0xAA
    shl     al, 0
    
    nop
    clc
    mov     al, 0xAA
    shl     al, 1
    
    nop
    clc
    mov     al, 0xAA
    shl     al, 7
    
    nop
    clc
    mov     al, 0xAA
    shl     al, 8
    
    nop
    clc
    mov     al, 0xAA
    shl     al, 30
    
    
    
    nop
    clc
    mov     al, 0x77
    shl     al, 0
    
    nop
    clc
    mov     al, 0x77
    shl     al, 1
    
    nop
    clc
    mov     al, 0x77
    shl     al, 7
    
    nop
    clc
    mov     al, 0x77
    shl     al, 8
    
    nop
    clc
    mov     al, 0x77
    shl     al, 30
    
    
    
    nop
    clc
    mov     al, 0xFF
    shl     al, 0
    
    nop
    clc
    mov     al, 0xFF
    shl     al, 1
    
    nop
    clc
    mov     al, 0xFF
    shl     al, 7
    
    nop
    clc
    mov     al, 0xFF
    shl     al, 8
    
    nop
    clc
    mov     al, 0xFF
    shl     al, 30
    
    int3