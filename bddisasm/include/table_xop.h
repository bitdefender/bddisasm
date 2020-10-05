#ifndef TABLE_XOP_H
#define TABLE_XOP_H

const ND_TABLE_INSTRUCTION gXopTable_root_0a_10_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[64]
};

const ND_TABLE_INSTRUCTION gXopTable_root_0a_12_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[597]
};

const ND_TABLE_INSTRUCTION gXopTable_root_0a_12_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[598]
};

const ND_TABLE_MODRM_REG gXopTable_root_0a_12_modrmreg = 
{
    ND_ILUT_MODRM_REG,
    { 
        /* 00 */ (const void *)&gXopTable_root_0a_12_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_0a_12_01_leaf,
        /* 02 */ NULL,
        /* 03 */ NULL,
        /* 04 */ NULL,
        /* 05 */ NULL,
        /* 06 */ NULL,
        /* 07 */ NULL,
    }
};

const ND_TABLE_OPCODE gXopTable_root_0a_opcode = 
{
    ND_ILUT_OPCODE,
    { 
        /* 00 */ NULL,
        /* 01 */ NULL,
        /* 02 */ NULL,
        /* 03 */ NULL,
        /* 04 */ NULL,
        /* 05 */ NULL,
        /* 06 */ NULL,
        /* 07 */ NULL,
        /* 08 */ NULL,
        /* 09 */ NULL,
        /* 0a */ NULL,
        /* 0b */ NULL,
        /* 0c */ NULL,
        /* 0d */ NULL,
        /* 0e */ NULL,
        /* 0f */ NULL,
        /* 10 */ (const void *)&gXopTable_root_0a_10_leaf,
        /* 11 */ NULL,
        /* 12 */ (const void *)&gXopTable_root_0a_12_modrmreg,
        /* 13 */ NULL,
        /* 14 */ NULL,
        /* 15 */ NULL,
        /* 16 */ NULL,
        /* 17 */ NULL,
        /* 18 */ NULL,
        /* 19 */ NULL,
        /* 1a */ NULL,
        /* 1b */ NULL,
        /* 1c */ NULL,
        /* 1d */ NULL,
        /* 1e */ NULL,
        /* 1f */ NULL,
        /* 20 */ NULL,
        /* 21 */ NULL,
        /* 22 */ NULL,
        /* 23 */ NULL,
        /* 24 */ NULL,
        /* 25 */ NULL,
        /* 26 */ NULL,
        /* 27 */ NULL,
        /* 28 */ NULL,
        /* 29 */ NULL,
        /* 2a */ NULL,
        /* 2b */ NULL,
        /* 2c */ NULL,
        /* 2d */ NULL,
        /* 2e */ NULL,
        /* 2f */ NULL,
        /* 30 */ NULL,
        /* 31 */ NULL,
        /* 32 */ NULL,
        /* 33 */ NULL,
        /* 34 */ NULL,
        /* 35 */ NULL,
        /* 36 */ NULL,
        /* 37 */ NULL,
        /* 38 */ NULL,
        /* 39 */ NULL,
        /* 3a */ NULL,
        /* 3b */ NULL,
        /* 3c */ NULL,
        /* 3d */ NULL,
        /* 3e */ NULL,
        /* 3f */ NULL,
        /* 40 */ NULL,
        /* 41 */ NULL,
        /* 42 */ NULL,
        /* 43 */ NULL,
        /* 44 */ NULL,
        /* 45 */ NULL,
        /* 46 */ NULL,
        /* 47 */ NULL,
        /* 48 */ NULL,
        /* 49 */ NULL,
        /* 4a */ NULL,
        /* 4b */ NULL,
        /* 4c */ NULL,
        /* 4d */ NULL,
        /* 4e */ NULL,
        /* 4f */ NULL,
        /* 50 */ NULL,
        /* 51 */ NULL,
        /* 52 */ NULL,
        /* 53 */ NULL,
        /* 54 */ NULL,
        /* 55 */ NULL,
        /* 56 */ NULL,
        /* 57 */ NULL,
        /* 58 */ NULL,
        /* 59 */ NULL,
        /* 5a */ NULL,
        /* 5b */ NULL,
        /* 5c */ NULL,
        /* 5d */ NULL,
        /* 5e */ NULL,
        /* 5f */ NULL,
        /* 60 */ NULL,
        /* 61 */ NULL,
        /* 62 */ NULL,
        /* 63 */ NULL,
        /* 64 */ NULL,
        /* 65 */ NULL,
        /* 66 */ NULL,
        /* 67 */ NULL,
        /* 68 */ NULL,
        /* 69 */ NULL,
        /* 6a */ NULL,
        /* 6b */ NULL,
        /* 6c */ NULL,
        /* 6d */ NULL,
        /* 6e */ NULL,
        /* 6f */ NULL,
        /* 70 */ NULL,
        /* 71 */ NULL,
        /* 72 */ NULL,
        /* 73 */ NULL,
        /* 74 */ NULL,
        /* 75 */ NULL,
        /* 76 */ NULL,
        /* 77 */ NULL,
        /* 78 */ NULL,
        /* 79 */ NULL,
        /* 7a */ NULL,
        /* 7b */ NULL,
        /* 7c */ NULL,
        /* 7d */ NULL,
        /* 7e */ NULL,
        /* 7f */ NULL,
        /* 80 */ NULL,
        /* 81 */ NULL,
        /* 82 */ NULL,
        /* 83 */ NULL,
        /* 84 */ NULL,
        /* 85 */ NULL,
        /* 86 */ NULL,
        /* 87 */ NULL,
        /* 88 */ NULL,
        /* 89 */ NULL,
        /* 8a */ NULL,
        /* 8b */ NULL,
        /* 8c */ NULL,
        /* 8d */ NULL,
        /* 8e */ NULL,
        /* 8f */ NULL,
        /* 90 */ NULL,
        /* 91 */ NULL,
        /* 92 */ NULL,
        /* 93 */ NULL,
        /* 94 */ NULL,
        /* 95 */ NULL,
        /* 96 */ NULL,
        /* 97 */ NULL,
        /* 98 */ NULL,
        /* 99 */ NULL,
        /* 9a */ NULL,
        /* 9b */ NULL,
        /* 9c */ NULL,
        /* 9d */ NULL,
        /* 9e */ NULL,
        /* 9f */ NULL,
        /* a0 */ NULL,
        /* a1 */ NULL,
        /* a2 */ NULL,
        /* a3 */ NULL,
        /* a4 */ NULL,
        /* a5 */ NULL,
        /* a6 */ NULL,
        /* a7 */ NULL,
        /* a8 */ NULL,
        /* a9 */ NULL,
        /* aa */ NULL,
        /* ab */ NULL,
        /* ac */ NULL,
        /* ad */ NULL,
        /* ae */ NULL,
        /* af */ NULL,
        /* b0 */ NULL,
        /* b1 */ NULL,
        /* b2 */ NULL,
        /* b3 */ NULL,
        /* b4 */ NULL,
        /* b5 */ NULL,
        /* b6 */ NULL,
        /* b7 */ NULL,
        /* b8 */ NULL,
        /* b9 */ NULL,
        /* ba */ NULL,
        /* bb */ NULL,
        /* bc */ NULL,
        /* bd */ NULL,
        /* be */ NULL,
        /* bf */ NULL,
        /* c0 */ NULL,
        /* c1 */ NULL,
        /* c2 */ NULL,
        /* c3 */ NULL,
        /* c4 */ NULL,
        /* c5 */ NULL,
        /* c6 */ NULL,
        /* c7 */ NULL,
        /* c8 */ NULL,
        /* c9 */ NULL,
        /* ca */ NULL,
        /* cb */ NULL,
        /* cc */ NULL,
        /* cd */ NULL,
        /* ce */ NULL,
        /* cf */ NULL,
        /* d0 */ NULL,
        /* d1 */ NULL,
        /* d2 */ NULL,
        /* d3 */ NULL,
        /* d4 */ NULL,
        /* d5 */ NULL,
        /* d6 */ NULL,
        /* d7 */ NULL,
        /* d8 */ NULL,
        /* d9 */ NULL,
        /* da */ NULL,
        /* db */ NULL,
        /* dc */ NULL,
        /* dd */ NULL,
        /* de */ NULL,
        /* df */ NULL,
        /* e0 */ NULL,
        /* e1 */ NULL,
        /* e2 */ NULL,
        /* e3 */ NULL,
        /* e4 */ NULL,
        /* e5 */ NULL,
        /* e6 */ NULL,
        /* e7 */ NULL,
        /* e8 */ NULL,
        /* e9 */ NULL,
        /* ea */ NULL,
        /* eb */ NULL,
        /* ec */ NULL,
        /* ed */ NULL,
        /* ee */ NULL,
        /* ef */ NULL,
        /* f0 */ NULL,
        /* f1 */ NULL,
        /* f2 */ NULL,
        /* f3 */ NULL,
        /* f4 */ NULL,
        /* f5 */ NULL,
        /* f6 */ NULL,
        /* f7 */ NULL,
        /* f8 */ NULL,
        /* f9 */ NULL,
        /* fa */ NULL,
        /* fb */ NULL,
        /* fc */ NULL,
        /* fd */ NULL,
        /* fe */ NULL,
        /* ff */ NULL,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_01_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[65]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_01_05_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[67]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_01_03_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[69]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_01_02_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[74]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_01_06_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[76]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_01_07_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1317]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_01_04_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1341]
};

