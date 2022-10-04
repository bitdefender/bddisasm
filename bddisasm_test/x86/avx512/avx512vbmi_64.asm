    bits 64
    
    vpcompressb xmm0, xmm2
    vpcompressb ymm15, ymm16
    vpcompressb zmm31, zmm24
    vpcompressb [rbx], xmm2
    vpcompressb [rbx], ymm16
    vpcompressb [rbx], zmm24
    vpcompressb [rbx+r11*8+256], xmm2
    vpcompressb [rbx+r11*8+256], ymm16
    vpcompressb [rbx+r11*8+256], zmm24
    vpcompressb [rbx+r11*8-256], xmm2
    vpcompressb [rbx+r11*8-256], ymm16
    vpcompressb [rbx+r11*8-256], zmm24
    vpcompressw xmm0, xmm2
    vpcompressw ymm15, ymm16
    vpcompressw zmm31, zmm24
    vpcompressw [rbx], xmm2
    vpcompressw [rbx], ymm16
    vpcompressw [rbx], zmm24
    vpcompressw [rbx+r11*8+256], xmm2
    vpcompressw [rbx+r11*8+256], ymm16
    vpcompressw [rbx+r11*8+256], zmm24
    vpcompressw [rbx+r11*8-256], xmm2
    vpcompressw [rbx+r11*8-256], ymm16
    vpcompressw [rbx+r11*8-256], zmm24
    vpermb xmm2, xmm7, xmm0
    vpermb xmm2, xmm7, [rbx]
    vpermb xmm2, xmm7, [rbx+r11*8+256]
    vpermb xmm2, xmm7, [rbx+r11*8-256]
    vpermb xmm2{k5}, xmm7, xmm0
    vpermb xmm2{k5}, xmm7, [rbx]
    vpermb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpermb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpermb xmm2{k5}{z}, xmm7, xmm0
    vpermb xmm2{k5}{z}, xmm7, [rbx]
    vpermb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpermb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpermb ymm16, ymm13, ymm15
    vpermb ymm16, ymm13, [rbx]
    vpermb ymm16, ymm13, [rbx+r11*8+256]
    vpermb ymm16, ymm13, [rbx+r11*8-256]
    vpermb ymm16{k5}, ymm13, ymm15
    vpermb ymm16{k5}, ymm13, [rbx]
    vpermb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpermb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpermb ymm16{k5}{z}, ymm13, ymm15
    vpermb ymm16{k5}{z}, ymm13, [rbx]
    vpermb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpermb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpermb zmm24, zmm24, zmm31
    vpermb zmm24, zmm24, [rbx]
    vpermb zmm24, zmm24, [rbx+r11*8+256]
    vpermb zmm24, zmm24, [rbx+r11*8-256]
    vpermb zmm24{k5}, zmm24, zmm31
    vpermb zmm24{k5}, zmm24, [rbx]
    vpermb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpermb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpermb zmm24{k5}{z}, zmm24, zmm31
    vpermb zmm24{k5}{z}, zmm24, [rbx]
    vpermb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpermb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpermi2b xmm2, xmm7, xmm0
    vpermi2b xmm2, xmm7, [rbx]
    vpermi2b xmm2, xmm7, [rbx+r11*8+256]
    vpermi2b xmm2, xmm7, [rbx+r11*8-256]
    vpermi2b xmm2{k5}, xmm7, xmm0
    vpermi2b xmm2{k5}, xmm7, [rbx]
    vpermi2b xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpermi2b xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpermi2b xmm2{k5}{z}, xmm7, xmm0
    vpermi2b xmm2{k5}{z}, xmm7, [rbx]
    vpermi2b xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpermi2b xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpermi2b ymm16, ymm13, ymm15
    vpermi2b ymm16, ymm13, [rbx]
    vpermi2b ymm16, ymm13, [rbx+r11*8+256]
    vpermi2b ymm16, ymm13, [rbx+r11*8-256]
    vpermi2b ymm16{k5}, ymm13, ymm15
    vpermi2b ymm16{k5}, ymm13, [rbx]
    vpermi2b ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpermi2b ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpermi2b ymm16{k5}{z}, ymm13, ymm15
    vpermi2b ymm16{k5}{z}, ymm13, [rbx]
    vpermi2b ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpermi2b ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpermi2b zmm24, zmm24, zmm31
    vpermi2b zmm24, zmm24, [rbx]
    vpermi2b zmm24, zmm24, [rbx+r11*8+256]
    vpermi2b zmm24, zmm24, [rbx+r11*8-256]
    vpermi2b zmm24{k5}, zmm24, zmm31
    vpermi2b zmm24{k5}, zmm24, [rbx]
    vpermi2b zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpermi2b zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpermi2b zmm24{k5}{z}, zmm24, zmm31
    vpermi2b zmm24{k5}{z}, zmm24, [rbx]
    vpermi2b zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpermi2b zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpermt2b xmm2, xmm7, xmm0
    vpermt2b xmm2, xmm7, [rbx]
    vpermt2b xmm2, xmm7, [rbx+r11*8+256]
    vpermt2b xmm2, xmm7, [rbx+r11*8-256]
    vpermt2b xmm2{k5}, xmm7, xmm0
    vpermt2b xmm2{k5}, xmm7, [rbx]
    vpermt2b xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpermt2b xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpermt2b xmm2{k5}{z}, xmm7, xmm0
    vpermt2b xmm2{k5}{z}, xmm7, [rbx]
    vpermt2b xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpermt2b xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpermt2b ymm16, ymm13, ymm15
    vpermt2b ymm16, ymm13, [rbx]
    vpermt2b ymm16, ymm13, [rbx+r11*8+256]
    vpermt2b ymm16, ymm13, [rbx+r11*8-256]
    vpermt2b ymm16{k5}, ymm13, ymm15
    vpermt2b ymm16{k5}, ymm13, [rbx]
    vpermt2b ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpermt2b ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpermt2b ymm16{k5}{z}, ymm13, ymm15
    vpermt2b ymm16{k5}{z}, ymm13, [rbx]
    vpermt2b ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpermt2b ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpermt2b zmm24, zmm24, zmm31
    vpermt2b zmm24, zmm24, [rbx]
    vpermt2b zmm24, zmm24, [rbx+r11*8+256]
    vpermt2b zmm24, zmm24, [rbx+r11*8-256]
    vpermt2b zmm24{k5}, zmm24, zmm31
    vpermt2b zmm24{k5}, zmm24, [rbx]
    vpermt2b zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpermt2b zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpermt2b zmm24{k5}{z}, zmm24, zmm31
    vpermt2b zmm24{k5}{z}, zmm24, [rbx]
    vpermt2b zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpermt2b zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpexpandb xmm2, xmm0
    vpexpandb xmm2{k5}, xmm0
    vpexpandb xmm2{k5}{z}, xmm0
    vpexpandb ymm16, ymm15
    vpexpandb ymm16{k5}, ymm15
    vpexpandb ymm16{k5}{z}, ymm15
    vpexpandb zmm24, zmm31
    vpexpandb zmm24{k5}, zmm31
    vpexpandb zmm24{k5}{z}, zmm31
    vpexpandw xmm2, xmm0
    vpexpandw xmm2{k5}, xmm0
    vpexpandw xmm2{k5}{z}, xmm0
    vpexpandw ymm16, ymm15
    vpexpandw ymm16{k5}, ymm15
    vpexpandw ymm16{k5}{z}, ymm15
    vpexpandw zmm24, zmm31
    vpexpandw zmm24{k5}, zmm31
    vpexpandw zmm24{k5}{z}, zmm31
    vpmultishiftqb xmm2, xmm7, xmm0
    vpmultishiftqb xmm2, xmm7, [rbx]
    vpmultishiftqb xmm2, xmm7, [rbx]{1to2}
    vpmultishiftqb xmm2, xmm7, [rbx+r11*8+256]
    vpmultishiftqb xmm2, xmm7, [rbx+r11*8-256]
    vpmultishiftqb xmm2{k5}, xmm7, xmm0
    vpmultishiftqb xmm2{k5}, xmm7, [rbx]
    vpmultishiftqb xmm2{k5}, xmm7, [rbx]{1to2}
    vpmultishiftqb xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpmultishiftqb xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpmultishiftqb xmm2{k5}{z}, xmm7, xmm0
    vpmultishiftqb xmm2{k5}{z}, xmm7, [rbx]
    vpmultishiftqb xmm2{k5}{z}, xmm7, [rbx]{1to2}
    vpmultishiftqb xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpmultishiftqb xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpmultishiftqb ymm16, ymm13, ymm15
    vpmultishiftqb ymm16, ymm13, [rbx]
    vpmultishiftqb ymm16, ymm13, [rbx]{1to4}
    vpmultishiftqb ymm16, ymm13, [rbx+r11*8+256]
    vpmultishiftqb ymm16, ymm13, [rbx+r11*8-256]
    vpmultishiftqb ymm16{k5}, ymm13, ymm15
    vpmultishiftqb ymm16{k5}, ymm13, [rbx]
    vpmultishiftqb ymm16{k5}, ymm13, [rbx]{1to4}
    vpmultishiftqb ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpmultishiftqb ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpmultishiftqb ymm16{k5}{z}, ymm13, ymm15
    vpmultishiftqb ymm16{k5}{z}, ymm13, [rbx]
    vpmultishiftqb ymm16{k5}{z}, ymm13, [rbx]{1to4}
    vpmultishiftqb ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpmultishiftqb ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpmultishiftqb zmm24, zmm24, zmm31
    vpmultishiftqb zmm24, zmm24, [rbx]
    vpmultishiftqb zmm24, zmm24, [rbx]{1to8}
    vpmultishiftqb zmm24, zmm24, [rbx+r11*8+256]
    vpmultishiftqb zmm24, zmm24, [rbx+r11*8-256]
    vpmultishiftqb zmm24{k5}, zmm24, zmm31
    vpmultishiftqb zmm24{k5}, zmm24, [rbx]
    vpmultishiftqb zmm24{k5}, zmm24, [rbx]{1to8}
    vpmultishiftqb zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpmultishiftqb zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpmultishiftqb zmm24{k5}{z}, zmm24, zmm31
    vpmultishiftqb zmm24{k5}{z}, zmm24, [rbx]
    vpmultishiftqb zmm24{k5}{z}, zmm24, [rbx]{1to8}
    vpmultishiftqb zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpmultishiftqb zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpshldd xmm2, xmm7, xmm0, 0x90
    vpshldd xmm2, xmm7, [rbx], 0x90
    vpshldd xmm2, xmm7, [rbx]{1to4}, 0x90
    vpshldd xmm2, xmm7, [rbx+r11*8+256], 0x90
    vpshldd xmm2, xmm7, [rbx+r11*8-256], 0x90
    vpshldd xmm2{k5}, xmm7, xmm0, 0x90
    vpshldd xmm2{k5}, xmm7, [rbx], 0x90
    vpshldd xmm2{k5}, xmm7, [rbx]{1to4}, 0x90
    vpshldd xmm2{k5}, xmm7, [rbx+r11*8+256], 0x90
    vpshldd xmm2{k5}, xmm7, [rbx+r11*8-256], 0x90
    vpshldd xmm2{k5}{z}, xmm7, xmm0, 0x90
    vpshldd xmm2{k5}{z}, xmm7, [rbx], 0x90
    vpshldd xmm2{k5}{z}, xmm7, [rbx]{1to4}, 0x90
    vpshldd xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 0x90
    vpshldd xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 0x90
    vpshldd ymm16, ymm13, ymm15, 0x90
    vpshldd ymm16, ymm13, [rbx], 0x90
    vpshldd ymm16, ymm13, [rbx]{1to8}, 0x90
    vpshldd ymm16, ymm13, [rbx+r11*8+256], 0x90
    vpshldd ymm16, ymm13, [rbx+r11*8-256], 0x90
    vpshldd ymm16{k5}, ymm13, ymm15, 0x90
    vpshldd ymm16{k5}, ymm13, [rbx], 0x90
    vpshldd ymm16{k5}, ymm13, [rbx]{1to8}, 0x90
    vpshldd ymm16{k5}, ymm13, [rbx+r11*8+256], 0x90
    vpshldd ymm16{k5}, ymm13, [rbx+r11*8-256], 0x90
    vpshldd ymm16{k5}{z}, ymm13, ymm15, 0x90
    vpshldd ymm16{k5}{z}, ymm13, [rbx], 0x90
    vpshldd ymm16{k5}{z}, ymm13, [rbx]{1to8}, 0x90
    vpshldd ymm16{k5}{z}, ymm13, [rbx+r11*8+256], 0x90
    vpshldd ymm16{k5}{z}, ymm13, [rbx+r11*8-256], 0x90
    vpshldd zmm24, zmm24, zmm31, 0x90
    vpshldd zmm24, zmm24, [rbx], 0x90
    vpshldd zmm24, zmm24, [rbx]{1to16}, 0x90
    vpshldd zmm24, zmm24, [rbx+r11*8+256], 0x90
    vpshldd zmm24, zmm24, [rbx+r11*8-256], 0x90
    vpshldd zmm24{k5}, zmm24, zmm31, 0x90
    vpshldd zmm24{k5}, zmm24, [rbx], 0x90
    vpshldd zmm24{k5}, zmm24, [rbx]{1to16}, 0x90
    vpshldd zmm24{k5}, zmm24, [rbx+r11*8+256], 0x90
    vpshldd zmm24{k5}, zmm24, [rbx+r11*8-256], 0x90
    vpshldd zmm24{k5}{z}, zmm24, zmm31, 0x90
    vpshldd zmm24{k5}{z}, zmm24, [rbx], 0x90
    vpshldd zmm24{k5}{z}, zmm24, [rbx]{1to16}, 0x90
    vpshldd zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 0x90
    vpshldd zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 0x90
    vpshldq xmm2, xmm7, xmm0, 0x90
    vpshldq xmm2, xmm7, [rbx], 0x90
    vpshldq xmm2, xmm7, [rbx]{1to2}, 0x90
    vpshldq xmm2, xmm7, [rbx+r11*8+256], 0x90
    vpshldq xmm2, xmm7, [rbx+r11*8-256], 0x90
    vpshldq xmm2{k5}, xmm7, xmm0, 0x90
    vpshldq xmm2{k5}, xmm7, [rbx], 0x90
    vpshldq xmm2{k5}, xmm7, [rbx]{1to2}, 0x90
    vpshldq xmm2{k5}, xmm7, [rbx+r11*8+256], 0x90
    vpshldq xmm2{k5}, xmm7, [rbx+r11*8-256], 0x90
    vpshldq xmm2{k5}{z}, xmm7, xmm0, 0x90
    vpshldq xmm2{k5}{z}, xmm7, [rbx], 0x90
    vpshldq xmm2{k5}{z}, xmm7, [rbx]{1to2}, 0x90
    vpshldq xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 0x90
    vpshldq xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 0x90
    vpshldq ymm16, ymm13, ymm15, 0x90
    vpshldq ymm16, ymm13, [rbx], 0x90
    vpshldq ymm16, ymm13, [rbx]{1to4}, 0x90
    vpshldq ymm16, ymm13, [rbx+r11*8+256], 0x90
    vpshldq ymm16, ymm13, [rbx+r11*8-256], 0x90
    vpshldq ymm16{k5}, ymm13, ymm15, 0x90
    vpshldq ymm16{k5}, ymm13, [rbx], 0x90
    vpshldq ymm16{k5}, ymm13, [rbx]{1to4}, 0x90
    vpshldq ymm16{k5}, ymm13, [rbx+r11*8+256], 0x90
    vpshldq ymm16{k5}, ymm13, [rbx+r11*8-256], 0x90
    vpshldq ymm16{k5}{z}, ymm13, ymm15, 0x90
    vpshldq ymm16{k5}{z}, ymm13, [rbx], 0x90
    vpshldq ymm16{k5}{z}, ymm13, [rbx]{1to4}, 0x90
    vpshldq ymm16{k5}{z}, ymm13, [rbx+r11*8+256], 0x90
    vpshldq ymm16{k5}{z}, ymm13, [rbx+r11*8-256], 0x90
    vpshldq zmm24, zmm24, zmm31, 0x90
    vpshldq zmm24, zmm24, [rbx], 0x90
    vpshldq zmm24, zmm24, [rbx]{1to8}, 0x90
    vpshldq zmm24, zmm24, [rbx+r11*8+256], 0x90
    vpshldq zmm24, zmm24, [rbx+r11*8-256], 0x90
    vpshldq zmm24{k5}, zmm24, zmm31, 0x90
    vpshldq zmm24{k5}, zmm24, [rbx], 0x90
    vpshldq zmm24{k5}, zmm24, [rbx]{1to8}, 0x90
    vpshldq zmm24{k5}, zmm24, [rbx+r11*8+256], 0x90
    vpshldq zmm24{k5}, zmm24, [rbx+r11*8-256], 0x90
    vpshldq zmm24{k5}{z}, zmm24, zmm31, 0x90
    vpshldq zmm24{k5}{z}, zmm24, [rbx], 0x90
    vpshldq zmm24{k5}{z}, zmm24, [rbx]{1to8}, 0x90
    vpshldq zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 0x90
    vpshldq zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 0x90
    vpshldvd xmm2, xmm7, xmm0
    vpshldvd xmm2, xmm7, [rbx]
    vpshldvd xmm2, xmm7, [rbx]{1to4}
    vpshldvd xmm2, xmm7, [rbx+r11*8+256]
    vpshldvd xmm2, xmm7, [rbx+r11*8-256]
    vpshldvd xmm2{k5}, xmm7, xmm0
    vpshldvd xmm2{k5}, xmm7, [rbx]
    vpshldvd xmm2{k5}, xmm7, [rbx]{1to4}
    vpshldvd xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpshldvd xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpshldvd xmm2{k5}{z}, xmm7, xmm0
    vpshldvd xmm2{k5}{z}, xmm7, [rbx]
    vpshldvd xmm2{k5}{z}, xmm7, [rbx]{1to4}
    vpshldvd xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpshldvd xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpshldvd ymm16, ymm13, ymm15
    vpshldvd ymm16, ymm13, [rbx]
    vpshldvd ymm16, ymm13, [rbx]{1to8}
    vpshldvd ymm16, ymm13, [rbx+r11*8+256]
    vpshldvd ymm16, ymm13, [rbx+r11*8-256]
    vpshldvd ymm16{k5}, ymm13, ymm15
    vpshldvd ymm16{k5}, ymm13, [rbx]
    vpshldvd ymm16{k5}, ymm13, [rbx]{1to8}
    vpshldvd ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpshldvd ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpshldvd ymm16{k5}{z}, ymm13, ymm15
    vpshldvd ymm16{k5}{z}, ymm13, [rbx]
    vpshldvd ymm16{k5}{z}, ymm13, [rbx]{1to8}
    vpshldvd ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpshldvd ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpshldvd zmm24, zmm24, zmm31
    vpshldvd zmm24, zmm24, [rbx]
    vpshldvd zmm24, zmm24, [rbx]{1to16}
    vpshldvd zmm24, zmm24, [rbx+r11*8+256]
    vpshldvd zmm24, zmm24, [rbx+r11*8-256]
    vpshldvd zmm24{k5}, zmm24, zmm31
    vpshldvd zmm24{k5}, zmm24, [rbx]
    vpshldvd zmm24{k5}, zmm24, [rbx]{1to16}
    vpshldvd zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpshldvd zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpshldvd zmm24{k5}{z}, zmm24, zmm31
    vpshldvd zmm24{k5}{z}, zmm24, [rbx]
    vpshldvd zmm24{k5}{z}, zmm24, [rbx]{1to16}
    vpshldvd zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpshldvd zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpshldvq xmm2, xmm7, xmm0
    vpshldvq xmm2, xmm7, [rbx]
    vpshldvq xmm2, xmm7, [rbx]{1to2}
    vpshldvq xmm2, xmm7, [rbx+r11*8+256]
    vpshldvq xmm2, xmm7, [rbx+r11*8-256]
    vpshldvq xmm2{k5}, xmm7, xmm0
    vpshldvq xmm2{k5}, xmm7, [rbx]
    vpshldvq xmm2{k5}, xmm7, [rbx]{1to2}
    vpshldvq xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpshldvq xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpshldvq xmm2{k5}{z}, xmm7, xmm0
    vpshldvq xmm2{k5}{z}, xmm7, [rbx]
    vpshldvq xmm2{k5}{z}, xmm7, [rbx]{1to2}
    vpshldvq xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpshldvq xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpshldvq ymm16, ymm13, ymm15
    vpshldvq ymm16, ymm13, [rbx]
    vpshldvq ymm16, ymm13, [rbx]{1to4}
    vpshldvq ymm16, ymm13, [rbx+r11*8+256]
    vpshldvq ymm16, ymm13, [rbx+r11*8-256]
    vpshldvq ymm16{k5}, ymm13, ymm15
    vpshldvq ymm16{k5}, ymm13, [rbx]
    vpshldvq ymm16{k5}, ymm13, [rbx]{1to4}
    vpshldvq ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpshldvq ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpshldvq ymm16{k5}{z}, ymm13, ymm15
    vpshldvq ymm16{k5}{z}, ymm13, [rbx]
    vpshldvq ymm16{k5}{z}, ymm13, [rbx]{1to4}
    vpshldvq ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpshldvq ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpshldvq zmm24, zmm24, zmm31
    vpshldvq zmm24, zmm24, [rbx]
    vpshldvq zmm24, zmm24, [rbx]{1to8}
    vpshldvq zmm24, zmm24, [rbx+r11*8+256]
    vpshldvq zmm24, zmm24, [rbx+r11*8-256]
    vpshldvq zmm24{k5}, zmm24, zmm31
    vpshldvq zmm24{k5}, zmm24, [rbx]
    vpshldvq zmm24{k5}, zmm24, [rbx]{1to8}
    vpshldvq zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpshldvq zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpshldvq zmm24{k5}{z}, zmm24, zmm31
    vpshldvq zmm24{k5}{z}, zmm24, [rbx]
    vpshldvq zmm24{k5}{z}, zmm24, [rbx]{1to8}
    vpshldvq zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpshldvq zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpshldvw xmm2, xmm7, xmm0
    vpshldvw xmm2, xmm7, [rbx]
    vpshldvw xmm2, xmm7, [rbx+r11*8+256]
    vpshldvw xmm2, xmm7, [rbx+r11*8-256]
    vpshldvw xmm2{k5}, xmm7, xmm0
    vpshldvw xmm2{k5}, xmm7, [rbx]
    vpshldvw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpshldvw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpshldvw xmm2{k5}{z}, xmm7, xmm0
    vpshldvw xmm2{k5}{z}, xmm7, [rbx]
    vpshldvw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpshldvw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpshldvw ymm16, ymm13, ymm15
    vpshldvw ymm16, ymm13, [rbx]
    vpshldvw ymm16, ymm13, [rbx+r11*8+256]
    vpshldvw ymm16, ymm13, [rbx+r11*8-256]
    vpshldvw ymm16{k5}, ymm13, ymm15
    vpshldvw ymm16{k5}, ymm13, [rbx]
    vpshldvw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpshldvw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpshldvw ymm16{k5}{z}, ymm13, ymm15
    vpshldvw ymm16{k5}{z}, ymm13, [rbx]
    vpshldvw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpshldvw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpshldvw zmm24, zmm24, zmm31
    vpshldvw zmm24, zmm24, [rbx]
    vpshldvw zmm24, zmm24, [rbx+r11*8+256]
    vpshldvw zmm24, zmm24, [rbx+r11*8-256]
    vpshldvw zmm24{k5}, zmm24, zmm31
    vpshldvw zmm24{k5}, zmm24, [rbx]
    vpshldvw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpshldvw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpshldvw zmm24{k5}{z}, zmm24, zmm31
    vpshldvw zmm24{k5}{z}, zmm24, [rbx]
    vpshldvw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpshldvw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpshldw xmm2, xmm7, xmm0, 0x90
    vpshldw xmm2, xmm7, [rbx], 0x90
    vpshldw xmm2, xmm7, [rbx+r11*8+256], 0x90
    vpshldw xmm2, xmm7, [rbx+r11*8-256], 0x90
    vpshldw xmm2{k5}, xmm7, xmm0, 0x90
    vpshldw xmm2{k5}, xmm7, [rbx], 0x90
    vpshldw xmm2{k5}, xmm7, [rbx+r11*8+256], 0x90
    vpshldw xmm2{k5}, xmm7, [rbx+r11*8-256], 0x90
    vpshldw xmm2{k5}{z}, xmm7, xmm0, 0x90
    vpshldw xmm2{k5}{z}, xmm7, [rbx], 0x90
    vpshldw xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 0x90
    vpshldw xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 0x90
    vpshldw ymm16, ymm13, ymm15, 0x90
    vpshldw ymm16, ymm13, [rbx], 0x90
    vpshldw ymm16, ymm13, [rbx+r11*8+256], 0x90
    vpshldw ymm16, ymm13, [rbx+r11*8-256], 0x90
    vpshldw ymm16{k5}, ymm13, ymm15, 0x90
    vpshldw ymm16{k5}, ymm13, [rbx], 0x90
    vpshldw ymm16{k5}, ymm13, [rbx+r11*8+256], 0x90
    vpshldw ymm16{k5}, ymm13, [rbx+r11*8-256], 0x90
    vpshldw ymm16{k5}{z}, ymm13, ymm15, 0x90
    vpshldw ymm16{k5}{z}, ymm13, [rbx], 0x90
    vpshldw ymm16{k5}{z}, ymm13, [rbx+r11*8+256], 0x90
    vpshldw ymm16{k5}{z}, ymm13, [rbx+r11*8-256], 0x90
    vpshldw zmm24, zmm24, zmm31, 0x90
    vpshldw zmm24, zmm24, [rbx], 0x90
    vpshldw zmm24, zmm24, [rbx+r11*8+256], 0x90
    vpshldw zmm24, zmm24, [rbx+r11*8-256], 0x90
    vpshldw zmm24{k5}, zmm24, zmm31, 0x90
    vpshldw zmm24{k5}, zmm24, [rbx], 0x90
    vpshldw zmm24{k5}, zmm24, [rbx+r11*8+256], 0x90
    vpshldw zmm24{k5}, zmm24, [rbx+r11*8-256], 0x90
    vpshldw zmm24{k5}{z}, zmm24, zmm31, 0x90
    vpshldw zmm24{k5}{z}, zmm24, [rbx], 0x90
    vpshldw zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 0x90
    vpshldw zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 0x90
    vpshrdd xmm2, xmm7, xmm0, 0x90
    vpshrdd xmm2, xmm7, [rbx], 0x90
    vpshrdd xmm2, xmm7, [rbx]{1to4}, 0x90
    vpshrdd xmm2, xmm7, [rbx+r11*8+256], 0x90
    vpshrdd xmm2, xmm7, [rbx+r11*8-256], 0x90
    vpshrdd xmm2{k5}, xmm7, xmm0, 0x90
    vpshrdd xmm2{k5}, xmm7, [rbx], 0x90
    vpshrdd xmm2{k5}, xmm7, [rbx]{1to4}, 0x90
    vpshrdd xmm2{k5}, xmm7, [rbx+r11*8+256], 0x90
    vpshrdd xmm2{k5}, xmm7, [rbx+r11*8-256], 0x90
    vpshrdd xmm2{k5}{z}, xmm7, xmm0, 0x90
    vpshrdd xmm2{k5}{z}, xmm7, [rbx], 0x90
    vpshrdd xmm2{k5}{z}, xmm7, [rbx]{1to4}, 0x90
    vpshrdd xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 0x90
    vpshrdd xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 0x90
    vpshrdd ymm16, ymm13, ymm15, 0x90
    vpshrdd ymm16, ymm13, [rbx], 0x90
    vpshrdd ymm16, ymm13, [rbx]{1to8}, 0x90
    vpshrdd ymm16, ymm13, [rbx+r11*8+256], 0x90
    vpshrdd ymm16, ymm13, [rbx+r11*8-256], 0x90
    vpshrdd ymm16{k5}, ymm13, ymm15, 0x90
    vpshrdd ymm16{k5}, ymm13, [rbx], 0x90
    vpshrdd ymm16{k5}, ymm13, [rbx]{1to8}, 0x90
    vpshrdd ymm16{k5}, ymm13, [rbx+r11*8+256], 0x90
    vpshrdd ymm16{k5}, ymm13, [rbx+r11*8-256], 0x90
    vpshrdd ymm16{k5}{z}, ymm13, ymm15, 0x90
    vpshrdd ymm16{k5}{z}, ymm13, [rbx], 0x90
    vpshrdd ymm16{k5}{z}, ymm13, [rbx]{1to8}, 0x90
    vpshrdd ymm16{k5}{z}, ymm13, [rbx+r11*8+256], 0x90
    vpshrdd ymm16{k5}{z}, ymm13, [rbx+r11*8-256], 0x90
    vpshrdd zmm24, zmm24, zmm31, 0x90
    vpshrdd zmm24, zmm24, [rbx], 0x90
    vpshrdd zmm24, zmm24, [rbx]{1to16}, 0x90
    vpshrdd zmm24, zmm24, [rbx+r11*8+256], 0x90
    vpshrdd zmm24, zmm24, [rbx+r11*8-256], 0x90
    vpshrdd zmm24{k5}, zmm24, zmm31, 0x90
    vpshrdd zmm24{k5}, zmm24, [rbx], 0x90
    vpshrdd zmm24{k5}, zmm24, [rbx]{1to16}, 0x90
    vpshrdd zmm24{k5}, zmm24, [rbx+r11*8+256], 0x90
    vpshrdd zmm24{k5}, zmm24, [rbx+r11*8-256], 0x90
    vpshrdd zmm24{k5}{z}, zmm24, zmm31, 0x90
    vpshrdd zmm24{k5}{z}, zmm24, [rbx], 0x90
    vpshrdd zmm24{k5}{z}, zmm24, [rbx]{1to16}, 0x90
    vpshrdd zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 0x90
    vpshrdd zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 0x90
    vpshrdq xmm2, xmm7, xmm0, 0x90
    vpshrdq xmm2, xmm7, [rbx], 0x90
    vpshrdq xmm2, xmm7, [rbx]{1to2}, 0x90
    vpshrdq xmm2, xmm7, [rbx+r11*8+256], 0x90
    vpshrdq xmm2, xmm7, [rbx+r11*8-256], 0x90
    vpshrdq xmm2{k5}, xmm7, xmm0, 0x90
    vpshrdq xmm2{k5}, xmm7, [rbx], 0x90
    vpshrdq xmm2{k5}, xmm7, [rbx]{1to2}, 0x90
    vpshrdq xmm2{k5}, xmm7, [rbx+r11*8+256], 0x90
    vpshrdq xmm2{k5}, xmm7, [rbx+r11*8-256], 0x90
    vpshrdq xmm2{k5}{z}, xmm7, xmm0, 0x90
    vpshrdq xmm2{k5}{z}, xmm7, [rbx], 0x90
    vpshrdq xmm2{k5}{z}, xmm7, [rbx]{1to2}, 0x90
    vpshrdq xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 0x90
    vpshrdq xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 0x90
    vpshrdq ymm16, ymm13, ymm15, 0x90
    vpshrdq ymm16, ymm13, [rbx], 0x90
    vpshrdq ymm16, ymm13, [rbx]{1to4}, 0x90
    vpshrdq ymm16, ymm13, [rbx+r11*8+256], 0x90
    vpshrdq ymm16, ymm13, [rbx+r11*8-256], 0x90
    vpshrdq ymm16{k5}, ymm13, ymm15, 0x90
    vpshrdq ymm16{k5}, ymm13, [rbx], 0x90
    vpshrdq ymm16{k5}, ymm13, [rbx]{1to4}, 0x90
    vpshrdq ymm16{k5}, ymm13, [rbx+r11*8+256], 0x90
    vpshrdq ymm16{k5}, ymm13, [rbx+r11*8-256], 0x90
    vpshrdq ymm16{k5}{z}, ymm13, ymm15, 0x90
    vpshrdq ymm16{k5}{z}, ymm13, [rbx], 0x90
    vpshrdq ymm16{k5}{z}, ymm13, [rbx]{1to4}, 0x90
    vpshrdq ymm16{k5}{z}, ymm13, [rbx+r11*8+256], 0x90
    vpshrdq ymm16{k5}{z}, ymm13, [rbx+r11*8-256], 0x90
    vpshrdq zmm24, zmm24, zmm31, 0x90
    vpshrdq zmm24, zmm24, [rbx], 0x90
    vpshrdq zmm24, zmm24, [rbx]{1to8}, 0x90
    vpshrdq zmm24, zmm24, [rbx+r11*8+256], 0x90
    vpshrdq zmm24, zmm24, [rbx+r11*8-256], 0x90
    vpshrdq zmm24{k5}, zmm24, zmm31, 0x90
    vpshrdq zmm24{k5}, zmm24, [rbx], 0x90
    vpshrdq zmm24{k5}, zmm24, [rbx]{1to8}, 0x90
    vpshrdq zmm24{k5}, zmm24, [rbx+r11*8+256], 0x90
    vpshrdq zmm24{k5}, zmm24, [rbx+r11*8-256], 0x90
    vpshrdq zmm24{k5}{z}, zmm24, zmm31, 0x90
    vpshrdq zmm24{k5}{z}, zmm24, [rbx], 0x90
    vpshrdq zmm24{k5}{z}, zmm24, [rbx]{1to8}, 0x90
    vpshrdq zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 0x90
    vpshrdq zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 0x90
    vpshrdvd xmm2, xmm7, xmm0
    vpshrdvd xmm2, xmm7, [rbx]
    vpshrdvd xmm2, xmm7, [rbx]{1to4}
    vpshrdvd xmm2, xmm7, [rbx+r11*8+256]
    vpshrdvd xmm2, xmm7, [rbx+r11*8-256]
    vpshrdvd xmm2{k5}, xmm7, xmm0
    vpshrdvd xmm2{k5}, xmm7, [rbx]
    vpshrdvd xmm2{k5}, xmm7, [rbx]{1to4}
    vpshrdvd xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpshrdvd xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpshrdvd xmm2{k5}{z}, xmm7, xmm0
    vpshrdvd xmm2{k5}{z}, xmm7, [rbx]
    vpshrdvd xmm2{k5}{z}, xmm7, [rbx]{1to4}
    vpshrdvd xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpshrdvd xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpshrdvd ymm16, ymm13, ymm15
    vpshrdvd ymm16, ymm13, [rbx]
    vpshrdvd ymm16, ymm13, [rbx]{1to8}
    vpshrdvd ymm16, ymm13, [rbx+r11*8+256]
    vpshrdvd ymm16, ymm13, [rbx+r11*8-256]
    vpshrdvd ymm16{k5}, ymm13, ymm15
    vpshrdvd ymm16{k5}, ymm13, [rbx]
    vpshrdvd ymm16{k5}, ymm13, [rbx]{1to8}
    vpshrdvd ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpshrdvd ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpshrdvd ymm16{k5}{z}, ymm13, ymm15
    vpshrdvd ymm16{k5}{z}, ymm13, [rbx]
    vpshrdvd ymm16{k5}{z}, ymm13, [rbx]{1to8}
    vpshrdvd ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpshrdvd ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpshrdvd zmm24, zmm24, zmm31
    vpshrdvd zmm24, zmm24, [rbx]
    vpshrdvd zmm24, zmm24, [rbx]{1to16}
    vpshrdvd zmm24, zmm24, [rbx+r11*8+256]
    vpshrdvd zmm24, zmm24, [rbx+r11*8-256]
    vpshrdvd zmm24{k5}, zmm24, zmm31
    vpshrdvd zmm24{k5}, zmm24, [rbx]
    vpshrdvd zmm24{k5}, zmm24, [rbx]{1to16}
    vpshrdvd zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpshrdvd zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpshrdvd zmm24{k5}{z}, zmm24, zmm31
    vpshrdvd zmm24{k5}{z}, zmm24, [rbx]
    vpshrdvd zmm24{k5}{z}, zmm24, [rbx]{1to16}
    vpshrdvd zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpshrdvd zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpshrdvq xmm2, xmm7, xmm0
    vpshrdvq xmm2, xmm7, [rbx]
    vpshrdvq xmm2, xmm7, [rbx]{1to2}
    vpshrdvq xmm2, xmm7, [rbx+r11*8+256]
    vpshrdvq xmm2, xmm7, [rbx+r11*8-256]
    vpshrdvq xmm2{k5}, xmm7, xmm0
    vpshrdvq xmm2{k5}, xmm7, [rbx]
    vpshrdvq xmm2{k5}, xmm7, [rbx]{1to2}
    vpshrdvq xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpshrdvq xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpshrdvq xmm2{k5}{z}, xmm7, xmm0
    vpshrdvq xmm2{k5}{z}, xmm7, [rbx]
    vpshrdvq xmm2{k5}{z}, xmm7, [rbx]{1to2}
    vpshrdvq xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpshrdvq xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpshrdvq ymm16, ymm13, ymm15
    vpshrdvq ymm16, ymm13, [rbx]
    vpshrdvq ymm16, ymm13, [rbx]{1to4}
    vpshrdvq ymm16, ymm13, [rbx+r11*8+256]
    vpshrdvq ymm16, ymm13, [rbx+r11*8-256]
    vpshrdvq ymm16{k5}, ymm13, ymm15
    vpshrdvq ymm16{k5}, ymm13, [rbx]
    vpshrdvq ymm16{k5}, ymm13, [rbx]{1to4}
    vpshrdvq ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpshrdvq ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpshrdvq ymm16{k5}{z}, ymm13, ymm15
    vpshrdvq ymm16{k5}{z}, ymm13, [rbx]
    vpshrdvq ymm16{k5}{z}, ymm13, [rbx]{1to4}
    vpshrdvq ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpshrdvq ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpshrdvq zmm24, zmm24, zmm31
    vpshrdvq zmm24, zmm24, [rbx]
    vpshrdvq zmm24, zmm24, [rbx]{1to8}
    vpshrdvq zmm24, zmm24, [rbx+r11*8+256]
    vpshrdvq zmm24, zmm24, [rbx+r11*8-256]
    vpshrdvq zmm24{k5}, zmm24, zmm31
    vpshrdvq zmm24{k5}, zmm24, [rbx]
    vpshrdvq zmm24{k5}, zmm24, [rbx]{1to8}
    vpshrdvq zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpshrdvq zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpshrdvq zmm24{k5}{z}, zmm24, zmm31
    vpshrdvq zmm24{k5}{z}, zmm24, [rbx]
    vpshrdvq zmm24{k5}{z}, zmm24, [rbx]{1to8}
    vpshrdvq zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpshrdvq zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpshrdvw xmm2, xmm7, xmm0
    vpshrdvw xmm2, xmm7, [rbx]
    vpshrdvw xmm2, xmm7, [rbx+r11*8+256]
    vpshrdvw xmm2, xmm7, [rbx+r11*8-256]
    vpshrdvw xmm2{k5}, xmm7, xmm0
    vpshrdvw xmm2{k5}, xmm7, [rbx]
    vpshrdvw xmm2{k5}, xmm7, [rbx+r11*8+256]
    vpshrdvw xmm2{k5}, xmm7, [rbx+r11*8-256]
    vpshrdvw xmm2{k5}{z}, xmm7, xmm0
    vpshrdvw xmm2{k5}{z}, xmm7, [rbx]
    vpshrdvw xmm2{k5}{z}, xmm7, [rbx+r11*8+256]
    vpshrdvw xmm2{k5}{z}, xmm7, [rbx+r11*8-256]
    vpshrdvw ymm16, ymm13, ymm15
    vpshrdvw ymm16, ymm13, [rbx]
    vpshrdvw ymm16, ymm13, [rbx+r11*8+256]
    vpshrdvw ymm16, ymm13, [rbx+r11*8-256]
    vpshrdvw ymm16{k5}, ymm13, ymm15
    vpshrdvw ymm16{k5}, ymm13, [rbx]
    vpshrdvw ymm16{k5}, ymm13, [rbx+r11*8+256]
    vpshrdvw ymm16{k5}, ymm13, [rbx+r11*8-256]
    vpshrdvw ymm16{k5}{z}, ymm13, ymm15
    vpshrdvw ymm16{k5}{z}, ymm13, [rbx]
    vpshrdvw ymm16{k5}{z}, ymm13, [rbx+r11*8+256]
    vpshrdvw ymm16{k5}{z}, ymm13, [rbx+r11*8-256]
    vpshrdvw zmm24, zmm24, zmm31
    vpshrdvw zmm24, zmm24, [rbx]
    vpshrdvw zmm24, zmm24, [rbx+r11*8+256]
    vpshrdvw zmm24, zmm24, [rbx+r11*8-256]
    vpshrdvw zmm24{k5}, zmm24, zmm31
    vpshrdvw zmm24{k5}, zmm24, [rbx]
    vpshrdvw zmm24{k5}, zmm24, [rbx+r11*8+256]
    vpshrdvw zmm24{k5}, zmm24, [rbx+r11*8-256]
    vpshrdvw zmm24{k5}{z}, zmm24, zmm31
    vpshrdvw zmm24{k5}{z}, zmm24, [rbx]
    vpshrdvw zmm24{k5}{z}, zmm24, [rbx+r11*8+256]
    vpshrdvw zmm24{k5}{z}, zmm24, [rbx+r11*8-256]
    vpshrdw xmm2, xmm7, xmm0, 0x90
    vpshrdw xmm2, xmm7, [rbx], 0x90
    vpshrdw xmm2, xmm7, [rbx+r11*8+256], 0x90
    vpshrdw xmm2, xmm7, [rbx+r11*8-256], 0x90
    vpshrdw xmm2{k5}, xmm7, xmm0, 0x90
    vpshrdw xmm2{k5}, xmm7, [rbx], 0x90
    vpshrdw xmm2{k5}, xmm7, [rbx+r11*8+256], 0x90
    vpshrdw xmm2{k5}, xmm7, [rbx+r11*8-256], 0x90
    vpshrdw xmm2{k5}{z}, xmm7, xmm0, 0x90
    vpshrdw xmm2{k5}{z}, xmm7, [rbx], 0x90
    vpshrdw xmm2{k5}{z}, xmm7, [rbx+r11*8+256], 0x90
    vpshrdw xmm2{k5}{z}, xmm7, [rbx+r11*8-256], 0x90
    vpshrdw ymm16, ymm13, ymm15, 0x90
    vpshrdw ymm16, ymm13, [rbx], 0x90
    vpshrdw ymm16, ymm13, [rbx+r11*8+256], 0x90
    vpshrdw ymm16, ymm13, [rbx+r11*8-256], 0x90
    vpshrdw ymm16{k5}, ymm13, ymm15, 0x90
    vpshrdw ymm16{k5}, ymm13, [rbx], 0x90
    vpshrdw ymm16{k5}, ymm13, [rbx+r11*8+256], 0x90
    vpshrdw ymm16{k5}, ymm13, [rbx+r11*8-256], 0x90
    vpshrdw ymm16{k5}{z}, ymm13, ymm15, 0x90
    vpshrdw ymm16{k5}{z}, ymm13, [rbx], 0x90
    vpshrdw ymm16{k5}{z}, ymm13, [rbx+r11*8+256], 0x90
    vpshrdw ymm16{k5}{z}, ymm13, [rbx+r11*8-256], 0x90
    vpshrdw zmm24, zmm24, zmm31, 0x90
    vpshrdw zmm24, zmm24, [rbx], 0x90
    vpshrdw zmm24, zmm24, [rbx+r11*8+256], 0x90
    vpshrdw zmm24, zmm24, [rbx+r11*8-256], 0x90
    vpshrdw zmm24{k5}, zmm24, zmm31, 0x90
    vpshrdw zmm24{k5}, zmm24, [rbx], 0x90
    vpshrdw zmm24{k5}, zmm24, [rbx+r11*8+256], 0x90
    vpshrdw zmm24{k5}, zmm24, [rbx+r11*8-256], 0x90
    vpshrdw zmm24{k5}{z}, zmm24, zmm31, 0x90
    vpshrdw zmm24{k5}{z}, zmm24, [rbx], 0x90
    vpshrdw zmm24{k5}{z}, zmm24, [rbx+r11*8+256], 0x90
    vpshrdw zmm24{k5}{z}, zmm24, [rbx+r11*8-256], 0x90
