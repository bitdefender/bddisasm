    bits 32
    
    ;
    ; 32 bit addressing
    ;
    mov     eax, dword [ecx]
    mov     eax, dword [ecx + edi]
    mov     eax, dword [ecx + edi * 4]
    mov     eax, dword [ecx + edi * 4 + 0x7f]
    mov     eax, dword [ecx + edi * 8 + 0x7fffffff]
    mov     eax, dword [0x7fffffff]
    
    
    ;
    ; 16 bit addressing
    ;
    mov     eax, dword [bp]
    mov     eax, dword [0x7FFF]
    mov     eax, dword [bx + 0x7F]
    mov     eax, dword [bx + 0x7FFF]
    mov     eax, dword [bp + di]
    mov     eax, dword [bp + si]
    
    
    ;
    ; Segment prefix.
    ;
    mov     eax, dword [fs:0x30]
    mov     eax, dword [fs:ecx]
    mov     eax, dword [fs:ecx + edi]
    mov     eax, dword [fs:ecx + edi * 2]
    mov     eax, dword [fs:ecx + edi * 2 + 0x1000]