const ND_TABLE_MODRM_REG gXopTable_root_09_01_modrmreg = 
{
    ND_ILUT_MODRM_REG,
    { 
        /* 00 */ NULL,
        /* 01 */ (const void *)&gXopTable_root_09_01_01_leaf,
        /* 02 */ (const void *)&gXopTable_root_09_01_02_leaf,
        /* 03 */ (const void *)&gXopTable_root_09_01_03_leaf,
        /* 04 */ (const void *)&gXopTable_root_09_01_04_leaf,
        /* 05 */ (const void *)&gXopTable_root_09_01_05_leaf,
        /* 06 */ (const void *)&gXopTable_root_09_01_06_leaf,
        /* 07 */ (const void *)&gXopTable_root_09_01_07_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_02_06_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[66]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_02_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[68]
};

const ND_TABLE_MODRM_REG gXopTable_root_09_02_modrmreg = 
{
    ND_ILUT_MODRM_REG,
    { 
        /* 00 */ NULL,
        /* 01 */ (const void *)&gXopTable_root_09_02_01_leaf,
        /* 02 */ NULL,
        /* 03 */ NULL,
        /* 04 */ NULL,
        /* 05 */ NULL,
        /* 06 */ (const void *)&gXopTable_root_09_02_06_leaf,
        /* 07 */ NULL,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_12_reg_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[579]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_12_reg_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1268]
};

const ND_TABLE_MODRM_REG gXopTable_root_09_12_reg_modrmreg = 
{
    ND_ILUT_MODRM_REG,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_12_reg_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_12_reg_01_leaf,
        /* 02 */ NULL,
        /* 03 */ NULL,
        /* 04 */ NULL,
        /* 05 */ NULL,
        /* 06 */ NULL,
        /* 07 */ NULL,
    }
};

