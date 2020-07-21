    bits 64
    
    ; legacy, reg - reg
    aesimc      xmm7, xmm13
    aesenc      xmm7, xmm13
    aesenclast  xmm7, xmm13
    aesdec      xmm7, xmm13
    aesdeclast  xmm7, xmm13
    aeskeygenassist xmm7, xmm13, 10
    
    ; legacy, reg - mem
    aesimc      xmm7, [rbx]
    aesenc      xmm7, [rbx]
    aesenclast  xmm7, [rbx]
    aesdec      xmm7, [rbx]
    aesdeclast  xmm7, [rbx]
    aeskeygenassist xmm7, [rbx], 10
    
    ; VEX, reg - reg - reg, 128 bit
    vaesimc     xmm7, xmm13
    vaesenc     xmm7, xmm15, xmm13
    vaesenclast xmm7, xmm15, xmm13
    vaesdec     xmm7, xmm15, xmm13
    vaesdeclast xmm7, xmm15, xmm13
    vaeskeygenassist xmm7, xmm13, 10
    
    ; VEX, reg - reg - mem, 128 bit
    vaesimc     xmm7, [rbx]
    vaesenc     xmm7, xmm15, [rbx]
    vaesenclast xmm7, xmm15, [rbx]
    vaesdec     xmm7, xmm15, [rbx]    
    vaesdeclast xmm7, xmm15, [rbx]
    vaeskeygenassist xmm7, [rbx], 10
    
    ; VEX, reg - reg - reg, 256 bit
    vaesenc     ymm7, ymm15, ymm13
    vaesenclast ymm7, ymm15, ymm13
    vaesdec     ymm7, ymm15, ymm13
    vaesdeclast ymm7, ymm15, ymm13
    
    ; VEX reg - reg - mem, 256 bit
    vaesenc     ymm7, ymm15, [rbx]
    vaesenclast ymm7, ymm15, [rbx]
    vaesdec     ymm7, ymm15, [rbx]    
    vaesdeclast ymm7, ymm15, [rbx]
    
    ; EVEX, reg - reg - reg, 512 bit
    vaesenc     zmm7, zmm31, zmm13
    vaesenclast zmm7, zmm31, zmm13
    vaesdec     zmm7, zmm31, zmm13
    vaesdeclast zmm7, zmm31, zmm13
    
    ; EVEX reg - reg - mem, 512 bit
    vaesenc     zmm7, zmm31, [rbx]
    vaesenclast zmm7, zmm31, [rbx]
    vaesdec     zmm7, zmm31, [rbx]    
    vaesdeclast zmm7, zmm31, [rbx]