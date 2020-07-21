    bits 64
    
    nop
    pause
    clc
    cmc
    stc
    cld
    std
    cli
    sti
    cbw
    cwde
    cdqe
    cwd
    cdq
    cqo
    wait
    
    sahf
    lahf
    salc
    
    lea     ax, [ebx]
    lea     eax, [ebx]
    lea     rax, [ebx]
    lea     ax, [rbx]
    lea     eax, [rbx]
    lea     rax, [rbx]
        
    lss     ax, [ebx]
    lss     ax, [rbx]
    lss     eax, [ebx]
    lss     eax, [rbx]
    lss     rax, [ebx]
    lss     rax, [rbx]
   
    lfs     ax, [ebx]
    lfs     ax, [rbx]
    lfs     eax, [ebx]
    lfs     eax, [rbx]
    lfs     rax, [ebx]
    lfs     rax, [rbx]
    
    lgs     ax, [ebx]
    lgs     ax, [rbx]
    lgs     eax, [ebx]
    lgs     eax, [rbx]
    lgs     rax, [ebx]
    lgs     rax, [rbx]
    
    rdfsbase    eax
    rdfsbase    rax
    rdgsbase    eax
    rdgsbase    rax
    wrfsbase    eax
    wrfsbase    rax
    wrgsbase    eax
    wrgsbase    rax
 
    movbe   ax, [rbx]
    movbe   eax, [rbx]
    movbe   rax, [rbx]
    
    movbe   [rbx], ax
    movbe   [rbx], eax
    movbe   [rbx], rax
    
    crc32   eax, al
    crc32   rax, al
    crc32   eax, byte [rbx]
    crc32   rax, byte [rbx]
    
    ;crc32   eax, ax
    ;crc32   rax, ax
    crc32   eax, word [rbx]
    ;crc32   rax, word [rbx]
    
    ;crc32   eax, eax
    ;crc32   rax, eax
    crc32   eax, dword [rbx]
    ;crc32   rax, dword [rbx]
    
    ;crc32   eax, eax
    ;crc32   rax, eax
    ;crc32   eax, qword [rbx]
    crc32   rax, qword [rbx]
    
    adcx    rax, rcx
    adcx    rax, [rbx]
    adox    rax, rcx
    adox    rax, [rbx]