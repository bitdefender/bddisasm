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