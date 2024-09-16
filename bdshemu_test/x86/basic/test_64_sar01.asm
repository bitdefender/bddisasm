    bits 64

    global _start
    
    section .text
    
_start:
    nop
    clc
    mov     al, 0xAA
    sar     al, 1
    ; CF = 0
    
    nop
    clc
    mov     al, 0xAA
    sar     al, 7
    ; CF = 0
    
    nop
    clc
    mov     al, 0xAA
    sar     al, 8
    ; CF = 1
    
    nop
    clc
    mov     al, 0xAA
    sar     al, 30
    ; CF = 1
    
    nop
    clc
    mov     al, 0x77
    sar     al, 1
    ; CF = 1
    
    nop
    clc
    mov     al, 0x77
    sar     al, 7
    ; CF = 1
    
    nop
    clc
    mov     al, 0x77
    sar     al, 8
    ; CF = 0
    
    nop
    clc
    mov     al, 0x77
    sar     al, 30
    ; CF = 0
    
    int3