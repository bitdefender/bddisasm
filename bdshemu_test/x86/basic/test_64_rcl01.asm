    bits 64

    global _start
    
    section .text
    
_start:
    nop
    clc
    mov     al, 0xAA
    rcl     al, 0
    
    nop
    clc
    mov     al, 0xAA
    rcl     al, 1
    
    nop
    clc
    mov     al, 0xAA
    rcl     al, 7
    
    nop
    clc
    mov     al, 0xAA
    rcl     al, 8
    
    nop
    clc
    mov     al, 0xAA
    rcl     al, 30
    
    
    
    nop
    clc
    mov     al, 0x77
    rcl     al, 0
    
    nop
    clc
    mov     al, 0x77
    rcl     al, 1
    
    nop
    clc
    mov     al, 0x77
    rcl     al, 7
    
    nop
    clc
    mov     al, 0x77
    rcl     al, 8
    
    nop
    clc
    mov     al, 0x77
    rcl     al, 30
    
    
    
    nop
    clc
    mov     al, 0xFF
    rcl     al, 0
    
    nop
    clc
    mov     al, 0xFF
    rcl     al, 1
    
    nop
    clc
    mov     al, 0xFF
    rcl     al, 7
    
    nop
    clc
    mov     al, 0xFF
    rcl     al, 8
    
    nop
    clc
    mov     al, 0xFF
    rcl     al, 30
    
    
    
    
    
    
    nop
    stc
    mov     al, 0xAA
    rcl     al, 0
    
    nop
    stc
    mov     al, 0xAA
    rcl     al, 1
    
    nop
    stc
    mov     al, 0xAA
    rcl     al, 7
    
    nop
    stc
    mov     al, 0xAA
    rcl     al, 8
    
    nop
    stc
    mov     al, 0xAA
    rcl     al, 30
    
    
    
    nop
    stc
    mov     al, 0x77
    rcl     al, 0
    
    nop
    stc
    mov     al, 0x77
    rcl     al, 1
    
    nop
    stc
    mov     al, 0x77
    rcl     al, 7
    
    nop
    stc
    mov     al, 0x77
    rcl     al, 8
    
    nop
    stc
    mov     al, 0x77
    rcl     al, 30
    
    
    
    nop
    stc
    mov     al, 0xFF
    rcl     al, 0
    
    nop
    stc
    mov     al, 0xFF
    rcl     al, 1
    
    nop
    stc
    mov     al, 0xFF
    rcl     al, 7
    
    nop
    stc
    mov     al, 0xFF
    rcl     al, 8
    
    nop
    stc
    mov     al, 0xFF
    rcl     al, 30
    
    int3