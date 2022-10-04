    bits 64
    
    bndldx      bnd0, [rbx + rsi]
    bndmov      bnd0, bnd3
    bndmov      bnd0, [rbx]
    bndcl       bnd0, rax
    bndcl       bnd0, [rbx]
    bndcu       bnd0, rax
    bndcu       bnd0, [rbx]
    bndstx      [rbx + rsi], bnd0
    bndmov      bnd0, bnd3
    bndmov      [rbx], bnd0
    bndmk       bnd0, [rbx]
    bndcn       bnd0, rax
    bndcn       bnd0, [rbx]