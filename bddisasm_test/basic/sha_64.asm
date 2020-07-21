    bits 64
    
    sha1nexte           xmm7,xmm13 
    sha1msg1            xmm7,xmm13 
    sha1msg2            xmm7,xmm13 
    sha256rnds2         xmm7,xmm13 
    sha256msg1          xmm7,xmm13 
    sha256msg2          xmm7,xmm13 
    sha1rnds4           xmm7,xmm13,10
    
    sha1nexte           xmm7,[rbx] 
    sha1msg1            xmm7,[rbx] 
    sha1msg2            xmm7,[rbx] 
    sha256rnds2         xmm7,[rbx] 
    sha256msg1          xmm7,[rbx] 
    sha256msg2          xmm7,[rbx]
    sha1rnds4           xmm7,[rbx],10