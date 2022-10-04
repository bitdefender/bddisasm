    bits 64
    
    call    $
    call    rax
    call    [ebx]
    call    [rbx]
    
    jmp     $
    jmp     rax
    jmp     [ebx]
    jmp     [rbx]
    
    call    far word [rbx]
    call    far dword [rbx]
    call    far qword [rbx]
    jmp     far word [rbx]
    jmp     far dword [rbx]
    jmp     far qword [rbx]
    
    jc      $
    jnc     $
    loop    $
    loopnz  $
    jrcxz   $
    jecxz   $
    
    int     0x21
    int3
    int1
    icebp
    
    ret
    ret     0x20
    retf
    retf    0x20
    
    iretw
    iretd
    
    syscall
    sysexit