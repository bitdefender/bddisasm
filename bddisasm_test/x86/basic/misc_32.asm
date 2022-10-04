    bits 32
    
    daa
    das
    aaa
    aas
    aam     100
    aad     100
    
    bound   ax, [ebx + esi]
    bound   eax, [ebx + esi]
    arpl    [bx], ax
    arpl    [ebx], ax
    
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
    cwd
    cdq
    wait
    
    sahf
    lahf
    salc
    
    lea     ax, [bx]
    lea     ax, [bx + si]
    
    les     ax, [bx]
    les     ax, [ebx]
    les     eax, [bx]
    les     eax, [ebx]
    
    lds     ax, [bx]
    lds     ax, [ebx]
    lds     eax, [bx]
    lds     eax, [ebx]
    
    lss     ax, [bx]
    lss     ax, [ebx]
    lss     eax, [bx]
    lss     eax, [ebx]
    
    lfs     ax, [bx]
    lfs     ax, [ebx]
    lfs     eax, [bx]
    lfs     eax, [ebx]
    
    lgs     ax, [bx]
    lgs     ax, [ebx]
    lgs     eax, [bx]
    lgs     eax, [ebx]
 
 
 
 
 