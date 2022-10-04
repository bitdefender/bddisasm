    bits 64
    
    ;
    ; 64 bit addressing
    ;
    
    ; Basic indirect addressing, using register.
    mov     eax, dword [rcx]
    mov     rax, qword [r15]
    
    ; Base + offset 1B
    mov     eax, dword [rcx + 0x7F]
    mov     rax, qword [r15 - 0x7F]
    
    ; Base + offset 4B
    mov     eax, dword [rcx + 0x7FFFFFFF]
    mov     rax, qword [r15 - 0x7FFFFFFF]
    
    ; Base + index
    mov     eax, dword [rcx + r8]
    mov     rax, qword [r15 + rdi]
    
    ; Base + index * scale
    mov     eax, dword [rcx + r8 * 4]
    mov     rax, qword [r15 + rdi * 8]
    
    ; Base + index * scale + displacement
    mov     eax, dword [rcx + r8 * 4 + 0x1000]
    mov     rax, qword [r15 + rdi * 8 - 0x1000]
    
    ; RIP-relative
    mov     eax, dword [rel $]
    mov     rax, qword [rel $]    
    
    ; RSP only
    mov     eax, dword [rsp]
    mov     rax, qword [rsp + 0x1000]
    
    
    
    ;
    ; 32 bit addressing
    ;
    
    ; Basic indirect addressing, using register.
    mov     eax, dword [ecx]
    mov     rax, qword [r15d]
    
    ; Base + offset 1B
    mov     eax, dword [ecx + 0x7F]
    mov     rax, qword [r15d - 0x7F]
    
    ; Base + offset 4B
    mov     eax, dword [ecx + 0x7FFFFFFF]
    mov     rax, qword [r15d - 0x7FFFFFFF]
    
    ; Base + index
    mov     eax, dword [ecx + r8d]
    mov     rax, qword [r15d + edi]
    
    ; Base + index * scale
    mov     eax, dword [ecx + r8d * 4]
    mov     rax, qword [r15d + edi * 8]
    
    ; Base + index * scale + displacement
    mov     eax, dword [ecx + r8d * 4 + 0x1000]
    mov     rax, qword [r15d + edi * 8 - 0x1000]
    
    ; ESP only
    mov     eax, dword [esp]
    mov     rax, qword [esp + 0x1000]
    
    
    ;
    ; Segment prefix.
    ;
    mov     rax, qword [gs:0x30]
    mov     rax, qword [gs:rcx]
    mov     rax, qword [gs:rcx + r15]
    mov     rax, qword [gs:rcx + r15 * 2]
    mov     rax, qword [gs:rcx + r15 * 2 + 0x1000]