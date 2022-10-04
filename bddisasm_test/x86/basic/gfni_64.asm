    bits 64
    
    ; Legacy
    gf2p8mulb           xmm7,xmm13
    gf2p8affineqb       xmm7,xmm13,10
    gf2p8affineinvqb    xmm7,xmm13,10
    
    gf2p8mulb           xmm7,[rbx]
    gf2p8affineqb       xmm7,[rbx],10
    gf2p8affineinvqb    xmm7,[rbx],10
    
    ; VEX 128
    vgf2p8mulb          xmm7,xmm15,xmm13
    vgf2p8affineqb      xmm7,xmm15,xmm13,10
    vgf2p8affineinvqb   xmm7,xmm15,xmm13,10
    
    vgf2p8mulb          xmm7,xmm15,[rbx]
    vgf2p8affineqb      xmm7,xmm15,[rbx],10
    vgf2p8affineinvqb   xmm7,xmm15,[rbx],10
    
    ; VEX 256
    vgf2p8mulb          ymm7,ymm15,ymm13
    vgf2p8affineqb      ymm7,ymm15,ymm13,10
    vgf2p8affineinvqb   ymm7,ymm15,ymm13,10
    
    vgf2p8mulb          ymm7,ymm15,[rbx]
    vgf2p8affineqb      ymm7,ymm15,[rbx],10
    vgf2p8affineinvqb   ymm7,ymm15,[rbx],10
    
    ; EVEX 512
    vgf2p8mulb          zmm7,zmm31,zmm13
    vgf2p8affineqb      zmm7,zmm31,zmm13,10
    vgf2p8affineinvqb   zmm7,zmm31,zmm13,10
    
    vgf2p8mulb          zmm7{k5},zmm31,zmm13
    vgf2p8affineqb      zmm7{k5},zmm31,zmm13,10
    vgf2p8affineinvqb   zmm7{k5},zmm31,zmm13,10
    
    vgf2p8mulb          zmm7{k5}{z},zmm31,zmm13
    vgf2p8affineqb      zmm7{k5}{z},zmm31,zmm13,10
    vgf2p8affineinvqb   zmm7{k5}{z},zmm31,zmm13,10
    
    vgf2p8mulb          zmm7,zmm31,[rbx]
    vgf2p8affineqb      zmm7,zmm31,[rbx],10
    vgf2p8affineinvqb   zmm7,zmm31,[rbx],10
    
    vgf2p8mulb          zmm7,zmm31,[rbx]
    vgf2p8affineqb      zmm7,zmm31,[rbx]{1to8},10
    vgf2p8affineinvqb   zmm7,zmm31,[rbx]{1to8},10
    
    vgf2p8mulb          zmm7{k5},zmm31,[rbx]
    vgf2p8affineqb      zmm7{k5},zmm31,[rbx]{1to8},10
    vgf2p8affineinvqb   zmm7{k5},zmm31,[rbx]{1to8},10
    
    vgf2p8mulb          zmm7{k5}{z},zmm31,[rbx]
    vgf2p8affineqb      zmm7{k5}{z},zmm31,[rbx]{1to8},10
    vgf2p8affineinvqb   zmm7{k5}{z},zmm31,[rbx]{1to8},10