const ND_TABLE_MODRM_MOD gXopTable_root_09_12_modrmmod = 
{
    ND_ILUT_MODRM_MOD,
    { 
        /* 00 */ NULL,
        /* 01 */ (const void *)&gXopTable_root_09_12_reg_modrmreg,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_81_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1693]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_80_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1694]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_83_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1695]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_82_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1696]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_c2_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2086]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_c3_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2087]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_c1_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2088]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_cb_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2090]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_d2_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2092]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_d3_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2093]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_d1_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2094]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_db_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2095]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_d6_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2096]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_d7_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2097]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_c6_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2099]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_c7_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2100]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_e1_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2102]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_e3_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2104]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_e2_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2107]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_90_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2269]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_90_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2270]
};

const ND_TABLE_VEX_W gXopTable_root_09_90_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_90_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_90_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_92_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2272]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_92_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2273]
};

const ND_TABLE_VEX_W gXopTable_root_09_92_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_92_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_92_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_93_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2275]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_93_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2276]
};

const ND_TABLE_VEX_W gXopTable_root_09_93_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_93_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_93_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_91_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2278]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_91_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2279]
};

const ND_TABLE_VEX_W gXopTable_root_09_91_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_91_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_91_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_98_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2286]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_98_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2287]
};

const ND_TABLE_VEX_W gXopTable_root_09_98_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_98_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_98_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_9a_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2288]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_9a_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2289]
};

const ND_TABLE_VEX_W gXopTable_root_09_9a_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_9a_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_9a_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_9b_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2290]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_9b_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2291]
};

const ND_TABLE_VEX_W gXopTable_root_09_9b_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_9b_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_9b_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_99_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2292]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_99_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2293]
};

const ND_TABLE_VEX_W gXopTable_root_09_99_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_99_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_99_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_94_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2294]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_94_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2295]
};

const ND_TABLE_VEX_W gXopTable_root_09_94_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_94_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_94_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_95_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2296]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_95_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2307]
};

