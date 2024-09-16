    
    bits 64

_xxx:    
    jmp     _yyy
    
    nop
    nop
    nop
    
_yyy:    
    jmp     _xxx - 1