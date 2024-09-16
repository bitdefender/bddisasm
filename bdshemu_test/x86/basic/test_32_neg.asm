    bits 32
    
    mov ecx, 2
    sub ecx, 1
    neg ecx
    sbb ecx, ecx
    inc ecx
    mov eax, ecx
    int3