const ND_TABLE_VEX_W gXopTable_root_09_95_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_95_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_95_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_96_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2297]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_96_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2298]
};

const ND_TABLE_VEX_W gXopTable_root_09_96_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_96_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_96_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_97_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2305]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_97_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2306]
};

const ND_TABLE_VEX_W gXopTable_root_09_97_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_97_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_97_01_leaf,
    }
};

const ND_TABLE_OPCODE gXopTable_root_09_opcode = 
{
    ND_ILUT_OPCODE,
    { 
        /* 00 */ NULL,
        /* 01 */ (const void *)&gXopTable_root_09_01_modrmreg,
        /* 02 */ (const void *)&gXopTable_root_09_02_modrmreg,
        /* 03 */ NULL,
        /* 04 */ NULL,
        /* 05 */ NULL,
        /* 06 */ NULL,
        /* 07 */ NULL,
        /* 08 */ NULL,
        /* 09 */ NULL,
        /* 0a */ NULL,
        /* 0b */ NULL,
        /* 0c */ NULL,
        /* 0d */ NULL,
        /* 0e */ NULL,
        /* 0f */ NULL,
        /* 10 */ NULL,
        /* 11 */ NULL,
        /* 12 */ (const void *)&gXopTable_root_09_12_modrmmod,
        /* 13 */ NULL,
        /* 14 */ NULL,
        /* 15 */ NULL,
        /* 16 */ NULL,
        /* 17 */ NULL,
        /* 18 */ NULL,
        /* 19 */ NULL,
        /* 1a */ NULL,
        /* 1b */ NULL,
        /* 1c */ NULL,
        /* 1d */ NULL,
        /* 1e */ NULL,
        /* 1f */ NULL,
        /* 20 */ NULL,
        /* 21 */ NULL,
        /* 22 */ NULL,
        /* 23 */ NULL,
        /* 24 */ NULL,
        /* 25 */ NULL,
        /* 26 */ NULL,
        /* 27 */ NULL,
        /* 28 */ NULL,
        /* 29 */ NULL,
        /* 2a */ NULL,
        /* 2b */ NULL,
        /* 2c */ NULL,
        /* 2d */ NULL,
        /* 2e */ NULL,
        /* 2f */ NULL,
        /* 30 */ NULL,
        /* 31 */ NULL,
        /* 32 */ NULL,
        /* 33 */ NULL,
        /* 34 */ NULL,
        /* 35 */ NULL,
        /* 36 */ NULL,
        /* 37 */ NULL,
        /* 38 */ NULL,
        /* 39 */ NULL,
        /* 3a */ NULL,
        /* 3b */ NULL,
        /* 3c */ NULL,
        /* 3d */ NULL,
        /* 3e */ NULL,
        /* 3f */ NULL,
        /* 40 */ NULL,
        /* 41 */ NULL,
        /* 42 */ NULL,
        /* 43 */ NULL,
        /* 44 */ NULL,
        /* 45 */ NULL,
        /* 46 */ NULL,
        /* 47 */ NULL,
        /* 48 */ NULL,
        /* 49 */ NULL,
        /* 4a */ NULL,
        /* 4b */ NULL,
        /* 4c */ NULL,
        /* 4d */ NULL,
        /* 4e */ NULL,
        /* 4f */ NULL,
        /* 50 */ NULL,
        /* 51 */ NULL,
        /* 52 */ NULL,
        /* 53 */ NULL,
        /* 54 */ NULL,
        /* 55 */ NULL,
        /* 56 */ NULL,
        /* 57 */ NULL,
        /* 58 */ NULL,
        /* 59 */ NULL,
        /* 5a */ NULL,
        /* 5b */ NULL,
        /* 5c */ NULL,
        /* 5d */ NULL,
        /* 5e */ NULL,
        /* 5f */ NULL,
        /* 60 */ NULL,
        /* 61 */ NULL,
        /* 62 */ NULL,
        /* 63 */ NULL,
        /* 64 */ NULL,
        /* 65 */ NULL,
        /* 66 */ NULL,
        /* 67 */ NULL,
        /* 68 */ NULL,
        /* 69 */ NULL,
        /* 6a */ NULL,
        /* 6b */ NULL,
        /* 6c */ NULL,
        /* 6d */ NULL,
        /* 6e */ NULL,
        /* 6f */ NULL,
        /* 70 */ NULL,
        /* 71 */ NULL,
        /* 72 */ NULL,
        /* 73 */ NULL,
        /* 74 */ NULL,
        /* 75 */ NULL,
        /* 76 */ NULL,
        /* 77 */ NULL,
        /* 78 */ NULL,
        /* 79 */ NULL,
        /* 7a */ NULL,
        /* 7b */ NULL,
        /* 7c */ NULL,
        /* 7d */ NULL,
        /* 7e */ NULL,
        /* 7f */ NULL,
        /* 80 */ (const void *)&gXopTable_root_09_80_leaf,
        /* 81 */ (const void *)&gXopTable_root_09_81_leaf,
        /* 82 */ (const void *)&gXopTable_root_09_82_leaf,
        /* 83 */ (const void *)&gXopTable_root_09_83_leaf,
        /* 84 */ NULL,
        /* 85 */ NULL,
        /* 86 */ NULL,
        /* 87 */ NULL,
        /* 88 */ NULL,
        /* 89 */ NULL,
        /* 8a */ NULL,
        /* 8b */ NULL,
        /* 8c */ NULL,
        /* 8d */ NULL,
        /* 8e */ NULL,
        /* 8f */ NULL,
        /* 90 */ (const void *)&gXopTable_root_09_90_w,
        /* 91 */ (const void *)&gXopTable_root_09_91_w,
        /* 92 */ (const void *)&gXopTable_root_09_92_w,
        /* 93 */ (const void *)&gXopTable_root_09_93_w,
        /* 94 */ (const void *)&gXopTable_root_09_94_w,
        /* 95 */ (const void *)&gXopTable_root_09_95_w,
        /* 96 */ (const void *)&gXopTable_root_09_96_w,
        /* 97 */ (const void *)&gXopTable_root_09_97_w,
        /* 98 */ (const void *)&gXopTable_root_09_98_w,
        /* 99 */ (const void *)&gXopTable_root_09_99_w,
        /* 9a */ (const void *)&gXopTable_root_09_9a_w,
        /* 9b */ (const void *)&gXopTable_root_09_9b_w,
        /* 9c */ NULL,
        /* 9d */ NULL,
        /* 9e */ NULL,
        /* 9f */ NULL,
        /* a0 */ NULL,
        /* a1 */ NULL,
        /* a2 */ NULL,
        /* a3 */ NULL,
        /* a4 */ NULL,
        /* a5 */ NULL,
        /* a6 */ NULL,
        /* a7 */ NULL,
        /* a8 */ NULL,
        /* a9 */ NULL,
        /* aa */ NULL,
        /* ab */ NULL,
        /* ac */ NULL,
        /* ad */ NULL,
        /* ae */ NULL,
        /* af */ NULL,
        /* b0 */ NULL,
        /* b1 */ NULL,
        /* b2 */ NULL,
        /* b3 */ NULL,
        /* b4 */ NULL,
        /* b5 */ NULL,
        /* b6 */ NULL,
        /* b7 */ NULL,
        /* b8 */ NULL,
        /* b9 */ NULL,
        /* ba */ NULL,
        /* bb */ NULL,
        /* bc */ NULL,
        /* bd */ NULL,
        /* be */ NULL,
        /* bf */ NULL,
        /* c0 */ NULL,
        /* c1 */ (const void *)&gXopTable_root_09_c1_leaf,
        /* c2 */ (const void *)&gXopTable_root_09_c2_leaf,
        /* c3 */ (const void *)&gXopTable_root_09_c3_leaf,
        /* c4 */ NULL,
        /* c5 */ NULL,
        /* c6 */ (const void *)&gXopTable_root_09_c6_leaf,
        /* c7 */ (const void *)&gXopTable_root_09_c7_leaf,
        /* c8 */ NULL,
        /* c9 */ NULL,
        /* ca */ NULL,
        /* cb */ (const void *)&gXopTable_root_09_cb_leaf,
        /* cc */ NULL,
        /* cd */ NULL,
        /* ce */ NULL,
        /* cf */ NULL,
        /* d0 */ NULL,
        /* d1 */ (const void *)&gXopTable_root_09_d1_leaf,
        /* d2 */ (const void *)&gXopTable_root_09_d2_leaf,
        /* d3 */ (const void *)&gXopTable_root_09_d3_leaf,
        /* d4 */ NULL,
        /* d5 */ NULL,
        /* d6 */ (const void *)&gXopTable_root_09_d6_leaf,
        /* d7 */ (const void *)&gXopTable_root_09_d7_leaf,
        /* d8 */ NULL,
        /* d9 */ NULL,
        /* da */ NULL,
        /* db */ (const void *)&gXopTable_root_09_db_leaf,
        /* dc */ NULL,
        /* dd */ NULL,
        /* de */ NULL,
        /* df */ NULL,
        /* e0 */ NULL,
        /* e1 */ (const void *)&gXopTable_root_09_e1_leaf,
        /* e2 */ (const void *)&gXopTable_root_09_e2_leaf,
        /* e3 */ (const void *)&gXopTable_root_09_e3_leaf,
        /* e4 */ NULL,
        /* e5 */ NULL,
        /* e6 */ NULL,
        /* e7 */ NULL,
        /* e8 */ NULL,
        /* e9 */ NULL,
        /* ea */ NULL,
        /* eb */ NULL,
        /* ec */ NULL,
        /* ed */ NULL,
        /* ee */ NULL,
        /* ef */ NULL,
        /* f0 */ NULL,
        /* f1 */ NULL,
        /* f2 */ NULL,
        /* f3 */ NULL,
        /* f4 */ NULL,
        /* f5 */ NULL,
        /* f6 */ NULL,
        /* f7 */ NULL,
        /* f8 */ NULL,
        /* f9 */ NULL,
        /* fa */ NULL,
        /* fb */ NULL,
        /* fc */ NULL,
        /* fd */ NULL,
        /* fe */ NULL,
        /* ff */ NULL,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_a2_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1968]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_a2_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1969]
};

