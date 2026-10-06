    bits 16
    
    ;
    ; 16 bit addressing
    ;
    mov     ax, word [bp]
    mov     ax, word [0x7FFF]
    mov     ax, word [bx + 0x7F]
    mov     ax, word [bx + 0x7FFF]
    mov     ax, word [bp + di]
    mov     ax, word [bp + si]
    
    lea     si, word [bx + si]
    lea     si, word [bx + di]
    lea     si, word [bp + si]
    lea     si, word [bp + di]
    lea     si, word [si]
    lea     si, word [di]
    lea     si, word [0x7FFF]
    lea     si, word [bx]
    
    lea     si, word [0x7F + bx + si]
    lea     si, word [0x7F + bx + di]
    lea     si, word [0x7F + bp + si]
    lea     si, word [0x7F + bp + di]
    lea     si, word [0x7F + si]
    lea     si, word [0x7F + di]
    lea     si, word [0x7F + bp]
    lea     si, word [0x7F + bx]
    
    lea     si, word [0x7FFF + bx + si]
    lea     si, word [0x7FFF + bx + di]
    lea     si, word [0x7FFF + bp + si]
    lea     si, word [0x7FFF + bp + di]
    lea     si, word [0x7FFF + si]
    lea     si, word [0x7FFF + di]
    lea     si, word [0x7FFF + bp]
    lea     si, word [0x7FFF + bx]
    
    
    
    ;
    ; 32 bit addressing
    ;
    mov     ax, word [ecx]
    mov     ax, word [ecx + edi]
    mov     ax, word [ecx + edi * 4]
    mov     ax, word [ecx + edi * 4 + 0x7f]
    mov     ax, word [ecx + edi * 8 + 0x7fffffff]
    mov     ax, word [0x7fffffff]
    
    
    ;
    ; Segment prefix.
    ;
    mov     ax, word [fs:0x30]
    mov     ax, word [fs:ecx]
    mov     ax, word [fs:ecx + edi]
    mov     ax, word [fs:ecx + edi * 2]
    mov     ax, word [fs:ecx + edi * 2 + 0x1000]