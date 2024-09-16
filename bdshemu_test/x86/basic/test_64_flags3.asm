    bits 64
    
    xor     eax, eax
    
    mov     eax, 0xAA
    mov     cl, 0
    shr     eax, cl
    shl     eax, cl
    sar     eax, cl
    
    mov     eax, 0xAA
    mov     cl, 255
    shr     eax, cl
    
    mov     eax, 0xAA
    mov     cl, 255
    sar     eax, cl
    
    mov     eax, 0xAA
    mov     cl, 255
    shl     eax, cl
    
    xor     eax, eax
    
    mov     eax, 0x0
    mov     cl, 1
    shr     eax, cl
    mov     eax, 0x1
    shr     eax, cl
    mov     eax, 0xff
    shr     eax, cl
    
    mov     eax, 0x0
    mov     cl, 2
    shr     eax, cl
    mov     eax, 0x1
    shr     eax, cl
    mov     eax, 0xff
    shr     eax, cl
    
    mov     eax, 0x0
    mov     cl, 1
    sar     eax, cl
    mov     eax, 0x1
    sar     eax, cl
    mov     eax, 0xff
    sar     eax, cl
    
    mov     eax, 0x0
    mov     cl, 2
    sar     eax, cl
    mov     eax, 0x1
    sar     eax, cl
    mov     eax, 0xff
    sar     eax, cl
    
    mov     eax, 0x0
    mov     cl, 1
    shl     eax, cl
    mov     eax, 0x80
    shl     eax, cl
    mov     eax, 0xff
    shl     eax, cl
    
    mov     eax, 0x0
    mov     cl, 2
    shl     eax, cl
    mov     eax, 0x80
    shl     eax, cl
    mov     eax, 0xff
    shl     eax, cl
    
    
    retn