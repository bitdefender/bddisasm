    bits 32

    db 0x66
    pusha
    pushad
    db 0x66
    popa
    popad
    
    db 0x26, 0x82, 0xc0, 0xe4                   ; add al, 0xe4
    db 0x26, 0x0f, 0x05                         ; syscall - even though SDM states it's invalid, it works in 32 bit
    db 0x26, 0x0f, 0x07                         ; sysret - even though SDM states it's invalid, it works in 32 bit
    
    db 0xf3, 0x66, 0x0f, 0xc7, 0xf8             ; rdpid eax - reg is 32 bit in 16/32 bit mode, 64 bit in 64 bit mode
    
    db 0xf3, 0x0f, 0x1b, 0x05, 0xa4, 0x27, 0x4e, 0x6d   ; bndmk bnd0, [0x6d4e27a4] - Works on 32, #UD in 64 bit mode if RIP relative.
    
    db 0xc4, 0xe1, 0x3a, 0x10, 0xca             ; vmovss xmm1, xmm0, xmm2 - bit 3 of vex.vvvv is ingored in 32 bit mode.
    