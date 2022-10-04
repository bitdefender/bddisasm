    bits 64
    
    vaddpd xmm2, xmm7, xmm0
    vaddpd xmm2, xmm7, [rbx]
    vaddpd xmm2, xmm7, [rbx+r11*8+256]
    vaddpd xmm2, xmm7, [rbx+r11*8-256]
    vaddpd ymm16, ymm13, ymm15
    vaddpd ymm16, ymm13, [rbx]
    vaddpd ymm16, ymm13, [rbx+r11*8+256]
    vaddpd ymm16, ymm13, [rbx+r11*8-256]
    vaddpd zmm24, zmm24, zmm31
    vaddpd zmm24, zmm24, [rbx]
    vaddpd zmm24, zmm24, [rbx+r11*8+256]
    vaddpd zmm24, zmm24, [rbx+r11*8-256]
    vaddps xmm2, xmm7, xmm0
    vaddps xmm2, xmm7, [rbx]
    vaddps xmm2, xmm7, [rbx+r11*8+256]
    vaddps xmm2, xmm7, [rbx+r11*8-256]
    vaddps ymm16, ymm13, ymm15
    vaddps ymm16, ymm13, [rbx]
    vaddps ymm16, ymm13, [rbx+r11*8+256]
    vaddps ymm16, ymm13, [rbx+r11*8-256]
    vaddps zmm24, zmm24, zmm31
    vaddps zmm24, zmm24, [rbx]
    vaddps zmm24, zmm24, [rbx+r11*8+256]
    vaddps zmm24, zmm24, [rbx+r11*8-256]
    vaddsd xmm2, xmm7, xmm0
    vaddsd xmm2, xmm7, [rbx]
    vaddsd xmm2, xmm7, [rbx+r11*8+256]
    vaddsd xmm2, xmm7, [rbx+r11*8-256]
    vaddss xmm2, xmm7, xmm0
    vaddss xmm2, xmm7, [rbx]
    vaddss xmm2, xmm7, [rbx+r11*8+256]
    vaddss xmm2, xmm7, [rbx+r11*8-256]
    vaddsubpd xmm2, xmm7, xmm0
    vaddsubpd xmm2, xmm7, [rbx]
    vaddsubpd xmm2, xmm7, [rbx+r11*8+256]
    vaddsubpd xmm2, xmm7, [rbx+r11*8-256]
    vaddsubps xmm2, xmm7, xmm0
    vaddsubps xmm2, xmm7, [rbx]
    vaddsubps xmm2, xmm7, [rbx+r11*8+256]
    vaddsubps xmm2, xmm7, [rbx+r11*8-256]
    vandnpd xmm2, xmm7, xmm0
    vandnpd xmm2, xmm7, [rbx]
    vandnpd xmm2, xmm7, [rbx+r11*8+256]
    vandnpd xmm2, xmm7, [rbx+r11*8-256]
    vandnpd ymm16, ymm13, ymm15
    vandnpd ymm16, ymm13, [rbx]
    vandnpd ymm16, ymm13, [rbx+r11*8+256]
    vandnpd ymm16, ymm13, [rbx+r11*8-256]
    vandnpd zmm24, zmm24, zmm31
    vandnpd zmm24, zmm24, [rbx]
    vandnpd zmm24, zmm24, [rbx+r11*8+256]
    vandnpd zmm24, zmm24, [rbx+r11*8-256]
    vandnps xmm2, xmm7, xmm0
    vandnps xmm2, xmm7, [rbx]
    vandnps xmm2, xmm7, [rbx+r11*8+256]
    vandnps xmm2, xmm7, [rbx+r11*8-256]
    vandnps ymm16, ymm13, ymm15
    vandnps ymm16, ymm13, [rbx]
    vandnps ymm16, ymm13, [rbx+r11*8+256]
    vandnps ymm16, ymm13, [rbx+r11*8-256]
    vandnps zmm24, zmm24, zmm31
    vandnps zmm24, zmm24, [rbx]
    vandnps zmm24, zmm24, [rbx+r11*8+256]
    vandnps zmm24, zmm24, [rbx+r11*8-256]
    vandpd xmm2, xmm7, xmm0
    vandpd xmm2, xmm7, [rbx]
    vandpd xmm2, xmm7, [rbx+r11*8+256]
    vandpd xmm2, xmm7, [rbx+r11*8-256]
    vandpd ymm16, ymm13, ymm15
    vandpd ymm16, ymm13, [rbx]
    vandpd ymm16, ymm13, [rbx+r11*8+256]
    vandpd ymm16, ymm13, [rbx+r11*8-256]
    vandpd zmm24, zmm24, zmm31
    vandpd zmm24, zmm24, [rbx]
    vandpd zmm24, zmm24, [rbx+r11*8+256]
    vandpd zmm24, zmm24, [rbx+r11*8-256]
    vandps xmm2, xmm7, xmm0
    vandps xmm2, xmm7, [rbx]
    vandps xmm2, xmm7, [rbx+r11*8+256]
    vandps xmm2, xmm7, [rbx+r11*8-256]
    vandps ymm16, ymm13, ymm15
    vandps ymm16, ymm13, [rbx]
    vandps ymm16, ymm13, [rbx+r11*8+256]
    vandps ymm16, ymm13, [rbx+r11*8-256]
    vandps zmm24, zmm24, zmm31
    vandps zmm24, zmm24, [rbx]
    vandps zmm24, zmm24, [rbx+r11*8+256]
    vandps zmm24, zmm24, [rbx+r11*8-256]
    vblendpd xmm2, xmm7, xmm0, 0x90
    vblendpd xmm2, xmm7, [rbx], 0x90
    vblendpd xmm2, xmm7, [rbx+r11*8+256], 0x90
    vblendpd xmm2, xmm7, [rbx+r11*8-256], 0x90
    vblendps xmm2, xmm7, xmm0, 0x90
    vblendps xmm2, xmm7, [rbx], 0x90
    vblendps xmm2, xmm7, [rbx+r11*8+256], 0x90
    vblendps xmm2, xmm7, [rbx+r11*8-256], 0x90
    vblendvpd xmm2, xmm7, xmm0, xmm3
    vblendvpd xmm2, xmm7, [rbx], xmm3
    vblendvpd xmm2, xmm7, [rbx+r11*8+256], xmm3
    vblendvpd xmm2, xmm7, [rbx+r11*8-256], xmm3
    vblendvps xmm2, xmm7, xmm0, xmm3
    vblendvps xmm2, xmm7, [rbx], xmm3
    vblendvps xmm2, xmm7, [rbx+r11*8+256], xmm3
    vblendvps xmm2, xmm7, [rbx+r11*8-256], xmm3
    vbroadcastsd ymm16, xmm0
    vbroadcastsd ymm16, [rbx]
    vbroadcastsd ymm16, [rbx+r11*8+256]
    vbroadcastsd ymm16, [rbx+r11*8-256]
    vbroadcastsd zmm24, xmm0
    vbroadcastsd zmm24, [rbx]
    vbroadcastsd zmm24, [rbx+r11*8+256]
    vbroadcastsd zmm24, [rbx+r11*8-256]
    vbroadcastss xmm2, xmm0
    vbroadcastss xmm2, [rbx]
    vbroadcastss xmm2, [rbx+r11*8+256]
    vbroadcastss xmm2, [rbx+r11*8-256]
    vbroadcastss ymm16, xmm0
    vbroadcastss ymm16, [rbx]
    vbroadcastss ymm16, [rbx+r11*8+256]
    vbroadcastss ymm16, [rbx+r11*8-256]
    vbroadcastss zmm24, xmm0
    vbroadcastss zmm24, [rbx]
    vbroadcastss zmm24, [rbx+r11*8+256]
    vbroadcastss zmm24, [rbx+r11*8-256]
    vcmppd xmm2, xmm7, xmm0, 0x90
    vcmppd xmm2, xmm7, [rbx], 0x90
    vcmppd xmm2, xmm7, [rbx+r11*8+256], 0x90
    vcmppd xmm2, xmm7, [rbx+r11*8-256], 0x90
    vcmpsd xmm2, xmm7, xmm0, 0x90
    vcmpsd xmm2, xmm7, [rbx], 0x90
    vcmpsd xmm2, xmm7, [rbx+r11*8+256], 0x90
    vcmpsd xmm2, xmm7, [rbx+r11*8-256], 0x90
    vcmpss xmm2, xmm7, xmm0, 0x90
    vcmpss xmm2, xmm7, [rbx], 0x90
    vcmpss xmm2, xmm7, [rbx+r11*8+256], 0x90
    vcmpss xmm2, xmm7, [rbx+r11*8-256], 0x90
    vcmpss xmm2, xmm7, xmm0, 0x90
    vcmpss xmm2, xmm7, [rbx], 0x90
    vcmpss xmm2, xmm7, [rbx+r11*8+256], 0x90
    vcmpss xmm2, xmm7, [rbx+r11*8-256], 0x90
    vcomisd xmm2, xmm0
    vcomisd xmm2, [rbx]
    vcomisd xmm2, [rbx+r11*8+256]
    vcomisd xmm2, [rbx+r11*8-256]
    vcomiss xmm2, xmm0
    vcomiss xmm2, [rbx]
    vcomiss xmm2, [rbx+r11*8+256]
    vcomiss xmm2, [rbx+r11*8-256]
    vcvtdq2pd xmm2, xmm0
    vcvtdq2pd xmm2, [rbx]
    vcvtdq2pd xmm2, [rbx+r11*8+256]
    vcvtdq2pd xmm2, [rbx+r11*8-256]
    vcvtdq2pd ymm16, xmm0
    vcvtdq2pd ymm16, [rbx]
    vcvtdq2pd ymm16, [rbx+r11*8+256]
    vcvtdq2pd ymm16, [rbx+r11*8-256]
    vcvtdq2pd zmm24, ymm15
    vcvtdq2pd zmm24, [rbx]
    vcvtdq2pd zmm24, [rbx+r11*8+256]
    vcvtdq2pd zmm24, [rbx+r11*8-256]
    vcvtdq2pd xmm2, xmm0
    vcvtdq2pd xmm2, [rbx]
    vcvtdq2pd xmm2, [rbx+r11*8+256]
    vcvtdq2pd xmm2, [rbx+r11*8-256]
    vcvtdq2pd ymm16, xmm0
    vcvtdq2pd ymm16, [rbx]
    vcvtdq2pd ymm16, [rbx+r11*8+256]
    vcvtdq2pd ymm16, [rbx+r11*8-256]
    vcvtdq2pd zmm24, ymm15
    vcvtdq2pd zmm24, [rbx]
    vcvtdq2pd zmm24, [rbx+r11*8+256]
    vcvtdq2pd zmm24, [rbx+r11*8-256]
    vcvtdq2ps xmm2, xmm0
    vcvtdq2ps xmm2, [rbx]
    vcvtdq2ps xmm2, [rbx+r11*8+256]
    vcvtdq2ps xmm2, [rbx+r11*8-256]
    vcvtdq2ps ymm16, ymm15
    vcvtdq2ps ymm16, [rbx]
    vcvtdq2ps ymm16, [rbx+r11*8+256]
    vcvtdq2ps ymm16, [rbx+r11*8-256]
    vcvtdq2ps zmm24, zmm31
    vcvtdq2ps zmm24, [rbx]
    vcvtdq2ps zmm24, [rbx+r11*8+256]
    vcvtdq2ps zmm24, [rbx+r11*8-256]
    vcvtpd2dq xmm2, xmm0
    vcvtpd2dq xmm2, ymm15
    vcvtpd2dq xmm2, [rbx]
    vcvtpd2dq xmm2, [rbx+r11*8+256]
    vcvtpd2dq xmm2, [rbx+r11*8-256]
    vcvtpd2dq ymm16, zmm31
    vcvtpd2dq ymm16, [rbx]
    vcvtpd2dq ymm16, [rbx+r11*8+256]
    vcvtpd2dq ymm16, [rbx+r11*8-256]
    vcvtpd2ps xmm2, xmm0
    vcvtpd2ps xmm2, ymm15
    vcvtpd2ps xmm2, [rbx]
    vcvtpd2ps xmm2, [rbx+r11*8+256]
    vcvtpd2ps xmm2, [rbx+r11*8-256]
    vcvtpd2ps ymm16, zmm31
    vcvtpd2ps ymm16, [rbx]
    vcvtpd2ps ymm16, [rbx+r11*8+256]
    vcvtpd2ps ymm16, [rbx+r11*8-256]
    vcvtpd2ps xmm2, xmm0
    vcvtpd2ps xmm2, ymm15
    vcvtpd2ps xmm2, [rbx]
    vcvtpd2ps xmm2, [rbx+r11*8+256]
    vcvtpd2ps xmm2, [rbx+r11*8-256]
    vcvtpd2ps ymm16, zmm31
    vcvtpd2ps ymm16, [rbx]
    vcvtpd2ps ymm16, [rbx+r11*8+256]
    vcvtpd2ps ymm16, [rbx+r11*8-256]
    vcvtps2dq xmm2, xmm0
    vcvtps2dq xmm2, [rbx]
    vcvtps2dq xmm2, [rbx+r11*8+256]
    vcvtps2dq xmm2, [rbx+r11*8-256]
    vcvtps2dq ymm16, ymm15
    vcvtps2dq ymm16, [rbx]
    vcvtps2dq ymm16, [rbx+r11*8+256]
    vcvtps2dq ymm16, [rbx+r11*8-256]
    vcvtps2dq zmm24, zmm31
    vcvtps2dq zmm24, [rbx]
    vcvtps2dq zmm24, [rbx+r11*8+256]
    vcvtps2dq zmm24, [rbx+r11*8-256]
    vcvtps2pd xmm2, xmm0
    vcvtps2pd xmm2, [rbx]
    vcvtps2pd xmm2, [rbx+r11*8+256]
    vcvtps2pd xmm2, [rbx+r11*8-256]
    vcvtps2pd ymm16, xmm0
    vcvtps2pd ymm16, [rbx]
    vcvtps2pd ymm16, [rbx+r11*8+256]
    vcvtps2pd ymm16, [rbx+r11*8-256]
    vcvtps2pd zmm24, ymm15
    vcvtps2pd zmm24, [rbx]
    vcvtps2pd zmm24, [rbx+r11*8+256]
    vcvtps2pd zmm24, [rbx+r11*8-256]
    vcvtps2pd xmm2, xmm0
    vcvtps2pd xmm2, [rbx]
    vcvtps2pd xmm2, [rbx+r11*8+256]
    vcvtps2pd xmm2, [rbx+r11*8-256]
    vcvtps2pd ymm16, xmm0
    vcvtps2pd ymm16, [rbx]
    vcvtps2pd ymm16, [rbx+r11*8+256]
    vcvtps2pd ymm16, [rbx+r11*8-256]
    vcvtps2pd zmm24, ymm15
    vcvtps2pd zmm24, [rbx]
    vcvtps2pd zmm24, [rbx+r11*8+256]
    vcvtps2pd zmm24, [rbx+r11*8-256]
    vcvtsd2si ecx, xmm0
    vcvtsd2si ecx, [rbx]
    vcvtsd2si ecx, [rbx+r11*8+256]
    vcvtsd2si ecx, [rbx+r11*8-256]
    vcvtsd2si rcx, xmm0
    vcvtsd2si rcx, [rbx]
    vcvtsd2si rcx, [rbx+r11*8+256]
    vcvtsd2si rcx, [rbx+r11*8-256]
    vcvtsd2ss xmm2, xmm7, xmm0
    vcvtsd2ss xmm2, xmm7, [rbx]
    vcvtsd2ss xmm2, xmm7, [rbx+r11*8+256]
    vcvtsd2ss xmm2, xmm7, [rbx+r11*8-256]
    vcvtsi2sd xmm2, xmm7, ecx
    vcvtsi2sd xmm2, xmm7, rcx
    vcvtsi2sd xmm2, xmm7, [rbx]
    vcvtsi2sd xmm2, xmm7, [rbx+rsi*8+256]
    vcvtsi2ss xmm2, xmm7, ecx
    vcvtsi2ss xmm2, xmm7, rcx
    vcvtsi2ss xmm2, xmm7, [rbx]
    vcvtsi2ss xmm2, xmm7, [rbx+rsi*8+256]
    vcvtss2sd xmm2, xmm7, xmm0
    vcvtss2sd xmm2, xmm7, [rbx]
    vcvtss2sd xmm2, xmm7, [rbx+r11*8+256]
    vcvtss2sd xmm2, xmm7, [rbx+r11*8-256]
    vcvtss2si ecx, xmm0
    vcvtss2si ecx, [rbx]
    vcvtss2si ecx, [rbx+r11*8+256]
    vcvtss2si ecx, [rbx+r11*8-256]
    vcvtss2si rcx, xmm0
    vcvtss2si rcx, [rbx]
    vcvtss2si rcx, [rbx+r11*8+256]
    vcvtss2si rcx, [rbx+r11*8-256]
    vcvttpd2dq xmm2, xmm0
    vcvttpd2dq xmm2, ymm15
    vcvttpd2dq xmm2, [rbx]
    vcvttpd2dq xmm2, [rbx+r11*8+256]
    vcvttpd2dq xmm2, [rbx+r11*8-256]
    vcvttpd2dq ymm16, zmm31
    vcvttpd2dq ymm16, [rbx]
    vcvttpd2dq ymm16, [rbx+r11*8+256]
    vcvttpd2dq ymm16, [rbx+r11*8-256]
    vcvttps2dq xmm2, xmm0
    vcvttps2dq xmm2, [rbx]
    vcvttps2dq xmm2, [rbx+r11*8+256]
    vcvttps2dq xmm2, [rbx+r11*8-256]
    vcvttps2dq ymm16, ymm15
    vcvttps2dq ymm16, [rbx]
    vcvttps2dq ymm16, [rbx+r11*8+256]
    vcvttps2dq ymm16, [rbx+r11*8-256]
    vcvttps2dq zmm24, zmm31
    vcvttps2dq zmm24, [rbx]
    vcvttps2dq zmm24, [rbx+r11*8+256]
    vcvttps2dq zmm24, [rbx+r11*8-256]
    vcvttsd2si ecx, xmm0
    vcvttsd2si ecx, [rbx]
    vcvttsd2si ecx, [rbx+r11*8+256]
    vcvttsd2si ecx, [rbx+r11*8-256]
    vcvttsd2si rcx, xmm0
    vcvttsd2si rcx, [rbx]
    vcvttsd2si rcx, [rbx+r11*8+256]
    vcvttsd2si rcx, [rbx+r11*8-256]
    vcvttss2si ecx, xmm0
    vcvttss2si ecx, [rbx]
    vcvttss2si ecx, [rbx+r11*8+256]
    vcvttss2si ecx, [rbx+r11*8-256]
    vcvttss2si rcx, xmm0
    vcvttss2si rcx, [rbx]
    vcvttss2si rcx, [rbx+r11*8+256]
    vcvttss2si rcx, [rbx+r11*8-256]
    vdivpd xmm2, xmm7, xmm0
    vdivpd xmm2, xmm7, [rbx]
    vdivpd xmm2, xmm7, [rbx+r11*8+256]
    vdivpd xmm2, xmm7, [rbx+r11*8-256]
    vdivpd ymm16, ymm13, ymm15
    vdivpd ymm16, ymm13, [rbx]
    vdivpd ymm16, ymm13, [rbx+r11*8+256]
    vdivpd ymm16, ymm13, [rbx+r11*8-256]
    vdivpd zmm24, zmm24, zmm31
    vdivpd zmm24, zmm24, [rbx]
    vdivpd zmm24, zmm24, [rbx+r11*8+256]
    vdivpd zmm24, zmm24, [rbx+r11*8-256]
    vdivps xmm2, xmm7, xmm0
    vdivps xmm2, xmm7, [rbx]
    vdivps xmm2, xmm7, [rbx+r11*8+256]
    vdivps xmm2, xmm7, [rbx+r11*8-256]
    vdivps ymm16, ymm13, ymm15
    vdivps ymm16, ymm13, [rbx]
    vdivps ymm16, ymm13, [rbx+r11*8+256]
    vdivps ymm16, ymm13, [rbx+r11*8-256]
    vdivps zmm24, zmm24, zmm31
    vdivps zmm24, zmm24, [rbx]
    vdivps zmm24, zmm24, [rbx+r11*8+256]
    vdivps zmm24, zmm24, [rbx+r11*8-256]
    vdivsd xmm2, xmm7, xmm0
    vdivsd xmm2, xmm7, [rbx]
    vdivsd xmm2, xmm7, [rbx+r11*8+256]
    vdivsd xmm2, xmm7, [rbx+r11*8-256]
    vdivss xmm2, xmm7, xmm0
    vdivss xmm2, xmm7, [rbx]
    vdivss xmm2, xmm7, [rbx+r11*8+256]
    vdivss xmm2, xmm7, [rbx+r11*8-256]
    vdppd xmm2, xmm7, xmm0, 0x90
    vdppd xmm2, xmm7, [rbx], 0x90
    vdppd xmm2, xmm7, [rbx+r11*8+256], 0x90
    vdppd xmm2, xmm7, [rbx+r11*8-256], 0x90
    vdpps xmm2, xmm7, xmm0, 0x90
    vdpps xmm2, xmm7, [rbx], 0x90
    vdpps xmm2, xmm7, [rbx+r11*8+256], 0x90
    vdpps xmm2, xmm7, [rbx+r11*8-256], 0x90
    vextractps [rbx], xmm2, 0x90
    vextractps [rbx+rsi*8+256], xmm2, 0x90
    vextractps [rbx+rsi*8-256], xmm2, 0x90
    vextractps ecx, xmm2, 0x90
    vextractps rcx, xmm2, 0x90
    vhaddpd xmm2, xmm7, xmm0
    vhaddpd xmm2, xmm7, [rbx]
    vhaddpd xmm2, xmm7, [rbx+r11*8+256]
    vhaddpd xmm2, xmm7, [rbx+r11*8-256]
    vhaddps xmm2, xmm7, xmm0
    vhaddps xmm2, xmm7, [rbx]
    vhaddps xmm2, xmm7, [rbx+r11*8+256]
    vhaddps xmm2, xmm7, [rbx+r11*8-256]
    vhsubpd xmm2, xmm7, xmm0
    vhsubpd xmm2, xmm7, [rbx]
    vhsubpd xmm2, xmm7, [rbx+r11*8+256]
    vhsubpd xmm2, xmm7, [rbx+r11*8-256]
    vhsubps xmm2, xmm7, xmm0
    vhsubps xmm2, xmm7, [rbx]
    vhsubps xmm2, xmm7, [rbx+r11*8+256]
    vhsubps xmm2, xmm7, [rbx+r11*8-256]
    vinsertps xmm2, xmm7, [rbx], 0x90
    vinsertps xmm2, xmm7, [rbx+rsi*8+256], 0x90
    vinsertps xmm2, xmm7, [rbx+rsi*8-256], 0x90
    vinsertps xmm2, xmm7, xmm0, 0x90
    vlddqu xmm2, [rbx]
    vlddqu xmm2, [rbx+rsi*8+256]
    vlddqu xmm2, [rbx+rsi*8-256]
    vldmxcsr [rbx]
    vldmxcsr [rbx+rsi*8+256]
    vldmxcsr [rbx+rsi*8-256]
    vmaskmovdqu xmm2, xmm0
    vmaskmovpd xmm2, xmm7, [rbx]
    vmaskmovpd xmm2, xmm7, [rbx+rsi*8+256]
    vmaskmovpd xmm2, xmm7, [rbx+rsi*8-256]
    vmaskmovpd [rbx], xmm7, xmm2
    vmaskmovpd [rbx+rsi*8+256], xmm7, xmm2
    vmaskmovpd [rbx+rsi*8-256], xmm7, xmm2
    vmaskmovps xmm2, xmm7, [rbx]
    vmaskmovps xmm2, xmm7, [rbx+rsi*8+256]
    vmaskmovps xmm2, xmm7, [rbx+rsi*8-256]
    vmaskmovps [rbx], xmm7, xmm2
    vmaskmovps [rbx+rsi*8+256], xmm7, xmm2
    vmaskmovps [rbx+rsi*8-256], xmm7, xmm2
    vmaxpd xmm2, xmm7, xmm0
    vmaxpd xmm2, xmm7, [rbx]
    vmaxpd xmm2, xmm7, [rbx+r11*8+256]
    vmaxpd xmm2, xmm7, [rbx+r11*8-256]
    vmaxpd ymm16, ymm13, ymm15
    vmaxpd ymm16, ymm13, [rbx]
    vmaxpd ymm16, ymm13, [rbx+r11*8+256]
    vmaxpd ymm16, ymm13, [rbx+r11*8-256]
    vmaxpd zmm24, zmm24, zmm31
    vmaxpd zmm24, zmm24, [rbx]
    vmaxpd zmm24, zmm24, [rbx+r11*8+256]
    vmaxpd zmm24, zmm24, [rbx+r11*8-256]
    vmaxps xmm2, xmm7, xmm0
    vmaxps xmm2, xmm7, [rbx]
    vmaxps xmm2, xmm7, [rbx+r11*8+256]
    vmaxps xmm2, xmm7, [rbx+r11*8-256]
    vmaxps ymm16, ymm13, ymm15
    vmaxps ymm16, ymm13, [rbx]
    vmaxps ymm16, ymm13, [rbx+r11*8+256]
    vmaxps ymm16, ymm13, [rbx+r11*8-256]
    vmaxps zmm24, zmm24, zmm31
    vmaxps zmm24, zmm24, [rbx]
    vmaxps zmm24, zmm24, [rbx+r11*8+256]
    vmaxps zmm24, zmm24, [rbx+r11*8-256]
    vmaxsd xmm2, xmm7, xmm0
    vmaxsd xmm2, xmm7, [rbx]
    vmaxsd xmm2, xmm7, [rbx+r11*8+256]
    vmaxsd xmm2, xmm7, [rbx+r11*8-256]
    vmaxss xmm2, xmm7, xmm0
    vmaxss xmm2, xmm7, [rbx]
    vmaxss xmm2, xmm7, [rbx+r11*8+256]
    vmaxss xmm2, xmm7, [rbx+r11*8-256]
    vminpd xmm2, xmm7, xmm0
    vminpd xmm2, xmm7, [rbx]
    vminpd xmm2, xmm7, [rbx+r11*8+256]
    vminpd xmm2, xmm7, [rbx+r11*8-256]
    vminpd ymm16, ymm13, ymm15
    vminpd ymm16, ymm13, [rbx]
    vminpd ymm16, ymm13, [rbx+r11*8+256]
    vminpd ymm16, ymm13, [rbx+r11*8-256]
    vminpd zmm24, zmm24, zmm31
    vminpd zmm24, zmm24, [rbx]
    vminpd zmm24, zmm24, [rbx+r11*8+256]
    vminpd zmm24, zmm24, [rbx+r11*8-256]
    vminps xmm2, xmm7, xmm0
    vminps xmm2, xmm7, [rbx]
    vminps xmm2, xmm7, [rbx+r11*8+256]
    vminps xmm2, xmm7, [rbx+r11*8-256]
    vminps ymm16, ymm13, ymm15
    vminps ymm16, ymm13, [rbx]
    vminps ymm16, ymm13, [rbx+r11*8+256]
    vminps ymm16, ymm13, [rbx+r11*8-256]
    vminps zmm24, zmm24, zmm31
    vminps zmm24, zmm24, [rbx]
    vminps zmm24, zmm24, [rbx+r11*8+256]
    vminps zmm24, zmm24, [rbx+r11*8-256]
    vminsd xmm2, xmm7, xmm0
    vminsd xmm2, xmm7, [rbx]
    vminsd xmm2, xmm7, [rbx+r11*8+256]
    vminsd xmm2, xmm7, [rbx+r11*8-256]
    vminss xmm2, xmm7, xmm0
    vminss xmm2, xmm7, [rbx]
    vminss xmm2, xmm7, [rbx+r11*8+256]
    vminss xmm2, xmm7, [rbx+r11*8-256]
    vmovapd xmm2, xmm0
    vmovapd xmm2, [rbx]
    vmovapd xmm2, [rbx+r11*8+256]
    vmovapd xmm2, [rbx+r11*8-256]
    vmovapd ymm16, ymm15
    vmovapd ymm16, [rbx]
    vmovapd ymm16, [rbx+r11*8+256]
    vmovapd ymm16, [rbx+r11*8-256]
    vmovapd zmm24, zmm31
    vmovapd zmm24, [rbx]
    vmovapd zmm24, [rbx+r11*8+256]
    vmovapd zmm24, [rbx+r11*8-256]
    vmovapd xmm0, xmm2
    vmovapd ymm15, ymm16
    vmovapd zmm31, zmm24
    vmovapd [rbx], xmm2
    vmovapd [rbx], ymm16
    vmovapd [rbx], zmm24
    vmovapd [rbx+r11*8+256], xmm2
    vmovapd [rbx+r11*8+256], ymm16
    vmovapd [rbx+r11*8+256], zmm24
    vmovapd [rbx+r11*8-256], xmm2
    vmovapd [rbx+r11*8-256], ymm16
    vmovapd [rbx+r11*8-256], zmm24
    vmovaps xmm2, xmm0
    vmovaps xmm2, [rbx]
    vmovaps xmm2, [rbx+r11*8+256]
    vmovaps xmm2, [rbx+r11*8-256]
    vmovaps ymm16, ymm15
    vmovaps ymm16, [rbx]
    vmovaps ymm16, [rbx+r11*8+256]
    vmovaps ymm16, [rbx+r11*8-256]
    vmovaps zmm24, zmm31
    vmovaps zmm24, [rbx]
    vmovaps zmm24, [rbx+r11*8+256]
    vmovaps zmm24, [rbx+r11*8-256]
    vmovaps xmm0, xmm2
    vmovaps ymm15, ymm16
    vmovaps zmm31, zmm24
    vmovaps [rbx], xmm2
    vmovaps [rbx], ymm16
    vmovaps [rbx], zmm24
    vmovaps [rbx+r11*8+256], xmm2
    vmovaps [rbx+r11*8+256], ymm16
    vmovaps [rbx+r11*8+256], zmm24
    vmovaps [rbx+r11*8-256], xmm2
    vmovaps [rbx+r11*8-256], ymm16
    vmovaps [rbx+r11*8-256], zmm24
    vmovd xmm2, ecx
    vmovd xmm2, [rbx]
    vmovd xmm2, [rbx+rsi*8+256]
    vmovd ecx, xmm2
    vmovd [rbx], xmm2
    vmovd [rbx+rsi*8+256], xmm2
    vmovddup xmm2, xmm0
    vmovddup xmm2, [rbx]
    vmovddup xmm2, [rbx+r11*8+256]
    vmovddup xmm2, [rbx+r11*8-256]
    vmovddup ymm16, ymm15
    vmovddup ymm16, [rbx]
    vmovddup ymm16, [rbx+r11*8+256]
    vmovddup ymm16, [rbx+r11*8-256]
    vmovddup zmm24, zmm31
    vmovddup zmm24, [rbx]
    vmovddup zmm24, [rbx+r11*8+256]
    vmovddup zmm24, [rbx+r11*8-256]
    vmovddup xmm2, xmm0
    vmovddup xmm2, [rbx]
    vmovddup xmm2, [rbx+r11*8+256]
    vmovddup xmm2, [rbx+r11*8-256]
    vmovddup ymm16, ymm15
    vmovddup ymm16, [rbx]
    vmovddup ymm16, [rbx+r11*8+256]
    vmovddup ymm16, [rbx+r11*8-256]
    vmovddup zmm24, zmm31
    vmovddup zmm24, [rbx]
    vmovddup zmm24, [rbx+r11*8+256]
    vmovddup zmm24, [rbx+r11*8-256]
    vmovdqa xmm2, xmm0
    vmovdqa xmm2, [rbx]
    vmovdqa xmm2, [rbx+r11*8+256]
    vmovdqa xmm2, [rbx+r11*8-256]
    vmovdqa xmm0, xmm2
    vmovdqa [rbx], xmm2
    vmovdqa [rbx+r11*8+256], xmm2
    vmovdqa [rbx+r11*8-256], xmm2
    vmovdqu xmm2, xmm0
    vmovdqu xmm2, [rbx]
    vmovdqu xmm2, [rbx+r11*8+256]
    vmovdqu xmm2, [rbx+r11*8-256]
    vmovdqu xmm0, xmm2
    vmovdqu [rbx], xmm2
    vmovdqu [rbx+r11*8+256], xmm2
    vmovdqu [rbx+r11*8-256], xmm2
    vmovhlps xmm2, xmm7, xmm0
    vmovhpd xmm2, xmm7, [rbx]
    vmovhpd xmm2, xmm7, [rbx+rsi*8+256]
    vmovhpd xmm2, xmm7, [rbx+rsi*8-256]
    vmovhpd [rbx], xmm2
    vmovhpd [rbx+rsi*8+256], xmm2
    vmovhpd [rbx+rsi*8-256], xmm2
    vmovhps xmm2, xmm7, [rbx]
    vmovhps xmm2, xmm7, [rbx+rsi*8+256]
    vmovhps xmm2, xmm7, [rbx+rsi*8-256]
    vmovhps [rbx], xmm2
    vmovhps [rbx+rsi*8+256], xmm2
    vmovhps [rbx+rsi*8-256], xmm2
    vmovlhps xmm2, xmm7, xmm0
    vmovlpd xmm2, xmm7, [rbx]
    vmovlpd xmm2, xmm7, [rbx+rsi*8+256]
    vmovlpd xmm2, xmm7, [rbx+rsi*8-256]
    vmovlpd [rbx], xmm2
    vmovlpd [rbx+rsi*8+256], xmm2
    vmovlpd [rbx+rsi*8-256], xmm2
    vmovlps xmm2, xmm7, [rbx]
    vmovlps xmm2, xmm7, [rbx+rsi*8+256]
    vmovlps xmm2, xmm7, [rbx+rsi*8-256]
    vmovlps [rbx], xmm2
    vmovlps [rbx+rsi*8+256], xmm2
    vmovlps [rbx+rsi*8-256], xmm2
    vmovmskpd ecx, xmm0
    vmovmskpd ecx, ymm15
    vmovmskpd rcx, xmm0
    vmovmskpd rcx, ymm15
    vmovmskps ecx, xmm0
    vmovmskps ecx, ymm15
    vmovmskps rcx, xmm0
    vmovmskps rcx, ymm15
    vmovntdq [rbx], xmm2
    vmovntdq [rbx], ymm16
    vmovntdq [rbx], zmm24
    vmovntdq [rbx+rsi*8+256], xmm2
    vmovntdq [rbx+rsi*8+256], ymm16
    vmovntdq [rbx+rsi*8+256], zmm24
    vmovntdq [rbx+rsi*8-256], xmm2
    vmovntdq [rbx+rsi*8-256], ymm16
    vmovntdq [rbx+rsi*8-256], zmm24
    vmovntdqa xmm2, [rbx]
    vmovntdqa xmm2, [rbx+rsi*8+256]
    vmovntdqa xmm2, [rbx+rsi*8-256]
    vmovntdqa ymm16, [rbx]
    vmovntdqa ymm16, [rbx+rsi*8+256]
    vmovntdqa ymm16, [rbx+rsi*8-256]
    vmovntdqa zmm24, [rbx]
    vmovntdqa zmm24, [rbx+rsi*8+256]
    vmovntdqa zmm24, [rbx+rsi*8-256]
    vmovntpd [rbx], xmm2
    vmovntpd [rbx], ymm16
    vmovntpd [rbx], zmm24
    vmovntpd [rbx+rsi*8+256], xmm2
    vmovntpd [rbx+rsi*8+256], ymm16
    vmovntpd [rbx+rsi*8+256], zmm24
    vmovntpd [rbx+rsi*8-256], xmm2
    vmovntpd [rbx+rsi*8-256], ymm16
    vmovntpd [rbx+rsi*8-256], zmm24
    vmovntps [rbx], xmm2
    vmovntps [rbx], ymm16
    vmovntps [rbx], zmm24
    vmovntps [rbx+rsi*8+256], xmm2
    vmovntps [rbx+rsi*8+256], ymm16
    vmovntps [rbx+rsi*8+256], zmm24
    vmovntps [rbx+rsi*8-256], xmm2
    vmovntps [rbx+rsi*8-256], ymm16
    vmovntps [rbx+rsi*8-256], zmm24
    vmovq xmm2, rcx
    vmovq xmm2, [rbx]
    vmovq xmm2, [rbx+rsi*8+256]
    vmovq rcx, xmm2
    vmovq [rbx], xmm2
    vmovq [rbx+rsi*8+256], xmm2
    vmovq xmm2, xmm0
    vmovq xmm2, [rbx]
    vmovq xmm2, [rbx+r11*8+256]
    vmovq xmm2, [rbx+r11*8-256]
    vmovq xmm0, xmm2
    vmovq [rbx], xmm2
    vmovq [rbx+r11*8+256], xmm2
    vmovq [rbx+r11*8-256], xmm2
    vmovsd xmm2, xmm7, xmm0
    vmovsd xmm2, [rbx]
    vmovsd xmm2, [rbx+rsi*8+256]
    vmovsd xmm2, [rbx+rsi*8-256]
    vmovsd xmm0, xmm7, xmm2
    vmovsd [rbx], xmm2
    vmovsd [rbx+rsi*8+256], xmm2
    vmovsd [rbx+rsi*8-256], xmm2
    vmovshdup xmm2, xmm0
    vmovshdup xmm2, [rbx]
    vmovshdup xmm2, [rbx+r11*8+256]
    vmovshdup xmm2, [rbx+r11*8-256]
    vmovshdup ymm16, ymm15
    vmovshdup ymm16, [rbx]
    vmovshdup ymm16, [rbx+r11*8+256]
    vmovshdup ymm16, [rbx+r11*8-256]
    vmovshdup zmm24, zmm31
    vmovshdup zmm24, [rbx]
    vmovshdup zmm24, [rbx+r11*8+256]
    vmovshdup zmm24, [rbx+r11*8-256]
    vmovsldup xmm2, xmm0
    vmovsldup xmm2, [rbx]
    vmovsldup xmm2, [rbx+r11*8+256]
    vmovsldup xmm2, [rbx+r11*8-256]
    vmovsldup ymm16, ymm15
    vmovsldup ymm16, [rbx]
    vmovsldup ymm16, [rbx+r11*8+256]
    vmovsldup ymm16, [rbx+r11*8-256]
    vmovsldup zmm24, zmm31
    vmovsldup zmm24, [rbx]
    vmovsldup zmm24, [rbx+r11*8+256]
    vmovsldup zmm24, [rbx+r11*8-256]
    vmovss xmm2, xmm7, xmm0
    vmovss xmm2, [rbx]
    vmovss xmm2, [rbx+rsi*8+256]
    vmovss xmm2, [rbx+rsi*8-256]
    vmovss xmm0, xmm7, xmm2
    vmovss [rbx], xmm2
    vmovss [rbx+rsi*8+256], xmm2
    vmovss [rbx+rsi*8-256], xmm2
    vmovupd xmm2, xmm0
    vmovupd xmm2, [rbx]
    vmovupd xmm2, [rbx+r11*8+256]
    vmovupd xmm2, [rbx+r11*8-256]
    vmovupd ymm16, ymm15
    vmovupd ymm16, [rbx]
    vmovupd ymm16, [rbx+r11*8+256]
    vmovupd ymm16, [rbx+r11*8-256]
    vmovupd zmm24, zmm31
    vmovupd zmm24, [rbx]
    vmovupd zmm24, [rbx+r11*8+256]
    vmovupd zmm24, [rbx+r11*8-256]
    vmovupd xmm0, xmm2
    vmovupd ymm15, ymm16
    vmovupd zmm31, zmm24
    vmovupd [rbx], xmm2
    vmovupd [rbx], ymm16
    vmovupd [rbx], zmm24
    vmovupd [rbx+r11*8+256], xmm2
    vmovupd [rbx+r11*8+256], ymm16
    vmovupd [rbx+r11*8+256], zmm24
    vmovupd [rbx+r11*8-256], xmm2
    vmovupd [rbx+r11*8-256], ymm16
    vmovupd [rbx+r11*8-256], zmm24
    vmovups xmm2, xmm0
    vmovups xmm2, [rbx]
    vmovups xmm2, [rbx+r11*8+256]
    vmovups xmm2, [rbx+r11*8-256]
    vmovups ymm16, ymm15
    vmovups ymm16, [rbx]
    vmovups ymm16, [rbx+r11*8+256]
    vmovups ymm16, [rbx+r11*8-256]
    vmovups zmm24, zmm31
    vmovups zmm24, [rbx]
    vmovups zmm24, [rbx+r11*8+256]
    vmovups zmm24, [rbx+r11*8-256]
    vmovups xmm0, xmm2
    vmovups ymm15, ymm16
    vmovups zmm31, zmm24
    vmovups [rbx], xmm2
    vmovups [rbx], ymm16
    vmovups [rbx], zmm24
    vmovups [rbx+r11*8+256], xmm2
    vmovups [rbx+r11*8+256], ymm16
    vmovups [rbx+r11*8+256], zmm24
    vmovups [rbx+r11*8-256], xmm2
    vmovups [rbx+r11*8-256], ymm16
    vmovups [rbx+r11*8-256], zmm24
    vmpsadbw xmm2, xmm7, xmm0, 0x90
    vmpsadbw xmm2, xmm7, [rbx], 0x90
    vmpsadbw xmm2, xmm7, [rbx+r11*8+256], 0x90
    vmpsadbw xmm2, xmm7, [rbx+r11*8-256], 0x90
    vmulpd xmm2, xmm7, xmm0
    vmulpd xmm2, xmm7, [rbx]
    vmulpd xmm2, xmm7, [rbx+r11*8+256]
    vmulpd xmm2, xmm7, [rbx+r11*8-256]
    vmulpd ymm16, ymm13, ymm15
    vmulpd ymm16, ymm13, [rbx]
    vmulpd ymm16, ymm13, [rbx+r11*8+256]
    vmulpd ymm16, ymm13, [rbx+r11*8-256]
    vmulpd zmm24, zmm24, zmm31
    vmulpd zmm24, zmm24, [rbx]
    vmulpd zmm24, zmm24, [rbx+r11*8+256]
    vmulpd zmm24, zmm24, [rbx+r11*8-256]
    vmulps xmm2, xmm7, xmm0
    vmulps xmm2, xmm7, [rbx]
    vmulps xmm2, xmm7, [rbx+r11*8+256]
    vmulps xmm2, xmm7, [rbx+r11*8-256]
    vmulps ymm16, ymm13, ymm15
    vmulps ymm16, ymm13, [rbx]
    vmulps ymm16, ymm13, [rbx+r11*8+256]
    vmulps ymm16, ymm13, [rbx+r11*8-256]
    vmulps zmm24, zmm24, zmm31
    vmulps zmm24, zmm24, [rbx]
    vmulps zmm24, zmm24, [rbx+r11*8+256]
    vmulps zmm24, zmm24, [rbx+r11*8-256]
    vmulsd xmm2, xmm7, xmm0
    vmulsd xmm2, xmm7, [rbx]
    vmulsd xmm2, xmm7, [rbx+r11*8+256]
    vmulsd xmm2, xmm7, [rbx+r11*8-256]
    vmulss xmm2, xmm7, xmm0
    vmulss xmm2, xmm7, [rbx]
    vmulss xmm2, xmm7, [rbx+r11*8+256]
    vmulss xmm2, xmm7, [rbx+r11*8-256]
    vorpd xmm2, xmm7, xmm0
    vorpd xmm2, xmm7, [rbx]
    vorpd xmm2, xmm7, [rbx+r11*8+256]
    vorpd xmm2, xmm7, [rbx+r11*8-256]
    vorpd ymm16, ymm13, ymm15
    vorpd ymm16, ymm13, [rbx]
    vorpd ymm16, ymm13, [rbx+r11*8+256]
    vorpd ymm16, ymm13, [rbx+r11*8-256]
    vorpd zmm24, zmm24, zmm31
    vorpd zmm24, zmm24, [rbx]
    vorpd zmm24, zmm24, [rbx+r11*8+256]
    vorpd zmm24, zmm24, [rbx+r11*8-256]
    vorps xmm2, xmm7, xmm0
    vorps xmm2, xmm7, [rbx]
    vorps xmm2, xmm7, [rbx+r11*8+256]
    vorps xmm2, xmm7, [rbx+r11*8-256]
    vorps ymm16, ymm13, ymm15
    vorps ymm16, ymm13, [rbx]
    vorps ymm16, ymm13, [rbx+r11*8+256]
    vorps ymm16, ymm13, [rbx+r11*8-256]
    vorps zmm24, zmm24, zmm31
    vorps zmm24, zmm24, [rbx]
    vorps zmm24, zmm24, [rbx+r11*8+256]
    vorps zmm24, zmm24, [rbx+r11*8-256]
    vpabsb xmm2, xmm0
    vpabsb xmm2, [rbx]
    vpabsb xmm2, [rbx+r11*8+256]
    vpabsb xmm2, [rbx+r11*8-256]
    vpabsb ymm16, ymm15
    vpabsb ymm16, [rbx]
    vpabsb ymm16, [rbx+r11*8+256]
    vpabsb ymm16, [rbx+r11*8-256]
    vpabsb zmm24, zmm31
    vpabsb zmm24, [rbx]
    vpabsb zmm24, [rbx+r11*8+256]
    vpabsb zmm24, [rbx+r11*8-256]
    vpabsd xmm2, xmm0
    vpabsd xmm2, [rbx]
    vpabsd xmm2, [rbx+r11*8+256]
    vpabsd xmm2, [rbx+r11*8-256]
    vpabsd ymm16, ymm15
    vpabsd ymm16, [rbx]
    vpabsd ymm16, [rbx+r11*8+256]
    vpabsd ymm16, [rbx+r11*8-256]
    vpabsd zmm24, zmm31
    vpabsd zmm24, [rbx]
    vpabsd zmm24, [rbx+r11*8+256]
    vpabsd zmm24, [rbx+r11*8-256]
    vpabsw xmm2, xmm0
    vpabsw xmm2, [rbx]
    vpabsw xmm2, [rbx+r11*8+256]
    vpabsw xmm2, [rbx+r11*8-256]
    vpabsw ymm16, ymm15
    vpabsw ymm16, [rbx]
    vpabsw ymm16, [rbx+r11*8+256]
    vpabsw ymm16, [rbx+r11*8-256]
    vpabsw zmm24, zmm31
    vpabsw zmm24, [rbx]
    vpabsw zmm24, [rbx+r11*8+256]
    vpabsw zmm24, [rbx+r11*8-256]
    vpackssdw xmm2, xmm7, xmm0
    vpackssdw xmm2, xmm7, [rbx]
    vpackssdw xmm2, xmm7, [rbx+r11*8+256]
    vpackssdw xmm2, xmm7, [rbx+r11*8-256]
    vpackssdw ymm16, ymm13, ymm15
    vpackssdw ymm16, ymm13, [rbx]
    vpackssdw ymm16, ymm13, [rbx+r11*8+256]
    vpackssdw ymm16, ymm13, [rbx+r11*8-256]
    vpackssdw zmm24, zmm24, zmm31
    vpackssdw zmm24, zmm24, [rbx]
    vpackssdw zmm24, zmm24, [rbx+r11*8+256]
    vpackssdw zmm24, zmm24, [rbx+r11*8-256]
    vpacksswb xmm2, xmm7, xmm0
    vpacksswb xmm2, xmm7, [rbx]
    vpacksswb xmm2, xmm7, [rbx+r11*8+256]
    vpacksswb xmm2, xmm7, [rbx+r11*8-256]
    vpacksswb ymm16, ymm13, ymm15
    vpacksswb ymm16, ymm13, [rbx]
    vpacksswb ymm16, ymm13, [rbx+r11*8+256]
    vpacksswb ymm16, ymm13, [rbx+r11*8-256]
    vpacksswb zmm24, zmm24, zmm31
    vpacksswb zmm24, zmm24, [rbx]
    vpacksswb zmm24, zmm24, [rbx+r11*8+256]
    vpacksswb zmm24, zmm24, [rbx+r11*8-256]
    vpackusdw xmm2, xmm7, xmm0
    vpackusdw xmm2, xmm7, [rbx]
    vpackusdw xmm2, xmm7, [rbx+r11*8+256]
    vpackusdw xmm2, xmm7, [rbx+r11*8-256]
    vpackusdw ymm16, ymm13, ymm15
    vpackusdw ymm16, ymm13, [rbx]
    vpackusdw ymm16, ymm13, [rbx+r11*8+256]
    vpackusdw ymm16, ymm13, [rbx+r11*8-256]
    vpackusdw zmm24, zmm24, zmm31
    vpackusdw zmm24, zmm24, [rbx]
    vpackusdw zmm24, zmm24, [rbx+r11*8+256]
    vpackusdw zmm24, zmm24, [rbx+r11*8-256]
    vpackuswb xmm2, xmm7, xmm0
    vpackuswb xmm2, xmm7, [rbx]
    vpackuswb xmm2, xmm7, [rbx+r11*8+256]
    vpackuswb xmm2, xmm7, [rbx+r11*8-256]
    vpackuswb ymm16, ymm13, ymm15
    vpackuswb ymm16, ymm13, [rbx]
    vpackuswb ymm16, ymm13, [rbx+r11*8+256]
    vpackuswb ymm16, ymm13, [rbx+r11*8-256]
    vpackuswb zmm24, zmm24, zmm31
    vpackuswb zmm24, zmm24, [rbx]
    vpackuswb zmm24, zmm24, [rbx+r11*8+256]
    vpackuswb zmm24, zmm24, [rbx+r11*8-256]
    vpaddb xmm2, xmm7, xmm0
    vpaddb xmm2, xmm7, [rbx]
    vpaddb xmm2, xmm7, [rbx+r11*8+256]
    vpaddb xmm2, xmm7, [rbx+r11*8-256]
    vpaddb ymm16, ymm13, ymm15
    vpaddb ymm16, ymm13, [rbx]
    vpaddb ymm16, ymm13, [rbx+r11*8+256]
    vpaddb ymm16, ymm13, [rbx+r11*8-256]
    vpaddb zmm24, zmm24, zmm31
    vpaddb zmm24, zmm24, [rbx]
    vpaddb zmm24, zmm24, [rbx+r11*8+256]
    vpaddb zmm24, zmm24, [rbx+r11*8-256]
    vpaddd xmm2, xmm7, xmm0
    vpaddd xmm2, xmm7, [rbx]
    vpaddd xmm2, xmm7, [rbx+r11*8+256]
    vpaddd xmm2, xmm7, [rbx+r11*8-256]
    vpaddd ymm16, ymm13, ymm15
    vpaddd ymm16, ymm13, [rbx]
    vpaddd ymm16, ymm13, [rbx+r11*8+256]
    vpaddd ymm16, ymm13, [rbx+r11*8-256]
    vpaddd zmm24, zmm24, zmm31
    vpaddd zmm24, zmm24, [rbx]
    vpaddd zmm24, zmm24, [rbx+r11*8+256]
    vpaddd zmm24, zmm24, [rbx+r11*8-256]
    vpaddq xmm2, xmm7, xmm0
    vpaddq xmm2, xmm7, [rbx]
    vpaddq xmm2, xmm7, [rbx+r11*8+256]
    vpaddq xmm2, xmm7, [rbx+r11*8-256]
    vpaddq ymm16, ymm13, ymm15
    vpaddq ymm16, ymm13, [rbx]
    vpaddq ymm16, ymm13, [rbx+r11*8+256]
    vpaddq ymm16, ymm13, [rbx+r11*8-256]
    vpaddq zmm24, zmm24, zmm31
    vpaddq zmm24, zmm24, [rbx]
    vpaddq zmm24, zmm24, [rbx+r11*8+256]
    vpaddq zmm24, zmm24, [rbx+r11*8-256]
    vpaddsb xmm2, xmm7, xmm0
    vpaddsb xmm2, xmm7, [rbx]
    vpaddsb xmm2, xmm7, [rbx+r11*8+256]
    vpaddsb xmm2, xmm7, [rbx+r11*8-256]
    vpaddsb ymm16, ymm13, ymm15
    vpaddsb ymm16, ymm13, [rbx]
    vpaddsb ymm16, ymm13, [rbx+r11*8+256]
    vpaddsb ymm16, ymm13, [rbx+r11*8-256]
    vpaddsb zmm24, zmm24, zmm31
    vpaddsb zmm24, zmm24, [rbx]
    vpaddsb zmm24, zmm24, [rbx+r11*8+256]
    vpaddsb zmm24, zmm24, [rbx+r11*8-256]
    vpaddsw xmm2, xmm7, xmm0
    vpaddsw xmm2, xmm7, [rbx]
    vpaddsw xmm2, xmm7, [rbx+r11*8+256]
    vpaddsw xmm2, xmm7, [rbx+r11*8-256]
    vpaddsw ymm16, ymm13, ymm15
    vpaddsw ymm16, ymm13, [rbx]
    vpaddsw ymm16, ymm13, [rbx+r11*8+256]
    vpaddsw ymm16, ymm13, [rbx+r11*8-256]
    vpaddsw zmm24, zmm24, zmm31
    vpaddsw zmm24, zmm24, [rbx]
    vpaddsw zmm24, zmm24, [rbx+r11*8+256]
    vpaddsw zmm24, zmm24, [rbx+r11*8-256]
    vpaddusb xmm2, xmm7, xmm0
    vpaddusb xmm2, xmm7, [rbx]
    vpaddusb xmm2, xmm7, [rbx+r11*8+256]
    vpaddusb xmm2, xmm7, [rbx+r11*8-256]
    vpaddusb ymm16, ymm13, ymm15
    vpaddusb ymm16, ymm13, [rbx]
    vpaddusb ymm16, ymm13, [rbx+r11*8+256]
    vpaddusb ymm16, ymm13, [rbx+r11*8-256]
    vpaddusb zmm24, zmm24, zmm31
    vpaddusb zmm24, zmm24, [rbx]
    vpaddusb zmm24, zmm24, [rbx+r11*8+256]
    vpaddusb zmm24, zmm24, [rbx+r11*8-256]
    vpaddusw xmm2, xmm7, xmm0
    vpaddusw xmm2, xmm7, [rbx]
    vpaddusw xmm2, xmm7, [rbx+r11*8+256]
    vpaddusw xmm2, xmm7, [rbx+r11*8-256]
    vpaddusw ymm16, ymm13, ymm15
    vpaddusw ymm16, ymm13, [rbx]
    vpaddusw ymm16, ymm13, [rbx+r11*8+256]
    vpaddusw ymm16, ymm13, [rbx+r11*8-256]
    vpaddusw zmm24, zmm24, zmm31
    vpaddusw zmm24, zmm24, [rbx]
    vpaddusw zmm24, zmm24, [rbx+r11*8+256]
    vpaddusw zmm24, zmm24, [rbx+r11*8-256]
    vpaddw xmm2, xmm7, xmm0
    vpaddw xmm2, xmm7, [rbx]
    vpaddw xmm2, xmm7, [rbx+r11*8+256]
    vpaddw xmm2, xmm7, [rbx+r11*8-256]
    vpaddw ymm16, ymm13, ymm15
    vpaddw ymm16, ymm13, [rbx]
    vpaddw ymm16, ymm13, [rbx+r11*8+256]
    vpaddw ymm16, ymm13, [rbx+r11*8-256]
    vpaddw zmm24, zmm24, zmm31
    vpaddw zmm24, zmm24, [rbx]
    vpaddw zmm24, zmm24, [rbx+r11*8+256]
    vpaddw zmm24, zmm24, [rbx+r11*8-256]
    vpalignr xmm2, xmm7, xmm0, 0x90
    vpalignr xmm2, xmm7, [rbx], 0x90
    vpalignr xmm2, xmm7, [rbx+r11*8+256], 0x90
    vpalignr xmm2, xmm7, [rbx+r11*8-256], 0x90
    vpalignr ymm16, ymm13, ymm15, 0x90
    vpalignr ymm16, ymm13, [rbx], 0x90
    vpalignr ymm16, ymm13, [rbx+r11*8+256], 0x90
    vpalignr ymm16, ymm13, [rbx+r11*8-256], 0x90
    vpalignr zmm24, zmm24, zmm31, 0x90
    vpalignr zmm24, zmm24, [rbx], 0x90
    vpalignr zmm24, zmm24, [rbx+r11*8+256], 0x90
    vpalignr zmm24, zmm24, [rbx+r11*8-256], 0x90
    vpand xmm2, xmm7, xmm0
    vpand xmm2, xmm7, [rbx]
    vpand xmm2, xmm7, [rbx+r11*8+256]
    vpand xmm2, xmm7, [rbx+r11*8-256]
    vpandn xmm2, xmm7, xmm0
    vpandn xmm2, xmm7, [rbx]
    vpandn xmm2, xmm7, [rbx+r11*8+256]
    vpandn xmm2, xmm7, [rbx+r11*8-256]
    vpavgb xmm2, xmm7, xmm0
    vpavgb xmm2, xmm7, [rbx]
    vpavgb xmm2, xmm7, [rbx+r11*8+256]
    vpavgb xmm2, xmm7, [rbx+r11*8-256]
    vpavgb ymm16, ymm13, ymm15
    vpavgb ymm16, ymm13, [rbx]
    vpavgb ymm16, ymm13, [rbx+r11*8+256]
    vpavgb ymm16, ymm13, [rbx+r11*8-256]
    vpavgb zmm24, zmm24, zmm31
    vpavgb zmm24, zmm24, [rbx]
    vpavgb zmm24, zmm24, [rbx+r11*8+256]
    vpavgb zmm24, zmm24, [rbx+r11*8-256]
    vpavgw xmm2, xmm7, xmm0
    vpavgw xmm2, xmm7, [rbx]
    vpavgw xmm2, xmm7, [rbx+r11*8+256]
    vpavgw xmm2, xmm7, [rbx+r11*8-256]
    vpavgw ymm16, ymm13, ymm15
    vpavgw ymm16, ymm13, [rbx]
    vpavgw ymm16, ymm13, [rbx+r11*8+256]
    vpavgw ymm16, ymm13, [rbx+r11*8-256]
    vpavgw zmm24, zmm24, zmm31
    vpavgw zmm24, zmm24, [rbx]
    vpavgw zmm24, zmm24, [rbx+r11*8+256]
    vpavgw zmm24, zmm24, [rbx+r11*8-256]
    vpblendvb xmm2, xmm7, xmm0, xmm3
    vpblendvb xmm2, xmm7, [rbx], xmm3
    vpblendvb xmm2, xmm7, [rbx+r11*8+256], xmm3
    vpblendvb xmm2, xmm7, [rbx+r11*8-256], xmm3
    vpblendw xmm2, xmm7, xmm0, 0x90
    vpblendw xmm2, xmm7, [rbx], 0x90
    vpblendw xmm2, xmm7, [rbx+r11*8+256], 0x90
    vpblendw xmm2, xmm7, [rbx+r11*8-256], 0x90
    vpcmpeqb xmm2, xmm7, xmm0
    vpcmpeqb xmm2, xmm7, [rbx]
    vpcmpeqb xmm2, xmm7, [rbx+r11*8+256]
    vpcmpeqb xmm2, xmm7, [rbx+r11*8-256]
    vpcmpeqd xmm2, xmm7, xmm0
    vpcmpeqd xmm2, xmm7, [rbx]
    vpcmpeqd xmm2, xmm7, [rbx+r11*8+256]
    vpcmpeqd xmm2, xmm7, [rbx+r11*8-256]
    vpcmpeqq xmm2, xmm7, xmm0
    vpcmpeqq xmm2, xmm7, [rbx]
    vpcmpeqq xmm2, xmm7, [rbx+r11*8+256]
    vpcmpeqq xmm2, xmm7, [rbx+r11*8-256]
    vpcmpeqw xmm2, xmm7, xmm0
    vpcmpeqw xmm2, xmm7, [rbx]
    vpcmpeqw xmm2, xmm7, [rbx+r11*8+256]
    vpcmpeqw xmm2, xmm7, [rbx+r11*8-256]
    vpcmpestri xmm2, xmm0, 0x90
    vpcmpestri xmm2, [rbx], 0x90
    vpcmpestri xmm2, [rbx+r11*8+256], 0x90
    vpcmpestri xmm2, [rbx+r11*8-256], 0x90
    vpcmpestrm xmm2, xmm0, 0x90
    vpcmpestrm xmm2, [rbx], 0x90
    vpcmpestrm xmm2, [rbx+r11*8+256], 0x90
    vpcmpestrm xmm2, [rbx+r11*8-256], 0x90
    vpcmpgtb xmm2, xmm7, xmm0
    vpcmpgtb xmm2, xmm7, [rbx]
    vpcmpgtb xmm2, xmm7, [rbx+r11*8+256]
    vpcmpgtb xmm2, xmm7, [rbx+r11*8-256]
    vpcmpgtd xmm2, xmm7, xmm0
    vpcmpgtd xmm2, xmm7, [rbx]
    vpcmpgtd xmm2, xmm7, [rbx+r11*8+256]
    vpcmpgtd xmm2, xmm7, [rbx+r11*8-256]
    vpcmpgtq xmm2, xmm7, xmm0
    vpcmpgtq xmm2, xmm7, [rbx]
    vpcmpgtq xmm2, xmm7, [rbx+r11*8+256]
    vpcmpgtq xmm2, xmm7, [rbx+r11*8-256]
    vpcmpgtw xmm2, xmm7, xmm0
    vpcmpgtw xmm2, xmm7, [rbx]
    vpcmpgtw xmm2, xmm7, [rbx+r11*8+256]
    vpcmpgtw xmm2, xmm7, [rbx+r11*8-256]
    vpcmpistri xmm2, xmm0, 0x90
    vpcmpistri xmm2, [rbx], 0x90
    vpcmpistri xmm2, [rbx+r11*8+256], 0x90
    vpcmpistri xmm2, [rbx+r11*8-256], 0x90
    vpcmpistrm xmm2, xmm0, 0x90
    vpcmpistrm xmm2, [rbx], 0x90
    vpcmpistrm xmm2, [rbx+r11*8+256], 0x90
    vpcmpistrm xmm2, [rbx+r11*8-256], 0x90
    vpermilpd xmm2, xmm7, xmm0
    vpermilpd xmm2, xmm7, [rbx]
    vpermilpd xmm2, xmm7, [rbx+r11*8+256]
    vpermilpd xmm2, xmm7, [rbx+r11*8-256]
    vpermilpd ymm16, ymm13, ymm15
    vpermilpd ymm16, ymm13, [rbx]
    vpermilpd ymm16, ymm13, [rbx+r11*8+256]
    vpermilpd ymm16, ymm13, [rbx+r11*8-256]
    vpermilpd zmm24, zmm24, zmm31
    vpermilpd zmm24, zmm24, [rbx]
    vpermilpd zmm24, zmm24, [rbx+r11*8+256]
    vpermilpd zmm24, zmm24, [rbx+r11*8-256]
    vpermilpd xmm2, xmm0, 0x90
    vpermilpd xmm2, [rbx], 0x90
    vpermilpd xmm2, [rbx+r11*8+256], 0x90
    vpermilpd xmm2, [rbx+r11*8-256], 0x90
    vpermilpd ymm16, ymm15, 0x90
    vpermilpd ymm16, [rbx], 0x90
    vpermilpd ymm16, [rbx+r11*8+256], 0x90
    vpermilpd ymm16, [rbx+r11*8-256], 0x90
    vpermilpd zmm24, zmm31, 0x90
    vpermilpd zmm24, [rbx], 0x90
    vpermilpd zmm24, [rbx+r11*8+256], 0x90
    vpermilpd zmm24, [rbx+r11*8-256], 0x90
    vpermilps xmm2, xmm7, xmm0
    vpermilps xmm2, xmm7, [rbx]
    vpermilps xmm2, xmm7, [rbx+r11*8+256]
    vpermilps xmm2, xmm7, [rbx+r11*8-256]
    vpermilps ymm16, ymm13, ymm15
    vpermilps ymm16, ymm13, [rbx]
    vpermilps ymm16, ymm13, [rbx+r11*8+256]
    vpermilps ymm16, ymm13, [rbx+r11*8-256]
    vpermilps zmm24, zmm24, zmm31
    vpermilps zmm24, zmm24, [rbx]
    vpermilps zmm24, zmm24, [rbx+r11*8+256]
    vpermilps zmm24, zmm24, [rbx+r11*8-256]
    vpermilps xmm2, xmm0, 0x90
    vpermilps xmm2, [rbx], 0x90
    vpermilps xmm2, [rbx+r11*8+256], 0x90
    vpermilps xmm2, [rbx+r11*8-256], 0x90
    vpermilps ymm16, ymm15, 0x90
    vpermilps ymm16, [rbx], 0x90
    vpermilps ymm16, [rbx+r11*8+256], 0x90
    vpermilps ymm16, [rbx+r11*8-256], 0x90
    vpermilps zmm24, zmm31, 0x90
    vpermilps zmm24, [rbx], 0x90
    vpermilps zmm24, [rbx+r11*8+256], 0x90
    vpermilps zmm24, [rbx+r11*8-256], 0x90
    vpextrb [rbx], xmm2, 0x90
    vpextrb [rbx+rsi*8+256], xmm2, 0x90
    vpextrb [rbx+rsi*8-256], xmm2, 0x90
    vpextrb cl, xmm2, 0x90
    vpextrb cx, xmm2, 0x90
    vpextrb ecx, xmm2, 0x90
    vpextrb rcx, xmm2, 0x90
    vpextrd ecx, xmm2, 0x90
    vpextrd rcx, xmm2, 0x90
    vpextrd [rbx], xmm2, 0x90
    vpextrd [rbx+rsi*8+256], xmm2, 0x90
    vpextrq rcx, xmm2, 0x90
    vpextrq [rbx], xmm2, 0x90
    vpextrq [rbx+rsi*8+256], xmm2, 0x90
    vpextrw cx, xmm0, 0x90
    vpextrw ecx, xmm0, 0x90
    vpextrw rcx, xmm0, 0x90
    vpextrw [rbx], xmm2, 0x90
    vpextrw [rbx+rsi*8+256], xmm2, 0x90
    vpextrw [rbx+rsi*8-256], xmm2, 0x90
    vpextrw cx, xmm2, 0x90
    vpextrw ecx, xmm2, 0x90
    vpextrw rcx, xmm2, 0x90
    vphaddd xmm2, xmm7, xmm0
    vphaddd xmm2, xmm7, [rbx]
    vphaddd xmm2, xmm7, [rbx+r11*8+256]
    vphaddd xmm2, xmm7, [rbx+r11*8-256]
    vphaddsw xmm2, xmm7, xmm0
    vphaddsw xmm2, xmm7, [rbx]
    vphaddsw xmm2, xmm7, [rbx+r11*8+256]
    vphaddsw xmm2, xmm7, [rbx+r11*8-256]
    vphaddw xmm2, xmm7, xmm0
    vphaddw xmm2, xmm7, [rbx]
    vphaddw xmm2, xmm7, [rbx+r11*8+256]
    vphaddw xmm2, xmm7, [rbx+r11*8-256]
    vphminposuw xmm2, xmm0
    vphminposuw xmm2, [rbx]
    vphminposuw xmm2, [rbx+r11*8+256]
    vphminposuw xmm2, [rbx+r11*8-256]
    vphsubd xmm2, xmm7, xmm0
    vphsubd xmm2, xmm7, [rbx]
    vphsubd xmm2, xmm7, [rbx+r11*8+256]
    vphsubd xmm2, xmm7, [rbx+r11*8-256]
    vphsubsw xmm2, xmm7, xmm0
    vphsubsw xmm2, xmm7, [rbx]
    vphsubsw xmm2, xmm7, [rbx+r11*8+256]
    vphsubsw xmm2, xmm7, [rbx+r11*8-256]
    vphsubw xmm2, xmm7, xmm0
    vphsubw xmm2, xmm7, [rbx]
    vphsubw xmm2, xmm7, [rbx+r11*8+256]
    vphsubw xmm2, xmm7, [rbx+r11*8-256]
    vpinsrb xmm2, xmm7, [rbx], 0x90
    vpinsrb xmm2, xmm7, [rbx+rsi*8+256], 0x90
    vpinsrb xmm2, xmm7, [rbx+rsi*8-256], 0x90
    vpinsrb xmm2, xmm7, cl, 0x90
    vpinsrb xmm2, xmm7, ecx, 0x90
    vpinsrd xmm2, xmm7, ecx, 0x90
    vpinsrd xmm2, xmm7, [rbx], 0x90
    vpinsrd xmm2, xmm7, [rbx+rsi*8+256], 0x90
    vpinsrq xmm2, xmm7, rcx, 0x90
    vpinsrq xmm2, xmm7, [rbx], 0x90
    vpinsrq xmm2, xmm7, [rbx+rsi*8+256], 0x90
    vpinsrw xmm2, xmm7, [rbx], 0x90
    vpinsrw xmm2, xmm7, [rbx+rsi*8+256], 0x90
    vpinsrw xmm2, xmm7, [rbx+rsi*8-256], 0x90
    vpinsrw xmm2, xmm7, cx, 0x90
    vpinsrw xmm2, xmm7, ecx, 0x90
    vpmaddubsw xmm2, xmm7, xmm0
    vpmaddubsw xmm2, xmm7, [rbx]
    vpmaddubsw xmm2, xmm7, [rbx+r11*8+256]
    vpmaddubsw xmm2, xmm7, [rbx+r11*8-256]
    vpmaddubsw ymm16, ymm13, ymm15
    vpmaddubsw ymm16, ymm13, [rbx]
    vpmaddubsw ymm16, ymm13, [rbx+r11*8+256]
    vpmaddubsw ymm16, ymm13, [rbx+r11*8-256]
    vpmaddubsw zmm24, zmm24, zmm31
    vpmaddubsw zmm24, zmm24, [rbx]
    vpmaddubsw zmm24, zmm24, [rbx+r11*8+256]
    vpmaddubsw zmm24, zmm24, [rbx+r11*8-256]
    vpmaddwd xmm2, xmm7, xmm0
    vpmaddwd xmm2, xmm7, [rbx]
    vpmaddwd xmm2, xmm7, [rbx+r11*8+256]
    vpmaddwd xmm2, xmm7, [rbx+r11*8-256]
    vpmaddwd ymm16, ymm13, ymm15
    vpmaddwd ymm16, ymm13, [rbx]
    vpmaddwd ymm16, ymm13, [rbx+r11*8+256]
    vpmaddwd ymm16, ymm13, [rbx+r11*8-256]
    vpmaddwd zmm24, zmm24, zmm31
    vpmaddwd zmm24, zmm24, [rbx]
    vpmaddwd zmm24, zmm24, [rbx+r11*8+256]
    vpmaddwd zmm24, zmm24, [rbx+r11*8-256]
    vpmaxsb xmm2, xmm7, xmm0
    vpmaxsb xmm2, xmm7, [rbx]
    vpmaxsb xmm2, xmm7, [rbx+r11*8+256]
    vpmaxsb xmm2, xmm7, [rbx+r11*8-256]
    vpmaxsb ymm16, ymm13, ymm15
    vpmaxsb ymm16, ymm13, [rbx]
    vpmaxsb ymm16, ymm13, [rbx+r11*8+256]
    vpmaxsb ymm16, ymm13, [rbx+r11*8-256]
    vpmaxsb zmm24, zmm24, zmm31
    vpmaxsb zmm24, zmm24, [rbx]
    vpmaxsb zmm24, zmm24, [rbx+r11*8+256]
    vpmaxsb zmm24, zmm24, [rbx+r11*8-256]
    vpmaxsd xmm2, xmm7, xmm0
    vpmaxsd xmm2, xmm7, [rbx]
    vpmaxsd xmm2, xmm7, [rbx+r11*8+256]
    vpmaxsd xmm2, xmm7, [rbx+r11*8-256]
    vpmaxsd ymm16, ymm13, ymm15
    vpmaxsd ymm16, ymm13, [rbx]
    vpmaxsd ymm16, ymm13, [rbx+r11*8+256]
    vpmaxsd ymm16, ymm13, [rbx+r11*8-256]
    vpmaxsd zmm24, zmm24, zmm31
    vpmaxsd zmm24, zmm24, [rbx]
    vpmaxsd zmm24, zmm24, [rbx+r11*8+256]
    vpmaxsd zmm24, zmm24, [rbx+r11*8-256]
    vpmaxsw xmm2, xmm7, xmm0
    vpmaxsw xmm2, xmm7, [rbx]
    vpmaxsw xmm2, xmm7, [rbx+r11*8+256]
    vpmaxsw xmm2, xmm7, [rbx+r11*8-256]
    vpmaxsw ymm16, ymm13, ymm15
    vpmaxsw ymm16, ymm13, [rbx]
    vpmaxsw ymm16, ymm13, [rbx+r11*8+256]
    vpmaxsw ymm16, ymm13, [rbx+r11*8-256]
    vpmaxsw zmm24, zmm24, zmm31
    vpmaxsw zmm24, zmm24, [rbx]
    vpmaxsw zmm24, zmm24, [rbx+r11*8+256]
    vpmaxsw zmm24, zmm24, [rbx+r11*8-256]
    vpmaxub xmm2, xmm7, xmm0
    vpmaxub xmm2, xmm7, [rbx]
    vpmaxub xmm2, xmm7, [rbx+r11*8+256]
    vpmaxub xmm2, xmm7, [rbx+r11*8-256]
    vpmaxub ymm16, ymm13, ymm15
    vpmaxub ymm16, ymm13, [rbx]
    vpmaxub ymm16, ymm13, [rbx+r11*8+256]
    vpmaxub ymm16, ymm13, [rbx+r11*8-256]
    vpmaxub zmm24, zmm24, zmm31
    vpmaxub zmm24, zmm24, [rbx]
    vpmaxub zmm24, zmm24, [rbx+r11*8+256]
    vpmaxub zmm24, zmm24, [rbx+r11*8-256]
    vpmaxud xmm2, xmm7, xmm0
    vpmaxud xmm2, xmm7, [rbx]
    vpmaxud xmm2, xmm7, [rbx+r11*8+256]
    vpmaxud xmm2, xmm7, [rbx+r11*8-256]
    vpmaxud ymm16, ymm13, ymm15
    vpmaxud ymm16, ymm13, [rbx]
    vpmaxud ymm16, ymm13, [rbx+r11*8+256]
    vpmaxud ymm16, ymm13, [rbx+r11*8-256]
    vpmaxud zmm24, zmm24, zmm31
    vpmaxud zmm24, zmm24, [rbx]
    vpmaxud zmm24, zmm24, [rbx+r11*8+256]
    vpmaxud zmm24, zmm24, [rbx+r11*8-256]
    vpmaxuw xmm2, xmm7, xmm0
    vpmaxuw xmm2, xmm7, [rbx]
    vpmaxuw xmm2, xmm7, [rbx+r11*8+256]
    vpmaxuw xmm2, xmm7, [rbx+r11*8-256]
    vpmaxuw ymm16, ymm13, ymm15
    vpmaxuw ymm16, ymm13, [rbx]
    vpmaxuw ymm16, ymm13, [rbx+r11*8+256]
    vpmaxuw ymm16, ymm13, [rbx+r11*8-256]
    vpmaxuw zmm24, zmm24, zmm31
    vpmaxuw zmm24, zmm24, [rbx]
    vpmaxuw zmm24, zmm24, [rbx+r11*8+256]
    vpmaxuw zmm24, zmm24, [rbx+r11*8-256]
    vpminsb xmm2, xmm7, xmm0
    vpminsb xmm2, xmm7, [rbx]
    vpminsb xmm2, xmm7, [rbx+r11*8+256]
    vpminsb xmm2, xmm7, [rbx+r11*8-256]
    vpminsb ymm16, ymm13, ymm15
    vpminsb ymm16, ymm13, [rbx]
    vpminsb ymm16, ymm13, [rbx+r11*8+256]
    vpminsb ymm16, ymm13, [rbx+r11*8-256]
    vpminsb zmm24, zmm24, zmm31
    vpminsb zmm24, zmm24, [rbx]
    vpminsb zmm24, zmm24, [rbx+r11*8+256]
    vpminsb zmm24, zmm24, [rbx+r11*8-256]
    vpminsd xmm2, xmm7, xmm0
    vpminsd xmm2, xmm7, [rbx]
    vpminsd xmm2, xmm7, [rbx+r11*8+256]
    vpminsd xmm2, xmm7, [rbx+r11*8-256]
    vpminsd ymm16, ymm13, ymm15
    vpminsd ymm16, ymm13, [rbx]
    vpminsd ymm16, ymm13, [rbx+r11*8+256]
    vpminsd ymm16, ymm13, [rbx+r11*8-256]
    vpminsd zmm24, zmm24, zmm31
    vpminsd zmm24, zmm24, [rbx]
    vpminsd zmm24, zmm24, [rbx+r11*8+256]
    vpminsd zmm24, zmm24, [rbx+r11*8-256]
    vpminsw xmm2, xmm7, xmm0
    vpminsw xmm2, xmm7, [rbx]
    vpminsw xmm2, xmm7, [rbx+r11*8+256]
    vpminsw xmm2, xmm7, [rbx+r11*8-256]
    vpminsw ymm16, ymm13, ymm15
    vpminsw ymm16, ymm13, [rbx]
    vpminsw ymm16, ymm13, [rbx+r11*8+256]
    vpminsw ymm16, ymm13, [rbx+r11*8-256]
    vpminsw zmm24, zmm24, zmm31
    vpminsw zmm24, zmm24, [rbx]
    vpminsw zmm24, zmm24, [rbx+r11*8+256]
    vpminsw zmm24, zmm24, [rbx+r11*8-256]
    vpminub xmm2, xmm7, xmm0
    vpminub xmm2, xmm7, [rbx]
    vpminub xmm2, xmm7, [rbx+r11*8+256]
    vpminub xmm2, xmm7, [rbx+r11*8-256]
    vpminub ymm16, ymm13, ymm15
    vpminub ymm16, ymm13, [rbx]
    vpminub ymm16, ymm13, [rbx+r11*8+256]
    vpminub ymm16, ymm13, [rbx+r11*8-256]
    vpminub zmm24, zmm24, zmm31
    vpminub zmm24, zmm24, [rbx]
    vpminub zmm24, zmm24, [rbx+r11*8+256]
    vpminub zmm24, zmm24, [rbx+r11*8-256]
    vpminud xmm2, xmm7, xmm0
    vpminud xmm2, xmm7, [rbx]
    vpminud xmm2, xmm7, [rbx+r11*8+256]
    vpminud xmm2, xmm7, [rbx+r11*8-256]
    vpminud ymm16, ymm13, ymm15
    vpminud ymm16, ymm13, [rbx]
    vpminud ymm16, ymm13, [rbx+r11*8+256]
    vpminud ymm16, ymm13, [rbx+r11*8-256]
    vpminud zmm24, zmm24, zmm31
    vpminud zmm24, zmm24, [rbx]
    vpminud zmm24, zmm24, [rbx+r11*8+256]
    vpminud zmm24, zmm24, [rbx+r11*8-256]
    vpminuw xmm2, xmm7, xmm0
    vpminuw xmm2, xmm7, [rbx]
    vpminuw xmm2, xmm7, [rbx+r11*8+256]
    vpminuw xmm2, xmm7, [rbx+r11*8-256]
    vpminuw ymm16, ymm13, ymm15
    vpminuw ymm16, ymm13, [rbx]
    vpminuw ymm16, ymm13, [rbx+r11*8+256]
    vpminuw ymm16, ymm13, [rbx+r11*8-256]
    vpminuw zmm24, zmm24, zmm31
    vpminuw zmm24, zmm24, [rbx]
    vpminuw zmm24, zmm24, [rbx+r11*8+256]
    vpminuw zmm24, zmm24, [rbx+r11*8-256]
    vpmovmskb ecx, xmm0
    vpmovmskb ecx, ymm15
    vpmovmskb rcx, xmm0
    vpmovmskb rcx, ymm15
    vpmovsxbd xmm2, xmm0
    vpmovsxbd xmm2, [rbx]
    vpmovsxbd xmm2, [rbx+r11*8+256]
    vpmovsxbd xmm2, [rbx+r11*8-256]
    vpmovsxbd ymm16, xmm0
    vpmovsxbd ymm16, [rbx]
    vpmovsxbd ymm16, [rbx+r11*8+256]
    vpmovsxbd ymm16, [rbx+r11*8-256]
    vpmovsxbd zmm24, xmm0
    vpmovsxbd zmm24, [rbx]
    vpmovsxbd zmm24, [rbx+r11*8+256]
    vpmovsxbd zmm24, [rbx+r11*8-256]
    vpmovsxbq xmm2, xmm0
    vpmovsxbq xmm2, [rbx]
    vpmovsxbq xmm2, [rbx+r11*8+256]
    vpmovsxbq xmm2, [rbx+r11*8-256]
    vpmovsxbq ymm16, xmm0
    vpmovsxbq ymm16, [rbx]
    vpmovsxbq ymm16, [rbx+r11*8+256]
    vpmovsxbq ymm16, [rbx+r11*8-256]
    vpmovsxbq zmm24, xmm0
    vpmovsxbq zmm24, [rbx]
    vpmovsxbq zmm24, [rbx+r11*8+256]
    vpmovsxbq zmm24, [rbx+r11*8-256]
    vpmovsxbw xmm2, xmm0
    vpmovsxbw xmm2, [rbx]
    vpmovsxbw xmm2, [rbx+r11*8+256]
    vpmovsxbw xmm2, [rbx+r11*8-256]
    vpmovsxbw ymm16, xmm0
    vpmovsxbw ymm16, [rbx]
    vpmovsxbw ymm16, [rbx+r11*8+256]
    vpmovsxbw ymm16, [rbx+r11*8-256]
    vpmovsxbw zmm24, ymm15
    vpmovsxbw zmm24, [rbx]
    vpmovsxbw zmm24, [rbx+r11*8+256]
    vpmovsxbw zmm24, [rbx+r11*8-256]
    vpmovsxdq xmm2, xmm0
    vpmovsxdq xmm2, [rbx]
    vpmovsxdq xmm2, [rbx+r11*8+256]
    vpmovsxdq xmm2, [rbx+r11*8-256]
    vpmovsxdq ymm16, xmm0
    vpmovsxdq ymm16, [rbx]
    vpmovsxdq ymm16, [rbx+r11*8+256]
    vpmovsxdq ymm16, [rbx+r11*8-256]
    vpmovsxdq zmm24, ymm15
    vpmovsxdq zmm24, [rbx]
    vpmovsxdq zmm24, [rbx+r11*8+256]
    vpmovsxdq zmm24, [rbx+r11*8-256]
    vpmovsxwd xmm2, xmm0
    vpmovsxwd xmm2, [rbx]
    vpmovsxwd xmm2, [rbx+r11*8+256]
    vpmovsxwd xmm2, [rbx+r11*8-256]
    vpmovsxwd ymm16, xmm0
    vpmovsxwd ymm16, [rbx]
    vpmovsxwd ymm16, [rbx+r11*8+256]
    vpmovsxwd ymm16, [rbx+r11*8-256]
    vpmovsxwd zmm24, ymm15
    vpmovsxwd zmm24, [rbx]
    vpmovsxwd zmm24, [rbx+r11*8+256]
    vpmovsxwd zmm24, [rbx+r11*8-256]
    vpmovsxwq xmm2, xmm0
    vpmovsxwq xmm2, [rbx]
    vpmovsxwq xmm2, [rbx+r11*8+256]
    vpmovsxwq xmm2, [rbx+r11*8-256]
    vpmovsxwq ymm16, xmm0
    vpmovsxwq ymm16, [rbx]
    vpmovsxwq ymm16, [rbx+r11*8+256]
    vpmovsxwq ymm16, [rbx+r11*8-256]
    vpmovsxwq zmm24, xmm0
    vpmovsxwq zmm24, [rbx]
    vpmovsxwq zmm24, [rbx+r11*8+256]
    vpmovsxwq zmm24, [rbx+r11*8-256]
    vpmovzxbd xmm2, xmm0
    vpmovzxbd xmm2, [rbx]
    vpmovzxbd xmm2, [rbx+r11*8+256]
    vpmovzxbd xmm2, [rbx+r11*8-256]
    vpmovzxbd ymm16, xmm0
    vpmovzxbd ymm16, [rbx]
    vpmovzxbd ymm16, [rbx+r11*8+256]
    vpmovzxbd ymm16, [rbx+r11*8-256]
    vpmovzxbd zmm24, xmm0
    vpmovzxbd zmm24, [rbx]
    vpmovzxbd zmm24, [rbx+r11*8+256]
    vpmovzxbd zmm24, [rbx+r11*8-256]
    vpmovzxbq xmm2, xmm0
    vpmovzxbq xmm2, [rbx]
    vpmovzxbq xmm2, [rbx+r11*8+256]
    vpmovzxbq xmm2, [rbx+r11*8-256]
    vpmovzxbq ymm16, xmm0
    vpmovzxbq ymm16, [rbx]
    vpmovzxbq ymm16, [rbx+r11*8+256]
    vpmovzxbq ymm16, [rbx+r11*8-256]
    vpmovzxbq zmm24, xmm0
    vpmovzxbq zmm24, [rbx]
    vpmovzxbq zmm24, [rbx+r11*8+256]
    vpmovzxbq zmm24, [rbx+r11*8-256]
    vpmovzxbw xmm2, xmm0
    vpmovzxbw xmm2, [rbx]
    vpmovzxbw xmm2, [rbx+r11*8+256]
    vpmovzxbw xmm2, [rbx+r11*8-256]
    vpmovzxbw ymm16, xmm0
    vpmovzxbw ymm16, [rbx]
    vpmovzxbw ymm16, [rbx+r11*8+256]
    vpmovzxbw ymm16, [rbx+r11*8-256]
    vpmovzxbw zmm24, ymm15
    vpmovzxbw zmm24, [rbx]
    vpmovzxbw zmm24, [rbx+r11*8+256]
    vpmovzxbw zmm24, [rbx+r11*8-256]
    vpmovzxdq xmm2, xmm0
    vpmovzxdq xmm2, [rbx]
    vpmovzxdq xmm2, [rbx+r11*8+256]
    vpmovzxdq xmm2, [rbx+r11*8-256]
    vpmovzxdq ymm16, xmm0
    vpmovzxdq ymm16, [rbx]
    vpmovzxdq ymm16, [rbx+r11*8+256]
    vpmovzxdq ymm16, [rbx+r11*8-256]
    vpmovzxdq zmm24, ymm15
    vpmovzxdq zmm24, [rbx]
    vpmovzxdq zmm24, [rbx+r11*8+256]
    vpmovzxdq zmm24, [rbx+r11*8-256]
    vpmovzxwd xmm2, xmm0
    vpmovzxwd xmm2, [rbx]
    vpmovzxwd xmm2, [rbx+r11*8+256]
    vpmovzxwd xmm2, [rbx+r11*8-256]
    vpmovzxwd ymm16, xmm0
    vpmovzxwd ymm16, [rbx]
    vpmovzxwd ymm16, [rbx+r11*8+256]
    vpmovzxwd ymm16, [rbx+r11*8-256]
    vpmovzxwd zmm24, ymm15
    vpmovzxwd zmm24, [rbx]
    vpmovzxwd zmm24, [rbx+r11*8+256]
    vpmovzxwd zmm24, [rbx+r11*8-256]
    vpmovzxwq xmm2, xmm0
    vpmovzxwq xmm2, [rbx]
    vpmovzxwq xmm2, [rbx+r11*8+256]
    vpmovzxwq xmm2, [rbx+r11*8-256]
    vpmovzxwq ymm16, xmm0
    vpmovzxwq ymm16, [rbx]
    vpmovzxwq ymm16, [rbx+r11*8+256]
    vpmovzxwq ymm16, [rbx+r11*8-256]
    vpmovzxwq zmm24, xmm0
    vpmovzxwq zmm24, [rbx]
    vpmovzxwq zmm24, [rbx+r11*8+256]
    vpmovzxwq zmm24, [rbx+r11*8-256]
    vpmuldq xmm2, xmm7, xmm0
    vpmuldq xmm2, xmm7, [rbx]
    vpmuldq xmm2, xmm7, [rbx+r11*8+256]
    vpmuldq xmm2, xmm7, [rbx+r11*8-256]
    vpmuldq ymm16, ymm13, ymm15
    vpmuldq ymm16, ymm13, [rbx]
    vpmuldq ymm16, ymm13, [rbx+r11*8+256]
    vpmuldq ymm16, ymm13, [rbx+r11*8-256]
    vpmuldq zmm24, zmm24, zmm31
    vpmuldq zmm24, zmm24, [rbx]
    vpmuldq zmm24, zmm24, [rbx+r11*8+256]
    vpmuldq zmm24, zmm24, [rbx+r11*8-256]
    vpmulhrsw xmm2, xmm7, xmm0
    vpmulhrsw xmm2, xmm7, [rbx]
    vpmulhrsw xmm2, xmm7, [rbx+r11*8+256]
    vpmulhrsw xmm2, xmm7, [rbx+r11*8-256]
    vpmulhrsw ymm16, ymm13, ymm15
    vpmulhrsw ymm16, ymm13, [rbx]
    vpmulhrsw ymm16, ymm13, [rbx+r11*8+256]
    vpmulhrsw ymm16, ymm13, [rbx+r11*8-256]
    vpmulhrsw zmm24, zmm24, zmm31
    vpmulhrsw zmm24, zmm24, [rbx]
    vpmulhrsw zmm24, zmm24, [rbx+r11*8+256]
    vpmulhrsw zmm24, zmm24, [rbx+r11*8-256]
    vpmulhuw xmm2, xmm7, xmm0
    vpmulhuw xmm2, xmm7, [rbx]
    vpmulhuw xmm2, xmm7, [rbx+r11*8+256]
    vpmulhuw xmm2, xmm7, [rbx+r11*8-256]
    vpmulhuw ymm16, ymm13, ymm15
    vpmulhuw ymm16, ymm13, [rbx]
    vpmulhuw ymm16, ymm13, [rbx+r11*8+256]
    vpmulhuw ymm16, ymm13, [rbx+r11*8-256]
    vpmulhuw zmm24, zmm24, zmm31
    vpmulhuw zmm24, zmm24, [rbx]
    vpmulhuw zmm24, zmm24, [rbx+r11*8+256]
    vpmulhuw zmm24, zmm24, [rbx+r11*8-256]
    vpmulhw xmm2, xmm7, xmm0
    vpmulhw xmm2, xmm7, [rbx]
    vpmulhw xmm2, xmm7, [rbx+r11*8+256]
    vpmulhw xmm2, xmm7, [rbx+r11*8-256]
    vpmulhw ymm16, ymm13, ymm15
    vpmulhw ymm16, ymm13, [rbx]
    vpmulhw ymm16, ymm13, [rbx+r11*8+256]
    vpmulhw ymm16, ymm13, [rbx+r11*8-256]
    vpmulhw zmm24, zmm24, zmm31
    vpmulhw zmm24, zmm24, [rbx]
    vpmulhw zmm24, zmm24, [rbx+r11*8+256]
    vpmulhw zmm24, zmm24, [rbx+r11*8-256]
    vpmulld xmm2, xmm7, xmm0
    vpmulld xmm2, xmm7, [rbx]
    vpmulld xmm2, xmm7, [rbx+r11*8+256]
    vpmulld xmm2, xmm7, [rbx+r11*8-256]
    vpmulld ymm16, ymm13, ymm15
    vpmulld ymm16, ymm13, [rbx]
    vpmulld ymm16, ymm13, [rbx+r11*8+256]
    vpmulld ymm16, ymm13, [rbx+r11*8-256]
    vpmulld zmm24, zmm24, zmm31
    vpmulld zmm24, zmm24, [rbx]
    vpmulld zmm24, zmm24, [rbx+r11*8+256]
    vpmulld zmm24, zmm24, [rbx+r11*8-256]
    vpmullw xmm2, xmm7, xmm0
    vpmullw xmm2, xmm7, [rbx]
    vpmullw xmm2, xmm7, [rbx+r11*8+256]
    vpmullw xmm2, xmm7, [rbx+r11*8-256]
    vpmullw ymm16, ymm13, ymm15
    vpmullw ymm16, ymm13, [rbx]
    vpmullw ymm16, ymm13, [rbx+r11*8+256]
    vpmullw ymm16, ymm13, [rbx+r11*8-256]
    vpmullw zmm24, zmm24, zmm31
    vpmullw zmm24, zmm24, [rbx]
    vpmullw zmm24, zmm24, [rbx+r11*8+256]
    vpmullw zmm24, zmm24, [rbx+r11*8-256]
    vpmuludq xmm2, xmm7, xmm0
    vpmuludq xmm2, xmm7, [rbx]
    vpmuludq xmm2, xmm7, [rbx+r11*8+256]
    vpmuludq xmm2, xmm7, [rbx+r11*8-256]
    vpmuludq ymm16, ymm13, ymm15
    vpmuludq ymm16, ymm13, [rbx]
    vpmuludq ymm16, ymm13, [rbx+r11*8+256]
    vpmuludq ymm16, ymm13, [rbx+r11*8-256]
    vpmuludq zmm24, zmm24, zmm31
    vpmuludq zmm24, zmm24, [rbx]
    vpmuludq zmm24, zmm24, [rbx+r11*8+256]
    vpmuludq zmm24, zmm24, [rbx+r11*8-256]
    vpor xmm2, xmm7, xmm0
    vpor xmm2, xmm7, [rbx]
    vpor xmm2, xmm7, [rbx+r11*8+256]
    vpor xmm2, xmm7, [rbx+r11*8-256]
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
    vpshufb ymm16, ymm13, ymm15
    vpshufb ymm16, ymm13, [rbx]
    vpshufb ymm16, ymm13, [rbx+r11*8+256]
    vpshufb ymm16, ymm13, [rbx+r11*8-256]
    vpshufb zmm24, zmm24, zmm31
    vpshufb zmm24, zmm24, [rbx]
    vpshufb zmm24, zmm24, [rbx+r11*8+256]
    vpshufb zmm24, zmm24, [rbx+r11*8-256]
    vpshufd xmm2, xmm0, 0x90
    vpshufd xmm2, [rbx], 0x90
    vpshufd xmm2, [rbx+r11*8+256], 0x90
    vpshufd xmm2, [rbx+r11*8-256], 0x90
    vpshufd ymm16, ymm15, 0x90
    vpshufd ymm16, [rbx], 0x90
    vpshufd ymm16, [rbx+r11*8+256], 0x90
    vpshufd ymm16, [rbx+r11*8-256], 0x90
    vpshufd zmm24, zmm31, 0x90
    vpshufd zmm24, [rbx], 0x90
    vpshufd zmm24, [rbx+r11*8+256], 0x90
    vpshufd zmm24, [rbx+r11*8-256], 0x90
    vpshufhw xmm2, xmm0, 0x90
    vpshufhw xmm2, [rbx], 0x90
    vpshufhw xmm2, [rbx+r11*8+256], 0x90
    vpshufhw xmm2, [rbx+r11*8-256], 0x90
    vpshufhw ymm16, ymm15, 0x90
    vpshufhw ymm16, [rbx], 0x90
    vpshufhw ymm16, [rbx+r11*8+256], 0x90
    vpshufhw ymm16, [rbx+r11*8-256], 0x90
    vpshufhw zmm24, zmm31, 0x90
    vpshufhw zmm24, [rbx], 0x90
    vpshufhw zmm24, [rbx+r11*8+256], 0x90
    vpshufhw zmm24, [rbx+r11*8-256], 0x90
    vpshuflw xmm2, xmm0, 0x90
    vpshuflw xmm2, [rbx], 0x90
    vpshuflw xmm2, [rbx+r11*8+256], 0x90
    vpshuflw xmm2, [rbx+r11*8-256], 0x90
    vpshuflw ymm16, ymm15, 0x90
    vpshuflw ymm16, [rbx], 0x90
    vpshuflw ymm16, [rbx+r11*8+256], 0x90
    vpshuflw ymm16, [rbx+r11*8-256], 0x90
    vpshuflw zmm24, zmm31, 0x90
    vpshuflw zmm24, [rbx], 0x90
    vpshuflw zmm24, [rbx+r11*8+256], 0x90
    vpshuflw zmm24, [rbx+r11*8-256], 0x90
    vpsignb xmm2, xmm7, xmm0
    vpsignb xmm2, xmm7, [rbx]
    vpsignb xmm2, xmm7, [rbx+r11*8+256]
    vpsignb xmm2, xmm7, [rbx+r11*8-256]
    vpsignd xmm2, xmm7, xmm0
    vpsignd xmm2, xmm7, [rbx]
    vpsignd xmm2, xmm7, [rbx+r11*8+256]
    vpsignd xmm2, xmm7, [rbx+r11*8-256]
    vpsignw xmm2, xmm7, xmm0
    vpsignw xmm2, xmm7, [rbx]
    vpsignw xmm2, xmm7, [rbx+r11*8+256]
    vpsignw xmm2, xmm7, [rbx+r11*8-256]
    vpslld xmm7, xmm0, 0x90
    vpslld ymm13, ymm15, 0x90
    vpslld zmm24, zmm31, 0x90
    vpslld xmm2, xmm7, xmm0
    vpslld xmm2, xmm7, [rbx]
    vpslld xmm2, xmm7, [rbx+r11*8+256]
    vpslld xmm2, xmm7, [rbx+r11*8-256]
    vpslld ymm16, ymm13, xmm0
    vpslld ymm16, ymm13, [rbx]
    vpslld ymm16, ymm13, [rbx+r11*8+256]
    vpslld ymm16, ymm13, [rbx+r11*8-256]
    vpslld zmm24, zmm24, xmm0
    vpslld zmm24, zmm24, [rbx]
    vpslld zmm24, zmm24, [rbx+r11*8+256]
    vpslld zmm24, zmm24, [rbx+r11*8-256]
    vpslldq xmm7, xmm0, 0x90
    vpslldq ymm13, ymm15, 0x90
    vpslldq zmm24, zmm31, 0x90
    vpsllq xmm7, xmm0, 0x90
    vpsllq ymm13, ymm15, 0x90
    vpsllq zmm24, zmm31, 0x90
    vpsllq xmm2, xmm7, xmm0
    vpsllq xmm2, xmm7, [rbx]
    vpsllq xmm2, xmm7, [rbx+r11*8+256]
    vpsllq xmm2, xmm7, [rbx+r11*8-256]
    vpsllq ymm16, ymm13, xmm0
    vpsllq ymm16, ymm13, [rbx]
    vpsllq ymm16, ymm13, [rbx+r11*8+256]
    vpsllq ymm16, ymm13, [rbx+r11*8-256]
    vpsllq zmm24, zmm24, xmm0
    vpsllq zmm24, zmm24, [rbx]
    vpsllq zmm24, zmm24, [rbx+r11*8+256]
    vpsllq zmm24, zmm24, [rbx+r11*8-256]
    vpsllw xmm7, xmm0, 0x90
    vpsllw ymm13, ymm15, 0x90
    vpsllw zmm24, zmm31, 0x90
    vpsllw xmm2, xmm7, xmm0
    vpsllw xmm2, xmm7, [rbx]
    vpsllw xmm2, xmm7, [rbx+r11*8+256]
    vpsllw xmm2, xmm7, [rbx+r11*8-256]
    vpsllw ymm16, ymm13, xmm0
    vpsllw ymm16, ymm13, [rbx]
    vpsllw ymm16, ymm13, [rbx+r11*8+256]
    vpsllw ymm16, ymm13, [rbx+r11*8-256]
    vpsllw zmm24, zmm24, xmm0
    vpsllw zmm24, zmm24, [rbx]
    vpsllw zmm24, zmm24, [rbx+r11*8+256]
    vpsllw zmm24, zmm24, [rbx+r11*8-256]
    vpsrad xmm7, xmm0, 0x90
    vpsrad ymm13, ymm15, 0x90
    vpsrad zmm24, zmm31, 0x90
    vpsrad xmm2, xmm7, xmm0
    vpsrad xmm2, xmm7, [rbx]
    vpsrad xmm2, xmm7, [rbx+r11*8+256]
    vpsrad xmm2, xmm7, [rbx+r11*8-256]
    vpsrad ymm16, ymm13, xmm0
    vpsrad ymm16, ymm13, [rbx]
    vpsrad ymm16, ymm13, [rbx+r11*8+256]
    vpsrad ymm16, ymm13, [rbx+r11*8-256]
    vpsrad zmm24, zmm24, xmm0
    vpsrad zmm24, zmm24, [rbx]
    vpsrad zmm24, zmm24, [rbx+r11*8+256]
    vpsrad zmm24, zmm24, [rbx+r11*8-256]
    vpsraw xmm7, xmm0, 0x90
    vpsraw ymm13, ymm15, 0x90
    vpsraw zmm24, zmm31, 0x90
    vpsraw xmm2, xmm7, xmm0
    vpsraw xmm2, xmm7, [rbx]
    vpsraw xmm2, xmm7, [rbx+r11*8+256]
    vpsraw xmm2, xmm7, [rbx+r11*8-256]
    vpsraw ymm16, ymm13, xmm0
    vpsraw ymm16, ymm13, [rbx]
    vpsraw ymm16, ymm13, [rbx+r11*8+256]
    vpsraw ymm16, ymm13, [rbx+r11*8-256]
    vpsraw zmm24, zmm24, xmm0
    vpsraw zmm24, zmm24, [rbx]
    vpsraw zmm24, zmm24, [rbx+r11*8+256]
    vpsraw zmm24, zmm24, [rbx+r11*8-256]
    vpsrld xmm7, xmm0, 0x90
    vpsrld ymm13, ymm15, 0x90
    vpsrld zmm24, zmm31, 0x90
    vpsrld xmm2, xmm7, xmm0
    vpsrld xmm2, xmm7, [rbx]
    vpsrld xmm2, xmm7, [rbx+r11*8+256]
    vpsrld xmm2, xmm7, [rbx+r11*8-256]
    vpsrld ymm16, ymm13, xmm0
    vpsrld ymm16, ymm13, [rbx]
    vpsrld ymm16, ymm13, [rbx+r11*8+256]
    vpsrld ymm16, ymm13, [rbx+r11*8-256]
    vpsrld zmm24, zmm24, xmm0
    vpsrld zmm24, zmm24, [rbx]
    vpsrld zmm24, zmm24, [rbx+r11*8+256]
    vpsrld zmm24, zmm24, [rbx+r11*8-256]
    vpsrldq xmm7, xmm0, 0x90
    vpsrldq ymm13, ymm15, 0x90
    vpsrldq zmm24, zmm31, 0x90
    vpsrlq xmm7, xmm0, 0x90
    vpsrlq ymm13, ymm15, 0x90
    vpsrlq zmm24, zmm31, 0x90
    vpsrlq xmm2, xmm7, xmm0
    vpsrlq xmm2, xmm7, [rbx]
    vpsrlq xmm2, xmm7, [rbx+r11*8+256]
    vpsrlq xmm2, xmm7, [rbx+r11*8-256]
    vpsrlq ymm16, ymm13, xmm0
    vpsrlq ymm16, ymm13, [rbx]
    vpsrlq ymm16, ymm13, [rbx+r11*8+256]
    vpsrlq ymm16, ymm13, [rbx+r11*8-256]
    vpsrlq zmm24, zmm24, xmm0
    vpsrlq zmm24, zmm24, [rbx]
    vpsrlq zmm24, zmm24, [rbx+r11*8+256]
    vpsrlq zmm24, zmm24, [rbx+r11*8-256]
    vpsrlw xmm7, xmm0, 0x90
    vpsrlw ymm13, ymm15, 0x90
    vpsrlw zmm24, zmm31, 0x90
    vpsrlw xmm2, xmm7, xmm0
    vpsrlw xmm2, xmm7, [rbx]
    vpsrlw xmm2, xmm7, [rbx+r11*8+256]
    vpsrlw xmm2, xmm7, [rbx+r11*8-256]
    vpsrlw ymm16, ymm13, xmm0
    vpsrlw ymm16, ymm13, [rbx]
    vpsrlw ymm16, ymm13, [rbx+r11*8+256]
    vpsrlw ymm16, ymm13, [rbx+r11*8-256]
    vpsrlw zmm24, zmm24, xmm0
    vpsrlw zmm24, zmm24, [rbx]
    vpsrlw zmm24, zmm24, [rbx+r11*8+256]
    vpsrlw zmm24, zmm24, [rbx+r11*8-256]
    vpsubb xmm2, xmm7, xmm0
    vpsubb xmm2, xmm7, [rbx]
    vpsubb xmm2, xmm7, [rbx+r11*8+256]
    vpsubb xmm2, xmm7, [rbx+r11*8-256]
    vpsubb ymm16, ymm13, ymm15
    vpsubb ymm16, ymm13, [rbx]
    vpsubb ymm16, ymm13, [rbx+r11*8+256]
    vpsubb ymm16, ymm13, [rbx+r11*8-256]
    vpsubb zmm24, zmm24, zmm31
    vpsubb zmm24, zmm24, [rbx]
    vpsubb zmm24, zmm24, [rbx+r11*8+256]
    vpsubb zmm24, zmm24, [rbx+r11*8-256]
    vpsubd xmm2, xmm7, xmm0
    vpsubd xmm2, xmm7, [rbx]
    vpsubd xmm2, xmm7, [rbx+r11*8+256]
    vpsubd xmm2, xmm7, [rbx+r11*8-256]
    vpsubd ymm16, ymm13, ymm15
    vpsubd ymm16, ymm13, [rbx]
    vpsubd ymm16, ymm13, [rbx+r11*8+256]
    vpsubd ymm16, ymm13, [rbx+r11*8-256]
    vpsubd zmm24, zmm24, zmm31
    vpsubd zmm24, zmm24, [rbx]
    vpsubd zmm24, zmm24, [rbx+r11*8+256]
    vpsubd zmm24, zmm24, [rbx+r11*8-256]
    vpsubq xmm2, xmm7, xmm0
    vpsubq xmm2, xmm7, [rbx]
    vpsubq xmm2, xmm7, [rbx+r11*8+256]
    vpsubq xmm2, xmm7, [rbx+r11*8-256]
    vpsubq ymm16, ymm13, ymm15
    vpsubq ymm16, ymm13, [rbx]
    vpsubq ymm16, ymm13, [rbx+r11*8+256]
    vpsubq ymm16, ymm13, [rbx+r11*8-256]
    vpsubq zmm24, zmm24, zmm31
    vpsubq zmm24, zmm24, [rbx]
    vpsubq zmm24, zmm24, [rbx+r11*8+256]
    vpsubq zmm24, zmm24, [rbx+r11*8-256]
    vpsubsb xmm2, xmm7, xmm0
    vpsubsb xmm2, xmm7, [rbx]
    vpsubsb xmm2, xmm7, [rbx+r11*8+256]
    vpsubsb xmm2, xmm7, [rbx+r11*8-256]
    vpsubsb ymm16, ymm13, ymm15
    vpsubsb ymm16, ymm13, [rbx]
    vpsubsb ymm16, ymm13, [rbx+r11*8+256]
    vpsubsb ymm16, ymm13, [rbx+r11*8-256]
    vpsubsb zmm24, zmm24, zmm31
    vpsubsb zmm24, zmm24, [rbx]
    vpsubsb zmm24, zmm24, [rbx+r11*8+256]
    vpsubsb zmm24, zmm24, [rbx+r11*8-256]
    vpsubsw xmm2, xmm7, xmm0
    vpsubsw xmm2, xmm7, [rbx]
    vpsubsw xmm2, xmm7, [rbx+r11*8+256]
    vpsubsw xmm2, xmm7, [rbx+r11*8-256]
    vpsubsw ymm16, ymm13, ymm15
    vpsubsw ymm16, ymm13, [rbx]
    vpsubsw ymm16, ymm13, [rbx+r11*8+256]
    vpsubsw ymm16, ymm13, [rbx+r11*8-256]
    vpsubsw zmm24, zmm24, zmm31
    vpsubsw zmm24, zmm24, [rbx]
    vpsubsw zmm24, zmm24, [rbx+r11*8+256]
    vpsubsw zmm24, zmm24, [rbx+r11*8-256]
    vpsubusb xmm2, xmm7, xmm0
    vpsubusb xmm2, xmm7, [rbx]
    vpsubusb xmm2, xmm7, [rbx+r11*8+256]
    vpsubusb xmm2, xmm7, [rbx+r11*8-256]
    vpsubusb ymm16, ymm13, ymm15
    vpsubusb ymm16, ymm13, [rbx]
    vpsubusb ymm16, ymm13, [rbx+r11*8+256]
    vpsubusb ymm16, ymm13, [rbx+r11*8-256]
    vpsubusb zmm24, zmm24, zmm31
    vpsubusb zmm24, zmm24, [rbx]
    vpsubusb zmm24, zmm24, [rbx+r11*8+256]
    vpsubusb zmm24, zmm24, [rbx+r11*8-256]
    vpsubusw xmm2, xmm7, xmm0
    vpsubusw xmm2, xmm7, [rbx]
    vpsubusw xmm2, xmm7, [rbx+r11*8+256]
    vpsubusw xmm2, xmm7, [rbx+r11*8-256]
    vpsubusw ymm16, ymm13, ymm15
    vpsubusw ymm16, ymm13, [rbx]
    vpsubusw ymm16, ymm13, [rbx+r11*8+256]
    vpsubusw ymm16, ymm13, [rbx+r11*8-256]
    vpsubusw zmm24, zmm24, zmm31
    vpsubusw zmm24, zmm24, [rbx]
    vpsubusw zmm24, zmm24, [rbx+r11*8+256]
    vpsubusw zmm24, zmm24, [rbx+r11*8-256]
    vpsubw xmm2, xmm7, xmm0
    vpsubw xmm2, xmm7, [rbx]
    vpsubw xmm2, xmm7, [rbx+r11*8+256]
    vpsubw xmm2, xmm7, [rbx+r11*8-256]
    vpsubw ymm16, ymm13, ymm15
    vpsubw ymm16, ymm13, [rbx]
    vpsubw ymm16, ymm13, [rbx+r11*8+256]
    vpsubw ymm16, ymm13, [rbx+r11*8-256]
    vpsubw zmm24, zmm24, zmm31
    vpsubw zmm24, zmm24, [rbx]
    vpsubw zmm24, zmm24, [rbx+r11*8+256]
    vpsubw zmm24, zmm24, [rbx+r11*8-256]
    vptest xmm2, xmm0
    vptest xmm2, [rbx]
    vptest xmm2, [rbx+r11*8+256]
    vptest xmm2, [rbx+r11*8-256]
    vpunpckhbw xmm2, xmm7, xmm0
    vpunpckhbw xmm2, xmm7, [rbx]
    vpunpckhbw xmm2, xmm7, [rbx+r11*8+256]
    vpunpckhbw xmm2, xmm7, [rbx+r11*8-256]
    vpunpckhbw ymm16, ymm13, ymm15
    vpunpckhbw ymm16, ymm13, [rbx]
    vpunpckhbw ymm16, ymm13, [rbx+r11*8+256]
    vpunpckhbw ymm16, ymm13, [rbx+r11*8-256]
    vpunpckhbw zmm24, zmm24, zmm31
    vpunpckhbw zmm24, zmm24, [rbx]
    vpunpckhbw zmm24, zmm24, [rbx+r11*8+256]
    vpunpckhbw zmm24, zmm24, [rbx+r11*8-256]
    vpunpckhdq xmm2, xmm7, xmm0
    vpunpckhdq xmm2, xmm7, [rbx]
    vpunpckhdq xmm2, xmm7, [rbx+r11*8+256]
    vpunpckhdq xmm2, xmm7, [rbx+r11*8-256]
    vpunpckhdq ymm16, ymm13, ymm15
    vpunpckhdq ymm16, ymm13, [rbx]
    vpunpckhdq ymm16, ymm13, [rbx+r11*8+256]
    vpunpckhdq ymm16, ymm13, [rbx+r11*8-256]
    vpunpckhdq zmm24, zmm24, zmm31
    vpunpckhdq zmm24, zmm24, [rbx]
    vpunpckhdq zmm24, zmm24, [rbx+r11*8+256]
    vpunpckhdq zmm24, zmm24, [rbx+r11*8-256]
    vpunpckhqdq xmm2, xmm7, xmm0
    vpunpckhqdq xmm2, xmm7, [rbx]
    vpunpckhqdq xmm2, xmm7, [rbx+r11*8+256]
    vpunpckhqdq xmm2, xmm7, [rbx+r11*8-256]
    vpunpckhqdq ymm16, ymm13, ymm15
    vpunpckhqdq ymm16, ymm13, [rbx]
    vpunpckhqdq ymm16, ymm13, [rbx+r11*8+256]
    vpunpckhqdq ymm16, ymm13, [rbx+r11*8-256]
    vpunpckhqdq zmm24, zmm24, zmm31
    vpunpckhqdq zmm24, zmm24, [rbx]
    vpunpckhqdq zmm24, zmm24, [rbx+r11*8+256]
    vpunpckhqdq zmm24, zmm24, [rbx+r11*8-256]
    vpunpckhwd xmm2, xmm7, xmm0
    vpunpckhwd xmm2, xmm7, [rbx]
    vpunpckhwd xmm2, xmm7, [rbx+r11*8+256]
    vpunpckhwd xmm2, xmm7, [rbx+r11*8-256]
    vpunpckhwd ymm16, ymm13, ymm15
    vpunpckhwd ymm16, ymm13, [rbx]
    vpunpckhwd ymm16, ymm13, [rbx+r11*8+256]
    vpunpckhwd ymm16, ymm13, [rbx+r11*8-256]
    vpunpckhwd zmm24, zmm24, zmm31
    vpunpckhwd zmm24, zmm24, [rbx]
    vpunpckhwd zmm24, zmm24, [rbx+r11*8+256]
    vpunpckhwd zmm24, zmm24, [rbx+r11*8-256]
    vpunpcklbw xmm2, xmm7, xmm0
    vpunpcklbw xmm2, xmm7, [rbx]
    vpunpcklbw xmm2, xmm7, [rbx+r11*8+256]
    vpunpcklbw xmm2, xmm7, [rbx+r11*8-256]
    vpunpcklbw ymm16, ymm13, ymm15
    vpunpcklbw ymm16, ymm13, [rbx]
    vpunpcklbw ymm16, ymm13, [rbx+r11*8+256]
    vpunpcklbw ymm16, ymm13, [rbx+r11*8-256]
    vpunpcklbw zmm24, zmm24, zmm31
    vpunpcklbw zmm24, zmm24, [rbx]
    vpunpcklbw zmm24, zmm24, [rbx+r11*8+256]
    vpunpcklbw zmm24, zmm24, [rbx+r11*8-256]
    vpunpckldq xmm2, xmm7, xmm0
    vpunpckldq xmm2, xmm7, [rbx]
    vpunpckldq xmm2, xmm7, [rbx+r11*8+256]
    vpunpckldq xmm2, xmm7, [rbx+r11*8-256]
    vpunpckldq ymm16, ymm13, ymm15
    vpunpckldq ymm16, ymm13, [rbx]
    vpunpckldq ymm16, ymm13, [rbx+r11*8+256]
    vpunpckldq ymm16, ymm13, [rbx+r11*8-256]
    vpunpckldq zmm24, zmm24, zmm31
    vpunpckldq zmm24, zmm24, [rbx]
    vpunpckldq zmm24, zmm24, [rbx+r11*8+256]
    vpunpckldq zmm24, zmm24, [rbx+r11*8-256]
    vpunpcklqdq xmm2, xmm7, xmm0
    vpunpcklqdq xmm2, xmm7, [rbx]
    vpunpcklqdq xmm2, xmm7, [rbx+r11*8+256]
    vpunpcklqdq xmm2, xmm7, [rbx+r11*8-256]
    vpunpcklqdq ymm16, ymm13, ymm15
    vpunpcklqdq ymm16, ymm13, [rbx]
    vpunpcklqdq ymm16, ymm13, [rbx+r11*8+256]
    vpunpcklqdq ymm16, ymm13, [rbx+r11*8-256]
    vpunpcklqdq zmm24, zmm24, zmm31
    vpunpcklqdq zmm24, zmm24, [rbx]
    vpunpcklqdq zmm24, zmm24, [rbx+r11*8+256]
    vpunpcklqdq zmm24, zmm24, [rbx+r11*8-256]
    vpunpcklwd xmm2, xmm7, xmm0
    vpunpcklwd xmm2, xmm7, [rbx]
    vpunpcklwd xmm2, xmm7, [rbx+r11*8+256]
    vpunpcklwd xmm2, xmm7, [rbx+r11*8-256]
    vpunpcklwd ymm16, ymm13, ymm15
    vpunpcklwd ymm16, ymm13, [rbx]
    vpunpcklwd ymm16, ymm13, [rbx+r11*8+256]
    vpunpcklwd ymm16, ymm13, [rbx+r11*8-256]
    vpunpcklwd zmm24, zmm24, zmm31
    vpunpcklwd zmm24, zmm24, [rbx]
    vpunpcklwd zmm24, zmm24, [rbx+r11*8+256]
    vpunpcklwd zmm24, zmm24, [rbx+r11*8-256]
    vpxor xmm2, xmm7, xmm0
    vpxor xmm2, xmm7, [rbx]
    vpxor xmm2, xmm7, [rbx+r11*8+256]
    vpxor xmm2, xmm7, [rbx+r11*8-256]
    vrcpps xmm2, xmm0
    vrcpps xmm2, [rbx]
    vrcpps xmm2, [rbx+r11*8+256]
    vrcpps xmm2, [rbx+r11*8-256]
    vrcpss xmm2, xmm7, xmm0
    vrcpss xmm2, xmm7, [rbx]
    vrcpss xmm2, xmm7, [rbx+r11*8+256]
    vrcpss xmm2, xmm7, [rbx+r11*8-256]
    vroundpd xmm2, xmm0, 0x90
    vroundpd xmm2, [rbx], 0x90
    vroundpd xmm2, [rbx+r11*8+256], 0x90
    vroundpd xmm2, [rbx+r11*8-256], 0x90
    vroundps xmm2, xmm0, 0x90
    vroundps xmm2, [rbx], 0x90
    vroundps xmm2, [rbx+r11*8+256], 0x90
    vroundps xmm2, [rbx+r11*8-256], 0x90
    vroundsd xmm2, xmm7, xmm0, 0x90
    vroundsd xmm2, xmm7, [rbx], 0x90
    vroundsd xmm2, xmm7, [rbx+r11*8+256], 0x90
    vroundsd xmm2, xmm7, [rbx+r11*8-256], 0x90
    vroundss xmm2, xmm7, xmm0, 0x90
    vroundss xmm2, xmm7, [rbx], 0x90
    vroundss xmm2, xmm7, [rbx+r11*8+256], 0x90
    vroundss xmm2, xmm7, [rbx+r11*8-256], 0x90
    vrsqrtps xmm2, xmm0
    vrsqrtps xmm2, [rbx]
    vrsqrtps xmm2, [rbx+r11*8+256]
    vrsqrtps xmm2, [rbx+r11*8-256]
    vrsqrtss xmm2, xmm7, xmm0
    vrsqrtss xmm2, xmm7, [rbx]
    vrsqrtss xmm2, xmm7, [rbx+r11*8+256]
    vrsqrtss xmm2, xmm7, [rbx+r11*8-256]
    vshufpd xmm2, xmm7, xmm0, 0x90
    vshufpd xmm2, xmm7, [rbx], 0x90
    vshufpd xmm2, xmm7, [rbx+r11*8+256], 0x90
    vshufpd xmm2, xmm7, [rbx+r11*8-256], 0x90
    vshufpd ymm16, ymm13, ymm15, 0x90
    vshufpd ymm16, ymm13, [rbx], 0x90
    vshufpd ymm16, ymm13, [rbx+r11*8+256], 0x90
    vshufpd ymm16, ymm13, [rbx+r11*8-256], 0x90
    vshufpd zmm24, zmm24, zmm31, 0x90
    vshufpd zmm24, zmm24, [rbx], 0x90
    vshufpd zmm24, zmm24, [rbx+r11*8+256], 0x90
    vshufpd zmm24, zmm24, [rbx+r11*8-256], 0x90
    vshufps xmm2, xmm7, xmm0, 0x90
    vshufps xmm2, xmm7, [rbx], 0x90
    vshufps xmm2, xmm7, [rbx+r11*8+256], 0x90
    vshufps xmm2, xmm7, [rbx+r11*8-256], 0x90
    vshufps ymm16, ymm13, ymm15, 0x90
    vshufps ymm16, ymm13, [rbx], 0x90
    vshufps ymm16, ymm13, [rbx+r11*8+256], 0x90
    vshufps ymm16, ymm13, [rbx+r11*8-256], 0x90
    vshufps zmm24, zmm24, zmm31, 0x90
    vshufps zmm24, zmm24, [rbx], 0x90
    vshufps zmm24, zmm24, [rbx+r11*8+256], 0x90
    vshufps zmm24, zmm24, [rbx+r11*8-256], 0x90
    vsqrtpd xmm2, xmm0
    vsqrtpd xmm2, [rbx]
    vsqrtpd xmm2, [rbx+r11*8+256]
    vsqrtpd xmm2, [rbx+r11*8-256]
    vsqrtpd ymm16, ymm15
    vsqrtpd ymm16, [rbx]
    vsqrtpd ymm16, [rbx+r11*8+256]
    vsqrtpd ymm16, [rbx+r11*8-256]
    vsqrtpd zmm24, zmm31
    vsqrtpd zmm24, [rbx]
    vsqrtpd zmm24, [rbx+r11*8+256]
    vsqrtpd zmm24, [rbx+r11*8-256]
    vsqrtps xmm2, xmm0
    vsqrtps xmm2, [rbx]
    vsqrtps xmm2, [rbx+r11*8+256]
    vsqrtps xmm2, [rbx+r11*8-256]
    vsqrtps ymm16, ymm15
    vsqrtps ymm16, [rbx]
    vsqrtps ymm16, [rbx+r11*8+256]
    vsqrtps ymm16, [rbx+r11*8-256]
    vsqrtps zmm24, zmm31
    vsqrtps zmm24, [rbx]
    vsqrtps zmm24, [rbx+r11*8+256]
    vsqrtps zmm24, [rbx+r11*8-256]
    vsqrtsd xmm2, xmm7, xmm0
    vsqrtsd xmm2, xmm7, [rbx]
    vsqrtsd xmm2, xmm7, [rbx+r11*8+256]
    vsqrtsd xmm2, xmm7, [rbx+r11*8-256]
    vsqrtss xmm2, xmm7, xmm0
    vsqrtss xmm2, xmm7, [rbx]
    vsqrtss xmm2, xmm7, [rbx+r11*8+256]
    vsqrtss xmm2, xmm7, [rbx+r11*8-256]
    vstmxcsr [rbx]
    vstmxcsr [rbx+rsi*8+256]
    vstmxcsr [rbx+rsi*8-256]
    vsubpd xmm2, xmm7, xmm0
    vsubpd xmm2, xmm7, [rbx]
    vsubpd xmm2, xmm7, [rbx+r11*8+256]
    vsubpd xmm2, xmm7, [rbx+r11*8-256]
    vsubpd ymm16, ymm13, ymm15
    vsubpd ymm16, ymm13, [rbx]
    vsubpd ymm16, ymm13, [rbx+r11*8+256]
    vsubpd ymm16, ymm13, [rbx+r11*8-256]
    vsubpd zmm24, zmm24, zmm31
    vsubpd zmm24, zmm24, [rbx]
    vsubpd zmm24, zmm24, [rbx+r11*8+256]
    vsubpd zmm24, zmm24, [rbx+r11*8-256]
    vsubps xmm2, xmm7, xmm0
    vsubps xmm2, xmm7, [rbx]
    vsubps xmm2, xmm7, [rbx+r11*8+256]
    vsubps xmm2, xmm7, [rbx+r11*8-256]
    vsubps ymm16, ymm13, ymm15
    vsubps ymm16, ymm13, [rbx]
    vsubps ymm16, ymm13, [rbx+r11*8+256]
    vsubps ymm16, ymm13, [rbx+r11*8-256]
    vsubps zmm24, zmm24, zmm31
    vsubps zmm24, zmm24, [rbx]
    vsubps zmm24, zmm24, [rbx+r11*8+256]
    vsubps zmm24, zmm24, [rbx+r11*8-256]
    vsubsd xmm2, xmm7, xmm0
    vsubsd xmm2, xmm7, [rbx]
    vsubsd xmm2, xmm7, [rbx+r11*8+256]
    vsubsd xmm2, xmm7, [rbx+r11*8-256]
    vsubss xmm2, xmm7, xmm0
    vsubss xmm2, xmm7, [rbx]
    vsubss xmm2, xmm7, [rbx+r11*8+256]
    vsubss xmm2, xmm7, [rbx+r11*8-256]
    vtestpd xmm2, xmm0
    vtestpd xmm2, [rbx]
    vtestpd xmm2, [rbx+r11*8+256]
    vtestpd xmm2, [rbx+r11*8-256]
    vtestps xmm2, xmm0
    vtestps xmm2, [rbx]
    vtestps xmm2, [rbx+r11*8+256]
    vtestps xmm2, [rbx+r11*8-256]
    vucomisd xmm2, xmm0
    vucomisd xmm2, [rbx]
    vucomisd xmm2, [rbx+r11*8+256]
    vucomisd xmm2, [rbx+r11*8-256]
    vucomiss xmm2, xmm0
    vucomiss xmm2, [rbx]
    vucomiss xmm2, [rbx+r11*8+256]
    vucomiss xmm2, [rbx+r11*8-256]
    vunpckhpd xmm2, xmm7, xmm0
    vunpckhpd xmm2, xmm7, [rbx]
    vunpckhpd xmm2, xmm7, [rbx+r11*8+256]
    vunpckhpd xmm2, xmm7, [rbx+r11*8-256]
    vunpckhpd ymm16, ymm13, ymm15
    vunpckhpd ymm16, ymm13, [rbx]
    vunpckhpd ymm16, ymm13, [rbx+r11*8+256]
    vunpckhpd ymm16, ymm13, [rbx+r11*8-256]
    vunpckhpd zmm24, zmm24, zmm31
    vunpckhpd zmm24, zmm24, [rbx]
    vunpckhpd zmm24, zmm24, [rbx+r11*8+256]
    vunpckhpd zmm24, zmm24, [rbx+r11*8-256]
    vunpckhps xmm2, xmm7, xmm0
    vunpckhps xmm2, xmm7, [rbx]
    vunpckhps xmm2, xmm7, [rbx+r11*8+256]
    vunpckhps xmm2, xmm7, [rbx+r11*8-256]
    vunpckhps ymm16, ymm13, ymm15
    vunpckhps ymm16, ymm13, [rbx]
    vunpckhps ymm16, ymm13, [rbx+r11*8+256]
    vunpckhps ymm16, ymm13, [rbx+r11*8-256]
    vunpckhps zmm24, zmm24, zmm31
    vunpckhps zmm24, zmm24, [rbx]
    vunpckhps zmm24, zmm24, [rbx+r11*8+256]
    vunpckhps zmm24, zmm24, [rbx+r11*8-256]
    vunpcklpd xmm2, xmm7, xmm0
    vunpcklpd xmm2, xmm7, [rbx]
    vunpcklpd xmm2, xmm7, [rbx+r11*8+256]
    vunpcklpd xmm2, xmm7, [rbx+r11*8-256]
    vunpcklpd ymm16, ymm13, ymm15
    vunpcklpd ymm16, ymm13, [rbx]
    vunpcklpd ymm16, ymm13, [rbx+r11*8+256]
    vunpcklpd ymm16, ymm13, [rbx+r11*8-256]
    vunpcklpd zmm24, zmm24, zmm31
    vunpcklpd zmm24, zmm24, [rbx]
    vunpcklpd zmm24, zmm24, [rbx+r11*8+256]
    vunpcklpd zmm24, zmm24, [rbx+r11*8-256]
    vunpcklps xmm2, xmm7, xmm0
    vunpcklps xmm2, xmm7, [rbx]
    vunpcklps xmm2, xmm7, [rbx+r11*8+256]
    vunpcklps xmm2, xmm7, [rbx+r11*8-256]
    vunpcklps ymm16, ymm13, ymm15
    vunpcklps ymm16, ymm13, [rbx]
    vunpcklps ymm16, ymm13, [rbx+r11*8+256]
    vunpcklps ymm16, ymm13, [rbx+r11*8-256]
    vunpcklps zmm24, zmm24, zmm31
    vunpcklps zmm24, zmm24, [rbx]
    vunpcklps zmm24, zmm24, [rbx+r11*8+256]
    vunpcklps zmm24, zmm24, [rbx+r11*8-256]
    vxorpd xmm2, xmm7, xmm0
    vxorpd xmm2, xmm7, [rbx]
    vxorpd xmm2, xmm7, [rbx+r11*8+256]
    vxorpd xmm2, xmm7, [rbx+r11*8-256]
    vxorpd ymm16, ymm13, ymm15
    vxorpd ymm16, ymm13, [rbx]
    vxorpd ymm16, ymm13, [rbx+r11*8+256]
    vxorpd ymm16, ymm13, [rbx+r11*8-256]
    vxorpd zmm24, zmm24, zmm31
    vxorpd zmm24, zmm24, [rbx]
    vxorpd zmm24, zmm24, [rbx+r11*8+256]
    vxorpd zmm24, zmm24, [rbx+r11*8-256]
    vxorps xmm2, xmm7, xmm0
    vxorps xmm2, xmm7, [rbx]
    vxorps xmm2, xmm7, [rbx+r11*8+256]
    vxorps xmm2, xmm7, [rbx+r11*8-256]
    vxorps ymm16, ymm13, ymm15
    vxorps ymm16, ymm13, [rbx]
    vxorps ymm16, ymm13, [rbx+r11*8+256]
    vxorps ymm16, ymm13, [rbx+r11*8-256]
    vxorps zmm24, zmm24, zmm31
    vxorps zmm24, zmm24, [rbx]
    vxorps zmm24, zmm24, [rbx+r11*8+256]
    vxorps zmm24, zmm24, [rbx+r11*8-256]