const ND_TABLE_VEX_W gXopTable_root_08_a2_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_08_a2_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_08_a2_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_cc_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1998]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_ce_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1999]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_cf_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2004]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_ec_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2005]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_ee_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2006]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_ef_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2007]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_ed_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2008]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_cd_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2009]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_9e_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2122]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_9f_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2123]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_97_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2124]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_8e_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2125]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_8f_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2126]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_87_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2127]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_86_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2128]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_85_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2129]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_96_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2130]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_95_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2131]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_a6_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2132]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_b6_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2133]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_a3_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2258]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_a3_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2259]
};

const ND_TABLE_VEX_W gXopTable_root_08_a3_w = 
{
    ND_ILUT_VEX_W,
    { 
        /* 00 */ (const void *)&gXopTable_root_08_a3_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_08_a3_01_leaf,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_c0_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2268]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_c2_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2271]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_c3_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2274]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_c1_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2277]
};

const ND_TABLE_OPCODE gXopTable_root_08_opcode = 
{
    ND_ILUT_OPCODE,
    { 
        /* 00 */ NULL,
        /* 01 */ NULL,
        /* 02 */ NULL,
        /* 03 */ NULL,
        /* 04 */ NULL,
        /* 05 */ NULL,
        /* 06 */ NULL,
        /* 07 */ NULL,
        /* 08 */ NULL,
        /* 09 */ NULL,
        /* 0a */ NULL,
        /* 0b */ NULL,
        /* 0c */ NULL,
        /* 0d */ NULL,
        /* 0e */ NULL,
        /* 0f */ NULL,
        /* 10 */ NULL,
        /* 11 */ NULL,
        /* 12 */ NULL,
        /* 13 */ NULL,
        /* 14 */ NULL,
        /* 15 */ NULL,
        /* 16 */ NULL,
        /* 17 */ NULL,
        /* 18 */ NULL,
        /* 19 */ NULL,
        /* 1a */ NULL,
        /* 1b */ NULL,
        /* 1c */ NULL,
        /* 1d */ NULL,
        /* 1e */ NULL,
        /* 1f */ NULL,
        /* 20 */ NULL,
        /* 21 */ NULL,
        /* 22 */ NULL,
        /* 23 */ NULL,
        /* 24 */ NULL,
        /* 25 */ NULL,
        /* 26 */ NULL,
        /* 27 */ NULL,
        /* 28 */ NULL,
        /* 29 */ NULL,
        /* 2a */ NULL,
        /* 2b */ NULL,
        /* 2c */ NULL,
        /* 2d */ NULL,
        /* 2e */ NULL,
        /* 2f */ NULL,
        /* 30 */ NULL,
        /* 31 */ NULL,
        /* 32 */ NULL,
        /* 33 */ NULL,
        /* 34 */ NULL,
        /* 35 */ NULL,
        /* 36 */ NULL,
        /* 37 */ NULL,
        /* 38 */ NULL,
        /* 39 */ NULL,
        /* 3a */ NULL,
        /* 3b */ NULL,
        /* 3c */ NULL,
        /* 3d */ NULL,
        /* 3e */ NULL,
        /* 3f */ NULL,
        /* 40 */ NULL,
        /* 41 */ NULL,
        /* 42 */ NULL,
        /* 43 */ NULL,
        /* 44 */ NULL,
        /* 45 */ NULL,
        /* 46 */ NULL,
        /* 47 */ NULL,
        /* 48 */ NULL,
        /* 49 */ NULL,
        /* 4a */ NULL,
        /* 4b */ NULL,
        /* 4c */ NULL,
        /* 4d */ NULL,
        /* 4e */ NULL,
        /* 4f */ NULL,
        /* 50 */ NULL,
        /* 51 */ NULL,
        /* 52 */ NULL,
        /* 53 */ NULL,
        /* 54 */ NULL,
        /* 55 */ NULL,
        /* 56 */ NULL,
        /* 57 */ NULL,
        /* 58 */ NULL,
        /* 59 */ NULL,
        /* 5a */ NULL,
        /* 5b */ NULL,
        /* 5c */ NULL,
        /* 5d */ NULL,
        /* 5e */ NULL,
        /* 5f */ NULL,
        /* 60 */ NULL,
        /* 61 */ NULL,
        /* 62 */ NULL,
        /* 63 */ NULL,
        /* 64 */ NULL,
        /* 65 */ NULL,
        /* 66 */ NULL,
        /* 67 */ NULL,
        /* 68 */ NULL,
        /* 69 */ NULL,
        /* 6a */ NULL,
        /* 6b */ NULL,
        /* 6c */ NULL,
        /* 6d */ NULL,
        /* 6e */ NULL,
        /* 6f */ NULL,
        /* 70 */ NULL,
        /* 71 */ NULL,
        /* 72 */ NULL,
        /* 73 */ NULL,
        /* 74 */ NULL,
        /* 75 */ NULL,
        /* 76 */ NULL,
        /* 77 */ NULL,
        /* 78 */ NULL,
        /* 79 */ NULL,
        /* 7a */ NULL,
        /* 7b */ NULL,
        /* 7c */ NULL,
        /* 7d */ NULL,
        /* 7e */ NULL,
        /* 7f */ NULL,
        /* 80 */ NULL,
        /* 81 */ NULL,
        /* 82 */ NULL,
        /* 83 */ NULL,
        /* 84 */ NULL,
        /* 85 */ (const void *)&gXopTable_root_08_85_leaf,
        /* 86 */ (const void *)&gXopTable_root_08_86_leaf,
        /* 87 */ (const void *)&gXopTable_root_08_87_leaf,
        /* 88 */ NULL,
        /* 89 */ NULL,
        /* 8a */ NULL,
        /* 8b */ NULL,
        /* 8c */ NULL,
        /* 8d */ NULL,
        /* 8e */ (const void *)&gXopTable_root_08_8e_leaf,
        /* 8f */ (const void *)&gXopTable_root_08_8f_leaf,
        /* 90 */ NULL,
        /* 91 */ NULL,
        /* 92 */ NULL,
        /* 93 */ NULL,
        /* 94 */ NULL,
        /* 95 */ (const void *)&gXopTable_root_08_95_leaf,
        /* 96 */ (const void *)&gXopTable_root_08_96_leaf,
        /* 97 */ (const void *)&gXopTable_root_08_97_leaf,
        /* 98 */ NULL,
        /* 99 */ NULL,
        /* 9a */ NULL,
        /* 9b */ NULL,
        /* 9c */ NULL,
        /* 9d */ NULL,
        /* 9e */ (const void *)&gXopTable_root_08_9e_leaf,
        /* 9f */ (const void *)&gXopTable_root_08_9f_leaf,
        /* a0 */ NULL,
        /* a1 */ NULL,
        /* a2 */ (const void *)&gXopTable_root_08_a2_w,
        /* a3 */ (const void *)&gXopTable_root_08_a3_w,
        /* a4 */ NULL,
        /* a5 */ NULL,
        /* a6 */ (const void *)&gXopTable_root_08_a6_leaf,
        /* a7 */ NULL,
        /* a8 */ NULL,
        /* a9 */ NULL,
        /* aa */ NULL,
        /* ab */ NULL,
        /* ac */ NULL,
        /* ad */ NULL,
        /* ae */ NULL,
        /* af */ NULL,
        /* b0 */ NULL,
        /* b1 */ NULL,
        /* b2 */ NULL,
        /* b3 */ NULL,
        /* b4 */ NULL,
        /* b5 */ NULL,
        /* b6 */ (const void *)&gXopTable_root_08_b6_leaf,
        /* b7 */ NULL,
        /* b8 */ NULL,
        /* b9 */ NULL,
        /* ba */ NULL,
        /* bb */ NULL,
        /* bc */ NULL,
        /* bd */ NULL,
        /* be */ NULL,
        /* bf */ NULL,
        /* c0 */ (const void *)&gXopTable_root_08_c0_leaf,
        /* c1 */ (const void *)&gXopTable_root_08_c1_leaf,
        /* c2 */ (const void *)&gXopTable_root_08_c2_leaf,
        /* c3 */ (const void *)&gXopTable_root_08_c3_leaf,
        /* c4 */ NULL,
        /* c5 */ NULL,
        /* c6 */ NULL,
        /* c7 */ NULL,
        /* c8 */ NULL,
        /* c9 */ NULL,
        /* ca */ NULL,
        /* cb */ NULL,
        /* cc */ (const void *)&gXopTable_root_08_cc_leaf,
        /* cd */ (const void *)&gXopTable_root_08_cd_leaf,
        /* ce */ (const void *)&gXopTable_root_08_ce_leaf,
        /* cf */ (const void *)&gXopTable_root_08_cf_leaf,
        /* d0 */ NULL,
        /* d1 */ NULL,
        /* d2 */ NULL,
        /* d3 */ NULL,
        /* d4 */ NULL,
        /* d5 */ NULL,
        /* d6 */ NULL,
        /* d7 */ NULL,
        /* d8 */ NULL,
        /* d9 */ NULL,
        /* da */ NULL,
        /* db */ NULL,
        /* dc */ NULL,
        /* dd */ NULL,
        /* de */ NULL,
        /* df */ NULL,
        /* e0 */ NULL,
        /* e1 */ NULL,
        /* e2 */ NULL,
        /* e3 */ NULL,
        /* e4 */ NULL,
        /* e5 */ NULL,
        /* e6 */ NULL,
        /* e7 */ NULL,
        /* e8 */ NULL,
        /* e9 */ NULL,
        /* ea */ NULL,
        /* eb */ NULL,
        /* ec */ (const void *)&gXopTable_root_08_ec_leaf,
        /* ed */ (const void *)&gXopTable_root_08_ed_leaf,
        /* ee */ (const void *)&gXopTable_root_08_ee_leaf,
        /* ef */ (const void *)&gXopTable_root_08_ef_leaf,
        /* f0 */ NULL,
        /* f1 */ NULL,
        /* f2 */ NULL,
        /* f3 */ NULL,
        /* f4 */ NULL,
        /* f5 */ NULL,
        /* f6 */ NULL,
        /* f7 */ NULL,
        /* f8 */ NULL,
        /* f9 */ NULL,
        /* fa */ NULL,
        /* fb */ NULL,
        /* fc */ NULL,
        /* fd */ NULL,
        /* fe */ NULL,
        /* ff */ NULL,
    }
};

