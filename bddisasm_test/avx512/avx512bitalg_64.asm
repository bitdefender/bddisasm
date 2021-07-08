    bits 64
    
    vpopcntb xmm2, xmm0
    vpopcntb xmm2, [rbx]
    vpopcntb xmm2, [rbx+r11*8+256]
    vpopcntb xmm2, [rbx+r11*8-256]
    vpopcntb xmm2{k5}, xmm0
    vpopcntb xmm2{k5}, [rbx]
    vpopcntb xmm2{k5}, [rbx+r11*8+256]
    vpopcntb xmm2{k5}, [rbx+r11*8-256]
    vpopcntb xmm2{k5}{z}, xmm0
    vpopcntb xmm2{k5}{z}, [rbx]
    vpopcntb xmm2{k5}{z}, [rbx+r11*8+256]
    vpopcntb xmm2{k5}{z}, [rbx+r11*8-256]
    vpopcntb ymm16, ymm15
    vpopcntb ymm16, [rbx]
    vpopcntb ymm16, [rbx+r11*8+256]
    vpopcntb ymm16, [rbx+r11*8-256]
    vpopcntb ymm16{k5}, ymm15
    vpopcntb ymm16{k5}, [rbx]
    vpopcntb ymm16{k5}, [rbx+r11*8+256]
    vpopcntb ymm16{k5}, [rbx+r11*8-256]
    vpopcntb ymm16{k5}{z}, ymm15
    vpopcntb ymm16{k5}{z}, [rbx]
    vpopcntb ymm16{k5}{z}, [rbx+r11*8+256]
    vpopcntb ymm16{k5}{z}, [rbx+r11*8-256]
    vpopcntb zmm24, zmm31
    vpopcntb zmm24, [rbx]
    vpopcntb zmm24, [rbx+r11*8+256]
    vpopcntb zmm24, [rbx+r11*8-256]
    vpopcntb zmm24{k5}, zmm31
    vpopcntb zmm24{k5}, [rbx]
    vpopcntb zmm24{k5}, [rbx+r11*8+256]
    vpopcntb zmm24{k5}, [rbx+r11*8-256]
    vpopcntb zmm24{k5}{z}, zmm31
    vpopcntb zmm24{k5}{z}, [rbx]
    vpopcntb zmm24{k5}{z}, [rbx+r11*8+256]
    vpopcntb zmm24{k5}{z}, [rbx+r11*8-256]
    vpopcntw xmm2, xmm0
    vpopcntw xmm2, [rbx]
    vpopcntw xmm2, [rbx+r11*8+256]
    vpopcntw xmm2, [rbx+r11*8-256]
    vpopcntw xmm2{k5}, xmm0
    vpopcntw xmm2{k5}, [rbx]
    vpopcntw xmm2{k5}, [rbx+r11*8+256]
    vpopcntw xmm2{k5}, [rbx+r11*8-256]
    vpopcntw xmm2{k5}{z}, xmm0
    vpopcntw xmm2{k5}{z}, [rbx]
    vpopcntw xmm2{k5}{z}, [rbx+r11*8+256]
    vpopcntw xmm2{k5}{z}, [rbx+r11*8-256]
    vpopcntw ymm16, ymm15
    vpopcntw ymm16, [rbx]
    vpopcntw ymm16, [rbx+r11*8+256]
    vpopcntw ymm16, [rbx+r11*8-256]
    vpopcntw ymm16{k5}, ymm15
    vpopcntw ymm16{k5}, [rbx]
    vpopcntw ymm16{k5}, [rbx+r11*8+256]
    vpopcntw ymm16{k5}, [rbx+r11*8-256]
    vpopcntw ymm16{k5}{z}, ymm15
    vpopcntw ymm16{k5}{z}, [rbx]
    vpopcntw ymm16{k5}{z}, [rbx+r11*8+256]
    vpopcntw ymm16{k5}{z}, [rbx+r11*8-256]
    vpopcntw zmm24, zmm31
    vpopcntw zmm24, [rbx]
    vpopcntw zmm24, [rbx+r11*8+256]
    vpopcntw zmm24, [rbx+r11*8-256]
    vpopcntw zmm24{k5}, zmm31
    vpopcntw zmm24{k5}, [rbx]
    vpopcntw zmm24{k5}, [rbx+r11*8+256]
    vpopcntw zmm24{k5}, [rbx+r11*8-256]
    vpopcntw zmm24{k5}{z}, zmm31
    vpopcntw zmm24{k5}{z}, [rbx]
    vpopcntw zmm24{k5}{z}, [rbx+r11*8+256]
    vpopcntw zmm24{k5}{z}, [rbx+r11*8-256]
    vpshufbitqmb k3, xmm7, xmm0
    vpshufbitqmb k3, xmm7, [rbx]
    vpshufbitqmb k3, xmm7, [rbx+r11*8+256]
    vpshufbitqmb k3, xmm7, [rbx+r11*8-256]
    vpshufbitqmb k3, ymm13, ymm15
    vpshufbitqmb k3, ymm13, [rbx]
    vpshufbitqmb k3, ymm13, [rbx+r11*8+256]
    vpshufbitqmb k3, ymm13, [rbx+r11*8-256]
    vpshufbitqmb k3, zmm24, zmm31
    vpshufbitqmb k3, zmm24, [rbx]
    vpshufbitqmb k3, zmm24, [rbx+r11*8+256]
    vpshufbitqmb k3, zmm24, [rbx+r11*8-256]
    vpshufbitqmb k3{k5}, xmm7, xmm0
    vpshufbitqmb k3{k5}, xmm7, [rbx]
    vpshufbitqmb k3{k5}, xmm7, [rbx+r11*8+256]
    vpshufbitqmb k3{k5}, xmm7, [rbx+r11*8-256]
    vpshufbitqmb k3{k5}, ymm13, ymm15
    vpshufbitqmb k3{k5}, ymm13, [rbx]
    vpshufbitqmb k3{k5}, ymm13, [rbx+r11*8+256]
    vpshufbitqmb k3{k5}, ymm13, [rbx+r11*8-256]
    vpshufbitqmb k3{k5}, zmm24, zmm31
    vpshufbitqmb k3{k5}, zmm24, [rbx]
    vpshufbitqmb k3{k5}, zmm24, [rbx+r11*8+256]
    vpshufbitqmb k3{k5}, zmm24, [rbx+r11*8-256]
