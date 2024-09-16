    bits 64

    global _start
    
    section .text
    
_start:
    nop
    stc
    mov     al, 0xFF
    sbb     al, 0xFF
    ; CF = 1
    
    nop
    stc
    mov     al, 0xFF
    sbb     al, 0
    ; CF = 0
    
    nop
    stc
    mov     al, 0
    sbb     al, 0xFF
    ; CF = 1
    
    nop
    stc
    mov     al, 0xFF
    sbb     al, 0x60
    ; CF = 0
    
    nop
    stc
    mov     al, 0x60
    sbb     al, 0xFF
    ; CF = 1
    
    nop
    clc
    mov     al, 0x20
    sbb     al, 0x20
    ; CF = 0
    
    nop
    stc
    mov     al, 0x20
    sbb     al, 0x20
    ; CF = 1
    
    
    int3