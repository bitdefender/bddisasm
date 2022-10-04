    bits 16
    
    call    $
    call    ax
    call    eax
    call    [bx]
    call    [ebx]
    call    word [ebx]
    call    dword [ebx]
    
    jmp     $
    jmp     ax
    jmp     eax
    jmp     [bx]
    jmp     [ebx]
    jmp     word [ebx]
    jmp     dword [ebx]
    
    call    word 0x20:0x1000
    call    dword 0x20:0x10000000
    jmp     word 0x20:0x1000
    call    dword 0x20:0x10000000
    
    call    far word [ebx]
    call    far word [bx]
    call    far dword [ebx]
    call    far dword [bx]
    jmp     far word [ebx]
    jmp     far word [bx]
    jmp     far dword [ebx]
    jmp     far dword [bx]
    
    jc      $
    jnc     $
    loop    $
    loopnz  $
    jcxz    $
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