    bits 64
    
    ud2
    sfence
    lfence
    mfence
    
    monitor
    mwait
    
    prefetcht0  [rbx]
    prefetcht1  [rbx]
    prefetcht2  [rbx]
    prefetchnta [rbx]
    
    cmovc       ax, cx
    cmovc       eax, ecx
    cmovc       rax, rcx
    cmovc       ax, [ebx]
    cmovc       ax, [rbx]
    cmovc       eax, [ebx]
    cmovc       eax, [rbx]
    cmovc       rax, [ebx]
    cmovc       eax, [rbx]

    setnz       al
    setae       cl
    seto        spl
    setnae      sil
    
    cpuid
    
    shrd        ax, cx, 10
    shrd        eax, ecx, 10
    shrd        rax, rcx, 10   
    shrd        ax, cx, cl
    shrd        eax, ecx, cl
    shrd        rax, rcx, cl   

    shld        ax, cx, 10
    shld        eax, ecx, 10
    shld        rax, rcx, 10   
    shld        ax, cx, cl
    shld        eax, ecx, cl
    shld        rax, rcx, cl       
    
    bt          ax, 1
    bt          ax, cx
    bt          eax, 1
    bt          eax, ecx
    bt          rax, 1
    bt          rax, rcx
    bt          word [ebx], 1    
    bt          word [ebx], cx
    bt          word [rbx], 1
    bt          word [rbx], cx
    bt          dword [ebx], 1
    bt          dword [ebx], ecx
    bt          dword [rbx], 1
    bt          dword [rbx], ecx
    bt          qword [ebx], 1
    bt          qword [ebx], rcx
    bt          qword [rbx], 1
    bt          qword [rbx], rcx
    
    bts         ax, 1
    bts         ax, cx
    bts         eax, 1
    bts         eax, ecx
    bts         rax, 1
    bts         rax, rcx
    bts         word [ebx], 1    
    bts         word [ebx], cx
    bts         word [rbx], 1
    bts         word [rbx], cx
    bts         dword [ebx], 1
    bts         dword [ebx], ecx
    bts         dword [rbx], 1
    bts         dword [rbx], ecx
    bts         qword [ebx], 1
    bts         qword [ebx], rcx
    bts         qword [rbx], 1
    bts         qword [rbx], rcx
    
    btr         ax, 1
    btr         ax, cx
    btr         eax, 1
    btr         eax, ecx
    btr         rax, 1
    btr         rax, rcx
    btr         word [ebx], 1    
    btr         word [ebx], cx
    btr         word [rbx], 1
    btr         word [rbx], cx
    btr         dword [ebx], 1
    btr         dword [ebx], ecx
    btr         dword [rbx], 1
    btr         dword [rbx], ecx
    btr         qword [ebx], 1
    btr         qword [ebx], rcx
    btr         qword [rbx], 1
    btr         qword [rbx], rcx
    
    btc         ax, 1
    btc         ax, cx
    btc         eax, 1
    btc         eax, ecx
    btc         rax, 1
    btc         rax, rcx
    btc         word [ebx], 1    
    btc         word [ebx], cx
    btc         word [rbx], 1
    btc         word [rbx], cx
    btc         dword [ebx], 1
    btc         dword [ebx], ecx
    btc         dword [rbx], 1
    btc         dword [rbx], ecx
    btc         qword [ebx], 1
    btc         qword [ebx], rcx
    btc         qword [rbx], 1
    btc         qword [rbx], rcx
    
    xsave       [rax]
    xrstor      [rax]
    xsavec64    [rbx]
    xsaves      [rbx]
    
    fxsave      [rbx]
    fxrstor     [rbx]
    emms
    
    cmpxchg     [eax], edx
    cmpxchg     [rax], rdx
    cmpxchg8b   [rax + rdx * 2]
    cmpxchg16b  [esi + edi * 4]
    
    xadd        [rax], rdx
    xadd        [ebx], eax
    
    bswap       eax
    bswap       rax
    bswap       r15d
    bswap       r14
    
    popcnt      ax, cx
    popcnt      eax, ecx
    popcnt      rax, rcx
    popcnt      ax, [rbx]
    popcnt      eax, [rbx]
    popcnt      rax, [rbx]
    
    lzcnt       ax, cx
    lzcnt       eax, ecx
    lzcnt       rax, rcx
    lzcnt       ax, [rbx]
    lzcnt       eax, [rbx]
    lzcnt       rax, [rbx]
    
    tzcnt       ax, cx
    tzcnt       eax, ecx
    tzcnt       rax, rcx
    tzcnt       ax, [rbx]
    tzcnt       eax, [rbx]
    tzcnt       rax, [rbx]
    
    rdrand      ax
    rdrand      eax
    rdrand      rax
    rdseed      r14w
    rdseed      r14d    
    rdseed      r14