const ND_TABLE_VEX_MMMMM gXopTable_root_mmmmm = 
{
    ND_ILUT_VEX_MMMMM,
    { 
        /* 00 */ NULL,
        /* 01 */ NULL,
        /* 02 */ NULL,
        /* 03 */ NULL,
        /* 04 */ NULL,
        /* 05 */ NULL,
        /* 06 */ NULL,
        /* 07 */ NULL,
        /* 08 */ (const void *)&gXopTable_root_08_opcode,
        /* 09 */ (const void *)&gXopTable_root_09_opcode,
        /* 0a */ (const void *)&gXopTable_root_0a_opcode,
        /* 0b */ NULL,
        /* 0c */ NULL,
        /* 0d */ NULL,
        /* 0e */ NULL,
        /* 0f */ NULL,
        /* 10 */ NULL,
        /* 11 */ NULL,
        /* 12 */ NULL,
        /* 13 */ NULL,
        /* 14 */ NULL,
        /* 15 */ NULL,
        /* 16 */ NULL,
        /* 17 */ NULL,
        /* 18 */ NULL,
        /* 19 */ NULL,
        /* 1a */ NULL,
        /* 1b */ NULL,
        /* 1c */ NULL,
        /* 1d */ NULL,
        /* 1e */ NULL,
        /* 1f */ NULL,
    }
};

const PND_TABLE gXopTable = (const PND_TABLE)&gXopTable_root_mmmmm;


#endif

