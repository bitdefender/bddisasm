    bits 64
    
    MOV       edx, 0x80000000
    NOT       edi
    IDIV      edi
    RETN