    bits 64
    
    vcvtph2ps           xmm7,xmm13
    vcvtph2ps           ymm7,xmm13
    vcvtps2ph           xmm13,xmm7,10
    vcvtps2ph           xmm13,ymm7,10 
    
    vcvtph2ps           xmm7,[rbx]
    vcvtph2ps           ymm7,[rbx]
    vcvtps2ph           [rbx],xmm7,10
    vcvtps2ph           [rbx],ymm7,10 