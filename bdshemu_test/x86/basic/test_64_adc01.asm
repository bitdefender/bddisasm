    bits 64

    global _start
    
    section .text
    
_start:
    nop
    stc
    mov     al, 0xFF
    adc     al, 0xFF
    
    nop
    stc
    mov     al, 0x60
    adc     al, 0xFF
    
    nop
    stc
    mov     al, 0xFF
    adc     al, 0x60
    
    nop
    stc
    mov     al, 1
    adc     al, 0xFE
    
    int3