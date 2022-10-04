    bits 64
    
    kaddd k3, k7, k1
    kaddq k3, k7, k1
    kandd k3, k7, k1
    kandnd k3, k7, k1
    kandnq k3, k7, k1
    kandq k3, k7, k1
    kmovd k3, [rbx]
    kmovd k3, [rbx+rsi*8+256]
    kmovd k3, [rbx+rsi*8-256]
    kmovd k3, k1
    kmovd k3, [rbx]
    kmovd [rbx], k3
    kmovd [rbx+rsi*8+256], k3
    kmovd [rbx+rsi*8-256], k3
    kmovd k3, ecx
    kmovd ecx, k1
    kmovq k3, [rbx]
    kmovq k3, [rbx+rsi*8+256]
    kmovq k3, [rbx+rsi*8-256]
    kmovq k3, k1
    kmovq k3, [rbx]
    kmovq [rbx], k3
    kmovq [rbx+rsi*8+256], k3
    kmovq [rbx+rsi*8-256], k3
    kmovq k3, rcx
    kmovq rcx, k1
    knotd k3, k1
    knotq k3, k1
    kord k3, k7, k1
    korq k3, k7, k1
    kortestd k3, k1
    kortestq k3, k1
    kshiftld k3, k1, 10
    kshiftlq k3, k1, 10
    kshiftrd k3, k1, 10
    kshiftrq k3, k1, 10
    ktestd k3, k1
    ktestq k3, k1
    kunpckdq k3, k7, k1
    kunpckwd k3, k7, k1
    kxnord k3, k7, k1
    kxnorq k3, k7, k1
    kxord k3, k7, k1
    kxorq k3, k7, k1
    vdbpsadbw xmm2, xmm7, xmm0, 10
    vdbpsadbw xmm2, xmm7, [rbx], 10
    vdbpsadbw xmm2, xmm7, [rbx+r11*8+256], 10
    vdbpsadbw xmm2, xmm7, [rbx+r11*8-256], 10
    vdbpsadbw xmm2{k5}, xmm7, xmm0, 10
    vdbpsadbw xmm2{k5}, xmm7, [rbx], 10
    vdbpsadbw xmm2{k5}, xmm7, [rbx+r11*8+256], 10
    vdbpsadbw xmm2{k5}, xmm7, [rbx+r11*8-256], 10
    vdbpsadbw xmm2{k5}{z}, xmm7, xmm0, 10
    vdbpsadbw xmm2{k5}{z}, xmm7, [rbx], 10
    vdbpsadbw xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 10
    vdbpsadbw xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 10
    vdbpsadbw ymm16, ymm13, ymm15, 10
    vdbpsadbw ymm16, ymm13, [rbx], 10
    vdbpsadbw ymm16, ymm13, [rbx+r11*8+256], 10
    vdbpsadbw ymm16, ymm13, [rbx+r11*8-256], 10
    vdbpsadbw ymm16{k5}, ymm13, ymm15, 10
    vdbpsadbw ymm16{k5}, ymm13, [rbx], 10
    vdbpsadbw ymm16{k5}, ymm13, [rbx+r11*8+256], 10
    vdbpsadbw ymm16{k5}, ymm13, [rbx+r11*8-256], 10
    vdbpsadbw ymm16{k5}{z}, ymm13, ymm15, 10
    vdbpsadbw ymm16{k5}{z}, ymm13, [rbx], 10
    vdbpsadbw ymm16{k5}{z}, ymm13, [rbx+r11*8+256], 10
    vdbpsadbw ymm16{k5}{z}, ymm13, [rbx+r11*8-256], 10
    vdbpsadbw zmm24, zmm24, zmm31, 10
    vdbpsadbw zmm24, zmm24, [rbx], 10
    vdbpsadbw zmm24, zmm24, [rbx+r11*8+256], 10
    vdbpsadbw zmm24, zmm24, [rbx+r11*8-256], 10
    vdbpsadbw zmm24{k5}, zmm24, zmm31, 10
    vdbpsadbw zmm24{k5}, zmm24, [rbx], 10
    vdbpsadbw zmm24{k5}, zmm24, [rbx+r11*8+256], 10
    vdbpsadbw zmm24{k5}, zmm24, [rbx+r11*8-256], 10
    vdbpsadbw zmm24{k5}{z}, zmm24, zmm31, 10
    vdbpsadbw zmm24{k5}{z}, zmm24, [rbx], 10
    vdbpsadbw zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 10
    vdbpsadbw zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 10
    vmovdqu16 xmm2, xmm0
    vmovdqu16 xmm2, [rbx]
    vmovdqu16 xmm2, [rbx+r11*8+256]
    vmovdqu16 xmm2, [rbx+r11*8-256]
    vmovdqu16 xmm2{k5}, xmm0
    vmovdqu16 xmm2{k5}, [rbx]
    vmovdqu16 xmm2{k5}, [rbx+r11*8+256]
    vmovdqu16 xmm2{k5}, [rbx+r11*8-256]
    vmovdqu16 xmm2{k5}{z}, xmm0
    vmovdqu16 xmm2{k5}{z}, [rbx]
    vmovdqu16 xmm2{k5}{z}, [rbx+r11*8+256]
    vmovdqu16 xmm2{k5}{z}, [rbx+r11*8-256]
    vmovdqu16 ymm16, ymm15
    vmovdqu16 ymm16, [rbx]
    vmovdqu16 ymm16, [rbx+r11*8+256]
    vmovdqu16 ymm16, [rbx+r11*8-256]
    vmovdqu16 ymm16{k5}, ymm15
    vmovdqu16 ymm16{k5}, [rbx]
    vmovdqu16 ymm16{k5}, [rbx+r11*8+256]
    vmovdqu16 ymm16{k5}, [rbx+r11*8-256]
    vmovdqu16 ymm16{k5}{z}, ymm15
    vmovdqu16 ymm16{k5}{z}, [rbx]
    vmovdqu16 ymm16{k5}{z}, [rbx+r11*8+256]
    vmovdqu16 ymm16{k5}{z}, [rbx+r11*8-256]
    vmovdqu16 zmm24, zmm31
    vmovdqu16 zmm24, [rbx]
    vmovdqu16 zmm24, [rbx+r11*8+256]
    vmovdqu16 zmm24, [rbx+r11*8-256]
    vmovdqu16 zmm24{k5}, zmm31
    vmovdqu16 zmm24{k5}, [rbx]
    vmovdqu16 zmm24{k5}, [rbx+r11*8+256]
    vmovdqu16 zmm24{k5}, [rbx+r11*8-256]
    vmovdqu16 zmm24{k5}{z}, zmm31
    vmovdqu16 zmm24{k5}{z}, [rbx]
    vmovdqu16 zmm24{k5}{z}, [rbx+r11*8+256]
    vmovdqu16 zmm24{k5}{z}, [rbx+r11*8-256]
    vmovdqu16 xmm0, xmm2
    vmovdqu16 ymm15, ymm16
    vmovdqu16 zmm31, zmm24
    vmovdqu16 [rbx], xmm2
    vmovdqu16 [rbx], ymm16
    vmovdqu16 [rbx], zmm24
    vmovdqu16 [rbx+r11*8+256], xmm2
    vmovdqu16 [rbx+r11*8+256], ymm16
    vmovdqu16 [rbx+r11*8+256], zmm24
    vmovdqu16 [rbx+r11*8-256], xmm2
    vmovdqu16 [rbx+r11*8-256], ymm16
    vmovdqu16 [rbx+r11*8-256], zmm24
    vmovdqu8 xmm2, xmm0
    vmovdqu8 xmm2, [rbx]
    vmovdqu8 xmm2, [rbx+r11*8+256]
    vmovdqu8 xmm2, [rbx+r11*8-256]
    vmovdqu8 xmm2{k5}, xmm0
    vmovdqu8 xmm2{k5}, [rbx]
    vmovdqu8 xmm2{k5}, [rbx+r11*8+256]
    vmovdqu8 xmm2{k5}, [rbx+r11*8-256]
    vmovdqu8 xmm2{k5}{z}, xmm0
    vmovdqu8 xmm2{k5}{z}, [rbx]
    vmovdqu8 xmm2{k5}{z}, [rbx+r11*8+256]
    vmovdqu8 xmm2{k5}{z}, [rbx+r11*8-256]
    vmovdqu8 ymm16, ymm15
    vmovdqu8 ymm16, [rbx]
    vmovdqu8 ymm16, [rbx+r11*8+256]
    vmovdqu8 ymm16, [rbx+r11*8-256]
    vmovdqu8 ymm16{k5}, ymm15
    vmovdqu8 ymm16{k5}, [rbx]
    vmovdqu8 ymm16{k5}, [rbx+r11*8+256]
    vmovdqu8 ymm16{k5}, [rbx+r11*8-256]
    vmovdqu8 ymm16{k5}{z}, ymm15
    vmovdqu8 ymm16{k5}{z}, [rbx]
    vmovdqu8 ymm16{k5}{z}, [rbx+r11*8+256]
    vmovdqu8 ymm16{k5}{z}, [rbx+r11*8-256]
    vmovdqu8 zmm24, zmm31
    vmovdqu8 zmm24, [rbx]
    vmovdqu8 zmm24, [rbx+r11*8+256]
    vmovdqu8 zmm24, [rbx+r11*8-256]
    vmovdqu8 zmm24{k5}, zmm31
    vmovdqu8 zmm24{k5}, [rbx]
    vmovdqu8 zmm24{k5}, [rbx+r11*8+256]
    vmovdqu8 zmm24{k5}, [rbx+r11*8-256]
    vmovdqu8 zmm24{k5}{z}, zmm31
    vmovdqu8 zmm24{k5}{z}, [rbx]
    vmovdqu8 zmm24{k5}{z}, [rbx+r11*8+256]
    vmovdqu8 zmm24{k5}{z}, [rbx+r11*8-256]
    vmovdqu8 xmm0, xmm2
    vmovdqu8 ymm15, ymm16
    vmovdqu8 zmm31, zmm24
    vmovdqu8 [rbx], xmm2
    vmovdqu8 [rbx], ymm16
    vmovdqu8 [rbx], zmm24
    vmovdqu8 [rbx+r11*8+256], xmm2
    vmovdqu8 [rbx+r11*8+256], ymm16
    vmovdqu8 [rbx+r11*8+256], zmm24
    vmovdqu8 [rbx+r11*8-256], xmm2
    vmovdqu8 [rbx+r11*8-256], ymm16
    vmovdqu8 [rbx+r11*8-256], zmm24
    vpabsb xmm2, xmm0
    vpabsb xmm2, [rbx]
    vpabsb xmm2, [rbx+r11*8+256]
    vpabsb xmm2, [rbx+r11*8-256]
    vpabsb xmm2{k5}, xmm0
    vpabsb xmm2{k5}, [rbx]
    vpabsb xmm2{k5}, [rbx+r11*8+256]
    vpabsb xmm2{k5}, [rbx+r11*8-256]
    vpabsb xmm2{k5}{z}, xmm0
    vpabsb xmm2{k5}{z}, [rbx]
    vpabsb xmm2{k5}{z}, [rbx+r11*8+256]
    vpabsb xmm2{k5}{z}, [rbx+r11*8-256]
    vpabsb ymm16, ymm15
    vpabsb ymm16, [rbx]
    vpabsb ymm16, [rbx+r11*8+256]
    vpabsb ymm16, [rbx+r11*8-256]
    vpabsb ymm16{k5}, ymm15
    vpabsb ymm16{k5}, [rbx]
    vpabsb ymm16{k5}, [rbx+r11*8+256]
    vpabsb ymm16{k5}, [rbx+r11*8-256]
    vpabsb ymm16{k5}{z}, ymm15
    vpabsb ymm16{k5}{z}, [rbx]
    vpabsb ymm16{k5}{z}, [rbx+r11*8+256]
    vpabsb ymm16{k5}{z}, [rbx+r11*8-256]
    vpabsb zmm24, zmm31
    vpabsb zmm24, [rbx]
    vpabsb zmm24, [rbx+r11*8+256]
    vpabsb zmm24, [rbx+r11*8-256]
    vpabsb zmm24{k5}, zmm31
    vpabsb zmm24{k5}, [rbx]
    vpabsb zmm24{k5}, [rbx+r11*8+256]
    vpabsb zmm24{k5}, [rbx+r11*8-256]
    vpabsb zmm24{k5}{z}, zmm31
    vpabsb zmm24{k5}{z}, [rbx]
    vpabsb zmm24{k5}{z}, [rbx+r11*8+256]
    vpabsb zmm24{k5}{z}, [rbx+r11*8-256]
    vpabsw xmm2, xmm0
    vpabsw xmm2, [rbx]
    vpabsw xmm2, [rbx+r11*8+256]
    vpabsw xmm2, [rbx+r11*8-256]
    vpabsw xmm2{k5}, xmm0
    vpabsw xmm2{k5}, [rbx]
    vpabsw xmm2{k5}, [rbx+r11*8+256]
    vpabsw xmm2{k5}, [rbx+r11*8-256]
    vpabsw xmm2{k5}{z}, xmm0
    vpabsw xmm2{k5}{z}, [rbx]
    vpabsw xmm2{k5}{z}, [rbx+r11*8+256]
    vpabsw xmm2{k5}{z}, [rbx+r11*8-256]
    vpabsw ymm16, ymm15
    vpabsw ymm16, [rbx]
    vpabsw ymm16, [rbx+r11*8+256]
    vpabsw ymm16, [rbx+r11*8-256]
    vpabsw ymm16{k5}, ymm15
    vpabsw ymm16{k5}, [rbx]
    vpabsw ymm16{k5}, [rbx+r11*8+256]
    vpabsw ymm16{k5}, [rbx+r11*8-256]
    vpabsw ymm16{k5}{z}, ymm15
    vpabsw ymm16{k5}{z}, [rbx]
    vpabsw ymm16{k5}{z}, [rbx+r11*8+256]
    vpabsw ymm16{k5}{z}, [rbx+r11*8-256]
    vpabsw zmm24, zmm31
    vpabsw zmm24, [rbx]
    vpabsw zmm24, [rbx+r11*8+256]
    vpabsw zmm24, [rbx+r11*8-256]
    vpabsw zmm24{k5}, zmm31
    vpabsw zmm24{k5}, [rbx]
    vpabsw zmm24{k5}, [rbx+r11*8+256]
    vpabsw zmm24{k5}, [rbx+r11*8-256]
    vpabsw zmm24{k5}{z}, zmm31
    vpabsw zmm24{k5}{z}, [rbx]
    vpabsw zmm24{k5}{z}, [rbx+r11*8+256]
    vpabsw zmm24{k5}{z}, [rbx+r11*8-256]
    vpackssdw xmm2, xmm7, xmm0
    vpackssdw xmm2, xmm7, [rbx]
    vpackssdw xmm2, xmm7, [rbx]{1to4}
    vpackssdw xmm2, xmm7, [rbx+r11*8+256]
    vpackssdw xmm2, xmm7, [rbx+r11*8-256]
    vpackssdw xmm2{k5}, xmm7, xmm0
    vpackssdw xmm2{k5}, xmm7, [rbx]
    vpackssdw xmm2{k5}, xmm7, [rbx]{1to4}
    vpackssdw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpackssdw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpackssdw xmm2{k5}{z}, xmm7, xmm0
    vpackssdw xmm2{k5}{z}, xmm7, [rbx]
    vpackssdw xmm2{k5}{z}, xmm7, [rbx]{1to4}
    vpackssdw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpackssdw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpackssdw ymm16, ymm13, ymm15
    vpackssdw ymm16, ymm13, [rbx]
    vpackssdw ymm16, ymm13, [rbx]{1to8}
    vpackssdw ymm16, ymm13, [rbx+r11*8+256]
    vpackssdw ymm16, ymm13, [rbx+r11*8-256]
    vpackssdw ymm16{k5}, ymm13, ymm15
    vpackssdw ymm16{k5}, ymm13, [rbx]
    vpackssdw ymm16{k5}, ymm13, [rbx]{1to8}
    vpackssdw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpackssdw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpackssdw ymm16{k5}{z}, ymm13, ymm15
    vpackssdw ymm16{k5}{z}, ymm13, [rbx]
    vpackssdw ymm16{k5}{z}, ymm13, [rbx]{1to8}
    vpackssdw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpackssdw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpackssdw zmm24, zmm24, zmm31
    vpackssdw zmm24, zmm24, [rbx]
    vpackssdw zmm24, zmm24, [rbx]{1to16}
    vpackssdw zmm24, zmm24, [rbx+r11*8+256]
    vpackssdw zmm24, zmm24, [rbx+r11*8-256]
    vpackssdw zmm24{k5}, zmm24, zmm31
    vpackssdw zmm24{k5}, zmm24, [rbx]
    vpackssdw zmm24{k5}, zmm24, [rbx]{1to16}
    vpackssdw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpackssdw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpackssdw zmm24{k5}{z}, zmm24, zmm31
    vpackssdw zmm24{k5}{z}, zmm24, [rbx]
    vpackssdw zmm24{k5}{z}, zmm24, [rbx]{1to16}
    vpackssdw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpackssdw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpacksswb xmm2, xmm7, xmm0
    vpacksswb xmm2, xmm7, [rbx]
    vpacksswb xmm2, xmm7, [rbx+r11*8+256]
    vpacksswb xmm2, xmm7, [rbx+r11*8-256]
    vpacksswb xmm2{k5}, xmm7, xmm0
    vpacksswb xmm2{k5}, xmm7, [rbx]
    vpacksswb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpacksswb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpacksswb xmm2{k5}{z}, xmm7, xmm0
    vpacksswb xmm2{k5}{z}, xmm7, [rbx]
    vpacksswb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpacksswb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpacksswb ymm16, ymm13, ymm15
    vpacksswb ymm16, ymm13, [rbx]
    vpacksswb ymm16, ymm13, [rbx+r11*8+256]
    vpacksswb ymm16, ymm13, [rbx+r11*8-256]
    vpacksswb ymm16{k5}, ymm13, ymm15
    vpacksswb ymm16{k5}, ymm13, [rbx]
    vpacksswb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpacksswb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpacksswb ymm16{k5}{z}, ymm13, ymm15
    vpacksswb ymm16{k5}{z}, ymm13, [rbx]
    vpacksswb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpacksswb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpacksswb zmm24, zmm24, zmm31
    vpacksswb zmm24, zmm24, [rbx]
    vpacksswb zmm24, zmm24, [rbx+r11*8+256]
    vpacksswb zmm24, zmm24, [rbx+r11*8-256]
    vpacksswb zmm24{k5}, zmm24, zmm31
    vpacksswb zmm24{k5}, zmm24, [rbx]
    vpacksswb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpacksswb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpacksswb zmm24{k5}{z}, zmm24, zmm31
    vpacksswb zmm24{k5}{z}, zmm24, [rbx]
    vpacksswb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpacksswb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpackusdw xmm2, xmm7, xmm0
    vpackusdw xmm2, xmm7, [rbx]
    vpackusdw xmm2, xmm7, [rbx]{1to4}
    vpackusdw xmm2, xmm7, [rbx+r11*8+256]
    vpackusdw xmm2, xmm7, [rbx+r11*8-256]
    vpackusdw xmm2{k5}, xmm7, xmm0
    vpackusdw xmm2{k5}, xmm7, [rbx]
    vpackusdw xmm2{k5}, xmm7, [rbx]{1to4}
    vpackusdw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpackusdw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpackusdw xmm2{k5}{z}, xmm7, xmm0
    vpackusdw xmm2{k5}{z}, xmm7, [rbx]
    vpackusdw xmm2{k5}{z}, xmm7, [rbx]{1to4}
    vpackusdw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpackusdw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpackusdw ymm16, ymm13, ymm15
    vpackusdw ymm16, ymm13, [rbx]
    vpackusdw ymm16, ymm13, [rbx]{1to8}
    vpackusdw ymm16, ymm13, [rbx+r11*8+256]
    vpackusdw ymm16, ymm13, [rbx+r11*8-256]
    vpackusdw ymm16{k5}, ymm13, ymm15
    vpackusdw ymm16{k5}, ymm13, [rbx]
    vpackusdw ymm16{k5}, ymm13, [rbx]{1to8}
    vpackusdw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpackusdw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpackusdw ymm16{k5}{z}, ymm13, ymm15
    vpackusdw ymm16{k5}{z}, ymm13, [rbx]
    vpackusdw ymm16{k5}{z}, ymm13, [rbx]{1to8}
    vpackusdw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpackusdw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpackusdw zmm24, zmm24, zmm31
    vpackusdw zmm24, zmm24, [rbx]
    vpackusdw zmm24, zmm24, [rbx]{1to16}
    vpackusdw zmm24, zmm24, [rbx+r11*8+256]
    vpackusdw zmm24, zmm24, [rbx+r11*8-256]
    vpackusdw zmm24{k5}, zmm24, zmm31
    vpackusdw zmm24{k5}, zmm24, [rbx]
    vpackusdw zmm24{k5}, zmm24, [rbx]{1to16}
    vpackusdw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpackusdw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpackusdw zmm24{k5}{z}, zmm24, zmm31
    vpackusdw zmm24{k5}{z}, zmm24, [rbx]
    vpackusdw zmm24{k5}{z}, zmm24, [rbx]{1to16}
    vpackusdw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpackusdw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpackuswb xmm2, xmm7, xmm0
    vpackuswb xmm2, xmm7, [rbx]
    vpackuswb xmm2, xmm7, [rbx+r11*8+256]
    vpackuswb xmm2, xmm7, [rbx+r11*8-256]
    vpackuswb xmm2{k5}, xmm7, xmm0
    vpackuswb xmm2{k5}, xmm7, [rbx]
    vpackuswb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpackuswb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpackuswb xmm2{k5}{z}, xmm7, xmm0
    vpackuswb xmm2{k5}{z}, xmm7, [rbx]
    vpackuswb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpackuswb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpackuswb ymm16, ymm13, ymm15
    vpackuswb ymm16, ymm13, [rbx]
    vpackuswb ymm16, ymm13, [rbx+r11*8+256]
    vpackuswb ymm16, ymm13, [rbx+r11*8-256]
    vpackuswb ymm16{k5}, ymm13, ymm15
    vpackuswb ymm16{k5}, ymm13, [rbx]
    vpackuswb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpackuswb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpackuswb ymm16{k5}{z}, ymm13, ymm15
    vpackuswb ymm16{k5}{z}, ymm13, [rbx]
    vpackuswb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpackuswb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpackuswb zmm24, zmm24, zmm31
    vpackuswb zmm24, zmm24, [rbx]
    vpackuswb zmm24, zmm24, [rbx+r11*8+256]
    vpackuswb zmm24, zmm24, [rbx+r11*8-256]
    vpackuswb zmm24{k5}, zmm24, zmm31
    vpackuswb zmm24{k5}, zmm24, [rbx]
    vpackuswb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpackuswb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpackuswb zmm24{k5}{z}, zmm24, zmm31
    vpackuswb zmm24{k5}{z}, zmm24, [rbx]
    vpackuswb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpackuswb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpaddb xmm2, xmm7, xmm0
    vpaddb xmm2, xmm7, [rbx]
    vpaddb xmm2, xmm7, [rbx+r11*8+256]
    vpaddb xmm2, xmm7, [rbx+r11*8-256]
    vpaddb xmm2{k5}, xmm7, xmm0
    vpaddb xmm2{k5}, xmm7, [rbx]
    vpaddb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpaddb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpaddb xmm2{k5}{z}, xmm7, xmm0
    vpaddb xmm2{k5}{z}, xmm7, [rbx]
    vpaddb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpaddb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpaddb ymm16, ymm13, ymm15
    vpaddb ymm16, ymm13, [rbx]
    vpaddb ymm16, ymm13, [rbx+r11*8+256]
    vpaddb ymm16, ymm13, [rbx+r11*8-256]
    vpaddb ymm16{k5}, ymm13, ymm15
    vpaddb ymm16{k5}, ymm13, [rbx]
    vpaddb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpaddb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpaddb ymm16{k5}{z}, ymm13, ymm15
    vpaddb ymm16{k5}{z}, ymm13, [rbx]
    vpaddb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpaddb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpaddb zmm24, zmm24, zmm31
    vpaddb zmm24, zmm24, [rbx]
    vpaddb zmm24, zmm24, [rbx+r11*8+256]
    vpaddb zmm24, zmm24, [rbx+r11*8-256]
    vpaddb zmm24{k5}, zmm24, zmm31
    vpaddb zmm24{k5}, zmm24, [rbx]
    vpaddb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpaddb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpaddb zmm24{k5}{z}, zmm24, zmm31
    vpaddb zmm24{k5}{z}, zmm24, [rbx]
    vpaddb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpaddb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpaddsb xmm2, xmm7, xmm0
    vpaddsb xmm2, xmm7, [rbx]
    vpaddsb xmm2, xmm7, [rbx+r11*8+256]
    vpaddsb xmm2, xmm7, [rbx+r11*8-256]
    vpaddsb xmm2{k5}, xmm7, xmm0
    vpaddsb xmm2{k5}, xmm7, [rbx]
    vpaddsb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpaddsb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpaddsb xmm2{k5}{z}, xmm7, xmm0
    vpaddsb xmm2{k5}{z}, xmm7, [rbx]
    vpaddsb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpaddsb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpaddsb ymm16, ymm13, ymm15
    vpaddsb ymm16, ymm13, [rbx]
    vpaddsb ymm16, ymm13, [rbx+r11*8+256]
    vpaddsb ymm16, ymm13, [rbx+r11*8-256]
    vpaddsb ymm16{k5}, ymm13, ymm15
    vpaddsb ymm16{k5}, ymm13, [rbx]
    vpaddsb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpaddsb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpaddsb ymm16{k5}{z}, ymm13, ymm15
    vpaddsb ymm16{k5}{z}, ymm13, [rbx]
    vpaddsb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpaddsb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpaddsb zmm24, zmm24, zmm31
    vpaddsb zmm24, zmm24, [rbx]
    vpaddsb zmm24, zmm24, [rbx+r11*8+256]
    vpaddsb zmm24, zmm24, [rbx+r11*8-256]
    vpaddsb zmm24{k5}, zmm24, zmm31
    vpaddsb zmm24{k5}, zmm24, [rbx]
    vpaddsb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpaddsb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpaddsb zmm24{k5}{z}, zmm24, zmm31
    vpaddsb zmm24{k5}{z}, zmm24, [rbx]
    vpaddsb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpaddsb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpaddsw xmm2, xmm7, xmm0
    vpaddsw xmm2, xmm7, [rbx]
    vpaddsw xmm2, xmm7, [rbx+r11*8+256]
    vpaddsw xmm2, xmm7, [rbx+r11*8-256]
    vpaddsw xmm2{k5}, xmm7, xmm0
    vpaddsw xmm2{k5}, xmm7, [rbx]
    vpaddsw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpaddsw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpaddsw xmm2{k5}{z}, xmm7, xmm0
    vpaddsw xmm2{k5}{z}, xmm7, [rbx]
    vpaddsw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpaddsw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpaddsw ymm16, ymm13, ymm15
    vpaddsw ymm16, ymm13, [rbx]
    vpaddsw ymm16, ymm13, [rbx+r11*8+256]
    vpaddsw ymm16, ymm13, [rbx+r11*8-256]
    vpaddsw ymm16{k5}, ymm13, ymm15
    vpaddsw ymm16{k5}, ymm13, [rbx]
    vpaddsw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpaddsw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpaddsw ymm16{k5}{z}, ymm13, ymm15
    vpaddsw ymm16{k5}{z}, ymm13, [rbx]
    vpaddsw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpaddsw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpaddsw zmm24, zmm24, zmm31
    vpaddsw zmm24, zmm24, [rbx]
    vpaddsw zmm24, zmm24, [rbx+r11*8+256]
    vpaddsw zmm24, zmm24, [rbx+r11*8-256]
    vpaddsw zmm24{k5}, zmm24, zmm31
    vpaddsw zmm24{k5}, zmm24, [rbx]
    vpaddsw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpaddsw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpaddsw zmm24{k5}{z}, zmm24, zmm31
    vpaddsw zmm24{k5}{z}, zmm24, [rbx]
    vpaddsw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpaddsw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpaddusb xmm2, xmm7, xmm0
    vpaddusb xmm2, xmm7, [rbx]
    vpaddusb xmm2, xmm7, [rbx+r11*8+256]
    vpaddusb xmm2, xmm7, [rbx+r11*8-256]
    vpaddusb xmm2{k5}, xmm7, xmm0
    vpaddusb xmm2{k5}, xmm7, [rbx]
    vpaddusb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpaddusb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpaddusb xmm2{k5}{z}, xmm7, xmm0
    vpaddusb xmm2{k5}{z}, xmm7, [rbx]
    vpaddusb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpaddusb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpaddusb ymm16, ymm13, ymm15
    vpaddusb ymm16, ymm13, [rbx]
    vpaddusb ymm16, ymm13, [rbx+r11*8+256]
    vpaddusb ymm16, ymm13, [rbx+r11*8-256]
    vpaddusb ymm16{k5}, ymm13, ymm15
    vpaddusb ymm16{k5}, ymm13, [rbx]
    vpaddusb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpaddusb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpaddusb ymm16{k5}{z}, ymm13, ymm15
    vpaddusb ymm16{k5}{z}, ymm13, [rbx]
    vpaddusb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpaddusb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpaddusb zmm24, zmm24, zmm31
    vpaddusb zmm24, zmm24, [rbx]
    vpaddusb zmm24, zmm24, [rbx+r11*8+256]
    vpaddusb zmm24, zmm24, [rbx+r11*8-256]
    vpaddusb zmm24{k5}, zmm24, zmm31
    vpaddusb zmm24{k5}, zmm24, [rbx]
    vpaddusb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpaddusb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpaddusb zmm24{k5}{z}, zmm24, zmm31
    vpaddusb zmm24{k5}{z}, zmm24, [rbx]
    vpaddusb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpaddusb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpaddusw xmm2, xmm7, xmm0
    vpaddusw xmm2, xmm7, [rbx]
    vpaddusw xmm2, xmm7, [rbx+r11*8+256]
    vpaddusw xmm2, xmm7, [rbx+r11*8-256]
    vpaddusw xmm2{k5}, xmm7, xmm0
    vpaddusw xmm2{k5}, xmm7, [rbx]
    vpaddusw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpaddusw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpaddusw xmm2{k5}{z}, xmm7, xmm0
    vpaddusw xmm2{k5}{z}, xmm7, [rbx]
    vpaddusw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpaddusw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpaddusw ymm16, ymm13, ymm15
    vpaddusw ymm16, ymm13, [rbx]
    vpaddusw ymm16, ymm13, [rbx+r11*8+256]
    vpaddusw ymm16, ymm13, [rbx+r11*8-256]
    vpaddusw ymm16{k5}, ymm13, ymm15
    vpaddusw ymm16{k5}, ymm13, [rbx]
    vpaddusw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpaddusw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpaddusw ymm16{k5}{z}, ymm13, ymm15
    vpaddusw ymm16{k5}{z}, ymm13, [rbx]
    vpaddusw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpaddusw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpaddusw zmm24, zmm24, zmm31
    vpaddusw zmm24, zmm24, [rbx]
    vpaddusw zmm24, zmm24, [rbx+r11*8+256]
    vpaddusw zmm24, zmm24, [rbx+r11*8-256]
    vpaddusw zmm24{k5}, zmm24, zmm31
    vpaddusw zmm24{k5}, zmm24, [rbx]
    vpaddusw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpaddusw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpaddusw zmm24{k5}{z}, zmm24, zmm31
    vpaddusw zmm24{k5}{z}, zmm24, [rbx]
    vpaddusw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpaddusw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpaddw xmm2, xmm7, xmm0
    vpaddw xmm2, xmm7, [rbx]
    vpaddw xmm2, xmm7, [rbx+r11*8+256]
    vpaddw xmm2, xmm7, [rbx+r11*8-256]
    vpaddw xmm2{k5}, xmm7, xmm0
    vpaddw xmm2{k5}, xmm7, [rbx]
    vpaddw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpaddw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpaddw xmm2{k5}{z}, xmm7, xmm0
    vpaddw xmm2{k5}{z}, xmm7, [rbx]
    vpaddw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpaddw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpaddw ymm16, ymm13, ymm15
    vpaddw ymm16, ymm13, [rbx]
    vpaddw ymm16, ymm13, [rbx+r11*8+256]
    vpaddw ymm16, ymm13, [rbx+r11*8-256]
    vpaddw ymm16{k5}, ymm13, ymm15
    vpaddw ymm16{k5}, ymm13, [rbx]
    vpaddw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpaddw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpaddw ymm16{k5}{z}, ymm13, ymm15
    vpaddw ymm16{k5}{z}, ymm13, [rbx]
    vpaddw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpaddw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpaddw zmm24, zmm24, zmm31
    vpaddw zmm24, zmm24, [rbx]
    vpaddw zmm24, zmm24, [rbx+r11*8+256]
    vpaddw zmm24, zmm24, [rbx+r11*8-256]
    vpaddw zmm24{k5}, zmm24, zmm31
    vpaddw zmm24{k5}, zmm24, [rbx]
    vpaddw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpaddw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpaddw zmm24{k5}{z}, zmm24, zmm31
    vpaddw zmm24{k5}{z}, zmm24, [rbx]
    vpaddw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpaddw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpalignr xmm2, xmm7, xmm0, 10
    vpalignr xmm2, xmm7, [rbx], 10
    vpalignr xmm2, xmm7, [rbx+r11*8+256], 10
    vpalignr xmm2, xmm7, [rbx+r11*8-256], 10
    vpalignr xmm2{k5}, xmm7, xmm0, 10
    vpalignr xmm2{k5}, xmm7, [rbx], 10
    vpalignr xmm2{k5}, xmm7, [rbx+r11*8+256], 10
    vpalignr xmm2{k5}, xmm7, [rbx+r11*8-256], 10
    vpalignr xmm2{k5}{z}, xmm7, xmm0, 10
    vpalignr xmm2{k5}{z}, xmm7, [rbx], 10
    vpalignr xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 10
    vpalignr xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 10
    vpalignr ymm16, ymm13, ymm15, 10
    vpalignr ymm16, ymm13, [rbx], 10
    vpalignr ymm16, ymm13, [rbx+r11*8+256], 10
    vpalignr ymm16, ymm13, [rbx+r11*8-256], 10
    vpalignr ymm16{k5}, ymm13, ymm15, 10
    vpalignr ymm16{k5}, ymm13, [rbx], 10
    vpalignr ymm16{k5}, ymm13, [rbx+r11*8+256], 10
    vpalignr ymm16{k5}, ymm13, [rbx+r11*8-256], 10
    vpalignr ymm16{k5}{z}, ymm13, ymm15, 10
    vpalignr ymm16{k5}{z}, ymm13, [rbx], 10
    vpalignr ymm16{k5}{z}, ymm13, [rbx+r11*8+256], 10
    vpalignr ymm16{k5}{z}, ymm13, [rbx+r11*8-256], 10
    vpalignr zmm24, zmm24, zmm31, 10
    vpalignr zmm24, zmm24, [rbx], 10
    vpalignr zmm24, zmm24, [rbx+r11*8+256], 10
    vpalignr zmm24, zmm24, [rbx+r11*8-256], 10
    vpalignr zmm24{k5}, zmm24, zmm31, 10
    vpalignr zmm24{k5}, zmm24, [rbx], 10
    vpalignr zmm24{k5}, zmm24, [rbx+r11*8+256], 10
    vpalignr zmm24{k5}, zmm24, [rbx+r11*8-256], 10
    vpalignr zmm24{k5}{z}, zmm24, zmm31, 10
    vpalignr zmm24{k5}{z}, zmm24, [rbx], 10
    vpalignr zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 10
    vpalignr zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 10
    vpavgb xmm2, xmm7, xmm0
    vpavgb xmm2, xmm7, [rbx]
    vpavgb xmm2, xmm7, [rbx+r11*8+256]
    vpavgb xmm2, xmm7, [rbx+r11*8-256]
    vpavgb xmm2{k5}, xmm7, xmm0
    vpavgb xmm2{k5}, xmm7, [rbx]
    vpavgb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpavgb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpavgb xmm2{k5}{z}, xmm7, xmm0
    vpavgb xmm2{k5}{z}, xmm7, [rbx]
    vpavgb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpavgb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpavgb ymm16, ymm13, ymm15
    vpavgb ymm16, ymm13, [rbx]
    vpavgb ymm16, ymm13, [rbx+r11*8+256]
    vpavgb ymm16, ymm13, [rbx+r11*8-256]
    vpavgb ymm16{k5}, ymm13, ymm15
    vpavgb ymm16{k5}, ymm13, [rbx]
    vpavgb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpavgb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpavgb ymm16{k5}{z}, ymm13, ymm15
    vpavgb ymm16{k5}{z}, ymm13, [rbx]
    vpavgb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpavgb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpavgb zmm24, zmm24, zmm31
    vpavgb zmm24, zmm24, [rbx]
    vpavgb zmm24, zmm24, [rbx+r11*8+256]
    vpavgb zmm24, zmm24, [rbx+r11*8-256]
    vpavgb zmm24{k5}, zmm24, zmm31
    vpavgb zmm24{k5}, zmm24, [rbx]
    vpavgb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpavgb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpavgb zmm24{k5}{z}, zmm24, zmm31
    vpavgb zmm24{k5}{z}, zmm24, [rbx]
    vpavgb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpavgb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpavgw xmm2, xmm7, xmm0
    vpavgw xmm2, xmm7, [rbx]
    vpavgw xmm2, xmm7, [rbx+r11*8+256]
    vpavgw xmm2, xmm7, [rbx+r11*8-256]
    vpavgw xmm2{k5}, xmm7, xmm0
    vpavgw xmm2{k5}, xmm7, [rbx]
    vpavgw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpavgw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpavgw xmm2{k5}{z}, xmm7, xmm0
    vpavgw xmm2{k5}{z}, xmm7, [rbx]
    vpavgw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpavgw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpavgw ymm16, ymm13, ymm15
    vpavgw ymm16, ymm13, [rbx]
    vpavgw ymm16, ymm13, [rbx+r11*8+256]
    vpavgw ymm16, ymm13, [rbx+r11*8-256]
    vpavgw ymm16{k5}, ymm13, ymm15
    vpavgw ymm16{k5}, ymm13, [rbx]
    vpavgw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpavgw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpavgw ymm16{k5}{z}, ymm13, ymm15
    vpavgw ymm16{k5}{z}, ymm13, [rbx]
    vpavgw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpavgw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpavgw zmm24, zmm24, zmm31
    vpavgw zmm24, zmm24, [rbx]
    vpavgw zmm24, zmm24, [rbx+r11*8+256]
    vpavgw zmm24, zmm24, [rbx+r11*8-256]
    vpavgw zmm24{k5}, zmm24, zmm31
    vpavgw zmm24{k5}, zmm24, [rbx]
    vpavgw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpavgw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpavgw zmm24{k5}{z}, zmm24, zmm31
    vpavgw zmm24{k5}{z}, zmm24, [rbx]
    vpavgw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpavgw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpblendmb xmm2, xmm7, xmm0
    vpblendmb xmm2, xmm7, [rbx]
    vpblendmb xmm2, xmm7, [rbx+r11*8+256]
    vpblendmb xmm2, xmm7, [rbx+r11*8-256]
    vpblendmb xmm2{k5}, xmm7, xmm0
    vpblendmb xmm2{k5}, xmm7, [rbx]
    vpblendmb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpblendmb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpblendmb xmm2{k5}{z}, xmm7, xmm0
    vpblendmb xmm2{k5}{z}, xmm7, [rbx]
    vpblendmb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpblendmb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpblendmb ymm16, ymm13, ymm15
    vpblendmb ymm16, ymm13, [rbx]
    vpblendmb ymm16, ymm13, [rbx+r11*8+256]
    vpblendmb ymm16, ymm13, [rbx+r11*8-256]
    vpblendmb ymm16{k5}, ymm13, ymm15
    vpblendmb ymm16{k5}, ymm13, [rbx]
    vpblendmb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpblendmb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpblendmb ymm16{k5}{z}, ymm13, ymm15
    vpblendmb ymm16{k5}{z}, ymm13, [rbx]
    vpblendmb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpblendmb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpblendmb zmm24, zmm24, zmm31
    vpblendmb zmm24, zmm24, [rbx]
    vpblendmb zmm24, zmm24, [rbx+r11*8+256]
    vpblendmb zmm24, zmm24, [rbx+r11*8-256]
    vpblendmb zmm24{k5}, zmm24, zmm31
    vpblendmb zmm24{k5}, zmm24, [rbx]
    vpblendmb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpblendmb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpblendmb zmm24{k5}{z}, zmm24, zmm31
    vpblendmb zmm24{k5}{z}, zmm24, [rbx]
    vpblendmb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpblendmb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpblendmw xmm2, xmm7, xmm0
    vpblendmw xmm2, xmm7, [rbx]
    vpblendmw xmm2, xmm7, [rbx+r11*8+256]
    vpblendmw xmm2, xmm7, [rbx+r11*8-256]
    vpblendmw xmm2{k5}, xmm7, xmm0
    vpblendmw xmm2{k5}, xmm7, [rbx]
    vpblendmw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpblendmw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpblendmw xmm2{k5}{z}, xmm7, xmm0
    vpblendmw xmm2{k5}{z}, xmm7, [rbx]
    vpblendmw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpblendmw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpblendmw ymm16, ymm13, ymm15
    vpblendmw ymm16, ymm13, [rbx]
    vpblendmw ymm16, ymm13, [rbx+r11*8+256]
    vpblendmw ymm16, ymm13, [rbx+r11*8-256]
    vpblendmw ymm16{k5}, ymm13, ymm15
    vpblendmw ymm16{k5}, ymm13, [rbx]
    vpblendmw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpblendmw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpblendmw ymm16{k5}{z}, ymm13, ymm15
    vpblendmw ymm16{k5}{z}, ymm13, [rbx]
    vpblendmw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpblendmw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpblendmw zmm24, zmm24, zmm31
    vpblendmw zmm24, zmm24, [rbx]
    vpblendmw zmm24, zmm24, [rbx+r11*8+256]
    vpblendmw zmm24, zmm24, [rbx+r11*8-256]
    vpblendmw zmm24{k5}, zmm24, zmm31
    vpblendmw zmm24{k5}, zmm24, [rbx]
    vpblendmw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpblendmw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpblendmw zmm24{k5}{z}, zmm24, zmm31
    vpblendmw zmm24{k5}{z}, zmm24, [rbx]
    vpblendmw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpblendmw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpbroadcastb xmm2, xmm0
    vpbroadcastb xmm2, [rbx]
    vpbroadcastb xmm2, [rbx+r11*8+256]
    vpbroadcastb xmm2, [rbx+r11*8-256]
    vpbroadcastb xmm2{k5}, xmm0
    vpbroadcastb xmm2{k5}, [rbx]
    vpbroadcastb xmm2{k5}, [rbx+r11*8+256]
    vpbroadcastb xmm2{k5}, [rbx+r11*8-256]
    vpbroadcastb xmm2{k5}{z}, xmm0
    vpbroadcastb xmm2{k5}{z}, [rbx]
    vpbroadcastb xmm2{k5}{z}, [rbx+r11*8+256]
    vpbroadcastb xmm2{k5}{z}, [rbx+r11*8-256]
    vpbroadcastb ymm16, xmm0
    vpbroadcastb ymm16, [rbx]
    vpbroadcastb ymm16, [rbx+r11*8+256]
    vpbroadcastb ymm16, [rbx+r11*8-256]
    vpbroadcastb ymm16{k5}, xmm0
    vpbroadcastb ymm16{k5}, [rbx]
    vpbroadcastb ymm16{k5}, [rbx+r11*8+256]
    vpbroadcastb ymm16{k5}, [rbx+r11*8-256]
    vpbroadcastb ymm16{k5}{z}, xmm0
    vpbroadcastb ymm16{k5}{z}, [rbx]
    vpbroadcastb ymm16{k5}{z}, [rbx+r11*8+256]
    vpbroadcastb ymm16{k5}{z}, [rbx+r11*8-256]
    vpbroadcastb zmm24, xmm0
    vpbroadcastb zmm24, [rbx]
    vpbroadcastb zmm24, [rbx+r11*8+256]
    vpbroadcastb zmm24, [rbx+r11*8-256]
    vpbroadcastb zmm24{k5}, xmm0
    vpbroadcastb zmm24{k5}, [rbx]
    vpbroadcastb zmm24{k5}, [rbx+r11*8+256]
    vpbroadcastb zmm24{k5}, [rbx+r11*8-256]
    vpbroadcastb zmm24{k5}{z}, xmm0
    vpbroadcastb zmm24{k5}{z}, [rbx]
    vpbroadcastb zmm24{k5}{z}, [rbx+r11*8+256]
    vpbroadcastb zmm24{k5}{z}, [rbx+r11*8-256]
    vpbroadcastb xmm2, cl
    vpbroadcastb xmm2, cx
    vpbroadcastb xmm2, ecx
    vpbroadcastb xmm2, rcx
    vpbroadcastb xmm2{k5}, cl
    vpbroadcastb xmm2{k5}, cx
    vpbroadcastb xmm2{k5}, ecx
    vpbroadcastb xmm2{k5}, rcx
    vpbroadcastb xmm2{k5}{z}, cl
    vpbroadcastb xmm2{k5}{z}, cx
    vpbroadcastb xmm2{k5}{z}, ecx
    vpbroadcastb xmm2{k5}{z}, rcx
    vpbroadcastb ymm16, cl
    vpbroadcastb ymm16, cx
    vpbroadcastb ymm16, ecx
    vpbroadcastb ymm16, rcx
    vpbroadcastb ymm16{k5}, cl
    vpbroadcastb ymm16{k5}, cx
    vpbroadcastb ymm16{k5}, ecx
    vpbroadcastb ymm16{k5}, rcx
    vpbroadcastb ymm16{k5}{z}, cl
    vpbroadcastb ymm16{k5}{z}, cx
    vpbroadcastb ymm16{k5}{z}, ecx
    vpbroadcastb ymm16{k5}{z}, rcx
    vpbroadcastb zmm24, cl
    vpbroadcastb zmm24, cx
    vpbroadcastb zmm24, ecx
    vpbroadcastb zmm24, rcx
    vpbroadcastb zmm24{k5}, cl
    vpbroadcastb zmm24{k5}, cx
    vpbroadcastb zmm24{k5}, ecx
    vpbroadcastb zmm24{k5}, rcx
    vpbroadcastb zmm24{k5}{z}, cl
    vpbroadcastb zmm24{k5}{z}, cx
    vpbroadcastb zmm24{k5}{z}, ecx
    vpbroadcastb zmm24{k5}{z}, rcx
    vpbroadcastw xmm2, xmm0
    vpbroadcastw xmm2, [rbx]
    vpbroadcastw xmm2, [rbx+r11*8+256]
    vpbroadcastw xmm2, [rbx+r11*8-256]
    vpbroadcastw xmm2{k5}, xmm0
    vpbroadcastw xmm2{k5}, [rbx]
    vpbroadcastw xmm2{k5}, [rbx+r11*8+256]
    vpbroadcastw xmm2{k5}, [rbx+r11*8-256]
    vpbroadcastw xmm2{k5}{z}, xmm0
    vpbroadcastw xmm2{k5}{z}, [rbx]
    vpbroadcastw xmm2{k5}{z}, [rbx+r11*8+256]
    vpbroadcastw xmm2{k5}{z}, [rbx+r11*8-256]
    vpbroadcastw ymm16, xmm0
    vpbroadcastw ymm16, [rbx]
    vpbroadcastw ymm16, [rbx+r11*8+256]
    vpbroadcastw ymm16, [rbx+r11*8-256]
    vpbroadcastw ymm16{k5}, xmm0
    vpbroadcastw ymm16{k5}, [rbx]
    vpbroadcastw ymm16{k5}, [rbx+r11*8+256]
    vpbroadcastw ymm16{k5}, [rbx+r11*8-256]
    vpbroadcastw ymm16{k5}{z}, xmm0
    vpbroadcastw ymm16{k5}{z}, [rbx]
    vpbroadcastw ymm16{k5}{z}, [rbx+r11*8+256]
    vpbroadcastw ymm16{k5}{z}, [rbx+r11*8-256]
    vpbroadcastw zmm24, xmm0
    vpbroadcastw zmm24, [rbx]
    vpbroadcastw zmm24, [rbx+r11*8+256]
    vpbroadcastw zmm24, [rbx+r11*8-256]
    vpbroadcastw zmm24{k5}, xmm0
    vpbroadcastw zmm24{k5}, [rbx]
    vpbroadcastw zmm24{k5}, [rbx+r11*8+256]
    vpbroadcastw zmm24{k5}, [rbx+r11*8-256]
    vpbroadcastw zmm24{k5}{z}, xmm0
    vpbroadcastw zmm24{k5}{z}, [rbx]
    vpbroadcastw zmm24{k5}{z}, [rbx+r11*8+256]
    vpbroadcastw zmm24{k5}{z}, [rbx+r11*8-256]
    vpbroadcastw xmm2, cx
    vpbroadcastw xmm2, ecx
    vpbroadcastw xmm2, rcx
    vpbroadcastw xmm2{k5}, cx
    vpbroadcastw xmm2{k5}, ecx
    vpbroadcastw xmm2{k5}, rcx
    vpbroadcastw xmm2{k5}{z}, cx
    vpbroadcastw xmm2{k5}{z}, ecx
    vpbroadcastw xmm2{k5}{z}, rcx
    vpbroadcastw ymm16, cx
    vpbroadcastw ymm16, ecx
    vpbroadcastw ymm16, rcx
    vpbroadcastw ymm16{k5}, cx
    vpbroadcastw ymm16{k5}, ecx
    vpbroadcastw ymm16{k5}, rcx
    vpbroadcastw ymm16{k5}{z}, cx
    vpbroadcastw ymm16{k5}{z}, ecx
    vpbroadcastw ymm16{k5}{z}, rcx
    vpbroadcastw zmm24, cx
    vpbroadcastw zmm24, ecx
    vpbroadcastw zmm24, rcx
    vpbroadcastw zmm24{k5}, cx
    vpbroadcastw zmm24{k5}, ecx
    vpbroadcastw zmm24{k5}, rcx
    vpbroadcastw zmm24{k5}{z}, cx
    vpbroadcastw zmm24{k5}{z}, ecx
    vpbroadcastw zmm24{k5}{z}, rcx
    vpcmpb k3, xmm7, xmm0, 10
    vpcmpb k3, xmm7, [rbx], 10
    vpcmpb k3, xmm7, [rbx+r11*8+256], 10
    vpcmpb k3, xmm7, [rbx+r11*8-256], 10
    vpcmpb k3, ymm13, ymm15, 10
    vpcmpb k3, ymm13, [rbx], 10
    vpcmpb k3, ymm13, [rbx+r11*8+256], 10
    vpcmpb k3, ymm13, [rbx+r11*8-256], 10
    vpcmpb k3, zmm24, zmm31, 10
    vpcmpb k3, zmm24, [rbx], 10
    vpcmpb k3, zmm24, [rbx+r11*8+256], 10
    vpcmpb k3, zmm24, [rbx+r11*8-256], 10
    vpcmpb k3{k5}, xmm7, xmm0, 10
    vpcmpb k3{k5}, xmm7, [rbx], 10
    vpcmpb k3{k5}, xmm7, [rbx+r11*8+256], 10
    vpcmpb k3{k5}, xmm7, [rbx+r11*8-256], 10
    vpcmpb k3{k5}, ymm13, ymm15, 10
    vpcmpb k3{k5}, ymm13, [rbx], 10
    vpcmpb k3{k5}, ymm13, [rbx+r11*8+256], 10
    vpcmpb k3{k5}, ymm13, [rbx+r11*8-256], 10
    vpcmpb k3{k5}, zmm24, zmm31, 10
    vpcmpb k3{k5}, zmm24, [rbx], 10
    vpcmpb k3{k5}, zmm24, [rbx+r11*8+256], 10
    vpcmpb k3{k5}, zmm24, [rbx+r11*8-256], 10
    vpcmpeqb k3, xmm7, xmm0
    vpcmpeqb k3, xmm7, [rbx]
    vpcmpeqb k3, xmm7, [rbx+r11*8+256]
    vpcmpeqb k3, xmm7, [rbx+r11*8-256]
    vpcmpeqb k3, ymm13, ymm15
    vpcmpeqb k3, ymm13, [rbx]
    vpcmpeqb k3, ymm13, [rbx+r11*8+256]
    vpcmpeqb k3, ymm13, [rbx+r11*8-256]
    vpcmpeqb k3, zmm24, zmm31
    vpcmpeqb k3, zmm24, [rbx]
    vpcmpeqb k3, zmm24, [rbx+r11*8+256]
    vpcmpeqb k3, zmm24, [rbx+r11*8-256]
    vpcmpeqb k3{k5}, xmm7, xmm0
    vpcmpeqb k3{k5}, xmm7, [rbx]
    vpcmpeqb k3{k5}, xmm7, [rbx+r11*8+256]
    vpcmpeqb k3{k5}, xmm7, [rbx+r11*8-256]
    vpcmpeqb k3{k5}, ymm13, ymm15
    vpcmpeqb k3{k5}, ymm13, [rbx]
    vpcmpeqb k3{k5}, ymm13, [rbx+r11*8+256]
    vpcmpeqb k3{k5}, ymm13, [rbx+r11*8-256]
    vpcmpeqb k3{k5}, zmm24, zmm31
    vpcmpeqb k3{k5}, zmm24, [rbx]
    vpcmpeqb k3{k5}, zmm24, [rbx+r11*8+256]
    vpcmpeqb k3{k5}, zmm24, [rbx+r11*8-256]
    vpcmpeqw k3, xmm7, xmm0
    vpcmpeqw k3, xmm7, [rbx]
    vpcmpeqw k3, xmm7, [rbx+r11*8+256]
    vpcmpeqw k3, xmm7, [rbx+r11*8-256]
    vpcmpeqw k3, ymm13, ymm15
    vpcmpeqw k3, ymm13, [rbx]
    vpcmpeqw k3, ymm13, [rbx+r11*8+256]
    vpcmpeqw k3, ymm13, [rbx+r11*8-256]
    vpcmpeqw k3, zmm24, zmm31
    vpcmpeqw k3, zmm24, [rbx]
    vpcmpeqw k3, zmm24, [rbx+r11*8+256]
    vpcmpeqw k3, zmm24, [rbx+r11*8-256]
    vpcmpeqw k3{k5}, xmm7, xmm0
    vpcmpeqw k3{k5}, xmm7, [rbx]
    vpcmpeqw k3{k5}, xmm7, [rbx+r11*8+256]
    vpcmpeqw k3{k5}, xmm7, [rbx+r11*8-256]
    vpcmpeqw k3{k5}, ymm13, ymm15
    vpcmpeqw k3{k5}, ymm13, [rbx]
    vpcmpeqw k3{k5}, ymm13, [rbx+r11*8+256]
    vpcmpeqw k3{k5}, ymm13, [rbx+r11*8-256]
    vpcmpeqw k3{k5}, zmm24, zmm31
    vpcmpeqw k3{k5}, zmm24, [rbx]
    vpcmpeqw k3{k5}, zmm24, [rbx+r11*8+256]
    vpcmpeqw k3{k5}, zmm24, [rbx+r11*8-256]
    vpcmpgtb k3, xmm7, xmm0
    vpcmpgtb k3, xmm7, [rbx]
    vpcmpgtb k3, xmm7, [rbx+r11*8+256]
    vpcmpgtb k3, xmm7, [rbx+r11*8-256]
    vpcmpgtb k3, ymm13, ymm15
    vpcmpgtb k3, ymm13, [rbx]
    vpcmpgtb k3, ymm13, [rbx+r11*8+256]
    vpcmpgtb k3, ymm13, [rbx+r11*8-256]
    vpcmpgtb k3, zmm24, zmm31
    vpcmpgtb k3, zmm24, [rbx]
    vpcmpgtb k3, zmm24, [rbx+r11*8+256]
    vpcmpgtb k3, zmm24, [rbx+r11*8-256]
    vpcmpgtb k3{k5}, xmm7, xmm0
    vpcmpgtb k3{k5}, xmm7, [rbx]
    vpcmpgtb k3{k5}, xmm7, [rbx+r11*8+256]
    vpcmpgtb k3{k5}, xmm7, [rbx+r11*8-256]
    vpcmpgtb k3{k5}, ymm13, ymm15
    vpcmpgtb k3{k5}, ymm13, [rbx]
    vpcmpgtb k3{k5}, ymm13, [rbx+r11*8+256]
    vpcmpgtb k3{k5}, ymm13, [rbx+r11*8-256]
    vpcmpgtb k3{k5}, zmm24, zmm31
    vpcmpgtb k3{k5}, zmm24, [rbx]
    vpcmpgtb k3{k5}, zmm24, [rbx+r11*8+256]
    vpcmpgtb k3{k5}, zmm24, [rbx+r11*8-256]
    vpcmpgtw k3, xmm7, xmm0
    vpcmpgtw k3, xmm7, [rbx]
    vpcmpgtw k3, xmm7, [rbx+r11*8+256]
    vpcmpgtw k3, xmm7, [rbx+r11*8-256]
    vpcmpgtw k3, ymm13, ymm15
    vpcmpgtw k3, ymm13, [rbx]
    vpcmpgtw k3, ymm13, [rbx+r11*8+256]
    vpcmpgtw k3, ymm13, [rbx+r11*8-256]
    vpcmpgtw k3, zmm24, zmm31
    vpcmpgtw k3, zmm24, [rbx]
    vpcmpgtw k3, zmm24, [rbx+r11*8+256]
    vpcmpgtw k3, zmm24, [rbx+r11*8-256]
    vpcmpgtw k3{k5}, xmm7, xmm0
    vpcmpgtw k3{k5}, xmm7, [rbx]
    vpcmpgtw k3{k5}, xmm7, [rbx+r11*8+256]
    vpcmpgtw k3{k5}, xmm7, [rbx+r11*8-256]
    vpcmpgtw k3{k5}, ymm13, ymm15
    vpcmpgtw k3{k5}, ymm13, [rbx]
    vpcmpgtw k3{k5}, ymm13, [rbx+r11*8+256]
    vpcmpgtw k3{k5}, ymm13, [rbx+r11*8-256]
    vpcmpgtw k3{k5}, zmm24, zmm31
    vpcmpgtw k3{k5}, zmm24, [rbx]
    vpcmpgtw k3{k5}, zmm24, [rbx+r11*8+256]
    vpcmpgtw k3{k5}, zmm24, [rbx+r11*8-256]
    vpcmpub k3, xmm7, xmm0, 10
    vpcmpub k3, xmm7, [rbx], 10
    vpcmpub k3, xmm7, [rbx+r11*8+256], 10
    vpcmpub k3, xmm7, [rbx+r11*8-256], 10
    vpcmpub k3, ymm13, ymm15, 10
    vpcmpub k3, ymm13, [rbx], 10
    vpcmpub k3, ymm13, [rbx+r11*8+256], 10
    vpcmpub k3, ymm13, [rbx+r11*8-256], 10
    vpcmpub k3, zmm24, zmm31, 10
    vpcmpub k3, zmm24, [rbx], 10
    vpcmpub k3, zmm24, [rbx+r11*8+256], 10
    vpcmpub k3, zmm24, [rbx+r11*8-256], 10
    vpcmpub k3{k5}, xmm7, xmm0, 10
    vpcmpub k3{k5}, xmm7, [rbx], 10
    vpcmpub k3{k5}, xmm7, [rbx+r11*8+256], 10
    vpcmpub k3{k5}, xmm7, [rbx+r11*8-256], 10
    vpcmpub k3{k5}, ymm13, ymm15, 10
    vpcmpub k3{k5}, ymm13, [rbx], 10
    vpcmpub k3{k5}, ymm13, [rbx+r11*8+256], 10
    vpcmpub k3{k5}, ymm13, [rbx+r11*8-256], 10
    vpcmpub k3{k5}, zmm24, zmm31, 10
    vpcmpub k3{k5}, zmm24, [rbx], 10
    vpcmpub k3{k5}, zmm24, [rbx+r11*8+256], 10
    vpcmpub k3{k5}, zmm24, [rbx+r11*8-256], 10
    vpcmpuw k3, xmm7, xmm0, 10
    vpcmpuw k3, xmm7, [rbx], 10
    vpcmpuw k3, xmm7, [rbx+r11*8+256], 10
    vpcmpuw k3, xmm7, [rbx+r11*8-256], 10
    vpcmpuw k3, ymm13, ymm15, 10
    vpcmpuw k3, ymm13, [rbx], 10
    vpcmpuw k3, ymm13, [rbx+r11*8+256], 10
    vpcmpuw k3, ymm13, [rbx+r11*8-256], 10
    vpcmpuw k3, zmm24, zmm31, 10
    vpcmpuw k3, zmm24, [rbx], 10
    vpcmpuw k3, zmm24, [rbx+r11*8+256], 10
    vpcmpuw k3, zmm24, [rbx+r11*8-256], 10
    vpcmpuw k3{k5}, xmm7, xmm0, 10
    vpcmpuw k3{k5}, xmm7, [rbx], 10
    vpcmpuw k3{k5}, xmm7, [rbx+r11*8+256], 10
    vpcmpuw k3{k5}, xmm7, [rbx+r11*8-256], 10
    vpcmpuw k3{k5}, ymm13, ymm15, 10
    vpcmpuw k3{k5}, ymm13, [rbx], 10
    vpcmpuw k3{k5}, ymm13, [rbx+r11*8+256], 10
    vpcmpuw k3{k5}, ymm13, [rbx+r11*8-256], 10
    vpcmpuw k3{k5}, zmm24, zmm31, 10
    vpcmpuw k3{k5}, zmm24, [rbx], 10
    vpcmpuw k3{k5}, zmm24, [rbx+r11*8+256], 10
    vpcmpuw k3{k5}, zmm24, [rbx+r11*8-256], 10
    vpcmpw k3, xmm7, xmm0, 10
    vpcmpw k3, xmm7, [rbx], 10
    vpcmpw k3, xmm7, [rbx+r11*8+256], 10
    vpcmpw k3, xmm7, [rbx+r11*8-256], 10
    vpcmpw k3, ymm13, ymm15, 10
    vpcmpw k3, ymm13, [rbx], 10
    vpcmpw k3, ymm13, [rbx+r11*8+256], 10
    vpcmpw k3, ymm13, [rbx+r11*8-256], 10
    vpcmpw k3, zmm24, zmm31, 10
    vpcmpw k3, zmm24, [rbx], 10
    vpcmpw k3, zmm24, [rbx+r11*8+256], 10
    vpcmpw k3, zmm24, [rbx+r11*8-256], 10
    vpcmpw k3{k5}, xmm7, xmm0, 10
    vpcmpw k3{k5}, xmm7, [rbx], 10
    vpcmpw k3{k5}, xmm7, [rbx+r11*8+256], 10
    vpcmpw k3{k5}, xmm7, [rbx+r11*8-256], 10
    vpcmpw k3{k5}, ymm13, ymm15, 10
    vpcmpw k3{k5}, ymm13, [rbx], 10
    vpcmpw k3{k5}, ymm13, [rbx+r11*8+256], 10
    vpcmpw k3{k5}, ymm13, [rbx+r11*8-256], 10
    vpcmpw k3{k5}, zmm24, zmm31, 10
    vpcmpw k3{k5}, zmm24, [rbx], 10
    vpcmpw k3{k5}, zmm24, [rbx+r11*8+256], 10
    vpcmpw k3{k5}, zmm24, [rbx+r11*8-256], 10
    vpermi2w xmm2, xmm7, xmm0
    vpermi2w xmm2, xmm7, [rbx]
    vpermi2w xmm2, xmm7, [rbx+r11*8+256]
    vpermi2w xmm2, xmm7, [rbx+r11*8-256]
    vpermi2w xmm2{k5}, xmm7, xmm0
    vpermi2w xmm2{k5}, xmm7, [rbx]
    vpermi2w xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpermi2w xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpermi2w xmm2{k5}{z}, xmm7, xmm0
    vpermi2w xmm2{k5}{z}, xmm7, [rbx]
    vpermi2w xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpermi2w xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpermi2w ymm16, ymm13, ymm15
    vpermi2w ymm16, ymm13, [rbx]
    vpermi2w ymm16, ymm13, [rbx+r11*8+256]
    vpermi2w ymm16, ymm13, [rbx+r11*8-256]
    vpermi2w ymm16{k5}, ymm13, ymm15
    vpermi2w ymm16{k5}, ymm13, [rbx]
    vpermi2w ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpermi2w ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpermi2w ymm16{k5}{z}, ymm13, ymm15
    vpermi2w ymm16{k5}{z}, ymm13, [rbx]
    vpermi2w ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpermi2w ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpermi2w zmm24, zmm24, zmm31
    vpermi2w zmm24, zmm24, [rbx]
    vpermi2w zmm24, zmm24, [rbx+r11*8+256]
    vpermi2w zmm24, zmm24, [rbx+r11*8-256]
    vpermi2w zmm24{k5}, zmm24, zmm31
    vpermi2w zmm24{k5}, zmm24, [rbx]
    vpermi2w zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpermi2w zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpermi2w zmm24{k5}{z}, zmm24, zmm31
    vpermi2w zmm24{k5}{z}, zmm24, [rbx]
    vpermi2w zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpermi2w zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpermt2w xmm2, xmm7, xmm0
    vpermt2w xmm2, xmm7, [rbx]
    vpermt2w xmm2, xmm7, [rbx+r11*8+256]
    vpermt2w xmm2, xmm7, [rbx+r11*8-256]
    vpermt2w xmm2{k5}, xmm7, xmm0
    vpermt2w xmm2{k5}, xmm7, [rbx]
    vpermt2w xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpermt2w xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpermt2w xmm2{k5}{z}, xmm7, xmm0
    vpermt2w xmm2{k5}{z}, xmm7, [rbx]
    vpermt2w xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpermt2w xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpermt2w ymm16, ymm13, ymm15
    vpermt2w ymm16, ymm13, [rbx]
    vpermt2w ymm16, ymm13, [rbx+r11*8+256]
    vpermt2w ymm16, ymm13, [rbx+r11*8-256]
    vpermt2w ymm16{k5}, ymm13, ymm15
    vpermt2w ymm16{k5}, ymm13, [rbx]
    vpermt2w ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpermt2w ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpermt2w ymm16{k5}{z}, ymm13, ymm15
    vpermt2w ymm16{k5}{z}, ymm13, [rbx]
    vpermt2w ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpermt2w ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpermt2w zmm24, zmm24, zmm31
    vpermt2w zmm24, zmm24, [rbx]
    vpermt2w zmm24, zmm24, [rbx+r11*8+256]
    vpermt2w zmm24, zmm24, [rbx+r11*8-256]
    vpermt2w zmm24{k5}, zmm24, zmm31
    vpermt2w zmm24{k5}, zmm24, [rbx]
    vpermt2w zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpermt2w zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpermt2w zmm24{k5}{z}, zmm24, zmm31
    vpermt2w zmm24{k5}{z}, zmm24, [rbx]
    vpermt2w zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpermt2w zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpermw xmm2, xmm7, xmm0
    vpermw xmm2, xmm7, [rbx]
    vpermw xmm2, xmm7, [rbx+r11*8+256]
    vpermw xmm2, xmm7, [rbx+r11*8-256]
    vpermw xmm2{k5}, xmm7, xmm0
    vpermw xmm2{k5}, xmm7, [rbx]
    vpermw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpermw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpermw xmm2{k5}{z}, xmm7, xmm0
    vpermw xmm2{k5}{z}, xmm7, [rbx]
    vpermw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpermw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpermw ymm16, ymm13, ymm15
    vpermw ymm16, ymm13, [rbx]
    vpermw ymm16, ymm13, [rbx+r11*8+256]
    vpermw ymm16, ymm13, [rbx+r11*8-256]
    vpermw ymm16{k5}, ymm13, ymm15
    vpermw ymm16{k5}, ymm13, [rbx]
    vpermw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpermw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpermw ymm16{k5}{z}, ymm13, ymm15
    vpermw ymm16{k5}{z}, ymm13, [rbx]
    vpermw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpermw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpermw zmm24, zmm24, zmm31
    vpermw zmm24, zmm24, [rbx]
    vpermw zmm24, zmm24, [rbx+r11*8+256]
    vpermw zmm24, zmm24, [rbx+r11*8-256]
    vpermw zmm24{k5}, zmm24, zmm31
    vpermw zmm24{k5}, zmm24, [rbx]
    vpermw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpermw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpermw zmm24{k5}{z}, zmm24, zmm31
    vpermw zmm24{k5}{z}, zmm24, [rbx]
    vpermw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpermw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpextrb [rbx], xmm2, 10
    vpextrb [rbx+rsi*8+256], xmm2, 10
    vpextrb [rbx+rsi*8-256], xmm2, 10
    vpextrb cl, xmm2, 10
    vpextrb cx, xmm2, 10
    vpextrb ecx, xmm2, 10
    vpextrb rcx, xmm2, 10
    vpextrw cx, xmm0, 10
    vpextrw ecx, xmm0, 10
    vpextrw rcx, xmm0, 10
    vpextrw [rbx], xmm2, 10
    vpextrw [rbx+rsi*8+256], xmm2, 10
    vpextrw [rbx+rsi*8-256], xmm2, 10
    vpextrw cx, xmm2, 10
    vpextrw ecx, xmm2, 10
    vpextrw rcx, xmm2, 10
    vpinsrb xmm2, xmm7, [rbx], 10
    vpinsrb xmm2, xmm7, [rbx+rsi*8+256], 10
    vpinsrb xmm2, xmm7, [rbx+rsi*8-256], 10
    vpinsrb xmm2, xmm7, cl, 10
    vpinsrb xmm2, xmm7, ecx, 10
    vpinsrw xmm2, xmm7, [rbx], 10
    vpinsrw xmm2, xmm7, [rbx+rsi*8+256], 10
    vpinsrw xmm2, xmm7, [rbx+rsi*8-256], 10
    vpinsrw xmm2, xmm7, cx, 10
    vpinsrw xmm2, xmm7, ecx, 10
    vpmaddubsw xmm2, xmm7, xmm0
    vpmaddubsw xmm2, xmm7, [rbx]
    vpmaddubsw xmm2, xmm7, [rbx+r11*8+256]
    vpmaddubsw xmm2, xmm7, [rbx+r11*8-256]
    vpmaddubsw xmm2{k5}, xmm7, xmm0
    vpmaddubsw xmm2{k5}, xmm7, [rbx]
    vpmaddubsw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpmaddubsw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpmaddubsw xmm2{k5}{z}, xmm7, xmm0
    vpmaddubsw xmm2{k5}{z}, xmm7, [rbx]
    vpmaddubsw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpmaddubsw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpmaddubsw ymm16, ymm13, ymm15
    vpmaddubsw ymm16, ymm13, [rbx]
    vpmaddubsw ymm16, ymm13, [rbx+r11*8+256]
    vpmaddubsw ymm16, ymm13, [rbx+r11*8-256]
    vpmaddubsw ymm16{k5}, ymm13, ymm15
    vpmaddubsw ymm16{k5}, ymm13, [rbx]
    vpmaddubsw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpmaddubsw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpmaddubsw ymm16{k5}{z}, ymm13, ymm15
    vpmaddubsw ymm16{k5}{z}, ymm13, [rbx]
    vpmaddubsw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpmaddubsw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpmaddubsw zmm24, zmm24, zmm31
    vpmaddubsw zmm24, zmm24, [rbx]
    vpmaddubsw zmm24, zmm24, [rbx+r11*8+256]
    vpmaddubsw zmm24, zmm24, [rbx+r11*8-256]
    vpmaddubsw zmm24{k5}, zmm24, zmm31
    vpmaddubsw zmm24{k5}, zmm24, [rbx]
    vpmaddubsw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpmaddubsw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpmaddubsw zmm24{k5}{z}, zmm24, zmm31
    vpmaddubsw zmm24{k5}{z}, zmm24, [rbx]
    vpmaddubsw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpmaddubsw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpmaddwd xmm2, xmm7, xmm0
    vpmaddwd xmm2, xmm7, [rbx]
    vpmaddwd xmm2, xmm7, [rbx+r11*8+256]
    vpmaddwd xmm2, xmm7, [rbx+r11*8-256]
    vpmaddwd xmm2{k5}, xmm7, xmm0
    vpmaddwd xmm2{k5}, xmm7, [rbx]
    vpmaddwd xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpmaddwd xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpmaddwd xmm2{k5}{z}, xmm7, xmm0
    vpmaddwd xmm2{k5}{z}, xmm7, [rbx]
    vpmaddwd xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpmaddwd xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpmaddwd ymm16, ymm13, ymm15
    vpmaddwd ymm16, ymm13, [rbx]
    vpmaddwd ymm16, ymm13, [rbx+r11*8+256]
    vpmaddwd ymm16, ymm13, [rbx+r11*8-256]
    vpmaddwd ymm16{k5}, ymm13, ymm15
    vpmaddwd ymm16{k5}, ymm13, [rbx]
    vpmaddwd ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpmaddwd ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpmaddwd ymm16{k5}{z}, ymm13, ymm15
    vpmaddwd ymm16{k5}{z}, ymm13, [rbx]
    vpmaddwd ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpmaddwd ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpmaddwd zmm24, zmm24, zmm31
    vpmaddwd zmm24, zmm24, [rbx]
    vpmaddwd zmm24, zmm24, [rbx+r11*8+256]
    vpmaddwd zmm24, zmm24, [rbx+r11*8-256]
    vpmaddwd zmm24{k5}, zmm24, zmm31
    vpmaddwd zmm24{k5}, zmm24, [rbx]
    vpmaddwd zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpmaddwd zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpmaddwd zmm24{k5}{z}, zmm24, zmm31
    vpmaddwd zmm24{k5}{z}, zmm24, [rbx]
    vpmaddwd zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpmaddwd zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpmaxsb xmm2, xmm7, xmm0
    vpmaxsb xmm2, xmm7, [rbx]
    vpmaxsb xmm2, xmm7, [rbx+r11*8+256]
    vpmaxsb xmm2, xmm7, [rbx+r11*8-256]
    vpmaxsb xmm2{k5}, xmm7, xmm0
    vpmaxsb xmm2{k5}, xmm7, [rbx]
    vpmaxsb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpmaxsb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpmaxsb xmm2{k5}{z}, xmm7, xmm0
    vpmaxsb xmm2{k5}{z}, xmm7, [rbx]
    vpmaxsb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpmaxsb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpmaxsb ymm16, ymm13, ymm15
    vpmaxsb ymm16, ymm13, [rbx]
    vpmaxsb ymm16, ymm13, [rbx+r11*8+256]
    vpmaxsb ymm16, ymm13, [rbx+r11*8-256]
    vpmaxsb ymm16{k5}, ymm13, ymm15
    vpmaxsb ymm16{k5}, ymm13, [rbx]
    vpmaxsb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpmaxsb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpmaxsb ymm16{k5}{z}, ymm13, ymm15
    vpmaxsb ymm16{k5}{z}, ymm13, [rbx]
    vpmaxsb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpmaxsb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpmaxsb zmm24, zmm24, zmm31
    vpmaxsb zmm24, zmm24, [rbx]
    vpmaxsb zmm24, zmm24, [rbx+r11*8+256]
    vpmaxsb zmm24, zmm24, [rbx+r11*8-256]
    vpmaxsb zmm24{k5}, zmm24, zmm31
    vpmaxsb zmm24{k5}, zmm24, [rbx]
    vpmaxsb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpmaxsb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpmaxsb zmm24{k5}{z}, zmm24, zmm31
    vpmaxsb zmm24{k5}{z}, zmm24, [rbx]
    vpmaxsb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpmaxsb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpmaxsw xmm2, xmm7, xmm0
    vpmaxsw xmm2, xmm7, [rbx]
    vpmaxsw xmm2, xmm7, [rbx+r11*8+256]
    vpmaxsw xmm2, xmm7, [rbx+r11*8-256]
    vpmaxsw xmm2{k5}, xmm7, xmm0
    vpmaxsw xmm2{k5}, xmm7, [rbx]
    vpmaxsw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpmaxsw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpmaxsw xmm2{k5}{z}, xmm7, xmm0
    vpmaxsw xmm2{k5}{z}, xmm7, [rbx]
    vpmaxsw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpmaxsw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpmaxsw ymm16, ymm13, ymm15
    vpmaxsw ymm16, ymm13, [rbx]
    vpmaxsw ymm16, ymm13, [rbx+r11*8+256]
    vpmaxsw ymm16, ymm13, [rbx+r11*8-256]
    vpmaxsw ymm16{k5}, ymm13, ymm15
    vpmaxsw ymm16{k5}, ymm13, [rbx]
    vpmaxsw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpmaxsw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpmaxsw ymm16{k5}{z}, ymm13, ymm15
    vpmaxsw ymm16{k5}{z}, ymm13, [rbx]
    vpmaxsw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpmaxsw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpmaxsw zmm24, zmm24, zmm31
    vpmaxsw zmm24, zmm24, [rbx]
    vpmaxsw zmm24, zmm24, [rbx+r11*8+256]
    vpmaxsw zmm24, zmm24, [rbx+r11*8-256]
    vpmaxsw zmm24{k5}, zmm24, zmm31
    vpmaxsw zmm24{k5}, zmm24, [rbx]
    vpmaxsw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpmaxsw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpmaxsw zmm24{k5}{z}, zmm24, zmm31
    vpmaxsw zmm24{k5}{z}, zmm24, [rbx]
    vpmaxsw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpmaxsw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpmaxub xmm2, xmm7, xmm0
    vpmaxub xmm2, xmm7, [rbx]
    vpmaxub xmm2, xmm7, [rbx+r11*8+256]
    vpmaxub xmm2, xmm7, [rbx+r11*8-256]
    vpmaxub xmm2{k5}, xmm7, xmm0
    vpmaxub xmm2{k5}, xmm7, [rbx]
    vpmaxub xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpmaxub xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpmaxub xmm2{k5}{z}, xmm7, xmm0
    vpmaxub xmm2{k5}{z}, xmm7, [rbx]
    vpmaxub xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpmaxub xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpmaxub ymm16, ymm13, ymm15
    vpmaxub ymm16, ymm13, [rbx]
    vpmaxub ymm16, ymm13, [rbx+r11*8+256]
    vpmaxub ymm16, ymm13, [rbx+r11*8-256]
    vpmaxub ymm16{k5}, ymm13, ymm15
    vpmaxub ymm16{k5}, ymm13, [rbx]
    vpmaxub ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpmaxub ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpmaxub ymm16{k5}{z}, ymm13, ymm15
    vpmaxub ymm16{k5}{z}, ymm13, [rbx]
    vpmaxub ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpmaxub ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpmaxub zmm24, zmm24, zmm31
    vpmaxub zmm24, zmm24, [rbx]
    vpmaxub zmm24, zmm24, [rbx+r11*8+256]
    vpmaxub zmm24, zmm24, [rbx+r11*8-256]
    vpmaxub zmm24{k5}, zmm24, zmm31
    vpmaxub zmm24{k5}, zmm24, [rbx]
    vpmaxub zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpmaxub zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpmaxub zmm24{k5}{z}, zmm24, zmm31
    vpmaxub zmm24{k5}{z}, zmm24, [rbx]
    vpmaxub zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpmaxub zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpmaxuw xmm2, xmm7, xmm0
    vpmaxuw xmm2, xmm7, [rbx]
    vpmaxuw xmm2, xmm7, [rbx+r11*8+256]
    vpmaxuw xmm2, xmm7, [rbx+r11*8-256]
    vpmaxuw xmm2{k5}, xmm7, xmm0
    vpmaxuw xmm2{k5}, xmm7, [rbx]
    vpmaxuw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpmaxuw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpmaxuw xmm2{k5}{z}, xmm7, xmm0
    vpmaxuw xmm2{k5}{z}, xmm7, [rbx]
    vpmaxuw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpmaxuw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpmaxuw ymm16, ymm13, ymm15
    vpmaxuw ymm16, ymm13, [rbx]
    vpmaxuw ymm16, ymm13, [rbx+r11*8+256]
    vpmaxuw ymm16, ymm13, [rbx+r11*8-256]
    vpmaxuw ymm16{k5}, ymm13, ymm15
    vpmaxuw ymm16{k5}, ymm13, [rbx]
    vpmaxuw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpmaxuw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpmaxuw ymm16{k5}{z}, ymm13, ymm15
    vpmaxuw ymm16{k5}{z}, ymm13, [rbx]
    vpmaxuw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpmaxuw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpmaxuw zmm24, zmm24, zmm31
    vpmaxuw zmm24, zmm24, [rbx]
    vpmaxuw zmm24, zmm24, [rbx+r11*8+256]
    vpmaxuw zmm24, zmm24, [rbx+r11*8-256]
    vpmaxuw zmm24{k5}, zmm24, zmm31
    vpmaxuw zmm24{k5}, zmm24, [rbx]
    vpmaxuw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpmaxuw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpmaxuw zmm24{k5}{z}, zmm24, zmm31
    vpmaxuw zmm24{k5}{z}, zmm24, [rbx]
    vpmaxuw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpmaxuw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpminsb xmm2, xmm7, xmm0
    vpminsb xmm2, xmm7, [rbx]
    vpminsb xmm2, xmm7, [rbx+r11*8+256]
    vpminsb xmm2, xmm7, [rbx+r11*8-256]
    vpminsb xmm2{k5}, xmm7, xmm0
    vpminsb xmm2{k5}, xmm7, [rbx]
    vpminsb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpminsb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpminsb xmm2{k5}{z}, xmm7, xmm0
    vpminsb xmm2{k5}{z}, xmm7, [rbx]
    vpminsb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpminsb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpminsb ymm16, ymm13, ymm15
    vpminsb ymm16, ymm13, [rbx]
    vpminsb ymm16, ymm13, [rbx+r11*8+256]
    vpminsb ymm16, ymm13, [rbx+r11*8-256]
    vpminsb ymm16{k5}, ymm13, ymm15
    vpminsb ymm16{k5}, ymm13, [rbx]
    vpminsb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpminsb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpminsb ymm16{k5}{z}, ymm13, ymm15
    vpminsb ymm16{k5}{z}, ymm13, [rbx]
    vpminsb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpminsb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpminsb zmm24, zmm24, zmm31
    vpminsb zmm24, zmm24, [rbx]
    vpminsb zmm24, zmm24, [rbx+r11*8+256]
    vpminsb zmm24, zmm24, [rbx+r11*8-256]
    vpminsb zmm24{k5}, zmm24, zmm31
    vpminsb zmm24{k5}, zmm24, [rbx]
    vpminsb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpminsb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpminsb zmm24{k5}{z}, zmm24, zmm31
    vpminsb zmm24{k5}{z}, zmm24, [rbx]
    vpminsb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpminsb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpminsw xmm2, xmm7, xmm0
    vpminsw xmm2, xmm7, [rbx]
    vpminsw xmm2, xmm7, [rbx+r11*8+256]
    vpminsw xmm2, xmm7, [rbx+r11*8-256]
    vpminsw xmm2{k5}, xmm7, xmm0
    vpminsw xmm2{k5}, xmm7, [rbx]
    vpminsw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpminsw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpminsw xmm2{k5}{z}, xmm7, xmm0
    vpminsw xmm2{k5}{z}, xmm7, [rbx]
    vpminsw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpminsw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpminsw ymm16, ymm13, ymm15
    vpminsw ymm16, ymm13, [rbx]
    vpminsw ymm16, ymm13, [rbx+r11*8+256]
    vpminsw ymm16, ymm13, [rbx+r11*8-256]
    vpminsw ymm16{k5}, ymm13, ymm15
    vpminsw ymm16{k5}, ymm13, [rbx]
    vpminsw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpminsw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpminsw ymm16{k5}{z}, ymm13, ymm15
    vpminsw ymm16{k5}{z}, ymm13, [rbx]
    vpminsw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpminsw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpminsw zmm24, zmm24, zmm31
    vpminsw zmm24, zmm24, [rbx]
    vpminsw zmm24, zmm24, [rbx+r11*8+256]
    vpminsw zmm24, zmm24, [rbx+r11*8-256]
    vpminsw zmm24{k5}, zmm24, zmm31
    vpminsw zmm24{k5}, zmm24, [rbx]
    vpminsw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpminsw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpminsw zmm24{k5}{z}, zmm24, zmm31
    vpminsw zmm24{k5}{z}, zmm24, [rbx]
    vpminsw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpminsw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpminub xmm2, xmm7, xmm0
    vpminub xmm2, xmm7, [rbx]
    vpminub xmm2, xmm7, [rbx+r11*8+256]
    vpminub xmm2, xmm7, [rbx+r11*8-256]
    vpminub xmm2{k5}, xmm7, xmm0
    vpminub xmm2{k5}, xmm7, [rbx]
    vpminub xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpminub xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpminub xmm2{k5}{z}, xmm7, xmm0
    vpminub xmm2{k5}{z}, xmm7, [rbx]
    vpminub xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpminub xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpminub ymm16, ymm13, ymm15
    vpminub ymm16, ymm13, [rbx]
    vpminub ymm16, ymm13, [rbx+r11*8+256]
    vpminub ymm16, ymm13, [rbx+r11*8-256]
    vpminub ymm16{k5}, ymm13, ymm15
    vpminub ymm16{k5}, ymm13, [rbx]
    vpminub ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpminub ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpminub ymm16{k5}{z}, ymm13, ymm15
    vpminub ymm16{k5}{z}, ymm13, [rbx]
    vpminub ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpminub ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpminub zmm24, zmm24, zmm31
    vpminub zmm24, zmm24, [rbx]
    vpminub zmm24, zmm24, [rbx+r11*8+256]
    vpminub zmm24, zmm24, [rbx+r11*8-256]
    vpminub zmm24{k5}, zmm24, zmm31
    vpminub zmm24{k5}, zmm24, [rbx]
    vpminub zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpminub zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpminub zmm24{k5}{z}, zmm24, zmm31
    vpminub zmm24{k5}{z}, zmm24, [rbx]
    vpminub zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpminub zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpminuw xmm2, xmm7, xmm0
    vpminuw xmm2, xmm7, [rbx]
    vpminuw xmm2, xmm7, [rbx+r11*8+256]
    vpminuw xmm2, xmm7, [rbx+r11*8-256]
    vpminuw xmm2{k5}, xmm7, xmm0
    vpminuw xmm2{k5}, xmm7, [rbx]
    vpminuw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpminuw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpminuw xmm2{k5}{z}, xmm7, xmm0
    vpminuw xmm2{k5}{z}, xmm7, [rbx]
    vpminuw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpminuw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpminuw ymm16, ymm13, ymm15
    vpminuw ymm16, ymm13, [rbx]
    vpminuw ymm16, ymm13, [rbx+r11*8+256]
    vpminuw ymm16, ymm13, [rbx+r11*8-256]
    vpminuw ymm16{k5}, ymm13, ymm15
    vpminuw ymm16{k5}, ymm13, [rbx]
    vpminuw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpminuw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpminuw ymm16{k5}{z}, ymm13, ymm15
    vpminuw ymm16{k5}{z}, ymm13, [rbx]
    vpminuw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpminuw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpminuw zmm24, zmm24, zmm31
    vpminuw zmm24, zmm24, [rbx]
    vpminuw zmm24, zmm24, [rbx+r11*8+256]
    vpminuw zmm24, zmm24, [rbx+r11*8-256]
    vpminuw zmm24{k5}, zmm24, zmm31
    vpminuw zmm24{k5}, zmm24, [rbx]
    vpminuw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpminuw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpminuw zmm24{k5}{z}, zmm24, zmm31
    vpminuw zmm24{k5}{z}, zmm24, [rbx]
    vpminuw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpminuw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpmovb2m k3, xmm0
    vpmovb2m k3, ymm15
    vpmovb2m k3, zmm31
    vpmovm2b xmm2, k1
    vpmovm2b ymm16, k1
    vpmovm2b zmm24, k1
    vpmovm2w xmm2, k1
    vpmovm2w ymm16, k1
    vpmovm2w zmm24, k1
    vpmovswb xmm0, xmm2
    vpmovswb xmm0, ymm16
    vpmovswb ymm15, zmm24
    vpmovswb [rbx], xmm2
    vpmovswb [rbx], ymm16
    vpmovswb [rbx], zmm24
    vpmovswb [rbx+r11*8+256], xmm2
    vpmovswb [rbx+r11*8+256], ymm16
    vpmovswb [rbx+r11*8+256], zmm24
    vpmovswb [rbx+r11*8-256], xmm2
    vpmovswb [rbx+r11*8-256], ymm16
    vpmovswb [rbx+r11*8-256], zmm24
    vpmovsxbw xmm2, xmm0
    vpmovsxbw xmm2, [rbx]
    vpmovsxbw xmm2, [rbx+r11*8+256]
    vpmovsxbw xmm2, [rbx+r11*8-256]
    vpmovsxbw xmm2{k5}, xmm0
    vpmovsxbw xmm2{k5}, [rbx]
    vpmovsxbw xmm2{k5}, [rbx+r11*8+256]
    vpmovsxbw xmm2{k5}, [rbx+r11*8-256]
    vpmovsxbw xmm2{k5}{z}, xmm0
    vpmovsxbw xmm2{k5}{z}, [rbx]
    vpmovsxbw xmm2{k5}{z}, [rbx+r11*8+256]
    vpmovsxbw xmm2{k5}{z}, [rbx+r11*8-256]
    vpmovsxbw ymm16, xmm0
    vpmovsxbw ymm16, [rbx]
    vpmovsxbw ymm16, [rbx+r11*8+256]
    vpmovsxbw ymm16, [rbx+r11*8-256]
    vpmovsxbw ymm16{k5}, xmm0
    vpmovsxbw ymm16{k5}, [rbx]
    vpmovsxbw ymm16{k5}, [rbx+r11*8+256]
    vpmovsxbw ymm16{k5}, [rbx+r11*8-256]
    vpmovsxbw ymm16{k5}{z}, xmm0
    vpmovsxbw ymm16{k5}{z}, [rbx]
    vpmovsxbw ymm16{k5}{z}, [rbx+r11*8+256]
    vpmovsxbw ymm16{k5}{z}, [rbx+r11*8-256]
    vpmovsxbw zmm24, ymm15
    vpmovsxbw zmm24, [rbx]
    vpmovsxbw zmm24, [rbx+r11*8+256]
    vpmovsxbw zmm24, [rbx+r11*8-256]
    vpmovsxbw zmm24{k5}, ymm15
    vpmovsxbw zmm24{k5}, [rbx]
    vpmovsxbw zmm24{k5}, [rbx+r11*8+256]
    vpmovsxbw zmm24{k5}, [rbx+r11*8-256]
    vpmovsxbw zmm24{k5}{z}, ymm15
    vpmovsxbw zmm24{k5}{z}, [rbx]
    vpmovsxbw zmm24{k5}{z}, [rbx+r11*8+256]
    vpmovsxbw zmm24{k5}{z}, [rbx+r11*8-256]
    vpmovuswb xmm0, xmm2
    vpmovuswb xmm0, ymm16
    vpmovuswb ymm15, zmm24
    vpmovuswb [rbx], xmm2
    vpmovuswb [rbx], ymm16
    vpmovuswb [rbx], zmm24
    vpmovuswb [rbx+r11*8+256], xmm2
    vpmovuswb [rbx+r11*8+256], ymm16
    vpmovuswb [rbx+r11*8+256], zmm24
    vpmovuswb [rbx+r11*8-256], xmm2
    vpmovuswb [rbx+r11*8-256], ymm16
    vpmovuswb [rbx+r11*8-256], zmm24
    vpmovw2m k3, xmm0
    vpmovw2m k3, ymm15
    vpmovw2m k3, zmm31
    vpmovwb xmm0, xmm2
    vpmovwb xmm0, ymm16
    vpmovwb ymm15, zmm24
    vpmovwb [rbx], xmm2
    vpmovwb [rbx], ymm16
    vpmovwb [rbx], zmm24
    vpmovwb [rbx+r11*8+256], xmm2
    vpmovwb [rbx+r11*8+256], ymm16
    vpmovwb [rbx+r11*8+256], zmm24
    vpmovwb [rbx+r11*8-256], xmm2
    vpmovwb [rbx+r11*8-256], ymm16
    vpmovwb [rbx+r11*8-256], zmm24
    vpmovzxbw xmm2, xmm0
    vpmovzxbw xmm2, [rbx]
    vpmovzxbw xmm2, [rbx+r11*8+256]
    vpmovzxbw xmm2, [rbx+r11*8-256]
    vpmovzxbw xmm2{k5}, xmm0
    vpmovzxbw xmm2{k5}, [rbx]
    vpmovzxbw xmm2{k5}, [rbx+r11*8+256]
    vpmovzxbw xmm2{k5}, [rbx+r11*8-256]
    vpmovzxbw xmm2{k5}{z}, xmm0
    vpmovzxbw xmm2{k5}{z}, [rbx]
    vpmovzxbw xmm2{k5}{z}, [rbx+r11*8+256]
    vpmovzxbw xmm2{k5}{z}, [rbx+r11*8-256]
    vpmovzxbw ymm16, xmm0
    vpmovzxbw ymm16, [rbx]
    vpmovzxbw ymm16, [rbx+r11*8+256]
    vpmovzxbw ymm16, [rbx+r11*8-256]
    vpmovzxbw ymm16{k5}, xmm0
    vpmovzxbw ymm16{k5}, [rbx]
    vpmovzxbw ymm16{k5}, [rbx+r11*8+256]
    vpmovzxbw ymm16{k5}, [rbx+r11*8-256]
    vpmovzxbw ymm16{k5}{z}, xmm0
    vpmovzxbw ymm16{k5}{z}, [rbx]
    vpmovzxbw ymm16{k5}{z}, [rbx+r11*8+256]
    vpmovzxbw ymm16{k5}{z}, [rbx+r11*8-256]
    vpmovzxbw zmm24, ymm15
    vpmovzxbw zmm24, [rbx]
    vpmovzxbw zmm24, [rbx+r11*8+256]
    vpmovzxbw zmm24, [rbx+r11*8-256]
    vpmovzxbw zmm24{k5}, ymm15
    vpmovzxbw zmm24{k5}, [rbx]
    vpmovzxbw zmm24{k5}, [rbx+r11*8+256]
    vpmovzxbw zmm24{k5}, [rbx+r11*8-256]
    vpmovzxbw zmm24{k5}{z}, ymm15
    vpmovzxbw zmm24{k5}{z}, [rbx]
    vpmovzxbw zmm24{k5}{z}, [rbx+r11*8+256]
    vpmovzxbw zmm24{k5}{z}, [rbx+r11*8-256]
    vpmulhrsw xmm2, xmm7, xmm0
    vpmulhrsw xmm2, xmm7, [rbx]
    vpmulhrsw xmm2, xmm7, [rbx+r11*8+256]
    vpmulhrsw xmm2, xmm7, [rbx+r11*8-256]
    vpmulhrsw xmm2{k5}, xmm7, xmm0
    vpmulhrsw xmm2{k5}, xmm7, [rbx]
    vpmulhrsw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpmulhrsw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpmulhrsw xmm2{k5}{z}, xmm7, xmm0
    vpmulhrsw xmm2{k5}{z}, xmm7, [rbx]
    vpmulhrsw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpmulhrsw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpmulhrsw ymm16, ymm13, ymm15
    vpmulhrsw ymm16, ymm13, [rbx]
    vpmulhrsw ymm16, ymm13, [rbx+r11*8+256]
    vpmulhrsw ymm16, ymm13, [rbx+r11*8-256]
    vpmulhrsw ymm16{k5}, ymm13, ymm15
    vpmulhrsw ymm16{k5}, ymm13, [rbx]
    vpmulhrsw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpmulhrsw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpmulhrsw ymm16{k5}{z}, ymm13, ymm15
    vpmulhrsw ymm16{k5}{z}, ymm13, [rbx]
    vpmulhrsw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpmulhrsw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpmulhrsw zmm24, zmm24, zmm31
    vpmulhrsw zmm24, zmm24, [rbx]
    vpmulhrsw zmm24, zmm24, [rbx+r11*8+256]
    vpmulhrsw zmm24, zmm24, [rbx+r11*8-256]
    vpmulhrsw zmm24{k5}, zmm24, zmm31
    vpmulhrsw zmm24{k5}, zmm24, [rbx]
    vpmulhrsw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpmulhrsw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpmulhrsw zmm24{k5}{z}, zmm24, zmm31
    vpmulhrsw zmm24{k5}{z}, zmm24, [rbx]
    vpmulhrsw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpmulhrsw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpmulhuw xmm2, xmm7, xmm0
    vpmulhuw xmm2, xmm7, [rbx]
    vpmulhuw xmm2, xmm7, [rbx+r11*8+256]
    vpmulhuw xmm2, xmm7, [rbx+r11*8-256]
    vpmulhuw xmm2{k5}, xmm7, xmm0
    vpmulhuw xmm2{k5}, xmm7, [rbx]
    vpmulhuw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpmulhuw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpmulhuw xmm2{k5}{z}, xmm7, xmm0
    vpmulhuw xmm2{k5}{z}, xmm7, [rbx]
    vpmulhuw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpmulhuw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpmulhuw ymm16, ymm13, ymm15
    vpmulhuw ymm16, ymm13, [rbx]
    vpmulhuw ymm16, ymm13, [rbx+r11*8+256]
    vpmulhuw ymm16, ymm13, [rbx+r11*8-256]
    vpmulhuw ymm16{k5}, ymm13, ymm15
    vpmulhuw ymm16{k5}, ymm13, [rbx]
    vpmulhuw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpmulhuw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpmulhuw ymm16{k5}{z}, ymm13, ymm15
    vpmulhuw ymm16{k5}{z}, ymm13, [rbx]
    vpmulhuw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpmulhuw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpmulhuw zmm24, zmm24, zmm31
    vpmulhuw zmm24, zmm24, [rbx]
    vpmulhuw zmm24, zmm24, [rbx+r11*8+256]
    vpmulhuw zmm24, zmm24, [rbx+r11*8-256]
    vpmulhuw zmm24{k5}, zmm24, zmm31
    vpmulhuw zmm24{k5}, zmm24, [rbx]
    vpmulhuw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpmulhuw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpmulhuw zmm24{k5}{z}, zmm24, zmm31
    vpmulhuw zmm24{k5}{z}, zmm24, [rbx]
    vpmulhuw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpmulhuw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpmulhw xmm2, xmm7, xmm0
    vpmulhw xmm2, xmm7, [rbx]
    vpmulhw xmm2, xmm7, [rbx+r11*8+256]
    vpmulhw xmm2, xmm7, [rbx+r11*8-256]
    vpmulhw xmm2{k5}, xmm7, xmm0
    vpmulhw xmm2{k5}, xmm7, [rbx]
    vpmulhw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpmulhw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpmulhw xmm2{k5}{z}, xmm7, xmm0
    vpmulhw xmm2{k5}{z}, xmm7, [rbx]
    vpmulhw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpmulhw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpmulhw ymm16, ymm13, ymm15
    vpmulhw ymm16, ymm13, [rbx]
    vpmulhw ymm16, ymm13, [rbx+r11*8+256]
    vpmulhw ymm16, ymm13, [rbx+r11*8-256]
    vpmulhw ymm16{k5}, ymm13, ymm15
    vpmulhw ymm16{k5}, ymm13, [rbx]
    vpmulhw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpmulhw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpmulhw ymm16{k5}{z}, ymm13, ymm15
    vpmulhw ymm16{k5}{z}, ymm13, [rbx]
    vpmulhw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpmulhw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpmulhw zmm24, zmm24, zmm31
    vpmulhw zmm24, zmm24, [rbx]
    vpmulhw zmm24, zmm24, [rbx+r11*8+256]
    vpmulhw zmm24, zmm24, [rbx+r11*8-256]
    vpmulhw zmm24{k5}, zmm24, zmm31
    vpmulhw zmm24{k5}, zmm24, [rbx]
    vpmulhw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpmulhw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpmulhw zmm24{k5}{z}, zmm24, zmm31
    vpmulhw zmm24{k5}{z}, zmm24, [rbx]
    vpmulhw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpmulhw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpmullw xmm2, xmm7, xmm0
    vpmullw xmm2, xmm7, [rbx]
    vpmullw xmm2, xmm7, [rbx+r11*8+256]
    vpmullw xmm2, xmm7, [rbx+r11*8-256]
    vpmullw xmm2{k5}, xmm7, xmm0
    vpmullw xmm2{k5}, xmm7, [rbx]
    vpmullw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpmullw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpmullw xmm2{k5}{z}, xmm7, xmm0
    vpmullw xmm2{k5}{z}, xmm7, [rbx]
    vpmullw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpmullw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpmullw ymm16, ymm13, ymm15
    vpmullw ymm16, ymm13, [rbx]
    vpmullw ymm16, ymm13, [rbx+r11*8+256]
    vpmullw ymm16, ymm13, [rbx+r11*8-256]
    vpmullw ymm16{k5}, ymm13, ymm15
    vpmullw ymm16{k5}, ymm13, [rbx]
    vpmullw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpmullw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpmullw ymm16{k5}{z}, ymm13, ymm15
    vpmullw ymm16{k5}{z}, ymm13, [rbx]
    vpmullw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpmullw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpmullw zmm24, zmm24, zmm31
    vpmullw zmm24, zmm24, [rbx]
    vpmullw zmm24, zmm24, [rbx+r11*8+256]
    vpmullw zmm24, zmm24, [rbx+r11*8-256]
    vpmullw zmm24{k5}, zmm24, zmm31
    vpmullw zmm24{k5}, zmm24, [rbx]
    vpmullw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpmullw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpmullw zmm24{k5}{z}, zmm24, zmm31
    vpmullw zmm24{k5}{z}, zmm24, [rbx]
    vpmullw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpmullw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpsadbw xmm2, xmm7, xmm0
    vpsadbw xmm2, xmm7, [rbx]
    vpsadbw xmm2, xmm7, [rbx+r11*8+256]
    vpsadbw xmm2, xmm7, [rbx+r11*8-256]
    vpsadbw ymm16, ymm13, ymm15
    vpsadbw ymm16, ymm13, [rbx]
    vpsadbw ymm16, ymm13, [rbx+r11*8+256]
    vpsadbw ymm16, ymm13, [rbx+r11*8-256]
    vpsadbw zmm24, zmm24, zmm31
    vpsadbw zmm24, zmm24, [rbx]
    vpsadbw zmm24, zmm24, [rbx+r11*8+256]
    vpsadbw zmm24, zmm24, [rbx+r11*8-256]
    vpshufb xmm2, xmm7, xmm0
    vpshufb xmm2, xmm7, [rbx]
    vpshufb xmm2, xmm7, [rbx+r11*8+256]
    vpshufb xmm2, xmm7, [rbx+r11*8-256]
    vpshufb xmm2{k5}, xmm7, xmm0
    vpshufb xmm2{k5}, xmm7, [rbx]
    vpshufb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpshufb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpshufb xmm2{k5}{z}, xmm7, xmm0
    vpshufb xmm2{k5}{z}, xmm7, [rbx]
    vpshufb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpshufb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpshufb ymm16, ymm13, ymm15
    vpshufb ymm16, ymm13, [rbx]
    vpshufb ymm16, ymm13, [rbx+r11*8+256]
    vpshufb ymm16, ymm13, [rbx+r11*8-256]
    vpshufb ymm16{k5}, ymm13, ymm15
    vpshufb ymm16{k5}, ymm13, [rbx]
    vpshufb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpshufb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpshufb ymm16{k5}{z}, ymm13, ymm15
    vpshufb ymm16{k5}{z}, ymm13, [rbx]
    vpshufb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpshufb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpshufb zmm24, zmm24, zmm31
    vpshufb zmm24, zmm24, [rbx]
    vpshufb zmm24, zmm24, [rbx+r11*8+256]
    vpshufb zmm24, zmm24, [rbx+r11*8-256]
    vpshufb zmm24{k5}, zmm24, zmm31
    vpshufb zmm24{k5}, zmm24, [rbx]
    vpshufb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpshufb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpshufb zmm24{k5}{z}, zmm24, zmm31
    vpshufb zmm24{k5}{z}, zmm24, [rbx]
    vpshufb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpshufb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpshufhw xmm2, xmm0, 10
    vpshufhw xmm2, [rbx], 10
    vpshufhw xmm2, [rbx+r11*8+256], 10
    vpshufhw xmm2, [rbx+r11*8-256], 10
    vpshufhw xmm2{k5}, xmm0, 10
    vpshufhw xmm2{k5}, [rbx], 10
    vpshufhw xmm2{k5}, [rbx+r11*8+256], 10
    vpshufhw xmm2{k5}, [rbx+r11*8-256], 10
    vpshufhw xmm2{k5}{z}, xmm0, 10
    vpshufhw xmm2{k5}{z}, [rbx], 10
    vpshufhw xmm2{k5}{z}, [rbx+r11*8+256], 10
    vpshufhw xmm2{k5}{z}, [rbx+r11*8-256], 10
    vpshufhw ymm16, ymm15, 10
    vpshufhw ymm16, [rbx], 10
    vpshufhw ymm16, [rbx+r11*8+256], 10
    vpshufhw ymm16, [rbx+r11*8-256], 10
    vpshufhw ymm16{k5}, ymm15, 10
    vpshufhw ymm16{k5}, [rbx], 10
    vpshufhw ymm16{k5}, [rbx+r11*8+256], 10
    vpshufhw ymm16{k5}, [rbx+r11*8-256], 10
    vpshufhw ymm16{k5}{z}, ymm15, 10
    vpshufhw ymm16{k5}{z}, [rbx], 10
    vpshufhw ymm16{k5}{z}, [rbx+r11*8+256], 10
    vpshufhw ymm16{k5}{z}, [rbx+r11*8-256], 10
    vpshufhw zmm24, zmm31, 10
    vpshufhw zmm24, [rbx], 10
    vpshufhw zmm24, [rbx+r11*8+256], 10
    vpshufhw zmm24, [rbx+r11*8-256], 10
    vpshufhw zmm24{k5}, zmm31, 10
    vpshufhw zmm24{k5}, [rbx], 10
    vpshufhw zmm24{k5}, [rbx+r11*8+256], 10
    vpshufhw zmm24{k5}, [rbx+r11*8-256], 10
    vpshufhw zmm24{k5}{z}, zmm31, 10
    vpshufhw zmm24{k5}{z}, [rbx], 10
    vpshufhw zmm24{k5}{z}, [rbx+r11*8+256], 10
    vpshufhw zmm24{k5}{z}, [rbx+r11*8-256], 10
    vpshuflw xmm2, xmm0, 10
    vpshuflw xmm2, [rbx], 10
    vpshuflw xmm2, [rbx+r11*8+256], 10
    vpshuflw xmm2, [rbx+r11*8-256], 10
    vpshuflw xmm2{k5}, xmm0, 10
    vpshuflw xmm2{k5}, [rbx], 10
    vpshuflw xmm2{k5}, [rbx+r11*8+256], 10
    vpshuflw xmm2{k5}, [rbx+r11*8-256], 10
    vpshuflw xmm2{k5}{z}, xmm0, 10
    vpshuflw xmm2{k5}{z}, [rbx], 10
    vpshuflw xmm2{k5}{z}, [rbx+r11*8+256], 10
    vpshuflw xmm2{k5}{z}, [rbx+r11*8-256], 10
    vpshuflw ymm16, ymm15, 10
    vpshuflw ymm16, [rbx], 10
    vpshuflw ymm16, [rbx+r11*8+256], 10
    vpshuflw ymm16, [rbx+r11*8-256], 10
    vpshuflw ymm16{k5}, ymm15, 10
    vpshuflw ymm16{k5}, [rbx], 10
    vpshuflw ymm16{k5}, [rbx+r11*8+256], 10
    vpshuflw ymm16{k5}, [rbx+r11*8-256], 10
    vpshuflw ymm16{k5}{z}, ymm15, 10
    vpshuflw ymm16{k5}{z}, [rbx], 10
    vpshuflw ymm16{k5}{z}, [rbx+r11*8+256], 10
    vpshuflw ymm16{k5}{z}, [rbx+r11*8-256], 10
    vpshuflw zmm24, zmm31, 10
    vpshuflw zmm24, [rbx], 10
    vpshuflw zmm24, [rbx+r11*8+256], 10
    vpshuflw zmm24, [rbx+r11*8-256], 10
    vpshuflw zmm24{k5}, zmm31, 10
    vpshuflw zmm24{k5}, [rbx], 10
    vpshuflw zmm24{k5}, [rbx+r11*8+256], 10
    vpshuflw zmm24{k5}, [rbx+r11*8-256], 10
    vpshuflw zmm24{k5}{z}, zmm31, 10
    vpshuflw zmm24{k5}{z}, [rbx], 10
    vpshuflw zmm24{k5}{z}, [rbx+r11*8+256], 10
    vpshuflw zmm24{k5}{z}, [rbx+r11*8-256], 10
    vpslldq xmm7, xmm0, 10
    vpslldq xmm7, [rbx], 10
    vpslldq xmm7, [rbx+r11*8+256], 10
    vpslldq xmm7, [rbx+r11*8-256], 10
    vpslldq ymm13, ymm15, 10
    vpslldq ymm13, [rbx], 10
    vpslldq ymm13, [rbx+r11*8+256], 10
    vpslldq ymm13, [rbx+r11*8-256], 10
    vpslldq zmm24, zmm31, 10
    vpslldq zmm24, [rbx], 10
    vpslldq zmm24, [rbx+r11*8+256], 10
    vpslldq zmm24, [rbx+r11*8-256], 10
    vpsllvw xmm2, xmm7, xmm0
    vpsllvw xmm2, xmm7, [rbx]
    vpsllvw xmm2, xmm7, [rbx+r11*8+256]
    vpsllvw xmm2, xmm7, [rbx+r11*8-256]
    vpsllvw xmm2{k5}, xmm7, xmm0
    vpsllvw xmm2{k5}, xmm7, [rbx]
    vpsllvw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpsllvw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpsllvw xmm2{k5}{z}, xmm7, xmm0
    vpsllvw xmm2{k5}{z}, xmm7, [rbx]
    vpsllvw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpsllvw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpsllvw ymm16, ymm13, ymm15
    vpsllvw ymm16, ymm13, [rbx]
    vpsllvw ymm16, ymm13, [rbx+r11*8+256]
    vpsllvw ymm16, ymm13, [rbx+r11*8-256]
    vpsllvw ymm16{k5}, ymm13, ymm15
    vpsllvw ymm16{k5}, ymm13, [rbx]
    vpsllvw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpsllvw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpsllvw ymm16{k5}{z}, ymm13, ymm15
    vpsllvw ymm16{k5}{z}, ymm13, [rbx]
    vpsllvw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpsllvw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpsllvw zmm24, zmm24, zmm31
    vpsllvw zmm24, zmm24, [rbx]
    vpsllvw zmm24, zmm24, [rbx+r11*8+256]
    vpsllvw zmm24, zmm24, [rbx+r11*8-256]
    vpsllvw zmm24{k5}, zmm24, zmm31
    vpsllvw zmm24{k5}, zmm24, [rbx]
    vpsllvw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpsllvw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpsllvw zmm24{k5}{z}, zmm24, zmm31
    vpsllvw zmm24{k5}{z}, zmm24, [rbx]
    vpsllvw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpsllvw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpsllw xmm7, xmm0, 10
    vpsllw xmm7, [rbx], 10
    vpsllw xmm7, [rbx+r11*8+256], 10
    vpsllw xmm7, [rbx+r11*8-256], 10
    vpsllw ymm13, ymm15, 10
    vpsllw ymm13, [rbx], 10
    vpsllw ymm13, [rbx+r11*8+256], 10
    vpsllw ymm13, [rbx+r11*8-256], 10
    vpsllw zmm24, zmm31, 10
    vpsllw zmm24, [rbx], 10
    vpsllw zmm24, [rbx+r11*8+256], 10
    vpsllw zmm24, [rbx+r11*8-256], 10
    vpsllw xmm2, xmm7, xmm0
    vpsllw xmm2, xmm7, [rbx]
    vpsllw xmm2, xmm7, [rbx+r11*8+256]
    vpsllw xmm2, xmm7, [rbx+r11*8-256]
    vpsllw xmm2{k5}, xmm7, xmm0
    vpsllw xmm2{k5}, xmm7, [rbx]
    vpsllw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpsllw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpsllw xmm2{k5}{z}, xmm7, xmm0
    vpsllw xmm2{k5}{z}, xmm7, [rbx]
    vpsllw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpsllw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpsllw ymm16, ymm13, xmm0
    vpsllw ymm16, ymm13, [rbx]
    vpsllw ymm16, ymm13, [rbx+r11*8+256]
    vpsllw ymm16, ymm13, [rbx+r11*8-256]
    vpsllw ymm16{k5}, ymm13, xmm0
    vpsllw ymm16{k5}, ymm13, [rbx]
    vpsllw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpsllw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpsllw ymm16{k5}{z}, ymm13, xmm0
    vpsllw ymm16{k5}{z}, ymm13, [rbx]
    vpsllw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpsllw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpsllw zmm24, zmm24, xmm0
    vpsllw zmm24, zmm24, [rbx]
    vpsllw zmm24, zmm24, [rbx+r11*8+256]
    vpsllw zmm24, zmm24, [rbx+r11*8-256]
    vpsllw zmm24{k5}, zmm24, xmm0
    vpsllw zmm24{k5}, zmm24, [rbx]
    vpsllw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpsllw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpsllw zmm24{k5}{z}, zmm24, xmm0
    vpsllw zmm24{k5}{z}, zmm24, [rbx]
    vpsllw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpsllw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpsravw xmm2, xmm7, xmm0
    vpsravw xmm2, xmm7, [rbx]
    vpsravw xmm2, xmm7, [rbx+r11*8+256]
    vpsravw xmm2, xmm7, [rbx+r11*8-256]
    vpsravw xmm2{k5}, xmm7, xmm0
    vpsravw xmm2{k5}, xmm7, [rbx]
    vpsravw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpsravw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpsravw xmm2{k5}{z}, xmm7, xmm0
    vpsravw xmm2{k5}{z}, xmm7, [rbx]
    vpsravw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpsravw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpsravw ymm16, ymm13, ymm15
    vpsravw ymm16, ymm13, [rbx]
    vpsravw ymm16, ymm13, [rbx+r11*8+256]
    vpsravw ymm16, ymm13, [rbx+r11*8-256]
    vpsravw ymm16{k5}, ymm13, ymm15
    vpsravw ymm16{k5}, ymm13, [rbx]
    vpsravw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpsravw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpsravw ymm16{k5}{z}, ymm13, ymm15
    vpsravw ymm16{k5}{z}, ymm13, [rbx]
    vpsravw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpsravw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpsravw zmm24, zmm24, zmm31
    vpsravw zmm24, zmm24, [rbx]
    vpsravw zmm24, zmm24, [rbx+r11*8+256]
    vpsravw zmm24, zmm24, [rbx+r11*8-256]
    vpsravw zmm24{k5}, zmm24, zmm31
    vpsravw zmm24{k5}, zmm24, [rbx]
    vpsravw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpsravw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpsravw zmm24{k5}{z}, zmm24, zmm31
    vpsravw zmm24{k5}{z}, zmm24, [rbx]
    vpsravw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpsravw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpsraw xmm7, xmm0, 10
    vpsraw xmm7, [rbx], 10
    vpsraw xmm7, [rbx+r11*8+256], 10
    vpsraw xmm7, [rbx+r11*8-256], 10
    vpsraw ymm13, ymm15, 10
    vpsraw ymm13, [rbx], 10
    vpsraw ymm13, [rbx+r11*8+256], 10
    vpsraw ymm13, [rbx+r11*8-256], 10
    vpsraw zmm24, zmm31, 10
    vpsraw zmm24, [rbx], 10
    vpsraw zmm24, [rbx+r11*8+256], 10
    vpsraw zmm24, [rbx+r11*8-256], 10
    vpsraw xmm2, xmm7, xmm0
    vpsraw xmm2, xmm7, [rbx]
    vpsraw xmm2, xmm7, [rbx+r11*8+256]
    vpsraw xmm2, xmm7, [rbx+r11*8-256]
    vpsraw xmm2{k5}, xmm7, xmm0
    vpsraw xmm2{k5}, xmm7, [rbx]
    vpsraw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpsraw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpsraw xmm2{k5}{z}, xmm7, xmm0
    vpsraw xmm2{k5}{z}, xmm7, [rbx]
    vpsraw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpsraw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpsraw ymm16, ymm13, xmm0
    vpsraw ymm16, ymm13, [rbx]
    vpsraw ymm16, ymm13, [rbx+r11*8+256]
    vpsraw ymm16, ymm13, [rbx+r11*8-256]
    vpsraw ymm16{k5}, ymm13, xmm0
    vpsraw ymm16{k5}, ymm13, [rbx]
    vpsraw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpsraw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpsraw ymm16{k5}{z}, ymm13, xmm0
    vpsraw ymm16{k5}{z}, ymm13, [rbx]
    vpsraw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpsraw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpsraw zmm24, zmm24, xmm0
    vpsraw zmm24, zmm24, [rbx]
    vpsraw zmm24, zmm24, [rbx+r11*8+256]
    vpsraw zmm24, zmm24, [rbx+r11*8-256]
    vpsraw zmm24{k5}, zmm24, xmm0
    vpsraw zmm24{k5}, zmm24, [rbx]
    vpsraw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpsraw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpsraw zmm24{k5}{z}, zmm24, xmm0
    vpsraw zmm24{k5}{z}, zmm24, [rbx]
    vpsraw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpsraw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpsrldq xmm7, xmm0, 10
    vpsrldq xmm7, [rbx], 10
    vpsrldq xmm7, [rbx+r11*8+256], 10
    vpsrldq xmm7, [rbx+r11*8-256], 10
    vpsrldq ymm13, ymm15, 10
    vpsrldq ymm13, [rbx], 10
    vpsrldq ymm13, [rbx+r11*8+256], 10
    vpsrldq ymm13, [rbx+r11*8-256], 10
    vpsrldq zmm24, zmm31, 10
    vpsrldq zmm24, [rbx], 10
    vpsrldq zmm24, [rbx+r11*8+256], 10
    vpsrldq zmm24, [rbx+r11*8-256], 10
    vpsrlvw xmm2, xmm7, xmm0
    vpsrlvw xmm2, xmm7, [rbx]
    vpsrlvw xmm2, xmm7, [rbx+r11*8+256]
    vpsrlvw xmm2, xmm7, [rbx+r11*8-256]
    vpsrlvw xmm2{k5}, xmm7, xmm0
    vpsrlvw xmm2{k5}, xmm7, [rbx]
    vpsrlvw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpsrlvw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpsrlvw xmm2{k5}{z}, xmm7, xmm0
    vpsrlvw xmm2{k5}{z}, xmm7, [rbx]
    vpsrlvw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpsrlvw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpsrlvw ymm16, ymm13, ymm15
    vpsrlvw ymm16, ymm13, [rbx]
    vpsrlvw ymm16, ymm13, [rbx+r11*8+256]
    vpsrlvw ymm16, ymm13, [rbx+r11*8-256]
    vpsrlvw ymm16{k5}, ymm13, ymm15
    vpsrlvw ymm16{k5}, ymm13, [rbx]
    vpsrlvw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpsrlvw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpsrlvw ymm16{k5}{z}, ymm13, ymm15
    vpsrlvw ymm16{k5}{z}, ymm13, [rbx]
    vpsrlvw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpsrlvw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpsrlvw zmm24, zmm24, zmm31
    vpsrlvw zmm24, zmm24, [rbx]
    vpsrlvw zmm24, zmm24, [rbx+r11*8+256]
    vpsrlvw zmm24, zmm24, [rbx+r11*8-256]
    vpsrlvw zmm24{k5}, zmm24, zmm31
    vpsrlvw zmm24{k5}, zmm24, [rbx]
    vpsrlvw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpsrlvw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpsrlvw zmm24{k5}{z}, zmm24, zmm31
    vpsrlvw zmm24{k5}{z}, zmm24, [rbx]
    vpsrlvw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpsrlvw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpsrlw xmm7, xmm0, 10
    vpsrlw xmm7, [rbx], 10
    vpsrlw xmm7, [rbx+r11*8+256], 10
    vpsrlw xmm7, [rbx+r11*8-256], 10
    vpsrlw ymm13, ymm15, 10
    vpsrlw ymm13, [rbx], 10
    vpsrlw ymm13, [rbx+r11*8+256], 10
    vpsrlw ymm13, [rbx+r11*8-256], 10
    vpsrlw zmm24, zmm31, 10
    vpsrlw zmm24, [rbx], 10
    vpsrlw zmm24, [rbx+r11*8+256], 10
    vpsrlw zmm24, [rbx+r11*8-256], 10
    vpsrlw xmm2, xmm7, xmm0
    vpsrlw xmm2, xmm7, [rbx]
    vpsrlw xmm2, xmm7, [rbx+r11*8+256]
    vpsrlw xmm2, xmm7, [rbx+r11*8-256]
    vpsrlw xmm2{k5}, xmm7, xmm0
    vpsrlw xmm2{k5}, xmm7, [rbx]
    vpsrlw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpsrlw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpsrlw xmm2{k5}{z}, xmm7, xmm0
    vpsrlw xmm2{k5}{z}, xmm7, [rbx]
    vpsrlw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpsrlw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpsrlw ymm16, ymm13, xmm0
    vpsrlw ymm16, ymm13, [rbx]
    vpsrlw ymm16, ymm13, [rbx+r11*8+256]
    vpsrlw ymm16, ymm13, [rbx+r11*8-256]
    vpsrlw ymm16{k5}, ymm13, xmm0
    vpsrlw ymm16{k5}, ymm13, [rbx]
    vpsrlw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpsrlw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpsrlw ymm16{k5}{z}, ymm13, xmm0
    vpsrlw ymm16{k5}{z}, ymm13, [rbx]
    vpsrlw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpsrlw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpsrlw zmm24, zmm24, xmm0
    vpsrlw zmm24, zmm24, [rbx]
    vpsrlw zmm24, zmm24, [rbx+r11*8+256]
    vpsrlw zmm24, zmm24, [rbx+r11*8-256]
    vpsrlw zmm24{k5}, zmm24, xmm0
    vpsrlw zmm24{k5}, zmm24, [rbx]
    vpsrlw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpsrlw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpsrlw zmm24{k5}{z}, zmm24, xmm0
    vpsrlw zmm24{k5}{z}, zmm24, [rbx]
    vpsrlw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpsrlw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpsubb xmm2, xmm7, xmm0
    vpsubb xmm2, xmm7, [rbx]
    vpsubb xmm2, xmm7, [rbx+r11*8+256]
    vpsubb xmm2, xmm7, [rbx+r11*8-256]
    vpsubb xmm2{k5}, xmm7, xmm0
    vpsubb xmm2{k5}, xmm7, [rbx]
    vpsubb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpsubb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpsubb xmm2{k5}{z}, xmm7, xmm0
    vpsubb xmm2{k5}{z}, xmm7, [rbx]
    vpsubb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpsubb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpsubb ymm16, ymm13, ymm15
    vpsubb ymm16, ymm13, [rbx]
    vpsubb ymm16, ymm13, [rbx+r11*8+256]
    vpsubb ymm16, ymm13, [rbx+r11*8-256]
    vpsubb ymm16{k5}, ymm13, ymm15
    vpsubb ymm16{k5}, ymm13, [rbx]
    vpsubb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpsubb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpsubb ymm16{k5}{z}, ymm13, ymm15
    vpsubb ymm16{k5}{z}, ymm13, [rbx]
    vpsubb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpsubb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpsubb zmm24, zmm24, zmm31
    vpsubb zmm24, zmm24, [rbx]
    vpsubb zmm24, zmm24, [rbx+r11*8+256]
    vpsubb zmm24, zmm24, [rbx+r11*8-256]
    vpsubb zmm24{k5}, zmm24, zmm31
    vpsubb zmm24{k5}, zmm24, [rbx]
    vpsubb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpsubb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpsubb zmm24{k5}{z}, zmm24, zmm31
    vpsubb zmm24{k5}{z}, zmm24, [rbx]
    vpsubb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpsubb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpsubsb xmm2, xmm7, xmm0
    vpsubsb xmm2, xmm7, [rbx]
    vpsubsb xmm2, xmm7, [rbx+r11*8+256]
    vpsubsb xmm2, xmm7, [rbx+r11*8-256]
    vpsubsb xmm2{k5}, xmm7, xmm0
    vpsubsb xmm2{k5}, xmm7, [rbx]
    vpsubsb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpsubsb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpsubsb xmm2{k5}{z}, xmm7, xmm0
    vpsubsb xmm2{k5}{z}, xmm7, [rbx]
    vpsubsb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpsubsb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpsubsb ymm16, ymm13, ymm15
    vpsubsb ymm16, ymm13, [rbx]
    vpsubsb ymm16, ymm13, [rbx+r11*8+256]
    vpsubsb ymm16, ymm13, [rbx+r11*8-256]
    vpsubsb ymm16{k5}, ymm13, ymm15
    vpsubsb ymm16{k5}, ymm13, [rbx]
    vpsubsb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpsubsb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpsubsb ymm16{k5}{z}, ymm13, ymm15
    vpsubsb ymm16{k5}{z}, ymm13, [rbx]
    vpsubsb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpsubsb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpsubsb zmm24, zmm24, zmm31
    vpsubsb zmm24, zmm24, [rbx]
    vpsubsb zmm24, zmm24, [rbx+r11*8+256]
    vpsubsb zmm24, zmm24, [rbx+r11*8-256]
    vpsubsb zmm24{k5}, zmm24, zmm31
    vpsubsb zmm24{k5}, zmm24, [rbx]
    vpsubsb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpsubsb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpsubsb zmm24{k5}{z}, zmm24, zmm31
    vpsubsb zmm24{k5}{z}, zmm24, [rbx]
    vpsubsb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpsubsb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpsubsw xmm2, xmm7, xmm0
    vpsubsw xmm2, xmm7, [rbx]
    vpsubsw xmm2, xmm7, [rbx+r11*8+256]
    vpsubsw xmm2, xmm7, [rbx+r11*8-256]
    vpsubsw xmm2{k5}, xmm7, xmm0
    vpsubsw xmm2{k5}, xmm7, [rbx]
    vpsubsw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpsubsw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpsubsw xmm2{k5}{z}, xmm7, xmm0
    vpsubsw xmm2{k5}{z}, xmm7, [rbx]
    vpsubsw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpsubsw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpsubsw ymm16, ymm13, ymm15
    vpsubsw ymm16, ymm13, [rbx]
    vpsubsw ymm16, ymm13, [rbx+r11*8+256]
    vpsubsw ymm16, ymm13, [rbx+r11*8-256]
    vpsubsw ymm16{k5}, ymm13, ymm15
    vpsubsw ymm16{k5}, ymm13, [rbx]
    vpsubsw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpsubsw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpsubsw ymm16{k5}{z}, ymm13, ymm15
    vpsubsw ymm16{k5}{z}, ymm13, [rbx]
    vpsubsw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpsubsw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpsubsw zmm24, zmm24, zmm31
    vpsubsw zmm24, zmm24, [rbx]
    vpsubsw zmm24, zmm24, [rbx+r11*8+256]
    vpsubsw zmm24, zmm24, [rbx+r11*8-256]
    vpsubsw zmm24{k5}, zmm24, zmm31
    vpsubsw zmm24{k5}, zmm24, [rbx]
    vpsubsw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpsubsw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpsubsw zmm24{k5}{z}, zmm24, zmm31
    vpsubsw zmm24{k5}{z}, zmm24, [rbx]
    vpsubsw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpsubsw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpsubusb xmm2, xmm7, xmm0
    vpsubusb xmm2, xmm7, [rbx]
    vpsubusb xmm2, xmm7, [rbx+r11*8+256]
    vpsubusb xmm2, xmm7, [rbx+r11*8-256]
    vpsubusb xmm2{k5}, xmm7, xmm0
    vpsubusb xmm2{k5}, xmm7, [rbx]
    vpsubusb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpsubusb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpsubusb xmm2{k5}{z}, xmm7, xmm0
    vpsubusb xmm2{k5}{z}, xmm7, [rbx]
    vpsubusb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpsubusb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpsubusb ymm16, ymm13, ymm15
    vpsubusb ymm16, ymm13, [rbx]
    vpsubusb ymm16, ymm13, [rbx+r11*8+256]
    vpsubusb ymm16, ymm13, [rbx+r11*8-256]
    vpsubusb ymm16{k5}, ymm13, ymm15
    vpsubusb ymm16{k5}, ymm13, [rbx]
    vpsubusb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpsubusb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpsubusb ymm16{k5}{z}, ymm13, ymm15
    vpsubusb ymm16{k5}{z}, ymm13, [rbx]
    vpsubusb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpsubusb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpsubusb zmm24, zmm24, zmm31
    vpsubusb zmm24, zmm24, [rbx]
    vpsubusb zmm24, zmm24, [rbx+r11*8+256]
    vpsubusb zmm24, zmm24, [rbx+r11*8-256]
    vpsubusb zmm24{k5}, zmm24, zmm31
    vpsubusb zmm24{k5}, zmm24, [rbx]
    vpsubusb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpsubusb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpsubusb zmm24{k5}{z}, zmm24, zmm31
    vpsubusb zmm24{k5}{z}, zmm24, [rbx]
    vpsubusb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpsubusb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpsubusw xmm2, xmm7, xmm0
    vpsubusw xmm2, xmm7, [rbx]
    vpsubusw xmm2, xmm7, [rbx+r11*8+256]
    vpsubusw xmm2, xmm7, [rbx+r11*8-256]
    vpsubusw xmm2{k5}, xmm7, xmm0
    vpsubusw xmm2{k5}, xmm7, [rbx]
    vpsubusw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpsubusw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpsubusw xmm2{k5}{z}, xmm7, xmm0
    vpsubusw xmm2{k5}{z}, xmm7, [rbx]
    vpsubusw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpsubusw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpsubusw ymm16, ymm13, ymm15
    vpsubusw ymm16, ymm13, [rbx]
    vpsubusw ymm16, ymm13, [rbx+r11*8+256]
    vpsubusw ymm16, ymm13, [rbx+r11*8-256]
    vpsubusw ymm16{k5}, ymm13, ymm15
    vpsubusw ymm16{k5}, ymm13, [rbx]
    vpsubusw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpsubusw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpsubusw ymm16{k5}{z}, ymm13, ymm15
    vpsubusw ymm16{k5}{z}, ymm13, [rbx]
    vpsubusw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpsubusw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpsubusw zmm24, zmm24, zmm31
    vpsubusw zmm24, zmm24, [rbx]
    vpsubusw zmm24, zmm24, [rbx+r11*8+256]
    vpsubusw zmm24, zmm24, [rbx+r11*8-256]
    vpsubusw zmm24{k5}, zmm24, zmm31
    vpsubusw zmm24{k5}, zmm24, [rbx]
    vpsubusw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpsubusw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpsubusw zmm24{k5}{z}, zmm24, zmm31
    vpsubusw zmm24{k5}{z}, zmm24, [rbx]
    vpsubusw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpsubusw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpsubw xmm2, xmm7, xmm0
    vpsubw xmm2, xmm7, [rbx]
    vpsubw xmm2, xmm7, [rbx+r11*8+256]
    vpsubw xmm2, xmm7, [rbx+r11*8-256]
    vpsubw xmm2{k5}, xmm7, xmm0
    vpsubw xmm2{k5}, xmm7, [rbx]
    vpsubw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpsubw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpsubw xmm2{k5}{z}, xmm7, xmm0
    vpsubw xmm2{k5}{z}, xmm7, [rbx]
    vpsubw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpsubw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpsubw ymm16, ymm13, ymm15
    vpsubw ymm16, ymm13, [rbx]
    vpsubw ymm16, ymm13, [rbx+r11*8+256]
    vpsubw ymm16, ymm13, [rbx+r11*8-256]
    vpsubw ymm16{k5}, ymm13, ymm15
    vpsubw ymm16{k5}, ymm13, [rbx]
    vpsubw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpsubw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpsubw ymm16{k5}{z}, ymm13, ymm15
    vpsubw ymm16{k5}{z}, ymm13, [rbx]
    vpsubw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpsubw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpsubw zmm24, zmm24, zmm31
    vpsubw zmm24, zmm24, [rbx]
    vpsubw zmm24, zmm24, [rbx+r11*8+256]
    vpsubw zmm24, zmm24, [rbx+r11*8-256]
    vpsubw zmm24{k5}, zmm24, zmm31
    vpsubw zmm24{k5}, zmm24, [rbx]
    vpsubw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpsubw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpsubw zmm24{k5}{z}, zmm24, zmm31
    vpsubw zmm24{k5}{z}, zmm24, [rbx]
    vpsubw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpsubw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vptestmb k3, xmm7, xmm0
    vptestmb k3, xmm7, [rbx]
    vptestmb k3, xmm7, [rbx+r11*8+256]
    vptestmb k3, xmm7, [rbx+r11*8-256]
    vptestmb k3, ymm13, ymm15
    vptestmb k3, ymm13, [rbx]
    vptestmb k3, ymm13, [rbx+r11*8+256]
    vptestmb k3, ymm13, [rbx+r11*8-256]
    vptestmb k3, zmm24, zmm31
    vptestmb k3, zmm24, [rbx]
    vptestmb k3, zmm24, [rbx+r11*8+256]
    vptestmb k3, zmm24, [rbx+r11*8-256]
    vptestmb k3{k5}, xmm7, xmm0
    vptestmb k3{k5}, xmm7, [rbx]
    vptestmb k3{k5}, xmm7, [rbx+r11*8+256]
    vptestmb k3{k5}, xmm7, [rbx+r11*8-256]
    vptestmb k3{k5}, ymm13, ymm15
    vptestmb k3{k5}, ymm13, [rbx]
    vptestmb k3{k5}, ymm13, [rbx+r11*8+256]
    vptestmb k3{k5}, ymm13, [rbx+r11*8-256]
    vptestmb k3{k5}, zmm24, zmm31
    vptestmb k3{k5}, zmm24, [rbx]
    vptestmb k3{k5}, zmm24, [rbx+r11*8+256]
    vptestmb k3{k5}, zmm24, [rbx+r11*8-256]
    vptestmw k3, xmm7, xmm0
    vptestmw k3, xmm7, [rbx]
    vptestmw k3, xmm7, [rbx+r11*8+256]
    vptestmw k3, xmm7, [rbx+r11*8-256]
    vptestmw k3, ymm13, ymm15
    vptestmw k3, ymm13, [rbx]
    vptestmw k3, ymm13, [rbx+r11*8+256]
    vptestmw k3, ymm13, [rbx+r11*8-256]
    vptestmw k3, zmm24, zmm31
    vptestmw k3, zmm24, [rbx]
    vptestmw k3, zmm24, [rbx+r11*8+256]
    vptestmw k3, zmm24, [rbx+r11*8-256]
    vptestmw k3{k5}, xmm7, xmm0
    vptestmw k3{k5}, xmm7, [rbx]
    vptestmw k3{k5}, xmm7, [rbx+r11*8+256]
    vptestmw k3{k5}, xmm7, [rbx+r11*8-256]
    vptestmw k3{k5}, ymm13, ymm15
    vptestmw k3{k5}, ymm13, [rbx]
    vptestmw k3{k5}, ymm13, [rbx+r11*8+256]
    vptestmw k3{k5}, ymm13, [rbx+r11*8-256]
    vptestmw k3{k5}, zmm24, zmm31
    vptestmw k3{k5}, zmm24, [rbx]
    vptestmw k3{k5}, zmm24, [rbx+r11*8+256]
    vptestmw k3{k5}, zmm24, [rbx+r11*8-256]
    vptestnmb k3, xmm7, xmm0
    vptestnmb k3, xmm7, [rbx]
    vptestnmb k3, xmm7, [rbx+r11*8+256]
    vptestnmb k3, xmm7, [rbx+r11*8-256]
    vptestnmb k3, ymm13, ymm15
    vptestnmb k3, ymm13, [rbx]
    vptestnmb k3, ymm13, [rbx+r11*8+256]
    vptestnmb k3, ymm13, [rbx+r11*8-256]
    vptestnmb k3, zmm24, zmm31
    vptestnmb k3, zmm24, [rbx]
    vptestnmb k3, zmm24, [rbx+r11*8+256]
    vptestnmb k3, zmm24, [rbx+r11*8-256]
    vptestnmb k3{k5}, xmm7, xmm0
    vptestnmb k3{k5}, xmm7, [rbx]
    vptestnmb k3{k5}, xmm7, [rbx+r11*8+256]
    vptestnmb k3{k5}, xmm7, [rbx+r11*8-256]
    vptestnmb k3{k5}, ymm13, ymm15
    vptestnmb k3{k5}, ymm13, [rbx]
    vptestnmb k3{k5}, ymm13, [rbx+r11*8+256]
    vptestnmb k3{k5}, ymm13, [rbx+r11*8-256]
    vptestnmb k3{k5}, zmm24, zmm31
    vptestnmb k3{k5}, zmm24, [rbx]
    vptestnmb k3{k5}, zmm24, [rbx+r11*8+256]
    vptestnmb k3{k5}, zmm24, [rbx+r11*8-256]
    vptestnmw k3, xmm7, xmm0
    vptestnmw k3, xmm7, [rbx]
    vptestnmw k3, xmm7, [rbx+r11*8+256]
    vptestnmw k3, xmm7, [rbx+r11*8-256]
    vptestnmw k3, ymm13, ymm15
    vptestnmw k3, ymm13, [rbx]
    vptestnmw k3, ymm13, [rbx+r11*8+256]
    vptestnmw k3, ymm13, [rbx+r11*8-256]
    vptestnmw k3, zmm24, zmm31
    vptestnmw k3, zmm24, [rbx]
    vptestnmw k3, zmm24, [rbx+r11*8+256]
    vptestnmw k3, zmm24, [rbx+r11*8-256]
    vptestnmw k3{k5}, xmm7, xmm0
    vptestnmw k3{k5}, xmm7, [rbx]
    vptestnmw k3{k5}, xmm7, [rbx+r11*8+256]
    vptestnmw k3{k5}, xmm7, [rbx+r11*8-256]
    vptestnmw k3{k5}, ymm13, ymm15
    vptestnmw k3{k5}, ymm13, [rbx]
    vptestnmw k3{k5}, ymm13, [rbx+r11*8+256]
    vptestnmw k3{k5}, ymm13, [rbx+r11*8-256]
    vptestnmw k3{k5}, zmm24, zmm31
    vptestnmw k3{k5}, zmm24, [rbx]
    vptestnmw k3{k5}, zmm24, [rbx+r11*8+256]
    vptestnmw k3{k5}, zmm24, [rbx+r11*8-256]
    vpunpckhbw xmm2, xmm7, xmm0
    vpunpckhbw xmm2, xmm7, [rbx]
    vpunpckhbw xmm2, xmm7, [rbx+r11*8+256]
    vpunpckhbw xmm2, xmm7, [rbx+r11*8-256]
    vpunpckhbw xmm2{k5}, xmm7, xmm0
    vpunpckhbw xmm2{k5}, xmm7, [rbx]
    vpunpckhbw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpunpckhbw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpunpckhbw xmm2{k5}{z}, xmm7, xmm0
    vpunpckhbw xmm2{k5}{z}, xmm7, [rbx]
    vpunpckhbw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpunpckhbw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpunpckhbw ymm16, ymm13, ymm15
    vpunpckhbw ymm16, ymm13, [rbx]
    vpunpckhbw ymm16, ymm13, [rbx+r11*8+256]
    vpunpckhbw ymm16, ymm13, [rbx+r11*8-256]
    vpunpckhbw ymm16{k5}, ymm13, ymm15
    vpunpckhbw ymm16{k5}, ymm13, [rbx]
    vpunpckhbw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpunpckhbw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpunpckhbw ymm16{k5}{z}, ymm13, ymm15
    vpunpckhbw ymm16{k5}{z}, ymm13, [rbx]
    vpunpckhbw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpunpckhbw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpunpckhbw zmm24, zmm24, zmm31
    vpunpckhbw zmm24, zmm24, [rbx]
    vpunpckhbw zmm24, zmm24, [rbx+r11*8+256]
    vpunpckhbw zmm24, zmm24, [rbx+r11*8-256]
    vpunpckhbw zmm24{k5}, zmm24, zmm31
    vpunpckhbw zmm24{k5}, zmm24, [rbx]
    vpunpckhbw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpunpckhbw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpunpckhbw zmm24{k5}{z}, zmm24, zmm31
    vpunpckhbw zmm24{k5}{z}, zmm24, [rbx]
    vpunpckhbw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpunpckhbw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpunpckhwd xmm2, xmm7, xmm0
    vpunpckhwd xmm2, xmm7, [rbx]
    vpunpckhwd xmm2, xmm7, [rbx+r11*8+256]
    vpunpckhwd xmm2, xmm7, [rbx+r11*8-256]
    vpunpckhwd xmm2{k5}, xmm7, xmm0
    vpunpckhwd xmm2{k5}, xmm7, [rbx]
    vpunpckhwd xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpunpckhwd xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpunpckhwd xmm2{k5}{z}, xmm7, xmm0
    vpunpckhwd xmm2{k5}{z}, xmm7, [rbx]
    vpunpckhwd xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpunpckhwd xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpunpckhwd ymm16, ymm13, ymm15
    vpunpckhwd ymm16, ymm13, [rbx]
    vpunpckhwd ymm16, ymm13, [rbx+r11*8+256]
    vpunpckhwd ymm16, ymm13, [rbx+r11*8-256]
    vpunpckhwd ymm16{k5}, ymm13, ymm15
    vpunpckhwd ymm16{k5}, ymm13, [rbx]
    vpunpckhwd ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpunpckhwd ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpunpckhwd ymm16{k5}{z}, ymm13, ymm15
    vpunpckhwd ymm16{k5}{z}, ymm13, [rbx]
    vpunpckhwd ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpunpckhwd ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpunpckhwd zmm24, zmm24, zmm31
    vpunpckhwd zmm24, zmm24, [rbx]
    vpunpckhwd zmm24, zmm24, [rbx+r11*8+256]
    vpunpckhwd zmm24, zmm24, [rbx+r11*8-256]
    vpunpckhwd zmm24{k5}, zmm24, zmm31
    vpunpckhwd zmm24{k5}, zmm24, [rbx]
    vpunpckhwd zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpunpckhwd zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpunpckhwd zmm24{k5}{z}, zmm24, zmm31
    vpunpckhwd zmm24{k5}{z}, zmm24, [rbx]
    vpunpckhwd zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpunpckhwd zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpunpcklbw xmm2, xmm7, xmm0
    vpunpcklbw xmm2, xmm7, [rbx]
    vpunpcklbw xmm2, xmm7, [rbx+r11*8+256]
    vpunpcklbw xmm2, xmm7, [rbx+r11*8-256]
    vpunpcklbw xmm2{k5}, xmm7, xmm0
    vpunpcklbw xmm2{k5}, xmm7, [rbx]
    vpunpcklbw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpunpcklbw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpunpcklbw xmm2{k5}{z}, xmm7, xmm0
    vpunpcklbw xmm2{k5}{z}, xmm7, [rbx]
    vpunpcklbw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpunpcklbw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpunpcklbw ymm16, ymm13, ymm15
    vpunpcklbw ymm16, ymm13, [rbx]
    vpunpcklbw ymm16, ymm13, [rbx+r11*8+256]
    vpunpcklbw ymm16, ymm13, [rbx+r11*8-256]
    vpunpcklbw ymm16{k5}, ymm13, ymm15
    vpunpcklbw ymm16{k5}, ymm13, [rbx]
    vpunpcklbw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpunpcklbw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpunpcklbw ymm16{k5}{z}, ymm13, ymm15
    vpunpcklbw ymm16{k5}{z}, ymm13, [rbx]
    vpunpcklbw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpunpcklbw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpunpcklbw zmm24, zmm24, zmm31
    vpunpcklbw zmm24, zmm24, [rbx]
    vpunpcklbw zmm24, zmm24, [rbx+r11*8+256]
    vpunpcklbw zmm24, zmm24, [rbx+r11*8-256]
    vpunpcklbw zmm24{k5}, zmm24, zmm31
    vpunpcklbw zmm24{k5}, zmm24, [rbx]
    vpunpcklbw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpunpcklbw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpunpcklbw zmm24{k5}{z}, zmm24, zmm31
    vpunpcklbw zmm24{k5}{z}, zmm24, [rbx]
    vpunpcklbw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpunpcklbw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpunpcklwd xmm2, xmm7, xmm0
    vpunpcklwd xmm2, xmm7, [rbx]
    vpunpcklwd xmm2, xmm7, [rbx+r11*8+256]
    vpunpcklwd xmm2, xmm7, [rbx+r11*8-256]
    vpunpcklwd xmm2{k5}, xmm7, xmm0
    vpunpcklwd xmm2{k5}, xmm7, [rbx]
    vpunpcklwd xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpunpcklwd xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpunpcklwd xmm2{k5}{z}, xmm7, xmm0
    vpunpcklwd xmm2{k5}{z}, xmm7, [rbx]
    vpunpcklwd xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpunpcklwd xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpunpcklwd ymm16, ymm13, ymm15
    vpunpcklwd ymm16, ymm13, [rbx]
    vpunpcklwd ymm16, ymm13, [rbx+r11*8+256]
    vpunpcklwd ymm16, ymm13, [rbx+r11*8-256]
    vpunpcklwd ymm16{k5}, ymm13, ymm15
    vpunpcklwd ymm16{k5}, ymm13, [rbx]
    vpunpcklwd ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpunpcklwd ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpunpcklwd ymm16{k5}{z}, ymm13, ymm15
    vpunpcklwd ymm16{k5}{z}, ymm13, [rbx]
    vpunpcklwd ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpunpcklwd ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpunpcklwd zmm24, zmm24, zmm31
    vpunpcklwd zmm24, zmm24, [rbx]
    vpunpcklwd zmm24, zmm24, [rbx+r11*8+256]
    vpunpcklwd zmm24, zmm24, [rbx+r11*8-256]
    vpunpcklwd zmm24{k5}, zmm24, zmm31
    vpunpcklwd zmm24{k5}, zmm24, [rbx]
    vpunpcklwd zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpunpcklwd zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpunpcklwd zmm24{k5}{z}, zmm24, zmm31
    vpunpcklwd zmm24{k5}{z}, zmm24, [rbx]
    vpunpcklwd zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpunpcklwd zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
