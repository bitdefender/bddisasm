    bits 64
    
    vmcall
    vmfunc
    vmlaunch
    vmresume
    vmxoff
    vmread      rax, rcx
    vmwrite     rsp, [edi]
    vmptrld     [rsp + 0x100]
    vmptrst     [edi]
    vmclear     [rax + rcx * 8]
    vmxon       [0x1000]
    invept      rax, [rsi]
    invvpid     rdx, [rsp]