    bits 64
    
    vmrun
    vmmcall
    db 0xF3, 0x0F, 0x01, 0xD9 ; vmgexit
    vmload
    vmsave
    stgi
    clgi
    skinit
    invlpga