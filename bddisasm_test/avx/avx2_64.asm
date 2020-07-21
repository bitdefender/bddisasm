    bits 64
    
    vpblendd xmm2, xmm7, xmm0, 0x90
    vpblendd xmm2, xmm7, [rbx], 0x90
    vpblendd xmm2, xmm7, [rbx+r11*8+256], 0x90
    vpblendd xmm2, xmm7, [rbx+r11*8-256], 0x90
    vpbroadcastb xmm2, xmm0
    vpbroadcastb xmm2, [rbx]
    vpbroadcastb xmm2, [rbx+r11*8+256]
    vpbroadcastb xmm2, [rbx+r11*8-256]
    vpbroadcastb ymm16, xmm0
    vpbroadcastb ymm16, [rbx]
    vpbroadcastb ymm16, [rbx+r11*8+256]
    vpbroadcastb ymm16, [rbx+r11*8-256]
    vpbroadcastb zmm24, xmm0
    vpbroadcastb zmm24, [rbx]
    vpbroadcastb zmm24, [rbx+r11*8+256]
    vpbroadcastb zmm24, [rbx+r11*8-256]
    vpbroadcastd xmm2, xmm0
    vpbroadcastd xmm2, [rbx]
    vpbroadcastd xmm2, [rbx+r11*8+256]
    vpbroadcastd xmm2, [rbx+r11*8-256]
    vpbroadcastd ymm16, xmm0
    vpbroadcastd ymm16, [rbx]
    vpbroadcastd ymm16, [rbx+r11*8+256]
    vpbroadcastd ymm16, [rbx+r11*8-256]
    vpbroadcastd zmm24, xmm0
    vpbroadcastd zmm24, [rbx]
    vpbroadcastd zmm24, [rbx+r11*8+256]
    vpbroadcastd zmm24, [rbx+r11*8-256]
    vpbroadcastq xmm2, xmm0
    vpbroadcastq xmm2, [rbx]
    vpbroadcastq xmm2, [rbx+r11*8+256]
    vpbroadcastq xmm2, [rbx+r11*8-256]
    vpbroadcastq ymm16, xmm0
    vpbroadcastq ymm16, [rbx]
    vpbroadcastq ymm16, [rbx+r11*8+256]
    vpbroadcastq ymm16, [rbx+r11*8-256]
    vpbroadcastq zmm24, xmm0
    vpbroadcastq zmm24, [rbx]
    vpbroadcastq zmm24, [rbx+r11*8+256]
    vpbroadcastq zmm24, [rbx+r11*8-256]
    vpbroadcastw xmm2, xmm0
    vpbroadcastw xmm2, [rbx]
    vpbroadcastw xmm2, [rbx+r11*8+256]
    vpbroadcastw xmm2, [rbx+r11*8-256]
    vpbroadcastw ymm16, xmm0
    vpbroadcastw ymm16, [rbx]
    vpbroadcastw ymm16, [rbx+r11*8+256]
    vpbroadcastw ymm16, [rbx+r11*8-256]
    vpbroadcastw zmm24, xmm0
    vpbroadcastw zmm24, [rbx]
    vpbroadcastw zmm24, [rbx+r11*8+256]
    vpbroadcastw zmm24, [rbx+r11*8-256]
    vpermd ymm16, ymm13, ymm15
    vpermd ymm16, ymm13, [rbx]
    vpermd ymm16, ymm13, [rbx+r11*8+256]
    vpermd ymm16, ymm13, [rbx+r11*8-256]
    vpermd zmm24, zmm24, zmm31
    vpermd zmm24, zmm24, [rbx]
    vpermd zmm24, zmm24, [rbx+r11*8+256]
    vpermd zmm24, zmm24, [rbx+r11*8-256]
    vpermpd ymm16, ymm15, 0x90
    vpermpd ymm16, [rbx], 0x90
    vpermpd ymm16, [rbx+r11*8+256], 0x90
    vpermpd ymm16, [rbx+r11*8-256], 0x90
    vpermpd zmm24, zmm31, 0x90
    vpermpd zmm24, [rbx], 0x90
    vpermpd zmm24, [rbx+r11*8+256], 0x90
    vpermpd zmm24, [rbx+r11*8-256], 0x90
    vpermps ymm16, ymm13, ymm15
    vpermps ymm16, ymm13, [rbx]
    vpermps ymm16, ymm13, [rbx+r11*8+256]
    vpermps ymm16, ymm13, [rbx+r11*8-256]
    vpermps zmm24, zmm24, zmm31
    vpermps zmm24, zmm24, [rbx]
    vpermps zmm24, zmm24, [rbx+r11*8+256]
    vpermps zmm24, zmm24, [rbx+r11*8-256]
    vpermq ymm16, ymm15, 0x90
    vpermq ymm16, [rbx], 0x90
    vpermq ymm16, [rbx+r11*8+256], 0x90
    vpermq ymm16, [rbx+r11*8-256], 0x90
    vpermq zmm24, zmm31, 0x90
    vpermq zmm24, [rbx], 0x90
    vpermq zmm24, [rbx+r11*8+256], 0x90
    vpermq zmm24, [rbx+r11*8-256], 0x90
    vpmaskmovd xmm2, xmm7, [rbx]
    vpmaskmovd xmm2, xmm7, [rbx+rsi*8+256]
    vpmaskmovd xmm2, xmm7, [rbx+rsi*8-256]
    vpmaskmovd [rbx], xmm7, xmm2
    vpmaskmovd [rbx+rsi*8+256], xmm7, xmm2
    vpmaskmovd [rbx+rsi*8-256], xmm7, xmm2
    vpmaskmovq xmm2, xmm7, [rbx]
    vpmaskmovq xmm2, xmm7, [rbx+rsi*8+256]
    vpmaskmovq xmm2, xmm7, [rbx+rsi*8-256]
    vpmaskmovq [rbx], xmm7, xmm2
    vpmaskmovq [rbx+rsi*8+256], xmm7, xmm2
    vpmaskmovq [rbx+rsi*8-256], xmm7, xmm2
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
    vpsllvd xmm2, xmm7, xmm0
    vpsllvd xmm2, xmm7, [rbx]
    vpsllvd xmm2, xmm7, [rbx+r11*8+256]
    vpsllvd xmm2, xmm7, [rbx+r11*8-256]
    vpsllvd ymm16, ymm13, ymm15
    vpsllvd ymm16, ymm13, [rbx]
    vpsllvd ymm16, ymm13, [rbx+r11*8+256]
    vpsllvd ymm16, ymm13, [rbx+r11*8-256]
    vpsllvd zmm24, zmm24, zmm31
    vpsllvd zmm24, zmm24, [rbx]
    vpsllvd zmm24, zmm24, [rbx+r11*8+256]
    vpsllvd zmm24, zmm24, [rbx+r11*8-256]
    vpsllvq xmm2, xmm7, xmm0
    vpsllvq xmm2, xmm7, [rbx]
    vpsllvq xmm2, xmm7, [rbx+r11*8+256]
    vpsllvq xmm2, xmm7, [rbx+r11*8-256]
    vpsllvq ymm16, ymm13, ymm15
    vpsllvq ymm16, ymm13, [rbx]
    vpsllvq ymm16, ymm13, [rbx+r11*8+256]
    vpsllvq ymm16, ymm13, [rbx+r11*8-256]
    vpsllvq zmm24, zmm24, zmm31
    vpsllvq zmm24, zmm24, [rbx]
    vpsllvq zmm24, zmm24, [rbx+r11*8+256]
    vpsllvq zmm24, zmm24, [rbx+r11*8-256]
    vpsravd xmm2, xmm7, xmm0
    vpsravd xmm2, xmm7, [rbx]
    vpsravd xmm2, xmm7, [rbx+r11*8+256]
    vpsravd xmm2, xmm7, [rbx+r11*8-256]
    vpsravd ymm16, ymm13, ymm15
    vpsravd ymm16, ymm13, [rbx]
    vpsravd ymm16, ymm13, [rbx+r11*8+256]
    vpsravd ymm16, ymm13, [rbx+r11*8-256]
    vpsravd zmm24, zmm24, zmm31
    vpsravd zmm24, zmm24, [rbx]
    vpsravd zmm24, zmm24, [rbx+r11*8+256]
    vpsravd zmm24, zmm24, [rbx+r11*8-256]
    vpsrlvd xmm2, xmm7, xmm0
    vpsrlvd xmm2, xmm7, [rbx]
    vpsrlvd xmm2, xmm7, [rbx+r11*8+256]
    vpsrlvd xmm2, xmm7, [rbx+r11*8-256]
    vpsrlvd ymm16, ymm13, ymm15
    vpsrlvd ymm16, ymm13, [rbx]
    vpsrlvd ymm16, ymm13, [rbx+r11*8+256]
    vpsrlvd ymm16, ymm13, [rbx+r11*8-256]
    vpsrlvd zmm24, zmm24, zmm31
    vpsrlvd zmm24, zmm24, [rbx]
    vpsrlvd zmm24, zmm24, [rbx+r11*8+256]
    vpsrlvd zmm24, zmm24, [rbx+r11*8-256]
    vpsrlvq xmm2, xmm7, xmm0
    vpsrlvq xmm2, xmm7, [rbx]
    vpsrlvq xmm2, xmm7, [rbx+r11*8+256]
    vpsrlvq xmm2, xmm7, [rbx+r11*8-256]
    vpsrlvq ymm16, ymm13, ymm15
    vpsrlvq ymm16, ymm13, [rbx]
    vpsrlvq ymm16, ymm13, [rbx+r11*8+256]
    vpsrlvq ymm16, ymm13, [rbx+r11*8-256]
    vpsrlvq zmm24, zmm24, zmm31
    vpsrlvq zmm24, zmm24, [rbx]
    vpsrlvq zmm24, zmm24, [rbx+r11*8+256]
    vpsrlvq zmm24, zmm24, [rbx+r11*8-256]
