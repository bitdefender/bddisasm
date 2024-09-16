    bits 64

    global _start
    
    section .text
    
_start:
    nop
    clc
    mov     al, 0xAA
    rcr     al, 0
    
    nop
    clc
    mov     al, 0xAA
    rcr     al, 1
    
    nop
    clc
    mov     al, 0xAA
    rcr     al, 7
    
    nop
    clc
    mov     al, 0xAA
    rcr     al, 8
    
    nop
    clc
    mov     al, 0xAA
    rcr     al, 30
    
    
    
    nop
    clc
    mov     al, 0x77
    rcr     al, 0
    
    nop
    clc
    mov     al, 0x77
    rcr     al, 1
    
    nop
    clc
    mov     al, 0x77
    rcr     al, 7
    
    nop
    clc
    mov     al, 0x77
    rcr     al, 8
    
    nop
    clc
    mov     al, 0x77
    rcr     al, 30
    
    
    
    nop
    clc
    mov     al, 0xFF
    rcr     al, 0
    
    nop
    clc
    mov     al, 0xFF
    rcr     al, 1
    
    nop
    clc
    mov     al, 0xFF
    rcr     al, 7
    
    nop
    clc
    mov     al, 0xFF
    rcr     al, 8
    
    nop
    clc
    mov     al, 0xFF
    rcr     al, 30
    
    
    
    
    
    
    nop
    stc
    mov     al, 0xAA
    rcr     al, 0
    
    nop
    stc
    mov     al, 0xAA
    rcr     al, 1
    
    nop
    stc
    mov     al, 0xAA
    rcr     al, 7
    
    nop
    stc
    mov     al, 0xAA
    rcr     al, 8
    
    nop
    stc
    mov     al, 0xAA
    rcr     al, 30
    
    
    
    nop
    stc
    mov     al, 0x77
    rcr     al, 0
    
    nop
    stc
    mov     al, 0x77
    rcr     al, 1
    
    nop
    stc
    mov     al, 0x77
    rcr     al, 7
    
    nop
    stc
    mov     al, 0x77
    rcr     al, 8
    
    nop
    stc
    mov     al, 0x77
    rcr     al, 30
    
    
    
    nop
    stc
    mov     al, 0xFF
    rcr     al, 0
    
    nop
    stc
    mov     al, 0xFF
    rcr     al, 1
    
    nop
    stc
    mov     al, 0xFF
    rcr     al, 7
    
    nop
    stc
    mov     al, 0xFF
    rcr     al, 8
    
    nop
    stc
    mov     al, 0xFF
    rcr     al, 30
    
    int3