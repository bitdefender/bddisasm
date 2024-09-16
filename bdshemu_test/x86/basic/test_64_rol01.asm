    bits 64

    global _start
    
    section .text
    
_start:
    nop
    clc
    mov     al, 0xAA
    rol     al, 0
    
    nop
    clc
    mov     al, 0xAA
    rol     al, 1
    
    nop
    clc
    mov     al, 0xAA
    rol     al, 7
    
    nop
    clc
    mov     al, 0xAA
    rol     al, 8
    
    nop
    clc
    mov     al, 0xAA
    rol     al, 30
    
    
    
    nop
    clc
    mov     al, 0x77
    rol     al, 0
    
    nop
    clc
    mov     al, 0x77
    rol     al, 1
    
    nop
    clc
    mov     al, 0x77
    rol     al, 7
    
    nop
    clc
    mov     al, 0x77
    rol     al, 8
    
    nop
    clc
    mov     al, 0x77
    rol     al, 30
    
    
    
    nop
    clc
    mov     al, 0xFF
    rol     al, 0
    
    nop
    clc
    mov     al, 0xFF
    rol     al, 1
    
    nop
    clc
    mov     al, 0xFF
    rol     al, 7
    
    nop
    clc
    mov     al, 0xFF
    rol     al, 8
    
    nop
    clc
    mov     al, 0xFF
    rol     al, 30
    
    int3