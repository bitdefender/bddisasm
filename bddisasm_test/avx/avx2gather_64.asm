    bits 64
    
    vgatherdpd xmm2, [rbx+xmm7*8+0x1000], xmm13
    vgatherdps xmm2, [rbx+xmm7*8+0x1000], xmm13
    vgatherqpd xmm2, [rbx+xmm7*8+0x1000], xmm13
    vgatherqps xmm2, [rbx+xmm7*8+0x1000], xmm13
    vgatherqps xmm2, [rbx+ymm17*8+0x1000], xmm13
    vpgatherdd xmm2, [rbx+xmm7*8+0x1000], xmm13
    vpgatherdq xmm2, [rbx+xmm7*8+0x1000], xmm13
    vpgatherqd xmm2, [rbx+xmm7*8+0x1000], xmm13
    vpgatherqd xmm2, [rbx+ymm17*8+0x1000], xmm13
    vpgatherqq xmm2, [rbx+xmm7*8+0x1000], xmm13
