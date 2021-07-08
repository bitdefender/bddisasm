    bits 64
    
    vexp2pd zmm24, zmm31
    vexp2pd zmm24, zmm31, {sae}
    vexp2pd zmm24, [rbx]
    vexp2pd zmm24, [rbx]{1to8}
    vexp2pd zmm24, [rbx+r11*8+256]
    vexp2pd zmm24, [rbx+r11*8-256]
    vexp2pd zmm24{k5}, zmm31
    vexp2pd zmm24{k5}, zmm31, {sae}
    vexp2pd zmm24{k5}, [rbx]
    vexp2pd zmm24{k5}, [rbx]{1to8}
    vexp2pd zmm24{k5}, [rbx+r11*8+256]
    vexp2pd zmm24{k5}, [rbx+r11*8-256]
    vexp2pd zmm24{k5}{z}, zmm31
    vexp2pd zmm24{k5}{z}, zmm31, {sae}
    vexp2pd zmm24{k5}{z}, [rbx]
    vexp2pd zmm24{k5}{z}, [rbx]{1to8}
    vexp2pd zmm24{k5}{z}, [rbx+r11*8+256]
    vexp2pd zmm24{k5}{z}, [rbx+r11*8-256]
    vexp2ps zmm24, zmm31
    vexp2ps zmm24, zmm31, {sae}
    vexp2ps zmm24, [rbx]
    vexp2ps zmm24, [rbx]{1to16}
    vexp2ps zmm24, [rbx+r11*8+256]
    vexp2ps zmm24, [rbx+r11*8-256]
    vexp2ps zmm24{k5}, zmm31
    vexp2ps zmm24{k5}, zmm31, {sae}
    vexp2ps zmm24{k5}, [rbx]
    vexp2ps zmm24{k5}, [rbx]{1to16}
    vexp2ps zmm24{k5}, [rbx+r11*8+256]
    vexp2ps zmm24{k5}, [rbx+r11*8-256]
    vexp2ps zmm24{k5}{z}, zmm31
    vexp2ps zmm24{k5}{z}, zmm31, {sae}
    vexp2ps zmm24{k5}{z}, [rbx]
    vexp2ps zmm24{k5}{z}, [rbx]{1to16}
    vexp2ps zmm24{k5}{z}, [rbx+r11*8+256]
    vexp2ps zmm24{k5}{z}, [rbx+r11*8-256]
    vrcp28pd zmm24, zmm31
    vrcp28pd zmm24, zmm31, {sae}
    vrcp28pd zmm24, [rbx]
    vrcp28pd zmm24, [rbx]{1to8}
    vrcp28pd zmm24, [rbx+r11*8+256]
    vrcp28pd zmm24, [rbx+r11*8-256]
    vrcp28pd zmm24{k5}, zmm31
    vrcp28pd zmm24{k5}, zmm31, {sae}
    vrcp28pd zmm24{k5}, [rbx]
    vrcp28pd zmm24{k5}, [rbx]{1to8}
    vrcp28pd zmm24{k5}, [rbx+r11*8+256]
    vrcp28pd zmm24{k5}, [rbx+r11*8-256]
    vrcp28pd zmm24{k5}{z}, zmm31
    vrcp28pd zmm24{k5}{z}, zmm31, {sae}
    vrcp28pd zmm24{k5}{z}, [rbx]
    vrcp28pd zmm24{k5}{z}, [rbx]{1to8}
    vrcp28pd zmm24{k5}{z}, [rbx+r11*8+256]
    vrcp28pd zmm24{k5}{z}, [rbx+r11*8-256]
    vrcp28ps zmm24, zmm31
    vrcp28ps zmm24, zmm31, {sae}
    vrcp28ps zmm24, [rbx]
    vrcp28ps zmm24, [rbx]{1to16}
    vrcp28ps zmm24, [rbx+r11*8+256]
    vrcp28ps zmm24, [rbx+r11*8-256]
    vrcp28ps zmm24{k5}, zmm31
    vrcp28ps zmm24{k5}, zmm31, {sae}
    vrcp28ps zmm24{k5}, [rbx]
    vrcp28ps zmm24{k5}, [rbx]{1to16}
    vrcp28ps zmm24{k5}, [rbx+r11*8+256]
    vrcp28ps zmm24{k5}, [rbx+r11*8-256]
    vrcp28ps zmm24{k5}{z}, zmm31
    vrcp28ps zmm24{k5}{z}, zmm31, {sae}
    vrcp28ps zmm24{k5}{z}, [rbx]
    vrcp28ps zmm24{k5}{z}, [rbx]{1to16}
    vrcp28ps zmm24{k5}{z}, [rbx+r11*8+256]
    vrcp28ps zmm24{k5}{z}, [rbx+r11*8-256]
    vrcp28sd xmm2, xmm7, xmm0
    vrcp28sd xmm2, xmm7, xmm0, {sae}
    vrcp28sd xmm2, xmm7, [rbx]
    vrcp28sd xmm2, xmm7, [rbx+r11*8+256]
    vrcp28sd xmm2, xmm7, [rbx+r11*8-256]
    vrcp28sd xmm2{k5}, xmm7, xmm0
    vrcp28sd xmm2{k5}, xmm7, xmm0, {sae}
    vrcp28sd xmm2{k5}, xmm7, [rbx]
    vrcp28sd xmm2{k5}, xmm7, [rbx+r11*8+256]
    vrcp28sd xmm2{k5}, xmm7, [rbx+r11*8-256]
    vrcp28sd xmm2{k5}{z}, xmm7, xmm0
    vrcp28sd xmm2{k5}{z}, xmm7, xmm0, {sae}
    vrcp28sd xmm2{k5}{z}, xmm7, [rbx]
    vrcp28sd xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vrcp28sd xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vrcp28ss xmm2, xmm7, xmm0
    vrcp28ss xmm2, xmm7, xmm0, {sae}
    vrcp28ss xmm2, xmm7, [rbx]
    vrcp28ss xmm2, xmm7, [rbx+r11*8+256]
    vrcp28ss xmm2, xmm7, [rbx+r11*8-256]
    vrcp28ss xmm2{k5}, xmm7, xmm0
    vrcp28ss xmm2{k5}, xmm7, xmm0, {sae}
    vrcp28ss xmm2{k5}, xmm7, [rbx]
    vrcp28ss xmm2{k5}, xmm7, [rbx+r11*8+256]
    vrcp28ss xmm2{k5}, xmm7, [rbx+r11*8-256]
    vrcp28ss xmm2{k5}{z}, xmm7, xmm0
    vrcp28ss xmm2{k5}{z}, xmm7, xmm0, {sae}
    vrcp28ss xmm2{k5}{z}, xmm7, [rbx]
    vrcp28ss xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vrcp28ss xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vrsqrt28pd zmm24, zmm31
    vrsqrt28pd zmm24, zmm31, {sae}
    vrsqrt28pd zmm24, [rbx]
    vrsqrt28pd zmm24, [rbx]{1to8}
    vrsqrt28pd zmm24, [rbx+r11*8+256]
    vrsqrt28pd zmm24, [rbx+r11*8-256]
    vrsqrt28pd zmm24{k5}, zmm31
    vrsqrt28pd zmm24{k5}, zmm31, {sae}
    vrsqrt28pd zmm24{k5}, [rbx]
    vrsqrt28pd zmm24{k5}, [rbx]{1to8}
    vrsqrt28pd zmm24{k5}, [rbx+r11*8+256]
    vrsqrt28pd zmm24{k5}, [rbx+r11*8-256]
    vrsqrt28pd zmm24{k5}{z}, zmm31
    vrsqrt28pd zmm24{k5}{z}, zmm31, {sae}
    vrsqrt28pd zmm24{k5}{z}, [rbx]
    vrsqrt28pd zmm24{k5}{z}, [rbx]{1to8}
    vrsqrt28pd zmm24{k5}{z}, [rbx+r11*8+256]
    vrsqrt28pd zmm24{k5}{z}, [rbx+r11*8-256]
    vrsqrt28ps zmm24, zmm31
    vrsqrt28ps zmm24, zmm31, {sae}
    vrsqrt28ps zmm24, [rbx]
    vrsqrt28ps zmm24, [rbx]{1to16}
    vrsqrt28ps zmm24, [rbx+r11*8+256]
    vrsqrt28ps zmm24, [rbx+r11*8-256]
    vrsqrt28ps zmm24{k5}, zmm31
    vrsqrt28ps zmm24{k5}, zmm31, {sae}
    vrsqrt28ps zmm24{k5}, [rbx]
    vrsqrt28ps zmm24{k5}, [rbx]{1to16}
    vrsqrt28ps zmm24{k5}, [rbx+r11*8+256]
    vrsqrt28ps zmm24{k5}, [rbx+r11*8-256]
    vrsqrt28ps zmm24{k5}{z}, zmm31
    vrsqrt28ps zmm24{k5}{z}, zmm31, {sae}
    vrsqrt28ps zmm24{k5}{z}, [rbx]
    vrsqrt28ps zmm24{k5}{z}, [rbx]{1to16}
    vrsqrt28ps zmm24{k5}{z}, [rbx+r11*8+256]
    vrsqrt28ps zmm24{k5}{z}, [rbx+r11*8-256]
    vrsqrt28sd xmm2, xmm7, xmm0
    vrsqrt28sd xmm2, xmm7, xmm0, {sae}
    vrsqrt28sd xmm2, xmm7, [rbx]
    vrsqrt28sd xmm2, xmm7, [rbx+r11*8+256]
    vrsqrt28sd xmm2, xmm7, [rbx+r11*8-256]
    vrsqrt28sd xmm2{k5}, xmm7, xmm0
    vrsqrt28sd xmm2{k5}, xmm7, xmm0, {sae}
    vrsqrt28sd xmm2{k5}, xmm7, [rbx]
    vrsqrt28sd xmm2{k5}, xmm7, [rbx+r11*8+256]
    vrsqrt28sd xmm2{k5}, xmm7, [rbx+r11*8-256]
    vrsqrt28sd xmm2{k5}{z}, xmm7, xmm0
    vrsqrt28sd xmm2{k5}{z}, xmm7, xmm0, {sae}
    vrsqrt28sd xmm2{k5}{z}, xmm7, [rbx]
    vrsqrt28sd xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vrsqrt28sd xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vrsqrt28ss xmm2, xmm7, xmm0
    vrsqrt28ss xmm2, xmm7, xmm0, {sae}
    vrsqrt28ss xmm2, xmm7, [rbx]
    vrsqrt28ss xmm2, xmm7, [rbx+r11*8+256]
    vrsqrt28ss xmm2, xmm7, [rbx+r11*8-256]
    vrsqrt28ss xmm2{k5}, xmm7, xmm0
    vrsqrt28ss xmm2{k5}, xmm7, xmm0, {sae}
    vrsqrt28ss xmm2{k5}, xmm7, [rbx]
    vrsqrt28ss xmm2{k5}, xmm7, [rbx+r11*8+256]
    vrsqrt28ss xmm2{k5}, xmm7, [rbx+r11*8-256]
    vrsqrt28ss xmm2{k5}{z}, xmm7, xmm0
    vrsqrt28ss xmm2{k5}{z}, xmm7, xmm0, {sae}
    vrsqrt28ss xmm2{k5}{z}, xmm7, [rbx]
    vrsqrt28ss xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vrsqrt28ss xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
