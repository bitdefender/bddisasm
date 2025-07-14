    bits 64
    
    ; REX2 R4, R3 set, but ignored for segment registers.
    db 0xD5, 0x44, 0x8E, 0xC0
    ; REX2 B4, B3 set, but ignored for FPU registers.
    db 0xD5, 0x11, 0xD8, 0xC0
    ; REX2 X4, X3 set, but ignored for instructions without index.
    db 0xD5, 0x22, 0x33, 0xC0
    ; REX2 R4 set, but ignored for SSE registers.
    db 0x66, 0xD5, 0xC4, 0xFE, 0xC0
    ; REX2 B4 set, but ignored for SSE registers.
    db 0x66, 0xD5, 0x91, 0xFE, 0xC0
    ; REX2 R4, R3 set, but ignored for MMX registers.
    db 0xD5, 0xC4, 0xFE, 0xC0
    ; REX2 B4, B3 set, but ignored for MMX registers.
    db 0xD5, 0x91, 0xFE, 0xC0
