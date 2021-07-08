    bits 64
    
    kaddb k3, k7, k1
    kaddw k3, k7, k1
    kandb k3, k7, k1
    kandnb k3, k7, k1
    kmovb k3, [rbx]
    kmovb k3, [rbx+rsi*8+256]
    kmovb k3, [rbx+rsi*8-256]
    kmovb k3, k1
    kmovb k3, [rbx]
    kmovb [rbx], k3
    kmovb [rbx+rsi*8+256], k3
    kmovb [rbx+rsi*8-256], k3
    kmovb k3, ecx
    kmovb ecx, k1
    knotb k3, k1
    korb k3, k7, k1
    kortestb k3, k1
    kshiftlb k3, k1, 10
    kshiftrb k3, k1, 10
    ktestb k3, k1
    ktestw k3, k1
    kxnorb k3, k7, k1
    kxorb k3, k7, k1
    vandnpd xmm2, xmm7, xmm0
    vandnpd xmm2, xmm7, [rbx]
    vandnpd xmm2, xmm7, [rbx]{1to2}
    vandnpd xmm2, xmm7, [rbx+r11*8+256]
    vandnpd xmm2, xmm7, [rbx+r11*8-256]
    vandnpd xmm2{k5}, xmm7, xmm0
    vandnpd xmm2{k5}, xmm7, [rbx]
    vandnpd xmm2{k5}, xmm7, [rbx]{1to2}
    vandnpd xmm2{k5}, xmm7, [rbx+r11*8+256]
    vandnpd xmm2{k5}, xmm7, [rbx+r11*8-256]
    vandnpd xmm2{k5}{z}, xmm7, xmm0
    vandnpd xmm2{k5}{z}, xmm7, [rbx]
    vandnpd xmm2{k5}{z}, xmm7, [rbx]{1to2}
    vandnpd xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vandnpd xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vandnpd ymm16, ymm13, ymm15
    vandnpd ymm16, ymm13, [rbx]
    vandnpd ymm16, ymm13, [rbx]{1to4}
    vandnpd ymm16, ymm13, [rbx+r11*8+256]
    vandnpd ymm16, ymm13, [rbx+r11*8-256]
    vandnpd ymm16{k5}, ymm13, ymm15
    vandnpd ymm16{k5}, ymm13, [rbx]
    vandnpd ymm16{k5}, ymm13, [rbx]{1to4}
    vandnpd ymm16{k5}, ymm13, [rbx+r11*8+256]
    vandnpd ymm16{k5}, ymm13, [rbx+r11*8-256]
    vandnpd ymm16{k5}{z}, ymm13, ymm15
    vandnpd ymm16{k5}{z}, ymm13, [rbx]
    vandnpd ymm16{k5}{z}, ymm13, [rbx]{1to4}
    vandnpd ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vandnpd ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vandnpd zmm24, zmm24, zmm31
    vandnpd zmm24, zmm24, [rbx]
    vandnpd zmm24, zmm24, [rbx]{1to8}
    vandnpd zmm24, zmm24, [rbx+r11*8+256]
    vandnpd zmm24, zmm24, [rbx+r11*8-256]
    vandnpd zmm24{k5}, zmm24, zmm31
    vandnpd zmm24{k5}, zmm24, [rbx]
    vandnpd zmm24{k5}, zmm24, [rbx]{1to8}
    vandnpd zmm24{k5}, zmm24, [rbx+r11*8+256]
    vandnpd zmm24{k5}, zmm24, [rbx+r11*8-256]
    vandnpd zmm24{k5}{z}, zmm24, zmm31
    vandnpd zmm24{k5}{z}, zmm24, [rbx]
    vandnpd zmm24{k5}{z}, zmm24, [rbx]{1to8}
    vandnpd zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vandnpd zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vandnps xmm2, xmm7, xmm0
    vandnps xmm2, xmm7, [rbx]
    vandnps xmm2, xmm7, [rbx]{1to4}
    vandnps xmm2, xmm7, [rbx+r11*8+256]
    vandnps xmm2, xmm7, [rbx+r11*8-256]
    vandnps xmm2{k5}, xmm7, xmm0
    vandnps xmm2{k5}, xmm7, [rbx]
    vandnps xmm2{k5}, xmm7, [rbx]{1to4}
    vandnps xmm2{k5}, xmm7, [rbx+r11*8+256]
    vandnps xmm2{k5}, xmm7, [rbx+r11*8-256]
    vandnps xmm2{k5}{z}, xmm7, xmm0
    vandnps xmm2{k5}{z}, xmm7, [rbx]
    vandnps xmm2{k5}{z}, xmm7, [rbx]{1to4}
    vandnps xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vandnps xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vandnps ymm16, ymm13, ymm15
    vandnps ymm16, ymm13, [rbx]
    vandnps ymm16, ymm13, [rbx]{1to8}
    vandnps ymm16, ymm13, [rbx+r11*8+256]
    vandnps ymm16, ymm13, [rbx+r11*8-256]
    vandnps ymm16{k5}, ymm13, ymm15
    vandnps ymm16{k5}, ymm13, [rbx]
    vandnps ymm16{k5}, ymm13, [rbx]{1to8}
    vandnps ymm16{k5}, ymm13, [rbx+r11*8+256]
    vandnps ymm16{k5}, ymm13, [rbx+r11*8-256]
    vandnps ymm16{k5}{z}, ymm13, ymm15
    vandnps ymm16{k5}{z}, ymm13, [rbx]
    vandnps ymm16{k5}{z}, ymm13, [rbx]{1to8}
    vandnps ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vandnps ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vandnps zmm24, zmm24, zmm31
    vandnps zmm24, zmm24, [rbx]
    vandnps zmm24, zmm24, [rbx]{1to16}
    vandnps zmm24, zmm24, [rbx+r11*8+256]
    vandnps zmm24, zmm24, [rbx+r11*8-256]
    vandnps zmm24{k5}, zmm24, zmm31
    vandnps zmm24{k5}, zmm24, [rbx]
    vandnps zmm24{k5}, zmm24, [rbx]{1to16}
    vandnps zmm24{k5}, zmm24, [rbx+r11*8+256]
    vandnps zmm24{k5}, zmm24, [rbx+r11*8-256]
    vandnps zmm24{k5}{z}, zmm24, zmm31
    vandnps zmm24{k5}{z}, zmm24, [rbx]
    vandnps zmm24{k5}{z}, zmm24, [rbx]{1to16}
    vandnps zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vandnps zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vandpd xmm2, xmm7, xmm0
    vandpd xmm2, xmm7, [rbx]
    vandpd xmm2, xmm7, [rbx]{1to2}
    vandpd xmm2, xmm7, [rbx+r11*8+256]
    vandpd xmm2, xmm7, [rbx+r11*8-256]
    vandpd xmm2{k5}, xmm7, xmm0
    vandpd xmm2{k5}, xmm7, [rbx]
    vandpd xmm2{k5}, xmm7, [rbx]{1to2}
    vandpd xmm2{k5}, xmm7, [rbx+r11*8+256]
    vandpd xmm2{k5}, xmm7, [rbx+r11*8-256]
    vandpd xmm2{k5}{z}, xmm7, xmm0
    vandpd xmm2{k5}{z}, xmm7, [rbx]
    vandpd xmm2{k5}{z}, xmm7, [rbx]{1to2}
    vandpd xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vandpd xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vandpd ymm16, ymm13, ymm15
    vandpd ymm16, ymm13, [rbx]
    vandpd ymm16, ymm13, [rbx]{1to4}
    vandpd ymm16, ymm13, [rbx+r11*8+256]
    vandpd ymm16, ymm13, [rbx+r11*8-256]
    vandpd ymm16{k5}, ymm13, ymm15
    vandpd ymm16{k5}, ymm13, [rbx]
    vandpd ymm16{k5}, ymm13, [rbx]{1to4}
    vandpd ymm16{k5}, ymm13, [rbx+r11*8+256]
    vandpd ymm16{k5}, ymm13, [rbx+r11*8-256]
    vandpd ymm16{k5}{z}, ymm13, ymm15
    vandpd ymm16{k5}{z}, ymm13, [rbx]
    vandpd ymm16{k5}{z}, ymm13, [rbx]{1to4}
    vandpd ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vandpd ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vandpd zmm24, zmm24, zmm31
    vandpd zmm24, zmm24, [rbx]
    vandpd zmm24, zmm24, [rbx]{1to8}
    vandpd zmm24, zmm24, [rbx+r11*8+256]
    vandpd zmm24, zmm24, [rbx+r11*8-256]
    vandpd zmm24{k5}, zmm24, zmm31
    vandpd zmm24{k5}, zmm24, [rbx]
    vandpd zmm24{k5}, zmm24, [rbx]{1to8}
    vandpd zmm24{k5}, zmm24, [rbx+r11*8+256]
    vandpd zmm24{k5}, zmm24, [rbx+r11*8-256]
    vandpd zmm24{k5}{z}, zmm24, zmm31
    vandpd zmm24{k5}{z}, zmm24, [rbx]
    vandpd zmm24{k5}{z}, zmm24, [rbx]{1to8}
    vandpd zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vandpd zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vandps xmm2, xmm7, xmm0
    vandps xmm2, xmm7, [rbx]
    vandps xmm2, xmm7, [rbx]{1to4}
    vandps xmm2, xmm7, [rbx+r11*8+256]
    vandps xmm2, xmm7, [rbx+r11*8-256]
    vandps xmm2{k5}, xmm7, xmm0
    vandps xmm2{k5}, xmm7, [rbx]
    vandps xmm2{k5}, xmm7, [rbx]{1to4}
    vandps xmm2{k5}, xmm7, [rbx+r11*8+256]
    vandps xmm2{k5}, xmm7, [rbx+r11*8-256]
    vandps xmm2{k5}{z}, xmm7, xmm0
    vandps xmm2{k5}{z}, xmm7, [rbx]
    vandps xmm2{k5}{z}, xmm7, [rbx]{1to4}
    vandps xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vandps xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vandps ymm16, ymm13, ymm15
    vandps ymm16, ymm13, [rbx]
    vandps ymm16, ymm13, [rbx]{1to8}
    vandps ymm16, ymm13, [rbx+r11*8+256]
    vandps ymm16, ymm13, [rbx+r11*8-256]
    vandps ymm16{k5}, ymm13, ymm15
    vandps ymm16{k5}, ymm13, [rbx]
    vandps ymm16{k5}, ymm13, [rbx]{1to8}
    vandps ymm16{k5}, ymm13, [rbx+r11*8+256]
    vandps ymm16{k5}, ymm13, [rbx+r11*8-256]
    vandps ymm16{k5}{z}, ymm13, ymm15
    vandps ymm16{k5}{z}, ymm13, [rbx]
    vandps ymm16{k5}{z}, ymm13, [rbx]{1to8}
    vandps ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vandps ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vandps zmm24, zmm24, zmm31
    vandps zmm24, zmm24, [rbx]
    vandps zmm24, zmm24, [rbx]{1to16}
    vandps zmm24, zmm24, [rbx+r11*8+256]
    vandps zmm24, zmm24, [rbx+r11*8-256]
    vandps zmm24{k5}, zmm24, zmm31
    vandps zmm24{k5}, zmm24, [rbx]
    vandps zmm24{k5}, zmm24, [rbx]{1to16}
    vandps zmm24{k5}, zmm24, [rbx+r11*8+256]
    vandps zmm24{k5}, zmm24, [rbx+r11*8-256]
    vandps zmm24{k5}{z}, zmm24, zmm31
    vandps zmm24{k5}{z}, zmm24, [rbx]
    vandps zmm24{k5}{z}, zmm24, [rbx]{1to16}
    vandps zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vandps zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vbroadcastf32x2 ymm16, xmm0
    vbroadcastf32x2 ymm16, [rbx]
    vbroadcastf32x2 ymm16, [rbx+r11*8+256]
    vbroadcastf32x2 ymm16, [rbx+r11*8-256]
    vbroadcastf32x2 ymm16{k5}, xmm0
    vbroadcastf32x2 ymm16{k5}, [rbx]
    vbroadcastf32x2 ymm16{k5}, [rbx+r11*8+256]
    vbroadcastf32x2 ymm16{k5}, [rbx+r11*8-256]
    vbroadcastf32x2 ymm16{k5}{z}, xmm0
    vbroadcastf32x2 ymm16{k5}{z}, [rbx]
    vbroadcastf32x2 ymm16{k5}{z}, [rbx+r11*8+256]
    vbroadcastf32x2 ymm16{k5}{z}, [rbx+r11*8-256]
    vbroadcastf32x2 zmm24, xmm0
    vbroadcastf32x2 zmm24, [rbx]
    vbroadcastf32x2 zmm24, [rbx+r11*8+256]
    vbroadcastf32x2 zmm24, [rbx+r11*8-256]
    vbroadcastf32x2 zmm24{k5}, xmm0
    vbroadcastf32x2 zmm24{k5}, [rbx]
    vbroadcastf32x2 zmm24{k5}, [rbx+r11*8+256]
    vbroadcastf32x2 zmm24{k5}, [rbx+r11*8-256]
    vbroadcastf32x2 zmm24{k5}{z}, xmm0
    vbroadcastf32x2 zmm24{k5}{z}, [rbx]
    vbroadcastf32x2 zmm24{k5}{z}, [rbx+r11*8+256]
    vbroadcastf32x2 zmm24{k5}{z}, [rbx+r11*8-256]
    vbroadcastf32x8 zmm24, [rbx]
    vbroadcastf32x8 zmm24, [rbx+rsi*8+256]
    vbroadcastf32x8 zmm24, [rbx+rsi*8-256]
    vbroadcastf32x8 zmm24{k5}, [rbx]
    vbroadcastf32x8 zmm24{k5}, [rbx+rsi*8+256]
    vbroadcastf32x8 zmm24{k5}, [rbx+rsi*8-256]
    vbroadcastf32x8 zmm24{k5}{z}, [rbx]
    vbroadcastf32x8 zmm24{k5}{z}, [rbx+rsi*8+256]
    vbroadcastf32x8 zmm24{k5}{z}, [rbx+rsi*8-256]
    vbroadcastf64x2 ymm16, [rbx]
    vbroadcastf64x2 ymm16, [rbx+rsi*8+256]
    vbroadcastf64x2 ymm16, [rbx+rsi*8-256]
    vbroadcastf64x2 ymm16{k5}, [rbx]
    vbroadcastf64x2 ymm16{k5}, [rbx+rsi*8+256]
    vbroadcastf64x2 ymm16{k5}, [rbx+rsi*8-256]
    vbroadcastf64x2 ymm16{k5}{z}, [rbx]
    vbroadcastf64x2 ymm16{k5}{z}, [rbx+rsi*8+256]
    vbroadcastf64x2 ymm16{k5}{z}, [rbx+rsi*8-256]
    vbroadcastf64x2 zmm24, [rbx]
    vbroadcastf64x2 zmm24, [rbx+rsi*8+256]
    vbroadcastf64x2 zmm24, [rbx+rsi*8-256]
    vbroadcastf64x2 zmm24{k5}, [rbx]
    vbroadcastf64x2 zmm24{k5}, [rbx+rsi*8+256]
    vbroadcastf64x2 zmm24{k5}, [rbx+rsi*8-256]
    vbroadcastf64x2 zmm24{k5}{z}, [rbx]
    vbroadcastf64x2 zmm24{k5}{z}, [rbx+rsi*8+256]
    vbroadcastf64x2 zmm24{k5}{z}, [rbx+rsi*8-256]
    vbroadcasti32x2 xmm2, xmm0
    vbroadcasti32x2 xmm2, [rbx]
    vbroadcasti32x2 xmm2, [rbx+r11*8+256]
    vbroadcasti32x2 xmm2, [rbx+r11*8-256]
    vbroadcasti32x2 xmm2{k5}, xmm0
    vbroadcasti32x2 xmm2{k5}, [rbx]
    vbroadcasti32x2 xmm2{k5}, [rbx+r11*8+256]
    vbroadcasti32x2 xmm2{k5}, [rbx+r11*8-256]
    vbroadcasti32x2 xmm2{k5}{z}, xmm0
    vbroadcasti32x2 xmm2{k5}{z}, [rbx]
    vbroadcasti32x2 xmm2{k5}{z}, [rbx+r11*8+256]
    vbroadcasti32x2 xmm2{k5}{z}, [rbx+r11*8-256]
    vbroadcasti32x2 ymm16, xmm0
    vbroadcasti32x2 ymm16, [rbx]
    vbroadcasti32x2 ymm16, [rbx+r11*8+256]
    vbroadcasti32x2 ymm16, [rbx+r11*8-256]
    vbroadcasti32x2 ymm16{k5}, xmm0
    vbroadcasti32x2 ymm16{k5}, [rbx]
    vbroadcasti32x2 ymm16{k5}, [rbx+r11*8+256]
    vbroadcasti32x2 ymm16{k5}, [rbx+r11*8-256]
    vbroadcasti32x2 ymm16{k5}{z}, xmm0
    vbroadcasti32x2 ymm16{k5}{z}, [rbx]
    vbroadcasti32x2 ymm16{k5}{z}, [rbx+r11*8+256]
    vbroadcasti32x2 ymm16{k5}{z}, [rbx+r11*8-256]
    vbroadcasti32x2 zmm24, xmm0
    vbroadcasti32x2 zmm24, [rbx]
    vbroadcasti32x2 zmm24, [rbx+r11*8+256]
    vbroadcasti32x2 zmm24, [rbx+r11*8-256]
    vbroadcasti32x2 zmm24{k5}, xmm0
    vbroadcasti32x2 zmm24{k5}, [rbx]
    vbroadcasti32x2 zmm24{k5}, [rbx+r11*8+256]
    vbroadcasti32x2 zmm24{k5}, [rbx+r11*8-256]
    vbroadcasti32x2 zmm24{k5}{z}, xmm0
    vbroadcasti32x2 zmm24{k5}{z}, [rbx]
    vbroadcasti32x2 zmm24{k5}{z}, [rbx+r11*8+256]
    vbroadcasti32x2 zmm24{k5}{z}, [rbx+r11*8-256]
    vbroadcasti32x8 zmm24, [rbx]
    vbroadcasti32x8 zmm24, [rbx+rsi*8+256]
    vbroadcasti32x8 zmm24, [rbx+rsi*8-256]
    vbroadcasti32x8 zmm24{k5}, [rbx]
    vbroadcasti32x8 zmm24{k5}, [rbx+rsi*8+256]
    vbroadcasti32x8 zmm24{k5}, [rbx+rsi*8-256]
    vbroadcasti32x8 zmm24{k5}{z}, [rbx]
    vbroadcasti32x8 zmm24{k5}{z}, [rbx+rsi*8+256]
    vbroadcasti32x8 zmm24{k5}{z}, [rbx+rsi*8-256]
    vbroadcasti64x2 ymm16, [rbx]
    vbroadcasti64x2 ymm16, [rbx+rsi*8+256]
    vbroadcasti64x2 ymm16, [rbx+rsi*8-256]
    vbroadcasti64x2 ymm16{k5}, [rbx]
    vbroadcasti64x2 ymm16{k5}, [rbx+rsi*8+256]
    vbroadcasti64x2 ymm16{k5}, [rbx+rsi*8-256]
    vbroadcasti64x2 ymm16{k5}{z}, [rbx]
    vbroadcasti64x2 ymm16{k5}{z}, [rbx+rsi*8+256]
    vbroadcasti64x2 ymm16{k5}{z}, [rbx+rsi*8-256]
    vbroadcasti64x2 zmm24, [rbx]
    vbroadcasti64x2 zmm24, [rbx+rsi*8+256]
    vbroadcasti64x2 zmm24, [rbx+rsi*8-256]
    vbroadcasti64x2 zmm24{k5}, [rbx]
    vbroadcasti64x2 zmm24{k5}, [rbx+rsi*8+256]
    vbroadcasti64x2 zmm24{k5}, [rbx+rsi*8-256]
    vbroadcasti64x2 zmm24{k5}{z}, [rbx]
    vbroadcasti64x2 zmm24{k5}{z}, [rbx+rsi*8+256]
    vbroadcasti64x2 zmm24{k5}{z}, [rbx+rsi*8-256]
    vcvtpd2qq xmm2, xmm0
    vcvtpd2qq xmm2, [rbx]
    vcvtpd2qq xmm2, [rbx]{1to2}
    vcvtpd2qq xmm2, [rbx+r11*8+256]
    vcvtpd2qq xmm2, [rbx+r11*8-256]
    vcvtpd2qq xmm2{k5}, xmm0
    vcvtpd2qq xmm2{k5}, [rbx]
    vcvtpd2qq xmm2{k5}, [rbx]{1to2}
    vcvtpd2qq xmm2{k5}, [rbx+r11*8+256]
    vcvtpd2qq xmm2{k5}, [rbx+r11*8-256]
    vcvtpd2qq xmm2{k5}{z}, xmm0
    vcvtpd2qq xmm2{k5}{z}, [rbx]
    vcvtpd2qq xmm2{k5}{z}, [rbx]{1to2}
    vcvtpd2qq xmm2{k5}{z}, [rbx+r11*8+256]
    vcvtpd2qq xmm2{k5}{z}, [rbx+r11*8-256]
    vcvtpd2qq ymm16, ymm15
    vcvtpd2qq ymm16, [rbx]
    vcvtpd2qq ymm16, [rbx]{1to4}
    vcvtpd2qq ymm16, [rbx+r11*8+256]
    vcvtpd2qq ymm16, [rbx+r11*8-256]
    vcvtpd2qq ymm16{k5}, ymm15
    vcvtpd2qq ymm16{k5}, [rbx]
    vcvtpd2qq ymm16{k5}, [rbx]{1to4}
    vcvtpd2qq ymm16{k5}, [rbx+r11*8+256]
    vcvtpd2qq ymm16{k5}, [rbx+r11*8-256]
    vcvtpd2qq ymm16{k5}{z}, ymm15
    vcvtpd2qq ymm16{k5}{z}, [rbx]
    vcvtpd2qq ymm16{k5}{z}, [rbx]{1to4}
    vcvtpd2qq ymm16{k5}{z}, [rbx+r11*8+256]
    vcvtpd2qq ymm16{k5}{z}, [rbx+r11*8-256]
    vcvtpd2qq zmm24, zmm31
    vcvtpd2qq zmm24, zmm31, {rd-sae}
    vcvtpd2qq zmm24, [rbx]
    vcvtpd2qq zmm24, [rbx]{1to8}
    vcvtpd2qq zmm24, [rbx+r11*8+256]
    vcvtpd2qq zmm24, [rbx+r11*8-256]
    vcvtpd2qq zmm24{k5}, zmm31
    vcvtpd2qq zmm24{k5}, zmm31, {rd-sae}
    vcvtpd2qq zmm24{k5}, [rbx]
    vcvtpd2qq zmm24{k5}, [rbx]{1to8}
    vcvtpd2qq zmm24{k5}, [rbx+r11*8+256]
    vcvtpd2qq zmm24{k5}, [rbx+r11*8-256]
    vcvtpd2qq zmm24{k5}{z}, zmm31
    vcvtpd2qq zmm24{k5}{z}, zmm31, {rd-sae}
    vcvtpd2qq zmm24{k5}{z}, [rbx]
    vcvtpd2qq zmm24{k5}{z}, [rbx]{1to8}
    vcvtpd2qq zmm24{k5}{z}, [rbx+r11*8+256]
    vcvtpd2qq zmm24{k5}{z}, [rbx+r11*8-256]
    vcvtpd2uqq xmm2, xmm0
    vcvtpd2uqq xmm2, [rbx]
    vcvtpd2uqq xmm2, [rbx]{1to2}
    vcvtpd2uqq xmm2, [rbx+r11*8+256]
    vcvtpd2uqq xmm2, [rbx+r11*8-256]
    vcvtpd2uqq xmm2{k5}, xmm0
    vcvtpd2uqq xmm2{k5}, [rbx]
    vcvtpd2uqq xmm2{k5}, [rbx]{1to2}
    vcvtpd2uqq xmm2{k5}, [rbx+r11*8+256]
    vcvtpd2uqq xmm2{k5}, [rbx+r11*8-256]
    vcvtpd2uqq xmm2{k5}{z}, xmm0
    vcvtpd2uqq xmm2{k5}{z}, [rbx]
    vcvtpd2uqq xmm2{k5}{z}, [rbx]{1to2}
    vcvtpd2uqq xmm2{k5}{z}, [rbx+r11*8+256]
    vcvtpd2uqq xmm2{k5}{z}, [rbx+r11*8-256]
    vcvtpd2uqq ymm16, ymm15
    vcvtpd2uqq ymm16, [rbx]
    vcvtpd2uqq ymm16, [rbx]{1to4}
    vcvtpd2uqq ymm16, [rbx+r11*8+256]
    vcvtpd2uqq ymm16, [rbx+r11*8-256]
    vcvtpd2uqq ymm16{k5}, ymm15
    vcvtpd2uqq ymm16{k5}, [rbx]
    vcvtpd2uqq ymm16{k5}, [rbx]{1to4}
    vcvtpd2uqq ymm16{k5}, [rbx+r11*8+256]
    vcvtpd2uqq ymm16{k5}, [rbx+r11*8-256]
    vcvtpd2uqq ymm16{k5}{z}, ymm15
    vcvtpd2uqq ymm16{k5}{z}, [rbx]
    vcvtpd2uqq ymm16{k5}{z}, [rbx]{1to4}
    vcvtpd2uqq ymm16{k5}{z}, [rbx+r11*8+256]
    vcvtpd2uqq ymm16{k5}{z}, [rbx+r11*8-256]
    vcvtpd2uqq zmm24, zmm31
    vcvtpd2uqq zmm24, zmm31, {rd-sae}
    vcvtpd2uqq zmm24, [rbx]
    vcvtpd2uqq zmm24, [rbx]{1to8}
    vcvtpd2uqq zmm24, [rbx+r11*8+256]
    vcvtpd2uqq zmm24, [rbx+r11*8-256]
    vcvtpd2uqq zmm24{k5}, zmm31
    vcvtpd2uqq zmm24{k5}, zmm31, {rd-sae}
    vcvtpd2uqq zmm24{k5}, [rbx]
    vcvtpd2uqq zmm24{k5}, [rbx]{1to8}
    vcvtpd2uqq zmm24{k5}, [rbx+r11*8+256]
    vcvtpd2uqq zmm24{k5}, [rbx+r11*8-256]
    vcvtpd2uqq zmm24{k5}{z}, zmm31
    vcvtpd2uqq zmm24{k5}{z}, zmm31, {rd-sae}
    vcvtpd2uqq zmm24{k5}{z}, [rbx]
    vcvtpd2uqq zmm24{k5}{z}, [rbx]{1to8}
    vcvtpd2uqq zmm24{k5}{z}, [rbx+r11*8+256]
    vcvtpd2uqq zmm24{k5}{z}, [rbx+r11*8-256]
    vcvtps2qq xmm2, xmm0
    vcvtps2qq xmm2, [rbx]
    vcvtps2qq xmm2, [rbx]{1to2}
    vcvtps2qq xmm2, [rbx+r11*8+256]
    vcvtps2qq xmm2, [rbx+r11*8-256]
    vcvtps2qq xmm2{k5}, xmm0
    vcvtps2qq xmm2{k5}, [rbx]
    vcvtps2qq xmm2{k5}, [rbx]{1to2}
    vcvtps2qq xmm2{k5}, [rbx+r11*8+256]
    vcvtps2qq xmm2{k5}, [rbx+r11*8-256]
    vcvtps2qq xmm2{k5}{z}, xmm0
    vcvtps2qq xmm2{k5}{z}, [rbx]
    vcvtps2qq xmm2{k5}{z}, [rbx]{1to2}
    vcvtps2qq xmm2{k5}{z}, [rbx+r11*8+256]
    vcvtps2qq xmm2{k5}{z}, [rbx+r11*8-256]
    vcvtps2qq ymm16, xmm0
    vcvtps2qq ymm16, [rbx]
    vcvtps2qq ymm16, [rbx]{1to4}
    vcvtps2qq ymm16, [rbx+r11*8+256]
    vcvtps2qq ymm16, [rbx+r11*8-256]
    vcvtps2qq ymm16{k5}, xmm0
    vcvtps2qq ymm16{k5}, [rbx]
    vcvtps2qq ymm16{k5}, [rbx]{1to4}
    vcvtps2qq ymm16{k5}, [rbx+r11*8+256]
    vcvtps2qq ymm16{k5}, [rbx+r11*8-256]
    vcvtps2qq ymm16{k5}{z}, xmm0
    vcvtps2qq ymm16{k5}{z}, [rbx]
    vcvtps2qq ymm16{k5}{z}, [rbx]{1to4}
    vcvtps2qq ymm16{k5}{z}, [rbx+r11*8+256]
    vcvtps2qq ymm16{k5}{z}, [rbx+r11*8-256]
    vcvtps2qq zmm24, ymm15
    vcvtps2qq zmm24, ymm15, {rd-sae}
    vcvtps2qq zmm24, [rbx]
    vcvtps2qq zmm24, [rbx]{1to8}
    vcvtps2qq zmm24, [rbx+r11*8+256]
    vcvtps2qq zmm24, [rbx+r11*8-256]
    vcvtps2qq zmm24{k5}, ymm15
    vcvtps2qq zmm24{k5}, ymm15, {rd-sae}
    vcvtps2qq zmm24{k5}, [rbx]
    vcvtps2qq zmm24{k5}, [rbx]{1to8}
    vcvtps2qq zmm24{k5}, [rbx+r11*8+256]
    vcvtps2qq zmm24{k5}, [rbx+r11*8-256]
    vcvtps2qq zmm24{k5}{z}, ymm15
    vcvtps2qq zmm24{k5}{z}, ymm15, {rd-sae}
    vcvtps2qq zmm24{k5}{z}, [rbx]
    vcvtps2qq zmm24{k5}{z}, [rbx]{1to8}
    vcvtps2qq zmm24{k5}{z}, [rbx+r11*8+256]
    vcvtps2qq zmm24{k5}{z}, [rbx+r11*8-256]
    vcvtps2uqq xmm2, xmm0
    vcvtps2uqq xmm2, [rbx]
    vcvtps2uqq xmm2, [rbx]{1to2}
    vcvtps2uqq xmm2, [rbx+r11*8+256]
    vcvtps2uqq xmm2, [rbx+r11*8-256]
    vcvtps2uqq xmm2{k5}, xmm0
    vcvtps2uqq xmm2{k5}, [rbx]
    vcvtps2uqq xmm2{k5}, [rbx]{1to2}
    vcvtps2uqq xmm2{k5}, [rbx+r11*8+256]
    vcvtps2uqq xmm2{k5}, [rbx+r11*8-256]
    vcvtps2uqq xmm2{k5}{z}, xmm0
    vcvtps2uqq xmm2{k5}{z}, [rbx]
    vcvtps2uqq xmm2{k5}{z}, [rbx]{1to2}
    vcvtps2uqq xmm2{k5}{z}, [rbx+r11*8+256]
    vcvtps2uqq xmm2{k5}{z}, [rbx+r11*8-256]
    vcvtps2uqq ymm16, xmm0
    vcvtps2uqq ymm16, [rbx]
    vcvtps2uqq ymm16, [rbx]{1to4}
    vcvtps2uqq ymm16, [rbx+r11*8+256]
    vcvtps2uqq ymm16, [rbx+r11*8-256]
    vcvtps2uqq ymm16{k5}, xmm0
    vcvtps2uqq ymm16{k5}, [rbx]
    vcvtps2uqq ymm16{k5}, [rbx]{1to4}
    vcvtps2uqq ymm16{k5}, [rbx+r11*8+256]
    vcvtps2uqq ymm16{k5}, [rbx+r11*8-256]
    vcvtps2uqq ymm16{k5}{z}, xmm0
    vcvtps2uqq ymm16{k5}{z}, [rbx]
    vcvtps2uqq ymm16{k5}{z}, [rbx]{1to4}
    vcvtps2uqq ymm16{k5}{z}, [rbx+r11*8+256]
    vcvtps2uqq ymm16{k5}{z}, [rbx+r11*8-256]
    vcvtps2uqq zmm24, ymm15
    vcvtps2uqq zmm24, ymm15, {rd-sae}
    vcvtps2uqq zmm24, [rbx]
    vcvtps2uqq zmm24, [rbx]{1to8}
    vcvtps2uqq zmm24, [rbx+r11*8+256]
    vcvtps2uqq zmm24, [rbx+r11*8-256]
    vcvtps2uqq zmm24{k5}, ymm15
    vcvtps2uqq zmm24{k5}, ymm15, {rd-sae}
    vcvtps2uqq zmm24{k5}, [rbx]
    vcvtps2uqq zmm24{k5}, [rbx]{1to8}
    vcvtps2uqq zmm24{k5}, [rbx+r11*8+256]
    vcvtps2uqq zmm24{k5}, [rbx+r11*8-256]
    vcvtps2uqq zmm24{k5}{z}, ymm15
    vcvtps2uqq zmm24{k5}{z}, ymm15, {rd-sae}
    vcvtps2uqq zmm24{k5}{z}, [rbx]
    vcvtps2uqq zmm24{k5}{z}, [rbx]{1to8}
    vcvtps2uqq zmm24{k5}{z}, [rbx+r11*8+256]
    vcvtps2uqq zmm24{k5}{z}, [rbx+r11*8-256]
    vcvtqq2pd xmm2, xmm0
    vcvtqq2pd xmm2, [rbx]
    vcvtqq2pd xmm2, [rbx]{1to2}
    vcvtqq2pd xmm2, [rbx+r11*8+256]
    vcvtqq2pd xmm2, [rbx+r11*8-256]
    vcvtqq2pd xmm2{k5}, xmm0
    vcvtqq2pd xmm2{k5}, [rbx]
    vcvtqq2pd xmm2{k5}, [rbx]{1to2}
    vcvtqq2pd xmm2{k5}, [rbx+r11*8+256]
    vcvtqq2pd xmm2{k5}, [rbx+r11*8-256]
    vcvtqq2pd xmm2{k5}{z}, xmm0
    vcvtqq2pd xmm2{k5}{z}, [rbx]
    vcvtqq2pd xmm2{k5}{z}, [rbx]{1to2}
    vcvtqq2pd xmm2{k5}{z}, [rbx+r11*8+256]
    vcvtqq2pd xmm2{k5}{z}, [rbx+r11*8-256]
    vcvtqq2pd ymm16, ymm15
    vcvtqq2pd ymm16, [rbx]
    vcvtqq2pd ymm16, [rbx]{1to4}
    vcvtqq2pd ymm16, [rbx+r11*8+256]
    vcvtqq2pd ymm16, [rbx+r11*8-256]
    vcvtqq2pd ymm16{k5}, ymm15
    vcvtqq2pd ymm16{k5}, [rbx]
    vcvtqq2pd ymm16{k5}, [rbx]{1to4}
    vcvtqq2pd ymm16{k5}, [rbx+r11*8+256]
    vcvtqq2pd ymm16{k5}, [rbx+r11*8-256]
    vcvtqq2pd ymm16{k5}{z}, ymm15
    vcvtqq2pd ymm16{k5}{z}, [rbx]
    vcvtqq2pd ymm16{k5}{z}, [rbx]{1to4}
    vcvtqq2pd ymm16{k5}{z}, [rbx+r11*8+256]
    vcvtqq2pd ymm16{k5}{z}, [rbx+r11*8-256]
    vcvtqq2pd zmm24, zmm31
    vcvtqq2pd zmm24, zmm31, {rd-sae}
    vcvtqq2pd zmm24, [rbx]
    vcvtqq2pd zmm24, [rbx]{1to8}
    vcvtqq2pd zmm24, [rbx+r11*8+256]
    vcvtqq2pd zmm24, [rbx+r11*8-256]
    vcvtqq2pd zmm24{k5}, zmm31
    vcvtqq2pd zmm24{k5}, zmm31, {rd-sae}
    vcvtqq2pd zmm24{k5}, [rbx]
    vcvtqq2pd zmm24{k5}, [rbx]{1to8}
    vcvtqq2pd zmm24{k5}, [rbx+r11*8+256]
    vcvtqq2pd zmm24{k5}, [rbx+r11*8-256]
    vcvtqq2pd zmm24{k5}{z}, zmm31
    vcvtqq2pd zmm24{k5}{z}, zmm31, {rd-sae}
    vcvtqq2pd zmm24{k5}{z}, [rbx]
    vcvtqq2pd zmm24{k5}{z}, [rbx]{1to8}
    vcvtqq2pd zmm24{k5}{z}, [rbx+r11*8+256]
    vcvtqq2pd zmm24{k5}{z}, [rbx+r11*8-256]
    vcvtqq2ps xmm2, xmm0
    vcvtqq2ps xmm2, ymm15
    vcvtqq2ps xmm2, [rbx]{1to2}
    vcvtqq2ps xmm2, [rbx]{1to4}
    vcvtqq2ps xmm2{k5}, xmm0
    vcvtqq2ps xmm2{k5}, ymm15
    vcvtqq2ps xmm2{k5}, [rbx]{1to2}
    vcvtqq2ps xmm2{k5}, [rbx]{1to4}
    vcvtqq2ps xmm2{k5}{z}, xmm0
    vcvtqq2ps xmm2{k5}{z}, ymm15
    vcvtqq2ps xmm2{k5}{z}, [rbx]{1to2}
    vcvtqq2ps xmm2{k5}{z}, [rbx]{1to4}
    vcvtqq2ps ymm16, zmm31
    vcvtqq2ps ymm16, zmm31, {rd-sae}
    vcvtqq2ps ymm16, [rbx]
    vcvtqq2ps ymm16, [rbx]{1to8}
    vcvtqq2ps ymm16, [rbx+r11*8+256]
    vcvtqq2ps ymm16, [rbx+r11*8-256]
    vcvtqq2ps ymm16{k5}, zmm31
    vcvtqq2ps ymm16{k5}, zmm31, {rd-sae}
    vcvtqq2ps ymm16{k5}, [rbx]
    vcvtqq2ps ymm16{k5}, [rbx]{1to8}
    vcvtqq2ps ymm16{k5}, [rbx+r11*8+256]
    vcvtqq2ps ymm16{k5}, [rbx+r11*8-256]
    vcvtqq2ps ymm16{k5}{z}, zmm31
    vcvtqq2ps ymm16{k5}{z}, zmm31, {rd-sae}
    vcvtqq2ps ymm16{k5}{z}, [rbx]
    vcvtqq2ps ymm16{k5}{z}, [rbx]{1to8}
    vcvtqq2ps ymm16{k5}{z}, [rbx+r11*8+256]
    vcvtqq2ps ymm16{k5}{z}, [rbx+r11*8-256]
    vcvttpd2qq xmm2, xmm0
    vcvttpd2qq xmm2, [rbx]
    vcvttpd2qq xmm2, [rbx]{1to2}
    vcvttpd2qq xmm2, [rbx+r11*8+256]
    vcvttpd2qq xmm2, [rbx+r11*8-256]
    vcvttpd2qq xmm2{k5}, xmm0
    vcvttpd2qq xmm2{k5}, [rbx]
    vcvttpd2qq xmm2{k5}, [rbx]{1to2}
    vcvttpd2qq xmm2{k5}, [rbx+r11*8+256]
    vcvttpd2qq xmm2{k5}, [rbx+r11*8-256]
    vcvttpd2qq xmm2{k5}{z}, xmm0
    vcvttpd2qq xmm2{k5}{z}, [rbx]
    vcvttpd2qq xmm2{k5}{z}, [rbx]{1to2}
    vcvttpd2qq xmm2{k5}{z}, [rbx+r11*8+256]
    vcvttpd2qq xmm2{k5}{z}, [rbx+r11*8-256]
    vcvttpd2qq ymm16, ymm15
    vcvttpd2qq ymm16, [rbx]
    vcvttpd2qq ymm16, [rbx]{1to4}
    vcvttpd2qq ymm16, [rbx+r11*8+256]
    vcvttpd2qq ymm16, [rbx+r11*8-256]
    vcvttpd2qq ymm16{k5}, ymm15
    vcvttpd2qq ymm16{k5}, [rbx]
    vcvttpd2qq ymm16{k5}, [rbx]{1to4}
    vcvttpd2qq ymm16{k5}, [rbx+r11*8+256]
    vcvttpd2qq ymm16{k5}, [rbx+r11*8-256]
    vcvttpd2qq ymm16{k5}{z}, ymm15
    vcvttpd2qq ymm16{k5}{z}, [rbx]
    vcvttpd2qq ymm16{k5}{z}, [rbx]{1to4}
    vcvttpd2qq ymm16{k5}{z}, [rbx+r11*8+256]
    vcvttpd2qq ymm16{k5}{z}, [rbx+r11*8-256]
    vcvttpd2qq zmm24, zmm31
    vcvttpd2qq zmm24, zmm31, {sae}
    vcvttpd2qq zmm24, [rbx]
    vcvttpd2qq zmm24, [rbx]{1to8}
    vcvttpd2qq zmm24, [rbx+r11*8+256]
    vcvttpd2qq zmm24, [rbx+r11*8-256]
    vcvttpd2qq zmm24{k5}, zmm31
    vcvttpd2qq zmm24{k5}, zmm31, {sae}
    vcvttpd2qq zmm24{k5}, [rbx]
    vcvttpd2qq zmm24{k5}, [rbx]{1to8}
    vcvttpd2qq zmm24{k5}, [rbx+r11*8+256]
    vcvttpd2qq zmm24{k5}, [rbx+r11*8-256]
    vcvttpd2qq zmm24{k5}{z}, zmm31
    vcvttpd2qq zmm24{k5}{z}, zmm31, {sae}
    vcvttpd2qq zmm24{k5}{z}, [rbx]
    vcvttpd2qq zmm24{k5}{z}, [rbx]{1to8}
    vcvttpd2qq zmm24{k5}{z}, [rbx+r11*8+256]
    vcvttpd2qq zmm24{k5}{z}, [rbx+r11*8-256]
    vcvttpd2uqq xmm2, xmm0
    vcvttpd2uqq xmm2, [rbx]
    vcvttpd2uqq xmm2, [rbx]{1to2}
    vcvttpd2uqq xmm2, [rbx+r11*8+256]
    vcvttpd2uqq xmm2, [rbx+r11*8-256]
    vcvttpd2uqq xmm2{k5}, xmm0
    vcvttpd2uqq xmm2{k5}, [rbx]
    vcvttpd2uqq xmm2{k5}, [rbx]{1to2}
    vcvttpd2uqq xmm2{k5}, [rbx+r11*8+256]
    vcvttpd2uqq xmm2{k5}, [rbx+r11*8-256]
    vcvttpd2uqq xmm2{k5}{z}, xmm0
    vcvttpd2uqq xmm2{k5}{z}, [rbx]
    vcvttpd2uqq xmm2{k5}{z}, [rbx]{1to2}
    vcvttpd2uqq xmm2{k5}{z}, [rbx+r11*8+256]
    vcvttpd2uqq xmm2{k5}{z}, [rbx+r11*8-256]
    vcvttpd2uqq ymm16, ymm15
    vcvttpd2uqq ymm16, [rbx]
    vcvttpd2uqq ymm16, [rbx]{1to4}
    vcvttpd2uqq ymm16, [rbx+r11*8+256]
    vcvttpd2uqq ymm16, [rbx+r11*8-256]
    vcvttpd2uqq ymm16{k5}, ymm15
    vcvttpd2uqq ymm16{k5}, [rbx]
    vcvttpd2uqq ymm16{k5}, [rbx]{1to4}
    vcvttpd2uqq ymm16{k5}, [rbx+r11*8+256]
    vcvttpd2uqq ymm16{k5}, [rbx+r11*8-256]
    vcvttpd2uqq ymm16{k5}{z}, ymm15
    vcvttpd2uqq ymm16{k5}{z}, [rbx]
    vcvttpd2uqq ymm16{k5}{z}, [rbx]{1to4}
    vcvttpd2uqq ymm16{k5}{z}, [rbx+r11*8+256]
    vcvttpd2uqq ymm16{k5}{z}, [rbx+r11*8-256]
    vcvttpd2uqq zmm24, zmm31
    vcvttpd2uqq zmm24, zmm31, {sae}
    vcvttpd2uqq zmm24, [rbx]
    vcvttpd2uqq zmm24, [rbx]{1to8}
    vcvttpd2uqq zmm24, [rbx+r11*8+256]
    vcvttpd2uqq zmm24, [rbx+r11*8-256]
    vcvttpd2uqq zmm24{k5}, zmm31
    vcvttpd2uqq zmm24{k5}, zmm31, {sae}
    vcvttpd2uqq zmm24{k5}, [rbx]
    vcvttpd2uqq zmm24{k5}, [rbx]{1to8}
    vcvttpd2uqq zmm24{k5}, [rbx+r11*8+256]
    vcvttpd2uqq zmm24{k5}, [rbx+r11*8-256]
    vcvttpd2uqq zmm24{k5}{z}, zmm31
    vcvttpd2uqq zmm24{k5}{z}, zmm31, {sae}
    vcvttpd2uqq zmm24{k5}{z}, [rbx]
    vcvttpd2uqq zmm24{k5}{z}, [rbx]{1to8}
    vcvttpd2uqq zmm24{k5}{z}, [rbx+r11*8+256]
    vcvttpd2uqq zmm24{k5}{z}, [rbx+r11*8-256]
    vcvttps2qq xmm2, xmm0
    vcvttps2qq xmm2, [rbx]
    vcvttps2qq xmm2, [rbx]{1to2}
    vcvttps2qq xmm2, [rbx+r11*8+256]
    vcvttps2qq xmm2, [rbx+r11*8-256]
    vcvttps2qq xmm2{k5}, xmm0
    vcvttps2qq xmm2{k5}, [rbx]
    vcvttps2qq xmm2{k5}, [rbx]{1to2}
    vcvttps2qq xmm2{k5}, [rbx+r11*8+256]
    vcvttps2qq xmm2{k5}, [rbx+r11*8-256]
    vcvttps2qq xmm2{k5}{z}, xmm0
    vcvttps2qq xmm2{k5}{z}, [rbx]
    vcvttps2qq xmm2{k5}{z}, [rbx]{1to2}
    vcvttps2qq xmm2{k5}{z}, [rbx+r11*8+256]
    vcvttps2qq xmm2{k5}{z}, [rbx+r11*8-256]
    vcvttps2qq ymm16, xmm0
    vcvttps2qq ymm16, [rbx]
    vcvttps2qq ymm16, [rbx]{1to4}
    vcvttps2qq ymm16, [rbx+r11*8+256]
    vcvttps2qq ymm16, [rbx+r11*8-256]
    vcvttps2qq ymm16{k5}, xmm0
    vcvttps2qq ymm16{k5}, [rbx]
    vcvttps2qq ymm16{k5}, [rbx]{1to4}
    vcvttps2qq ymm16{k5}, [rbx+r11*8+256]
    vcvttps2qq ymm16{k5}, [rbx+r11*8-256]
    vcvttps2qq ymm16{k5}{z}, xmm0
    vcvttps2qq ymm16{k5}{z}, [rbx]
    vcvttps2qq ymm16{k5}{z}, [rbx]{1to4}
    vcvttps2qq ymm16{k5}{z}, [rbx+r11*8+256]
    vcvttps2qq ymm16{k5}{z}, [rbx+r11*8-256]
    vcvttps2qq zmm24, ymm15
    vcvttps2qq zmm24, ymm15, {sae}
    vcvttps2qq zmm24, [rbx]
    vcvttps2qq zmm24, [rbx]{1to8}
    vcvttps2qq zmm24, [rbx+r11*8+256]
    vcvttps2qq zmm24, [rbx+r11*8-256]
    vcvttps2qq zmm24{k5}, ymm15
    vcvttps2qq zmm24{k5}, ymm15, {sae}
    vcvttps2qq zmm24{k5}, [rbx]
    vcvttps2qq zmm24{k5}, [rbx]{1to8}
    vcvttps2qq zmm24{k5}, [rbx+r11*8+256]
    vcvttps2qq zmm24{k5}, [rbx+r11*8-256]
    vcvttps2qq zmm24{k5}{z}, ymm15
    vcvttps2qq zmm24{k5}{z}, ymm15, {sae}
    vcvttps2qq zmm24{k5}{z}, [rbx]
    vcvttps2qq zmm24{k5}{z}, [rbx]{1to8}
    vcvttps2qq zmm24{k5}{z}, [rbx+r11*8+256]
    vcvttps2qq zmm24{k5}{z}, [rbx+r11*8-256]
    vcvttps2uqq xmm2, xmm0
    vcvttps2uqq xmm2, [rbx]
    vcvttps2uqq xmm2, [rbx]{1to2}
    vcvttps2uqq xmm2, [rbx+r11*8+256]
    vcvttps2uqq xmm2, [rbx+r11*8-256]
    vcvttps2uqq xmm2{k5}, xmm0
    vcvttps2uqq xmm2{k5}, [rbx]
    vcvttps2uqq xmm2{k5}, [rbx]{1to2}
    vcvttps2uqq xmm2{k5}, [rbx+r11*8+256]
    vcvttps2uqq xmm2{k5}, [rbx+r11*8-256]
    vcvttps2uqq xmm2{k5}{z}, xmm0
    vcvttps2uqq xmm2{k5}{z}, [rbx]
    vcvttps2uqq xmm2{k5}{z}, [rbx]{1to2}
    vcvttps2uqq xmm2{k5}{z}, [rbx+r11*8+256]
    vcvttps2uqq xmm2{k5}{z}, [rbx+r11*8-256]
    vcvttps2uqq ymm16, xmm0
    vcvttps2uqq ymm16, [rbx]
    vcvttps2uqq ymm16, [rbx]{1to4}
    vcvttps2uqq ymm16, [rbx+r11*8+256]
    vcvttps2uqq ymm16, [rbx+r11*8-256]
    vcvttps2uqq ymm16{k5}, xmm0
    vcvttps2uqq ymm16{k5}, [rbx]
    vcvttps2uqq ymm16{k5}, [rbx]{1to4}
    vcvttps2uqq ymm16{k5}, [rbx+r11*8+256]
    vcvttps2uqq ymm16{k5}, [rbx+r11*8-256]
    vcvttps2uqq ymm16{k5}{z}, xmm0
    vcvttps2uqq ymm16{k5}{z}, [rbx]
    vcvttps2uqq ymm16{k5}{z}, [rbx]{1to4}
    vcvttps2uqq ymm16{k5}{z}, [rbx+r11*8+256]
    vcvttps2uqq ymm16{k5}{z}, [rbx+r11*8-256]
    vcvttps2uqq zmm24, ymm15
    vcvttps2uqq zmm24, ymm15, {sae}
    vcvttps2uqq zmm24, [rbx]
    vcvttps2uqq zmm24, [rbx]{1to8}
    vcvttps2uqq zmm24, [rbx+r11*8+256]
    vcvttps2uqq zmm24, [rbx+r11*8-256]
    vcvttps2uqq zmm24{k5}, ymm15
    vcvttps2uqq zmm24{k5}, ymm15, {sae}
    vcvttps2uqq zmm24{k5}, [rbx]
    vcvttps2uqq zmm24{k5}, [rbx]{1to8}
    vcvttps2uqq zmm24{k5}, [rbx+r11*8+256]
    vcvttps2uqq zmm24{k5}, [rbx+r11*8-256]
    vcvttps2uqq zmm24{k5}{z}, ymm15
    vcvttps2uqq zmm24{k5}{z}, ymm15, {sae}
    vcvttps2uqq zmm24{k5}{z}, [rbx]
    vcvttps2uqq zmm24{k5}{z}, [rbx]{1to8}
    vcvttps2uqq zmm24{k5}{z}, [rbx+r11*8+256]
    vcvttps2uqq zmm24{k5}{z}, [rbx+r11*8-256]
    vcvtuqq2pd xmm2, xmm0
    vcvtuqq2pd xmm2, [rbx]
    vcvtuqq2pd xmm2, [rbx]{1to2}
    vcvtuqq2pd xmm2, [rbx+r11*8+256]
    vcvtuqq2pd xmm2, [rbx+r11*8-256]
    vcvtuqq2pd xmm2{k5}, xmm0
    vcvtuqq2pd xmm2{k5}, [rbx]
    vcvtuqq2pd xmm2{k5}, [rbx]{1to2}
    vcvtuqq2pd xmm2{k5}, [rbx+r11*8+256]
    vcvtuqq2pd xmm2{k5}, [rbx+r11*8-256]
    vcvtuqq2pd xmm2{k5}{z}, xmm0
    vcvtuqq2pd xmm2{k5}{z}, [rbx]
    vcvtuqq2pd xmm2{k5}{z}, [rbx]{1to2}
    vcvtuqq2pd xmm2{k5}{z}, [rbx+r11*8+256]
    vcvtuqq2pd xmm2{k5}{z}, [rbx+r11*8-256]
    vcvtuqq2pd ymm16, ymm15
    vcvtuqq2pd ymm16, [rbx]
    vcvtuqq2pd ymm16, [rbx]{1to4}
    vcvtuqq2pd ymm16, [rbx+r11*8+256]
    vcvtuqq2pd ymm16, [rbx+r11*8-256]
    vcvtuqq2pd ymm16{k5}, ymm15
    vcvtuqq2pd ymm16{k5}, [rbx]
    vcvtuqq2pd ymm16{k5}, [rbx]{1to4}
    vcvtuqq2pd ymm16{k5}, [rbx+r11*8+256]
    vcvtuqq2pd ymm16{k5}, [rbx+r11*8-256]
    vcvtuqq2pd ymm16{k5}{z}, ymm15
    vcvtuqq2pd ymm16{k5}{z}, [rbx]
    vcvtuqq2pd ymm16{k5}{z}, [rbx]{1to4}
    vcvtuqq2pd ymm16{k5}{z}, [rbx+r11*8+256]
    vcvtuqq2pd ymm16{k5}{z}, [rbx+r11*8-256]
    vcvtuqq2pd zmm24, zmm31
    vcvtuqq2pd zmm24, zmm31, {rd-sae}
    vcvtuqq2pd zmm24, [rbx]
    vcvtuqq2pd zmm24, [rbx]{1to8}
    vcvtuqq2pd zmm24, [rbx+r11*8+256]
    vcvtuqq2pd zmm24, [rbx+r11*8-256]
    vcvtuqq2pd zmm24{k5}, zmm31
    vcvtuqq2pd zmm24{k5}, zmm31, {rd-sae}
    vcvtuqq2pd zmm24{k5}, [rbx]
    vcvtuqq2pd zmm24{k5}, [rbx]{1to8}
    vcvtuqq2pd zmm24{k5}, [rbx+r11*8+256]
    vcvtuqq2pd zmm24{k5}, [rbx+r11*8-256]
    vcvtuqq2pd zmm24{k5}{z}, zmm31
    vcvtuqq2pd zmm24{k5}{z}, zmm31, {rd-sae}
    vcvtuqq2pd zmm24{k5}{z}, [rbx]
    vcvtuqq2pd zmm24{k5}{z}, [rbx]{1to8}
    vcvtuqq2pd zmm24{k5}{z}, [rbx+r11*8+256]
    vcvtuqq2pd zmm24{k5}{z}, [rbx+r11*8-256]
    vcvtuqq2ps xmm2, xmm0
    vcvtuqq2ps xmm2, ymm15
    vcvtuqq2ps xmm2, [rbx]{1to2}
    vcvtuqq2ps xmm2, [rbx]{1to4}
    vcvtuqq2ps xmm2{k5}, xmm0
    vcvtuqq2ps xmm2{k5}, ymm15
    vcvtuqq2ps xmm2{k5}, [rbx]{1to2}
    vcvtuqq2ps xmm2{k5}, [rbx]{1to4}
    vcvtuqq2ps xmm2{k5}{z}, xmm0
    vcvtuqq2ps xmm2{k5}{z}, ymm15
    vcvtuqq2ps xmm2{k5}{z}, [rbx]{1to2}
    vcvtuqq2ps xmm2{k5}{z}, [rbx]{1to4}
    vcvtuqq2ps ymm16, zmm31
    vcvtuqq2ps ymm16, zmm31, {rd-sae}
    vcvtuqq2ps ymm16, [rbx]
    vcvtuqq2ps ymm16, [rbx]{1to8}
    vcvtuqq2ps ymm16, [rbx+r11*8+256]
    vcvtuqq2ps ymm16, [rbx+r11*8-256]
    vcvtuqq2ps ymm16{k5}, zmm31
    vcvtuqq2ps ymm16{k5}, zmm31, {rd-sae}
    vcvtuqq2ps ymm16{k5}, [rbx]
    vcvtuqq2ps ymm16{k5}, [rbx]{1to8}
    vcvtuqq2ps ymm16{k5}, [rbx+r11*8+256]
    vcvtuqq2ps ymm16{k5}, [rbx+r11*8-256]
    vcvtuqq2ps ymm16{k5}{z}, zmm31
    vcvtuqq2ps ymm16{k5}{z}, zmm31, {rd-sae}
    vcvtuqq2ps ymm16{k5}{z}, [rbx]
    vcvtuqq2ps ymm16{k5}{z}, [rbx]{1to8}
    vcvtuqq2ps ymm16{k5}{z}, [rbx+r11*8+256]
    vcvtuqq2ps ymm16{k5}{z}, [rbx+r11*8-256]
    vextractf32x8 ymm15, zmm24, 10
    vextractf32x8 [rbx], zmm24, 10
    vextractf32x8 [rbx+r11*8+256], zmm24, 10
    vextractf32x8 [rbx+r11*8-256], zmm24, 10
    vextractf64x2 xmm0, ymm16, 10
    vextractf64x2 xmm0, zmm24, 10
    vextractf64x2 [rbx], ymm16, 10
    vextractf64x2 [rbx], zmm24, 10
    vextractf64x2 [rbx+r11*8+256], ymm16, 10
    vextractf64x2 [rbx+r11*8+256], zmm24, 10
    vextractf64x2 [rbx+r11*8-256], ymm16, 10
    vextractf64x2 [rbx+r11*8-256], zmm24, 10
    vextracti32x8 ymm15, zmm24, 10
    vextracti32x8 [rbx], zmm24, 10
    vextracti32x8 [rbx+r11*8+256], zmm24, 10
    vextracti32x8 [rbx+r11*8-256], zmm24, 10
    vextracti64x2 xmm0, ymm16, 10
    vextracti64x2 xmm0, zmm24, 10
    vextracti64x2 [rbx], ymm16, 10
    vextracti64x2 [rbx], zmm24, 10
    vextracti64x2 [rbx+r11*8+256], ymm16, 10
    vextracti64x2 [rbx+r11*8+256], zmm24, 10
    vextracti64x2 [rbx+r11*8-256], ymm16, 10
    vextracti64x2 [rbx+r11*8-256], zmm24, 10
    vfpclasspd k3, xmm0, 10
    vfpclasspd k3, ymm15, 10
    vfpclasspd k3, zmm31, 10
    vfpclasspd k3, [rbx]{1to2}, 10
    vfpclasspd k3, [rbx]{1to4}, 10
    vfpclasspd k3, [rbx]{1to8}, 10
    vfpclasspd k3{k5}, xmm0, 10
    vfpclasspd k3{k5}, ymm15, 10
    vfpclasspd k3{k5}, zmm31, 10
    vfpclasspd k3{k5}, [rbx]{1to2}, 10
    vfpclasspd k3{k5}, [rbx]{1to4}, 10
    vfpclasspd k3{k5}, [rbx]{1to8}, 10
    vfpclassps k3, xmm0, 10
    vfpclassps k3, ymm15, 10
    vfpclassps k3, zmm31, 10
    vfpclassps k3, [rbx]{1to4}, 10
    vfpclassps k3, [rbx]{1to8}, 10
    vfpclassps k3, [rbx]{1to16}, 10
    vfpclassps k3{k5}, xmm0, 10
    vfpclassps k3{k5}, ymm15, 10
    vfpclassps k3{k5}, zmm31, 10
    vfpclassps k3{k5}, [rbx]{1to4}, 10
    vfpclassps k3{k5}, [rbx]{1to8}, 10
    vfpclassps k3{k5}, [rbx]{1to16}, 10
    vfpclasssd k3, xmm0, 10
    vfpclasssd k3, [rbx], 10
    vfpclasssd k3, [rbx+r11*8+256], 10
    vfpclasssd k3, [rbx+r11*8-256], 10
    vfpclasssd k3{k5}, xmm0, 10
    vfpclasssd k3{k5}, [rbx], 10
    vfpclasssd k3{k5}, [rbx+r11*8+256], 10
    vfpclasssd k3{k5}, [rbx+r11*8-256], 10
    vfpclassss k3, xmm0, 10
    vfpclassss k3, [rbx], 10
    vfpclassss k3, [rbx+r11*8+256], 10
    vfpclassss k3, [rbx+r11*8-256], 10
    vfpclassss k3{k5}, xmm0, 10
    vfpclassss k3{k5}, [rbx], 10
    vfpclassss k3{k5}, [rbx+r11*8+256], 10
    vfpclassss k3{k5}, [rbx+r11*8-256], 10
    vinsertf32x8 zmm24, zmm24, ymm15, 10
    vinsertf32x8 zmm24, zmm24, [rbx], 10
    vinsertf32x8 zmm24, zmm24, [rbx+r11*8+256], 10
    vinsertf32x8 zmm24, zmm24, [rbx+r11*8-256], 10
    vinsertf32x8 zmm24{k5}, zmm24, ymm15, 10
    vinsertf32x8 zmm24{k5}, zmm24, [rbx], 10
    vinsertf32x8 zmm24{k5}, zmm24, [rbx+r11*8+256], 10
    vinsertf32x8 zmm24{k5}, zmm24, [rbx+r11*8-256], 10
    vinsertf32x8 zmm24{k5}{z}, zmm24, ymm15, 10
    vinsertf32x8 zmm24{k5}{z}, zmm24, [rbx], 10
    vinsertf32x8 zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 10
    vinsertf32x8 zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 10
    vinsertf64x2 ymm16, ymm13, xmm0, 10
    vinsertf64x2 ymm16, ymm13, [rbx], 10
    vinsertf64x2 ymm16, ymm13, [rbx+r11*8+256], 10
    vinsertf64x2 ymm16, ymm13, [rbx+r11*8-256], 10
    vinsertf64x2 ymm16{k5}, ymm13, xmm0, 10
    vinsertf64x2 ymm16{k5}, ymm13, [rbx], 10
    vinsertf64x2 ymm16{k5}, ymm13, [rbx+r11*8+256], 10
    vinsertf64x2 ymm16{k5}, ymm13, [rbx+r11*8-256], 10
    vinsertf64x2 ymm16{k5}{z}, ymm13, xmm0, 10
    vinsertf64x2 ymm16{k5}{z}, ymm13, [rbx], 10
    vinsertf64x2 ymm16{k5}{z}, ymm13, [rbx+r11*8+256], 10
    vinsertf64x2 ymm16{k5}{z}, ymm13, [rbx+r11*8-256], 10
    vinsertf64x2 zmm24, zmm24, xmm0, 10
    vinsertf64x2 zmm24, zmm24, [rbx], 10
    vinsertf64x2 zmm24, zmm24, [rbx+r11*8+256], 10
    vinsertf64x2 zmm24, zmm24, [rbx+r11*8-256], 10
    vinsertf64x2 zmm24{k5}, zmm24, xmm0, 10
    vinsertf64x2 zmm24{k5}, zmm24, [rbx], 10
    vinsertf64x2 zmm24{k5}, zmm24, [rbx+r11*8+256], 10
    vinsertf64x2 zmm24{k5}, zmm24, [rbx+r11*8-256], 10
    vinsertf64x2 zmm24{k5}{z}, zmm24, xmm0, 10
    vinsertf64x2 zmm24{k5}{z}, zmm24, [rbx], 10
    vinsertf64x2 zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 10
    vinsertf64x2 zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 10
    vinserti32x8 zmm24, zmm24, ymm15, 10
    vinserti32x8 zmm24, zmm24, [rbx], 10
    vinserti32x8 zmm24, zmm24, [rbx+r11*8+256], 10
    vinserti32x8 zmm24, zmm24, [rbx+r11*8-256], 10
    vinserti32x8 zmm24{k5}, zmm24, ymm15, 10
    vinserti32x8 zmm24{k5}, zmm24, [rbx], 10
    vinserti32x8 zmm24{k5}, zmm24, [rbx+r11*8+256], 10
    vinserti32x8 zmm24{k5}, zmm24, [rbx+r11*8-256], 10
    vinserti32x8 zmm24{k5}{z}, zmm24, ymm15, 10
    vinserti32x8 zmm24{k5}{z}, zmm24, [rbx], 10
    vinserti32x8 zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 10
    vinserti32x8 zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 10
    vinserti64x2 ymm16, ymm13, xmm0, 10
    vinserti64x2 ymm16, ymm13, [rbx], 10
    vinserti64x2 ymm16, ymm13, [rbx+r11*8+256], 10
    vinserti64x2 ymm16, ymm13, [rbx+r11*8-256], 10
    vinserti64x2 ymm16{k5}, ymm13, xmm0, 10
    vinserti64x2 ymm16{k5}, ymm13, [rbx], 10
    vinserti64x2 ymm16{k5}, ymm13, [rbx+r11*8+256], 10
    vinserti64x2 ymm16{k5}, ymm13, [rbx+r11*8-256], 10
    vinserti64x2 ymm16{k5}{z}, ymm13, xmm0, 10
    vinserti64x2 ymm16{k5}{z}, ymm13, [rbx], 10
    vinserti64x2 ymm16{k5}{z}, ymm13, [rbx+r11*8+256], 10
    vinserti64x2 ymm16{k5}{z}, ymm13, [rbx+r11*8-256], 10
    vinserti64x2 zmm24, zmm24, xmm0, 10
    vinserti64x2 zmm24, zmm24, [rbx], 10
    vinserti64x2 zmm24, zmm24, [rbx+r11*8+256], 10
    vinserti64x2 zmm24, zmm24, [rbx+r11*8-256], 10
    vinserti64x2 zmm24{k5}, zmm24, xmm0, 10
    vinserti64x2 zmm24{k5}, zmm24, [rbx], 10
    vinserti64x2 zmm24{k5}, zmm24, [rbx+r11*8+256], 10
    vinserti64x2 zmm24{k5}, zmm24, [rbx+r11*8-256], 10
    vinserti64x2 zmm24{k5}{z}, zmm24, xmm0, 10
    vinserti64x2 zmm24{k5}{z}, zmm24, [rbx], 10
    vinserti64x2 zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 10
    vinserti64x2 zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 10
    vorpd xmm2, xmm7, xmm0
    vorpd xmm2, xmm7, [rbx]
    vorpd xmm2, xmm7, [rbx]{1to2}
    vorpd xmm2, xmm7, [rbx+r11*8+256]
    vorpd xmm2, xmm7, [rbx+r11*8-256]
    vorpd xmm2{k5}, xmm7, xmm0
    vorpd xmm2{k5}, xmm7, [rbx]
    vorpd xmm2{k5}, xmm7, [rbx]{1to2}
    vorpd xmm2{k5}, xmm7, [rbx+r11*8+256]
    vorpd xmm2{k5}, xmm7, [rbx+r11*8-256]
    vorpd xmm2{k5}{z}, xmm7, xmm0
    vorpd xmm2{k5}{z}, xmm7, [rbx]
    vorpd xmm2{k5}{z}, xmm7, [rbx]{1to2}
    vorpd xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vorpd xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vorpd ymm16, ymm13, ymm15
    vorpd ymm16, ymm13, [rbx]
    vorpd ymm16, ymm13, [rbx]{1to4}
    vorpd ymm16, ymm13, [rbx+r11*8+256]
    vorpd ymm16, ymm13, [rbx+r11*8-256]
    vorpd ymm16{k5}, ymm13, ymm15
    vorpd ymm16{k5}, ymm13, [rbx]
    vorpd ymm16{k5}, ymm13, [rbx]{1to4}
    vorpd ymm16{k5}, ymm13, [rbx+r11*8+256]
    vorpd ymm16{k5}, ymm13, [rbx+r11*8-256]
    vorpd ymm16{k5}{z}, ymm13, ymm15
    vorpd ymm16{k5}{z}, ymm13, [rbx]
    vorpd ymm16{k5}{z}, ymm13, [rbx]{1to4}
    vorpd ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vorpd ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vorpd zmm24, zmm24, zmm31
    vorpd zmm24, zmm24, [rbx]
    vorpd zmm24, zmm24, [rbx]{1to8}
    vorpd zmm24, zmm24, [rbx+r11*8+256]
    vorpd zmm24, zmm24, [rbx+r11*8-256]
    vorpd zmm24{k5}, zmm24, zmm31
    vorpd zmm24{k5}, zmm24, [rbx]
    vorpd zmm24{k5}, zmm24, [rbx]{1to8}
    vorpd zmm24{k5}, zmm24, [rbx+r11*8+256]
    vorpd zmm24{k5}, zmm24, [rbx+r11*8-256]
    vorpd zmm24{k5}{z}, zmm24, zmm31
    vorpd zmm24{k5}{z}, zmm24, [rbx]
    vorpd zmm24{k5}{z}, zmm24, [rbx]{1to8}
    vorpd zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vorpd zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vorps xmm2, xmm7, xmm0
    vorps xmm2, xmm7, [rbx]
    vorps xmm2, xmm7, [rbx]{1to4}
    vorps xmm2, xmm7, [rbx+r11*8+256]
    vorps xmm2, xmm7, [rbx+r11*8-256]
    vorps xmm2{k5}, xmm7, xmm0
    vorps xmm2{k5}, xmm7, [rbx]
    vorps xmm2{k5}, xmm7, [rbx]{1to4}
    vorps xmm2{k5}, xmm7, [rbx+r11*8+256]
    vorps xmm2{k5}, xmm7, [rbx+r11*8-256]
    vorps xmm2{k5}{z}, xmm7, xmm0
    vorps xmm2{k5}{z}, xmm7, [rbx]
    vorps xmm2{k5}{z}, xmm7, [rbx]{1to4}
    vorps xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vorps xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vorps ymm16, ymm13, ymm15
    vorps ymm16, ymm13, [rbx]
    vorps ymm16, ymm13, [rbx]{1to8}
    vorps ymm16, ymm13, [rbx+r11*8+256]
    vorps ymm16, ymm13, [rbx+r11*8-256]
    vorps ymm16{k5}, ymm13, ymm15
    vorps ymm16{k5}, ymm13, [rbx]
    vorps ymm16{k5}, ymm13, [rbx]{1to8}
    vorps ymm16{k5}, ymm13, [rbx+r11*8+256]
    vorps ymm16{k5}, ymm13, [rbx+r11*8-256]
    vorps ymm16{k5}{z}, ymm13, ymm15
    vorps ymm16{k5}{z}, ymm13, [rbx]
    vorps ymm16{k5}{z}, ymm13, [rbx]{1to8}
    vorps ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vorps ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vorps zmm24, zmm24, zmm31
    vorps zmm24, zmm24, [rbx]
    vorps zmm24, zmm24, [rbx]{1to16}
    vorps zmm24, zmm24, [rbx+r11*8+256]
    vorps zmm24, zmm24, [rbx+r11*8-256]
    vorps zmm24{k5}, zmm24, zmm31
    vorps zmm24{k5}, zmm24, [rbx]
    vorps zmm24{k5}, zmm24, [rbx]{1to16}
    vorps zmm24{k5}, zmm24, [rbx+r11*8+256]
    vorps zmm24{k5}, zmm24, [rbx+r11*8-256]
    vorps zmm24{k5}{z}, zmm24, zmm31
    vorps zmm24{k5}{z}, zmm24, [rbx]
    vorps zmm24{k5}{z}, zmm24, [rbx]{1to16}
    vorps zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vorps zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpextrd ecx, xmm2, 10
    vpextrd rcx, xmm2, 10
    vpextrd [rbx], xmm2, 10
    vpextrd [rbx+rsi*8+256], xmm2, 10
    vpextrq rcx, xmm2, 10
    vpextrq [rbx], xmm2, 10
    vpextrq [rbx+rsi*8+256], xmm2, 10
    vpinsrd xmm2, xmm7, ecx, 10
    vpinsrd xmm2, xmm7, [rbx], 10
    vpinsrd xmm2, xmm7, [rbx+rsi*8+256], 10
    vpinsrq xmm2, xmm7, rcx, 10
    vpinsrq xmm2, xmm7, [rbx], 10
    vpinsrq xmm2, xmm7, [rbx+rsi*8+256], 10
    vpmovd2m k3, xmm0
    vpmovd2m k3, ymm15
    vpmovd2m k3, zmm31
    vpmovm2d xmm2, k1
    vpmovm2d ymm16, k1
    vpmovm2d zmm24, k1
    vpmovm2q xmm2, k1
    vpmovm2q ymm16, k1
    vpmovm2q zmm24, k1
    vpmovq2m k3, xmm0
    vpmovq2m k3, ymm15
    vpmovq2m k3, zmm31
    vpmullq xmm2, xmm7, xmm0
    vpmullq xmm2, xmm7, [rbx]
    vpmullq xmm2, xmm7, [rbx]{1to2}
    vpmullq xmm2, xmm7, [rbx+r11*8+256]
    vpmullq xmm2, xmm7, [rbx+r11*8-256]
    vpmullq xmm2{k5}, xmm7, xmm0
    vpmullq xmm2{k5}, xmm7, [rbx]
    vpmullq xmm2{k5}, xmm7, [rbx]{1to2}
    vpmullq xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpmullq xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpmullq xmm2{k5}{z}, xmm7, xmm0
    vpmullq xmm2{k5}{z}, xmm7, [rbx]
    vpmullq xmm2{k5}{z}, xmm7, [rbx]{1to2}
    vpmullq xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpmullq xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpmullq ymm16, ymm13, ymm15
    vpmullq ymm16, ymm13, [rbx]
    vpmullq ymm16, ymm13, [rbx]{1to4}
    vpmullq ymm16, ymm13, [rbx+r11*8+256]
    vpmullq ymm16, ymm13, [rbx+r11*8-256]
    vpmullq ymm16{k5}, ymm13, ymm15
    vpmullq ymm16{k5}, ymm13, [rbx]
    vpmullq ymm16{k5}, ymm13, [rbx]{1to4}
    vpmullq ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpmullq ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpmullq ymm16{k5}{z}, ymm13, ymm15
    vpmullq ymm16{k5}{z}, ymm13, [rbx]
    vpmullq ymm16{k5}{z}, ymm13, [rbx]{1to4}
    vpmullq ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpmullq ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpmullq zmm24, zmm24, zmm31
    vpmullq zmm24, zmm24, [rbx]
    vpmullq zmm24, zmm24, [rbx]{1to8}
    vpmullq zmm24, zmm24, [rbx+r11*8+256]
    vpmullq zmm24, zmm24, [rbx+r11*8-256]
    vpmullq zmm24{k5}, zmm24, zmm31
    vpmullq zmm24{k5}, zmm24, [rbx]
    vpmullq zmm24{k5}, zmm24, [rbx]{1to8}
    vpmullq zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpmullq zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpmullq zmm24{k5}{z}, zmm24, zmm31
    vpmullq zmm24{k5}{z}, zmm24, [rbx]
    vpmullq zmm24{k5}{z}, zmm24, [rbx]{1to8}
    vpmullq zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpmullq zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vrangepd xmm2, xmm7, xmm0, 10
    vrangepd xmm2, xmm7, [rbx], 10
    vrangepd xmm2, xmm7, [rbx]{1to2}, 10
    vrangepd xmm2, xmm7, [rbx+r11*8+256], 10
    vrangepd xmm2, xmm7, [rbx+r11*8-256], 10
    vrangepd xmm2{k5}, xmm7, xmm0, 10
    vrangepd xmm2{k5}, xmm7, [rbx], 10
    vrangepd xmm2{k5}, xmm7, [rbx]{1to2}, 10
    vrangepd xmm2{k5}, xmm7, [rbx+r11*8+256], 10
    vrangepd xmm2{k5}, xmm7, [rbx+r11*8-256], 10
    vrangepd xmm2{k5}{z}, xmm7, xmm0, 10
    vrangepd xmm2{k5}{z}, xmm7, [rbx], 10
    vrangepd xmm2{k5}{z}, xmm7, [rbx]{1to2}, 10
    vrangepd xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 10
    vrangepd xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 10
    vrangepd ymm16, ymm13, ymm15, 10
    vrangepd ymm16, ymm13, [rbx], 10
    vrangepd ymm16, ymm13, [rbx]{1to4}, 10
    vrangepd ymm16, ymm13, [rbx+r11*8+256], 10
    vrangepd ymm16, ymm13, [rbx+r11*8-256], 10
    vrangepd ymm16{k5}, ymm13, ymm15, 10
    vrangepd ymm16{k5}, ymm13, [rbx], 10
    vrangepd ymm16{k5}, ymm13, [rbx]{1to4}, 10
    vrangepd ymm16{k5}, ymm13, [rbx+r11*8+256], 10
    vrangepd ymm16{k5}, ymm13, [rbx+r11*8-256], 10
    vrangepd ymm16{k5}{z}, ymm13, ymm15, 10
    vrangepd ymm16{k5}{z}, ymm13, [rbx], 10
    vrangepd ymm16{k5}{z}, ymm13, [rbx]{1to4}, 10
    vrangepd ymm16{k5}{z}, ymm13, [rbx+r11*8+256], 10
    vrangepd ymm16{k5}{z}, ymm13, [rbx+r11*8-256], 10
    vrangepd zmm24, zmm24, zmm31, 10
    vrangepd zmm24, zmm24, [rbx], 10
    vrangepd zmm24, zmm24, [rbx]{1to8}, 10
    vrangepd zmm24, zmm24, [rbx+r11*8+256], 10
    vrangepd zmm24, zmm24, [rbx+r11*8-256], 10
    vrangepd zmm24{k5}, zmm24, zmm31, 10
    vrangepd zmm24{k5}, zmm24, [rbx], 10
    vrangepd zmm24{k5}, zmm24, [rbx]{1to8}, 10
    vrangepd zmm24{k5}, zmm24, [rbx+r11*8+256], 10
    vrangepd zmm24{k5}, zmm24, [rbx+r11*8-256], 10
    vrangepd zmm24{k5}{z}, zmm24, zmm31, 10
    vrangepd zmm24{k5}{z}, zmm24, [rbx], 10
    vrangepd zmm24{k5}{z}, zmm24, [rbx]{1to8}, 10
    vrangepd zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 10
    vrangepd zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 10
    vrangeps xmm2, xmm7, xmm0, 10
    vrangeps xmm2, xmm7, [rbx], 10
    vrangeps xmm2, xmm7, [rbx]{1to4}, 10
    vrangeps xmm2, xmm7, [rbx+r11*8+256], 10
    vrangeps xmm2, xmm7, [rbx+r11*8-256], 10
    vrangeps xmm2{k5}, xmm7, xmm0, 10
    vrangeps xmm2{k5}, xmm7, [rbx], 10
    vrangeps xmm2{k5}, xmm7, [rbx]{1to4}, 10
    vrangeps xmm2{k5}, xmm7, [rbx+r11*8+256], 10
    vrangeps xmm2{k5}, xmm7, [rbx+r11*8-256], 10
    vrangeps xmm2{k5}{z}, xmm7, xmm0, 10
    vrangeps xmm2{k5}{z}, xmm7, [rbx], 10
    vrangeps xmm2{k5}{z}, xmm7, [rbx]{1to4}, 10
    vrangeps xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 10
    vrangeps xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 10
    vrangeps ymm16, ymm13, ymm15, 10
    vrangeps ymm16, ymm13, [rbx], 10
    vrangeps ymm16, ymm13, [rbx]{1to8}, 10
    vrangeps ymm16, ymm13, [rbx+r11*8+256], 10
    vrangeps ymm16, ymm13, [rbx+r11*8-256], 10
    vrangeps ymm16{k5}, ymm13, ymm15, 10
    vrangeps ymm16{k5}, ymm13, [rbx], 10
    vrangeps ymm16{k5}, ymm13, [rbx]{1to8}, 10
    vrangeps ymm16{k5}, ymm13, [rbx+r11*8+256], 10
    vrangeps ymm16{k5}, ymm13, [rbx+r11*8-256], 10
    vrangeps ymm16{k5}{z}, ymm13, ymm15, 10
    vrangeps ymm16{k5}{z}, ymm13, [rbx], 10
    vrangeps ymm16{k5}{z}, ymm13, [rbx]{1to8}, 10
    vrangeps ymm16{k5}{z}, ymm13, [rbx+r11*8+256], 10
    vrangeps ymm16{k5}{z}, ymm13, [rbx+r11*8-256], 10
    vrangeps zmm24, zmm24, zmm31, 10
    vrangeps zmm24, zmm24, [rbx], 10
    vrangeps zmm24, zmm24, [rbx]{1to16}, 10
    vrangeps zmm24, zmm24, [rbx+r11*8+256], 10
    vrangeps zmm24, zmm24, [rbx+r11*8-256], 10
    vrangeps zmm24{k5}, zmm24, zmm31, 10
    vrangeps zmm24{k5}, zmm24, [rbx], 10
    vrangeps zmm24{k5}, zmm24, [rbx]{1to16}, 10
    vrangeps zmm24{k5}, zmm24, [rbx+r11*8+256], 10
    vrangeps zmm24{k5}, zmm24, [rbx+r11*8-256], 10
    vrangeps zmm24{k5}{z}, zmm24, zmm31, 10
    vrangeps zmm24{k5}{z}, zmm24, [rbx], 10
    vrangeps zmm24{k5}{z}, zmm24, [rbx]{1to16}, 10
    vrangeps zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 10
    vrangeps zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 10
    vrangesd xmm2, xmm7, xmm0, 10
    vrangesd xmm2, xmm7, [rbx], 10
    vrangesd xmm2, xmm7, [rbx+r11*8+256], 10
    vrangesd xmm2, xmm7, [rbx+r11*8-256], 10
    vrangesd xmm2{k5}, xmm7, xmm0, 10
    vrangesd xmm2{k5}, xmm7, [rbx], 10
    vrangesd xmm2{k5}, xmm7, [rbx+r11*8+256], 10
    vrangesd xmm2{k5}, xmm7, [rbx+r11*8-256], 10
    vrangesd xmm2{k5}{z}, xmm7, xmm0, 10
    vrangesd xmm2{k5}{z}, xmm7, [rbx], 10
    vrangesd xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 10
    vrangesd xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 10
    vrangess xmm2, xmm7, xmm0, 10
    vrangess xmm2, xmm7, [rbx], 10
    vrangess xmm2, xmm7, [rbx+r11*8+256], 10
    vrangess xmm2, xmm7, [rbx+r11*8-256], 10
    vrangess xmm2{k5}, xmm7, xmm0, 10
    vrangess xmm2{k5}, xmm7, [rbx], 10
    vrangess xmm2{k5}, xmm7, [rbx+r11*8+256], 10
    vrangess xmm2{k5}, xmm7, [rbx+r11*8-256], 10
    vrangess xmm2{k5}{z}, xmm7, xmm0, 10
    vrangess xmm2{k5}{z}, xmm7, [rbx], 10
    vrangess xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 10
    vrangess xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 10
    vreducepd xmm2, xmm0, 10
    vreducepd xmm2, [rbx], 10
    vreducepd xmm2, [rbx]{1to2}, 10
    vreducepd xmm2, [rbx+r11*8+256], 10
    vreducepd xmm2, [rbx+r11*8-256], 10
    vreducepd xmm2{k5}, xmm0, 10
    vreducepd xmm2{k5}, [rbx], 10
    vreducepd xmm2{k5}, [rbx]{1to2}, 10
    vreducepd xmm2{k5}, [rbx+r11*8+256], 10
    vreducepd xmm2{k5}, [rbx+r11*8-256], 10
    vreducepd xmm2{k5}{z}, xmm0, 10
    vreducepd xmm2{k5}{z}, [rbx], 10
    vreducepd xmm2{k5}{z}, [rbx]{1to2}, 10
    vreducepd xmm2{k5}{z}, [rbx+r11*8+256], 10
    vreducepd xmm2{k5}{z}, [rbx+r11*8-256], 10
    vreducepd ymm16, ymm15, 10
    vreducepd ymm16, [rbx], 10
    vreducepd ymm16, [rbx]{1to4}, 10
    vreducepd ymm16, [rbx+r11*8+256], 10
    vreducepd ymm16, [rbx+r11*8-256], 10
    vreducepd ymm16{k5}, ymm15, 10
    vreducepd ymm16{k5}, [rbx], 10
    vreducepd ymm16{k5}, [rbx]{1to4}, 10
    vreducepd ymm16{k5}, [rbx+r11*8+256], 10
    vreducepd ymm16{k5}, [rbx+r11*8-256], 10
    vreducepd ymm16{k5}{z}, ymm15, 10
    vreducepd ymm16{k5}{z}, [rbx], 10
    vreducepd ymm16{k5}{z}, [rbx]{1to4}, 10
    vreducepd ymm16{k5}{z}, [rbx+r11*8+256], 10
    vreducepd ymm16{k5}{z}, [rbx+r11*8-256], 10
    vreducepd zmm24, zmm31, 10
    vreducepd zmm24, [rbx], 10
    vreducepd zmm24, [rbx]{1to8}, 10
    vreducepd zmm24, [rbx+r11*8+256], 10
    vreducepd zmm24, [rbx+r11*8-256], 10
    vreducepd zmm24{k5}, zmm31, 10
    vreducepd zmm24{k5}, [rbx], 10
    vreducepd zmm24{k5}, [rbx]{1to8}, 10
    vreducepd zmm24{k5}, [rbx+r11*8+256], 10
    vreducepd zmm24{k5}, [rbx+r11*8-256], 10
    vreducepd zmm24{k5}{z}, zmm31, 10
    vreducepd zmm24{k5}{z}, [rbx], 10
    vreducepd zmm24{k5}{z}, [rbx]{1to8}, 10
    vreducepd zmm24{k5}{z}, [rbx+r11*8+256], 10
    vreducepd zmm24{k5}{z}, [rbx+r11*8-256], 10
    vreduceps xmm2, xmm0, 10
    vreduceps xmm2, [rbx], 10
    vreduceps xmm2, [rbx]{1to4}, 10
    vreduceps xmm2, [rbx+r11*8+256], 10
    vreduceps xmm2, [rbx+r11*8-256], 10
    vreduceps xmm2{k5}, xmm0, 10
    vreduceps xmm2{k5}, [rbx], 10
    vreduceps xmm2{k5}, [rbx]{1to4}, 10
    vreduceps xmm2{k5}, [rbx+r11*8+256], 10
    vreduceps xmm2{k5}, [rbx+r11*8-256], 10
    vreduceps xmm2{k5}{z}, xmm0, 10
    vreduceps xmm2{k5}{z}, [rbx], 10
    vreduceps xmm2{k5}{z}, [rbx]{1to4}, 10
    vreduceps xmm2{k5}{z}, [rbx+r11*8+256], 10
    vreduceps xmm2{k5}{z}, [rbx+r11*8-256], 10
    vreduceps ymm16, ymm15, 10
    vreduceps ymm16, [rbx], 10
    vreduceps ymm16, [rbx]{1to8}, 10
    vreduceps ymm16, [rbx+r11*8+256], 10
    vreduceps ymm16, [rbx+r11*8-256], 10
    vreduceps ymm16{k5}, ymm15, 10
    vreduceps ymm16{k5}, [rbx], 10
    vreduceps ymm16{k5}, [rbx]{1to8}, 10
    vreduceps ymm16{k5}, [rbx+r11*8+256], 10
    vreduceps ymm16{k5}, [rbx+r11*8-256], 10
    vreduceps ymm16{k5}{z}, ymm15, 10
    vreduceps ymm16{k5}{z}, [rbx], 10
    vreduceps ymm16{k5}{z}, [rbx]{1to8}, 10
    vreduceps ymm16{k5}{z}, [rbx+r11*8+256], 10
    vreduceps ymm16{k5}{z}, [rbx+r11*8-256], 10
    vreduceps zmm24, zmm31, 10
    vreduceps zmm24, [rbx], 10
    vreduceps zmm24, [rbx]{1to16}, 10
    vreduceps zmm24, [rbx+r11*8+256], 10
    vreduceps zmm24, [rbx+r11*8-256], 10
    vreduceps zmm24{k5}, zmm31, 10
    vreduceps zmm24{k5}, [rbx], 10
    vreduceps zmm24{k5}, [rbx]{1to16}, 10
    vreduceps zmm24{k5}, [rbx+r11*8+256], 10
    vreduceps zmm24{k5}, [rbx+r11*8-256], 10
    vreduceps zmm24{k5}{z}, zmm31, 10
    vreduceps zmm24{k5}{z}, [rbx], 10
    vreduceps zmm24{k5}{z}, [rbx]{1to16}, 10
    vreduceps zmm24{k5}{z}, [rbx+r11*8+256], 10
    vreduceps zmm24{k5}{z}, [rbx+r11*8-256], 10
    vreducesd xmm2, xmm7, xmm0, 10
    vreducesd xmm2, xmm7, [rbx], 10
    vreducesd xmm2, xmm7, [rbx+r11*8+256], 10
    vreducesd xmm2, xmm7, [rbx+r11*8-256], 10
    vreducesd xmm2{k5}, xmm7, xmm0, 10
    vreducesd xmm2{k5}, xmm7, [rbx], 10
    vreducesd xmm2{k5}, xmm7, [rbx+r11*8+256], 10
    vreducesd xmm2{k5}, xmm7, [rbx+r11*8-256], 10
    vreducesd xmm2{k5}{z}, xmm7, xmm0, 10
    vreducesd xmm2{k5}{z}, xmm7, [rbx], 10
    vreducesd xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 10
    vreducesd xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 10
    vreducess xmm2, xmm7, xmm0, 10
    vreducess xmm2, xmm7, [rbx], 10
    vreducess xmm2, xmm7, [rbx+r11*8+256], 10
    vreducess xmm2, xmm7, [rbx+r11*8-256], 10
    vreducess xmm2{k5}, xmm7, xmm0, 10
    vreducess xmm2{k5}, xmm7, [rbx], 10
    vreducess xmm2{k5}, xmm7, [rbx+r11*8+256], 10
    vreducess xmm2{k5}, xmm7, [rbx+r11*8-256], 10
    vreducess xmm2{k5}{z}, xmm7, xmm0, 10
    vreducess xmm2{k5}{z}, xmm7, [rbx], 10
    vreducess xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 10
    vreducess xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 10
    vxorpd xmm2, xmm7, xmm0
    vxorpd xmm2, xmm7, [rbx]
    vxorpd xmm2, xmm7, [rbx]{1to2}
    vxorpd xmm2, xmm7, [rbx+r11*8+256]
    vxorpd xmm2, xmm7, [rbx+r11*8-256]
    vxorpd xmm2{k5}, xmm7, xmm0
    vxorpd xmm2{k5}, xmm7, [rbx]
    vxorpd xmm2{k5}, xmm7, [rbx]{1to2}
    vxorpd xmm2{k5}, xmm7, [rbx+r11*8+256]
    vxorpd xmm2{k5}, xmm7, [rbx+r11*8-256]
    vxorpd xmm2{k5}{z}, xmm7, xmm0
    vxorpd xmm2{k5}{z}, xmm7, [rbx]
    vxorpd xmm2{k5}{z}, xmm7, [rbx]{1to2}
    vxorpd xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vxorpd xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vxorpd ymm16, ymm13, ymm15
    vxorpd ymm16, ymm13, [rbx]
    vxorpd ymm16, ymm13, [rbx]{1to4}
    vxorpd ymm16, ymm13, [rbx+r11*8+256]
    vxorpd ymm16, ymm13, [rbx+r11*8-256]
    vxorpd ymm16{k5}, ymm13, ymm15
    vxorpd ymm16{k5}, ymm13, [rbx]
    vxorpd ymm16{k5}, ymm13, [rbx]{1to4}
    vxorpd ymm16{k5}, ymm13, [rbx+r11*8+256]
    vxorpd ymm16{k5}, ymm13, [rbx+r11*8-256]
    vxorpd ymm16{k5}{z}, ymm13, ymm15
    vxorpd ymm16{k5}{z}, ymm13, [rbx]
    vxorpd ymm16{k5}{z}, ymm13, [rbx]{1to4}
    vxorpd ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vxorpd ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vxorpd zmm24, zmm24, zmm31
    vxorpd zmm24, zmm24, [rbx]
    vxorpd zmm24, zmm24, [rbx]{1to8}
    vxorpd zmm24, zmm24, [rbx+r11*8+256]
    vxorpd zmm24, zmm24, [rbx+r11*8-256]
    vxorpd zmm24{k5}, zmm24, zmm31
    vxorpd zmm24{k5}, zmm24, [rbx]
    vxorpd zmm24{k5}, zmm24, [rbx]{1to8}
    vxorpd zmm24{k5}, zmm24, [rbx+r11*8+256]
    vxorpd zmm24{k5}, zmm24, [rbx+r11*8-256]
    vxorpd zmm24{k5}{z}, zmm24, zmm31
    vxorpd zmm24{k5}{z}, zmm24, [rbx]
    vxorpd zmm24{k5}{z}, zmm24, [rbx]{1to8}
    vxorpd zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vxorpd zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vxorps xmm2, xmm7, xmm0
    vxorps xmm2, xmm7, [rbx]
    vxorps xmm2, xmm7, [rbx]{1to4}
    vxorps xmm2, xmm7, [rbx+r11*8+256]
    vxorps xmm2, xmm7, [rbx+r11*8-256]
    vxorps xmm2{k5}, xmm7, xmm0
    vxorps xmm2{k5}, xmm7, [rbx]
    vxorps xmm2{k5}, xmm7, [rbx]{1to4}
    vxorps xmm2{k5}, xmm7, [rbx+r11*8+256]
    vxorps xmm2{k5}, xmm7, [rbx+r11*8-256]
    vxorps xmm2{k5}{z}, xmm7, xmm0
    vxorps xmm2{k5}{z}, xmm7, [rbx]
    vxorps xmm2{k5}{z}, xmm7, [rbx]{1to4}
    vxorps xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vxorps xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vxorps ymm16, ymm13, ymm15
    vxorps ymm16, ymm13, [rbx]
    vxorps ymm16, ymm13, [rbx]{1to8}
    vxorps ymm16, ymm13, [rbx+r11*8+256]
    vxorps ymm16, ymm13, [rbx+r11*8-256]
    vxorps ymm16{k5}, ymm13, ymm15
    vxorps ymm16{k5}, ymm13, [rbx]
    vxorps ymm16{k5}, ymm13, [rbx]{1to8}
    vxorps ymm16{k5}, ymm13, [rbx+r11*8+256]
    vxorps ymm16{k5}, ymm13, [rbx+r11*8-256]
    vxorps ymm16{k5}{z}, ymm13, ymm15
    vxorps ymm16{k5}{z}, ymm13, [rbx]
    vxorps ymm16{k5}{z}, ymm13, [rbx]{1to8}
    vxorps ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vxorps ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vxorps zmm24, zmm24, zmm31
    vxorps zmm24, zmm24, [rbx]
    vxorps zmm24, zmm24, [rbx]{1to16}
    vxorps zmm24, zmm24, [rbx+r11*8+256]
    vxorps zmm24, zmm24, [rbx+r11*8-256]
    vxorps zmm24{k5}, zmm24, zmm31
    vxorps zmm24{k5}, zmm24, [rbx]
    vxorps zmm24{k5}, zmm24, [rbx]{1to16}
    vxorps zmm24{k5}, zmm24, [rbx+r11*8+256]
    vxorps zmm24{k5}, zmm24, [rbx+r11*8-256]
    vxorps zmm24{k5}{z}, zmm24, zmm31
    vxorps zmm24{k5}{z}, zmm24, [rbx]
    vxorps zmm24{k5}{z}, zmm24, [rbx]{1to16}
    vxorps zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vxorps zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
