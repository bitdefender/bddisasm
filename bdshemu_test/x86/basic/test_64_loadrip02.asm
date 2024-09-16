    
    bits 64

    FFREE     st3
    FXSAVE    [rsp+0x0]
    ADD       rsp, 0x0000000000000008
    MOV       rax, qword [rsp+0x0]
    int3