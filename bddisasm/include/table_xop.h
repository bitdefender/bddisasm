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
    (const void *)&gInstructions[600]
};

const ND_TABLE_INSTRUCTION gXopTable_root_0a_12_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[601]
};

const ND_TABLE_MODRM_REG gXopTable_root_0a_12_modrmreg = 
{
    ND_ILUT_MODRM_REG,
    { 
        /* 00 */ (const void *)&gXopTable_root_0a_12_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_0a_12_01_leaf,
        /* 02 */ ND_NULL,
        /* 03 */ ND_NULL,
        /* 04 */ ND_NULL,
        /* 05 */ ND_NULL,
        /* 06 */ ND_NULL,
        /* 07 */ ND_NULL,
    }
};

const ND_TABLE_OPCODE gXopTable_root_0a_opcode = 
{
    ND_ILUT_OPCODE,
    { 
        /* 00 */ ND_NULL,
        /* 01 */ ND_NULL,
        /* 02 */ ND_NULL,
        /* 03 */ ND_NULL,
        /* 04 */ ND_NULL,
        /* 05 */ ND_NULL,
        /* 06 */ ND_NULL,
        /* 07 */ ND_NULL,
        /* 08 */ ND_NULL,
        /* 09 */ ND_NULL,
        /* 0a */ ND_NULL,
        /* 0b */ ND_NULL,
        /* 0c */ ND_NULL,
        /* 0d */ ND_NULL,
        /* 0e */ ND_NULL,
        /* 0f */ ND_NULL,
        /* 10 */ (const void *)&gXopTable_root_0a_10_leaf,
        /* 11 */ ND_NULL,
        /* 12 */ (const void *)&gXopTable_root_0a_12_modrmreg,
        /* 13 */ ND_NULL,
        /* 14 */ ND_NULL,
        /* 15 */ ND_NULL,
        /* 16 */ ND_NULL,
        /* 17 */ ND_NULL,
        /* 18 */ ND_NULL,
        /* 19 */ ND_NULL,
        /* 1a */ ND_NULL,
        /* 1b */ ND_NULL,
        /* 1c */ ND_NULL,
        /* 1d */ ND_NULL,
        /* 1e */ ND_NULL,
        /* 1f */ ND_NULL,
        /* 20 */ ND_NULL,
        /* 21 */ ND_NULL,
        /* 22 */ ND_NULL,
        /* 23 */ ND_NULL,
        /* 24 */ ND_NULL,
        /* 25 */ ND_NULL,
        /* 26 */ ND_NULL,
        /* 27 */ ND_NULL,
        /* 28 */ ND_NULL,
        /* 29 */ ND_NULL,
        /* 2a */ ND_NULL,
        /* 2b */ ND_NULL,
        /* 2c */ ND_NULL,
        /* 2d */ ND_NULL,
        /* 2e */ ND_NULL,
        /* 2f */ ND_NULL,
        /* 30 */ ND_NULL,
        /* 31 */ ND_NULL,
        /* 32 */ ND_NULL,
        /* 33 */ ND_NULL,
        /* 34 */ ND_NULL,
        /* 35 */ ND_NULL,
        /* 36 */ ND_NULL,
        /* 37 */ ND_NULL,
        /* 38 */ ND_NULL,
        /* 39 */ ND_NULL,
        /* 3a */ ND_NULL,
        /* 3b */ ND_NULL,
        /* 3c */ ND_NULL,
        /* 3d */ ND_NULL,
        /* 3e */ ND_NULL,
        /* 3f */ ND_NULL,
        /* 40 */ ND_NULL,
        /* 41 */ ND_NULL,
        /* 42 */ ND_NULL,
        /* 43 */ ND_NULL,
        /* 44 */ ND_NULL,
        /* 45 */ ND_NULL,
        /* 46 */ ND_NULL,
        /* 47 */ ND_NULL,
        /* 48 */ ND_NULL,
        /* 49 */ ND_NULL,
        /* 4a */ ND_NULL,
        /* 4b */ ND_NULL,
        /* 4c */ ND_NULL,
        /* 4d */ ND_NULL,
        /* 4e */ ND_NULL,
        /* 4f */ ND_NULL,
        /* 50 */ ND_NULL,
        /* 51 */ ND_NULL,
        /* 52 */ ND_NULL,
        /* 53 */ ND_NULL,
        /* 54 */ ND_NULL,
        /* 55 */ ND_NULL,
        /* 56 */ ND_NULL,
        /* 57 */ ND_NULL,
        /* 58 */ ND_NULL,
        /* 59 */ ND_NULL,
        /* 5a */ ND_NULL,
        /* 5b */ ND_NULL,
        /* 5c */ ND_NULL,
        /* 5d */ ND_NULL,
        /* 5e */ ND_NULL,
        /* 5f */ ND_NULL,
        /* 60 */ ND_NULL,
        /* 61 */ ND_NULL,
        /* 62 */ ND_NULL,
        /* 63 */ ND_NULL,
        /* 64 */ ND_NULL,
        /* 65 */ ND_NULL,
        /* 66 */ ND_NULL,
        /* 67 */ ND_NULL,
        /* 68 */ ND_NULL,
        /* 69 */ ND_NULL,
        /* 6a */ ND_NULL,
        /* 6b */ ND_NULL,
        /* 6c */ ND_NULL,
        /* 6d */ ND_NULL,
        /* 6e */ ND_NULL,
        /* 6f */ ND_NULL,
        /* 70 */ ND_NULL,
        /* 71 */ ND_NULL,
        /* 72 */ ND_NULL,
        /* 73 */ ND_NULL,
        /* 74 */ ND_NULL,
        /* 75 */ ND_NULL,
        /* 76 */ ND_NULL,
        /* 77 */ ND_NULL,
        /* 78 */ ND_NULL,
        /* 79 */ ND_NULL,
        /* 7a */ ND_NULL,
        /* 7b */ ND_NULL,
        /* 7c */ ND_NULL,
        /* 7d */ ND_NULL,
        /* 7e */ ND_NULL,
        /* 7f */ ND_NULL,
        /* 80 */ ND_NULL,
        /* 81 */ ND_NULL,
        /* 82 */ ND_NULL,
        /* 83 */ ND_NULL,
        /* 84 */ ND_NULL,
        /* 85 */ ND_NULL,
        /* 86 */ ND_NULL,
        /* 87 */ ND_NULL,
        /* 88 */ ND_NULL,
        /* 89 */ ND_NULL,
        /* 8a */ ND_NULL,
        /* 8b */ ND_NULL,
        /* 8c */ ND_NULL,
        /* 8d */ ND_NULL,
        /* 8e */ ND_NULL,
        /* 8f */ ND_NULL,
        /* 90 */ ND_NULL,
        /* 91 */ ND_NULL,
        /* 92 */ ND_NULL,
        /* 93 */ ND_NULL,
        /* 94 */ ND_NULL,
        /* 95 */ ND_NULL,
        /* 96 */ ND_NULL,
        /* 97 */ ND_NULL,
        /* 98 */ ND_NULL,
        /* 99 */ ND_NULL,
        /* 9a */ ND_NULL,
        /* 9b */ ND_NULL,
        /* 9c */ ND_NULL,
        /* 9d */ ND_NULL,
        /* 9e */ ND_NULL,
        /* 9f */ ND_NULL,
        /* a0 */ ND_NULL,
        /* a1 */ ND_NULL,
        /* a2 */ ND_NULL,
        /* a3 */ ND_NULL,
        /* a4 */ ND_NULL,
        /* a5 */ ND_NULL,
        /* a6 */ ND_NULL,
        /* a7 */ ND_NULL,
        /* a8 */ ND_NULL,
        /* a9 */ ND_NULL,
        /* aa */ ND_NULL,
        /* ab */ ND_NULL,
        /* ac */ ND_NULL,
        /* ad */ ND_NULL,
        /* ae */ ND_NULL,
        /* af */ ND_NULL,
        /* b0 */ ND_NULL,
        /* b1 */ ND_NULL,
        /* b2 */ ND_NULL,
        /* b3 */ ND_NULL,
        /* b4 */ ND_NULL,
        /* b5 */ ND_NULL,
        /* b6 */ ND_NULL,
        /* b7 */ ND_NULL,
        /* b8 */ ND_NULL,
        /* b9 */ ND_NULL,
        /* ba */ ND_NULL,
        /* bb */ ND_NULL,
        /* bc */ ND_NULL,
        /* bd */ ND_NULL,
        /* be */ ND_NULL,
        /* bf */ ND_NULL,
        /* c0 */ ND_NULL,
        /* c1 */ ND_NULL,
        /* c2 */ ND_NULL,
        /* c3 */ ND_NULL,
        /* c4 */ ND_NULL,
        /* c5 */ ND_NULL,
        /* c6 */ ND_NULL,
        /* c7 */ ND_NULL,
        /* c8 */ ND_NULL,
        /* c9 */ ND_NULL,
        /* ca */ ND_NULL,
        /* cb */ ND_NULL,
        /* cc */ ND_NULL,
        /* cd */ ND_NULL,
        /* ce */ ND_NULL,
        /* cf */ ND_NULL,
        /* d0 */ ND_NULL,
        /* d1 */ ND_NULL,
        /* d2 */ ND_NULL,
        /* d3 */ ND_NULL,
        /* d4 */ ND_NULL,
        /* d5 */ ND_NULL,
        /* d6 */ ND_NULL,
        /* d7 */ ND_NULL,
        /* d8 */ ND_NULL,
        /* d9 */ ND_NULL,
        /* da */ ND_NULL,
        /* db */ ND_NULL,
        /* dc */ ND_NULL,
        /* dd */ ND_NULL,
        /* de */ ND_NULL,
        /* df */ ND_NULL,
        /* e0 */ ND_NULL,
        /* e1 */ ND_NULL,
        /* e2 */ ND_NULL,
        /* e3 */ ND_NULL,
        /* e4 */ ND_NULL,
        /* e5 */ ND_NULL,
        /* e6 */ ND_NULL,
        /* e7 */ ND_NULL,
        /* e8 */ ND_NULL,
        /* e9 */ ND_NULL,
        /* ea */ ND_NULL,
        /* eb */ ND_NULL,
        /* ec */ ND_NULL,
        /* ed */ ND_NULL,
        /* ee */ ND_NULL,
        /* ef */ ND_NULL,
        /* f0 */ ND_NULL,
        /* f1 */ ND_NULL,
        /* f2 */ ND_NULL,
        /* f3 */ ND_NULL,
        /* f4 */ ND_NULL,
        /* f5 */ ND_NULL,
        /* f6 */ ND_NULL,
        /* f7 */ ND_NULL,
        /* f8 */ ND_NULL,
        /* f9 */ ND_NULL,
        /* fa */ ND_NULL,
        /* fb */ ND_NULL,
        /* fc */ ND_NULL,
        /* fd */ ND_NULL,
        /* fe */ ND_NULL,
        /* ff */ ND_NULL,
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
    (const void *)&gInstructions[1319]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_01_04_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1343]
};

const ND_TABLE_MODRM_REG gXopTable_root_09_01_modrmreg = 
{
    ND_ILUT_MODRM_REG,
    { 
        /* 00 */ ND_NULL,
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
        /* 00 */ ND_NULL,
        /* 01 */ (const void *)&gXopTable_root_09_02_01_leaf,
        /* 02 */ ND_NULL,
        /* 03 */ ND_NULL,
        /* 04 */ ND_NULL,
        /* 05 */ ND_NULL,
        /* 06 */ (const void *)&gXopTable_root_09_02_06_leaf,
        /* 07 */ ND_NULL,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_12_reg_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[582]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_12_reg_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1270]
};

const ND_TABLE_MODRM_REG gXopTable_root_09_12_reg_modrmreg = 
{
    ND_ILUT_MODRM_REG,
    { 
        /* 00 */ (const void *)&gXopTable_root_09_12_reg_00_leaf,
        /* 01 */ (const void *)&gXopTable_root_09_12_reg_01_leaf,
        /* 02 */ ND_NULL,
        /* 03 */ ND_NULL,
        /* 04 */ ND_NULL,
        /* 05 */ ND_NULL,
        /* 06 */ ND_NULL,
        /* 07 */ ND_NULL,
    }
};

const ND_TABLE_MODRM_MOD gXopTable_root_09_12_modrmmod = 
{
    ND_ILUT_MODRM_MOD,
    { 
        /* 00 */ ND_NULL,
        /* 01 */ (const void *)&gXopTable_root_09_12_reg_modrmreg,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_81_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1774]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_80_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1775]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_83_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1776]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_82_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[1777]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_c2_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2185]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_c3_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2186]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_c1_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2187]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_cb_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2189]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_d2_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2191]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_d3_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2192]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_d1_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2193]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_db_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2194]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_d6_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2195]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_d7_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2196]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_c6_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2198]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_c7_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2199]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_e1_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2201]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_e3_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2203]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_e2_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2206]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_90_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2368]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_90_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2369]
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
    (const void *)&gInstructions[2371]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_92_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2372]
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
    (const void *)&gInstructions[2374]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_93_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2375]
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
    (const void *)&gInstructions[2377]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_91_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2378]
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
    (const void *)&gInstructions[2385]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_98_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2386]
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
    (const void *)&gInstructions[2387]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_9a_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2388]
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
    (const void *)&gInstructions[2389]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_9b_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2390]
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
    (const void *)&gInstructions[2391]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_99_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2392]
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
    (const void *)&gInstructions[2393]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_94_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2394]
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
    (const void *)&gInstructions[2395]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_95_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2406]
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
    (const void *)&gInstructions[2396]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_96_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2397]
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
    (const void *)&gInstructions[2404]
};

const ND_TABLE_INSTRUCTION gXopTable_root_09_97_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2405]
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
        /* 00 */ ND_NULL,
        /* 01 */ (const void *)&gXopTable_root_09_01_modrmreg,
        /* 02 */ (const void *)&gXopTable_root_09_02_modrmreg,
        /* 03 */ ND_NULL,
        /* 04 */ ND_NULL,
        /* 05 */ ND_NULL,
        /* 06 */ ND_NULL,
        /* 07 */ ND_NULL,
        /* 08 */ ND_NULL,
        /* 09 */ ND_NULL,
        /* 0a */ ND_NULL,
        /* 0b */ ND_NULL,
        /* 0c */ ND_NULL,
        /* 0d */ ND_NULL,
        /* 0e */ ND_NULL,
        /* 0f */ ND_NULL,
        /* 10 */ ND_NULL,
        /* 11 */ ND_NULL,
        /* 12 */ (const void *)&gXopTable_root_09_12_modrmmod,
        /* 13 */ ND_NULL,
        /* 14 */ ND_NULL,
        /* 15 */ ND_NULL,
        /* 16 */ ND_NULL,
        /* 17 */ ND_NULL,
        /* 18 */ ND_NULL,
        /* 19 */ ND_NULL,
        /* 1a */ ND_NULL,
        /* 1b */ ND_NULL,
        /* 1c */ ND_NULL,
        /* 1d */ ND_NULL,
        /* 1e */ ND_NULL,
        /* 1f */ ND_NULL,
        /* 20 */ ND_NULL,
        /* 21 */ ND_NULL,
        /* 22 */ ND_NULL,
        /* 23 */ ND_NULL,
        /* 24 */ ND_NULL,
        /* 25 */ ND_NULL,
        /* 26 */ ND_NULL,
        /* 27 */ ND_NULL,
        /* 28 */ ND_NULL,
        /* 29 */ ND_NULL,
        /* 2a */ ND_NULL,
        /* 2b */ ND_NULL,
        /* 2c */ ND_NULL,
        /* 2d */ ND_NULL,
        /* 2e */ ND_NULL,
        /* 2f */ ND_NULL,
        /* 30 */ ND_NULL,
        /* 31 */ ND_NULL,
        /* 32 */ ND_NULL,
        /* 33 */ ND_NULL,
        /* 34 */ ND_NULL,
        /* 35 */ ND_NULL,
        /* 36 */ ND_NULL,
        /* 37 */ ND_NULL,
        /* 38 */ ND_NULL,
        /* 39 */ ND_NULL,
        /* 3a */ ND_NULL,
        /* 3b */ ND_NULL,
        /* 3c */ ND_NULL,
        /* 3d */ ND_NULL,
        /* 3e */ ND_NULL,
        /* 3f */ ND_NULL,
        /* 40 */ ND_NULL,
        /* 41 */ ND_NULL,
        /* 42 */ ND_NULL,
        /* 43 */ ND_NULL,
        /* 44 */ ND_NULL,
        /* 45 */ ND_NULL,
        /* 46 */ ND_NULL,
        /* 47 */ ND_NULL,
        /* 48 */ ND_NULL,
        /* 49 */ ND_NULL,
        /* 4a */ ND_NULL,
        /* 4b */ ND_NULL,
        /* 4c */ ND_NULL,
        /* 4d */ ND_NULL,
        /* 4e */ ND_NULL,
        /* 4f */ ND_NULL,
        /* 50 */ ND_NULL,
        /* 51 */ ND_NULL,
        /* 52 */ ND_NULL,
        /* 53 */ ND_NULL,
        /* 54 */ ND_NULL,
        /* 55 */ ND_NULL,
        /* 56 */ ND_NULL,
        /* 57 */ ND_NULL,
        /* 58 */ ND_NULL,
        /* 59 */ ND_NULL,
        /* 5a */ ND_NULL,
        /* 5b */ ND_NULL,
        /* 5c */ ND_NULL,
        /* 5d */ ND_NULL,
        /* 5e */ ND_NULL,
        /* 5f */ ND_NULL,
        /* 60 */ ND_NULL,
        /* 61 */ ND_NULL,
        /* 62 */ ND_NULL,
        /* 63 */ ND_NULL,
        /* 64 */ ND_NULL,
        /* 65 */ ND_NULL,
        /* 66 */ ND_NULL,
        /* 67 */ ND_NULL,
        /* 68 */ ND_NULL,
        /* 69 */ ND_NULL,
        /* 6a */ ND_NULL,
        /* 6b */ ND_NULL,
        /* 6c */ ND_NULL,
        /* 6d */ ND_NULL,
        /* 6e */ ND_NULL,
        /* 6f */ ND_NULL,
        /* 70 */ ND_NULL,
        /* 71 */ ND_NULL,
        /* 72 */ ND_NULL,
        /* 73 */ ND_NULL,
        /* 74 */ ND_NULL,
        /* 75 */ ND_NULL,
        /* 76 */ ND_NULL,
        /* 77 */ ND_NULL,
        /* 78 */ ND_NULL,
        /* 79 */ ND_NULL,
        /* 7a */ ND_NULL,
        /* 7b */ ND_NULL,
        /* 7c */ ND_NULL,
        /* 7d */ ND_NULL,
        /* 7e */ ND_NULL,
        /* 7f */ ND_NULL,
        /* 80 */ (const void *)&gXopTable_root_09_80_leaf,
        /* 81 */ (const void *)&gXopTable_root_09_81_leaf,
        /* 82 */ (const void *)&gXopTable_root_09_82_leaf,
        /* 83 */ (const void *)&gXopTable_root_09_83_leaf,
        /* 84 */ ND_NULL,
        /* 85 */ ND_NULL,
        /* 86 */ ND_NULL,
        /* 87 */ ND_NULL,
        /* 88 */ ND_NULL,
        /* 89 */ ND_NULL,
        /* 8a */ ND_NULL,
        /* 8b */ ND_NULL,
        /* 8c */ ND_NULL,
        /* 8d */ ND_NULL,
        /* 8e */ ND_NULL,
        /* 8f */ ND_NULL,
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
        /* 9c */ ND_NULL,
        /* 9d */ ND_NULL,
        /* 9e */ ND_NULL,
        /* 9f */ ND_NULL,
        /* a0 */ ND_NULL,
        /* a1 */ ND_NULL,
        /* a2 */ ND_NULL,
        /* a3 */ ND_NULL,
        /* a4 */ ND_NULL,
        /* a5 */ ND_NULL,
        /* a6 */ ND_NULL,
        /* a7 */ ND_NULL,
        /* a8 */ ND_NULL,
        /* a9 */ ND_NULL,
        /* aa */ ND_NULL,
        /* ab */ ND_NULL,
        /* ac */ ND_NULL,
        /* ad */ ND_NULL,
        /* ae */ ND_NULL,
        /* af */ ND_NULL,
        /* b0 */ ND_NULL,
        /* b1 */ ND_NULL,
        /* b2 */ ND_NULL,
        /* b3 */ ND_NULL,
        /* b4 */ ND_NULL,
        /* b5 */ ND_NULL,
        /* b6 */ ND_NULL,
        /* b7 */ ND_NULL,
        /* b8 */ ND_NULL,
        /* b9 */ ND_NULL,
        /* ba */ ND_NULL,
        /* bb */ ND_NULL,
        /* bc */ ND_NULL,
        /* bd */ ND_NULL,
        /* be */ ND_NULL,
        /* bf */ ND_NULL,
        /* c0 */ ND_NULL,
        /* c1 */ (const void *)&gXopTable_root_09_c1_leaf,
        /* c2 */ (const void *)&gXopTable_root_09_c2_leaf,
        /* c3 */ (const void *)&gXopTable_root_09_c3_leaf,
        /* c4 */ ND_NULL,
        /* c5 */ ND_NULL,
        /* c6 */ (const void *)&gXopTable_root_09_c6_leaf,
        /* c7 */ (const void *)&gXopTable_root_09_c7_leaf,
        /* c8 */ ND_NULL,
        /* c9 */ ND_NULL,
        /* ca */ ND_NULL,
        /* cb */ (const void *)&gXopTable_root_09_cb_leaf,
        /* cc */ ND_NULL,
        /* cd */ ND_NULL,
        /* ce */ ND_NULL,
        /* cf */ ND_NULL,
        /* d0 */ ND_NULL,
        /* d1 */ (const void *)&gXopTable_root_09_d1_leaf,
        /* d2 */ (const void *)&gXopTable_root_09_d2_leaf,
        /* d3 */ (const void *)&gXopTable_root_09_d3_leaf,
        /* d4 */ ND_NULL,
        /* d5 */ ND_NULL,
        /* d6 */ (const void *)&gXopTable_root_09_d6_leaf,
        /* d7 */ (const void *)&gXopTable_root_09_d7_leaf,
        /* d8 */ ND_NULL,
        /* d9 */ ND_NULL,
        /* da */ ND_NULL,
        /* db */ (const void *)&gXopTable_root_09_db_leaf,
        /* dc */ ND_NULL,
        /* dd */ ND_NULL,
        /* de */ ND_NULL,
        /* df */ ND_NULL,
        /* e0 */ ND_NULL,
        /* e1 */ (const void *)&gXopTable_root_09_e1_leaf,
        /* e2 */ (const void *)&gXopTable_root_09_e2_leaf,
        /* e3 */ (const void *)&gXopTable_root_09_e3_leaf,
        /* e4 */ ND_NULL,
        /* e5 */ ND_NULL,
        /* e6 */ ND_NULL,
        /* e7 */ ND_NULL,
        /* e8 */ ND_NULL,
        /* e9 */ ND_NULL,
        /* ea */ ND_NULL,
        /* eb */ ND_NULL,
        /* ec */ ND_NULL,
        /* ed */ ND_NULL,
        /* ee */ ND_NULL,
        /* ef */ ND_NULL,
        /* f0 */ ND_NULL,
        /* f1 */ ND_NULL,
        /* f2 */ ND_NULL,
        /* f3 */ ND_NULL,
        /* f4 */ ND_NULL,
        /* f5 */ ND_NULL,
        /* f6 */ ND_NULL,
        /* f7 */ ND_NULL,
        /* f8 */ ND_NULL,
        /* f9 */ ND_NULL,
        /* fa */ ND_NULL,
        /* fb */ ND_NULL,
        /* fc */ ND_NULL,
        /* fd */ ND_NULL,
        /* fe */ ND_NULL,
        /* ff */ ND_NULL,
    }
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_a2_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2067]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_a2_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2068]
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
    (const void *)&gInstructions[2097]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_ce_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2098]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_cf_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2103]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_ec_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2104]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_ee_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2105]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_ef_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2106]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_ed_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2107]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_cd_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2108]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_9e_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2221]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_9f_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2222]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_97_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2223]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_8e_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2224]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_8f_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2225]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_87_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2226]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_86_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2227]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_85_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2228]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_96_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2229]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_95_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2230]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_a6_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2231]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_b6_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2232]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_a3_00_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2357]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_a3_01_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2358]
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
    (const void *)&gInstructions[2367]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_c2_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2370]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_c3_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2373]
};

const ND_TABLE_INSTRUCTION gXopTable_root_08_c1_leaf = 
{
    ND_ILUT_INSTRUCTION,
    (const void *)&gInstructions[2376]
};

const ND_TABLE_OPCODE gXopTable_root_08_opcode = 
{
    ND_ILUT_OPCODE,
    { 
        /* 00 */ ND_NULL,
        /* 01 */ ND_NULL,
        /* 02 */ ND_NULL,
        /* 03 */ ND_NULL,
        /* 04 */ ND_NULL,
        /* 05 */ ND_NULL,
        /* 06 */ ND_NULL,
        /* 07 */ ND_NULL,
        /* 08 */ ND_NULL,
        /* 09 */ ND_NULL,
        /* 0a */ ND_NULL,
        /* 0b */ ND_NULL,
        /* 0c */ ND_NULL,
        /* 0d */ ND_NULL,
        /* 0e */ ND_NULL,
        /* 0f */ ND_NULL,
        /* 10 */ ND_NULL,
        /* 11 */ ND_NULL,
        /* 12 */ ND_NULL,
        /* 13 */ ND_NULL,
        /* 14 */ ND_NULL,
        /* 15 */ ND_NULL,
        /* 16 */ ND_NULL,
        /* 17 */ ND_NULL,
        /* 18 */ ND_NULL,
        /* 19 */ ND_NULL,
        /* 1a */ ND_NULL,
        /* 1b */ ND_NULL,
        /* 1c */ ND_NULL,
        /* 1d */ ND_NULL,
        /* 1e */ ND_NULL,
        /* 1f */ ND_NULL,
        /* 20 */ ND_NULL,
        /* 21 */ ND_NULL,
        /* 22 */ ND_NULL,
        /* 23 */ ND_NULL,
        /* 24 */ ND_NULL,
        /* 25 */ ND_NULL,
        /* 26 */ ND_NULL,
        /* 27 */ ND_NULL,
        /* 28 */ ND_NULL,
        /* 29 */ ND_NULL,
        /* 2a */ ND_NULL,
        /* 2b */ ND_NULL,
        /* 2c */ ND_NULL,
        /* 2d */ ND_NULL,
        /* 2e */ ND_NULL,
        /* 2f */ ND_NULL,
        /* 30 */ ND_NULL,
        /* 31 */ ND_NULL,
        /* 32 */ ND_NULL,
        /* 33 */ ND_NULL,
        /* 34 */ ND_NULL,
        /* 35 */ ND_NULL,
        /* 36 */ ND_NULL,
        /* 37 */ ND_NULL,
        /* 38 */ ND_NULL,
        /* 39 */ ND_NULL,
        /* 3a */ ND_NULL,
        /* 3b */ ND_NULL,
        /* 3c */ ND_NULL,
        /* 3d */ ND_NULL,
        /* 3e */ ND_NULL,
        /* 3f */ ND_NULL,
        /* 40 */ ND_NULL,
        /* 41 */ ND_NULL,
        /* 42 */ ND_NULL,
        /* 43 */ ND_NULL,
        /* 44 */ ND_NULL,
        /* 45 */ ND_NULL,
        /* 46 */ ND_NULL,
        /* 47 */ ND_NULL,
        /* 48 */ ND_NULL,
        /* 49 */ ND_NULL,
        /* 4a */ ND_NULL,
        /* 4b */ ND_NULL,
        /* 4c */ ND_NULL,
        /* 4d */ ND_NULL,
        /* 4e */ ND_NULL,
        /* 4f */ ND_NULL,
        /* 50 */ ND_NULL,
        /* 51 */ ND_NULL,
        /* 52 */ ND_NULL,
        /* 53 */ ND_NULL,
        /* 54 */ ND_NULL,
        /* 55 */ ND_NULL,
        /* 56 */ ND_NULL,
        /* 57 */ ND_NULL,
        /* 58 */ ND_NULL,
        /* 59 */ ND_NULL,
        /* 5a */ ND_NULL,
        /* 5b */ ND_NULL,
        /* 5c */ ND_NULL,
        /* 5d */ ND_NULL,
        /* 5e */ ND_NULL,
        /* 5f */ ND_NULL,
        /* 60 */ ND_NULL,
        /* 61 */ ND_NULL,
        /* 62 */ ND_NULL,
        /* 63 */ ND_NULL,
        /* 64 */ ND_NULL,
        /* 65 */ ND_NULL,
        /* 66 */ ND_NULL,
        /* 67 */ ND_NULL,
        /* 68 */ ND_NULL,
        /* 69 */ ND_NULL,
        /* 6a */ ND_NULL,
        /* 6b */ ND_NULL,
        /* 6c */ ND_NULL,
        /* 6d */ ND_NULL,
        /* 6e */ ND_NULL,
        /* 6f */ ND_NULL,
        /* 70 */ ND_NULL,
        /* 71 */ ND_NULL,
        /* 72 */ ND_NULL,
        /* 73 */ ND_NULL,
        /* 74 */ ND_NULL,
        /* 75 */ ND_NULL,
        /* 76 */ ND_NULL,
        /* 77 */ ND_NULL,
        /* 78 */ ND_NULL,
        /* 79 */ ND_NULL,
        /* 7a */ ND_NULL,
        /* 7b */ ND_NULL,
        /* 7c */ ND_NULL,
        /* 7d */ ND_NULL,
        /* 7e */ ND_NULL,
        /* 7f */ ND_NULL,
        /* 80 */ ND_NULL,
        /* 81 */ ND_NULL,
        /* 82 */ ND_NULL,
        /* 83 */ ND_NULL,
        /* 84 */ ND_NULL,
        /* 85 */ (const void *)&gXopTable_root_08_85_leaf,
        /* 86 */ (const void *)&gXopTable_root_08_86_leaf,
        /* 87 */ (const void *)&gXopTable_root_08_87_leaf,
        /* 88 */ ND_NULL,
        /* 89 */ ND_NULL,
        /* 8a */ ND_NULL,
        /* 8b */ ND_NULL,
        /* 8c */ ND_NULL,
        /* 8d */ ND_NULL,
        /* 8e */ (const void *)&gXopTable_root_08_8e_leaf,
        /* 8f */ (const void *)&gXopTable_root_08_8f_leaf,
        /* 90 */ ND_NULL,
        /* 91 */ ND_NULL,
        /* 92 */ ND_NULL,
        /* 93 */ ND_NULL,
        /* 94 */ ND_NULL,
        /* 95 */ (const void *)&gXopTable_root_08_95_leaf,
        /* 96 */ (const void *)&gXopTable_root_08_96_leaf,
        /* 97 */ (const void *)&gXopTable_root_08_97_leaf,
        /* 98 */ ND_NULL,
        /* 99 */ ND_NULL,
        /* 9a */ ND_NULL,
        /* 9b */ ND_NULL,
        /* 9c */ ND_NULL,
        /* 9d */ ND_NULL,
        /* 9e */ (const void *)&gXopTable_root_08_9e_leaf,
        /* 9f */ (const void *)&gXopTable_root_08_9f_leaf,
        /* a0 */ ND_NULL,
        /* a1 */ ND_NULL,
        /* a2 */ (const void *)&gXopTable_root_08_a2_w,
        /* a3 */ (const void *)&gXopTable_root_08_a3_w,
        /* a4 */ ND_NULL,
        /* a5 */ ND_NULL,
        /* a6 */ (const void *)&gXopTable_root_08_a6_leaf,
        /* a7 */ ND_NULL,
        /* a8 */ ND_NULL,
        /* a9 */ ND_NULL,
        /* aa */ ND_NULL,
        /* ab */ ND_NULL,
        /* ac */ ND_NULL,
        /* ad */ ND_NULL,
        /* ae */ ND_NULL,
        /* af */ ND_NULL,
        /* b0 */ ND_NULL,
        /* b1 */ ND_NULL,
        /* b2 */ ND_NULL,
        /* b3 */ ND_NULL,
        /* b4 */ ND_NULL,
        /* b5 */ ND_NULL,
        /* b6 */ (const void *)&gXopTable_root_08_b6_leaf,
        /* b7 */ ND_NULL,
        /* b8 */ ND_NULL,
        /* b9 */ ND_NULL,
        /* ba */ ND_NULL,
        /* bb */ ND_NULL,
        /* bc */ ND_NULL,
        /* bd */ ND_NULL,
        /* be */ ND_NULL,
        /* bf */ ND_NULL,
        /* c0 */ (const void *)&gXopTable_root_08_c0_leaf,
        /* c1 */ (const void *)&gXopTable_root_08_c1_leaf,
        /* c2 */ (const void *)&gXopTable_root_08_c2_leaf,
        /* c3 */ (const void *)&gXopTable_root_08_c3_leaf,
        /* c4 */ ND_NULL,
        /* c5 */ ND_NULL,
        /* c6 */ ND_NULL,
        /* c7 */ ND_NULL,
        /* c8 */ ND_NULL,
        /* c9 */ ND_NULL,
        /* ca */ ND_NULL,
        /* cb */ ND_NULL,
        /* cc */ (const void *)&gXopTable_root_08_cc_leaf,
        /* cd */ (const void *)&gXopTable_root_08_cd_leaf,
        /* ce */ (const void *)&gXopTable_root_08_ce_leaf,
        /* cf */ (const void *)&gXopTable_root_08_cf_leaf,
        /* d0 */ ND_NULL,
        /* d1 */ ND_NULL,
        /* d2 */ ND_NULL,
        /* d3 */ ND_NULL,
        /* d4 */ ND_NULL,
        /* d5 */ ND_NULL,
        /* d6 */ ND_NULL,
        /* d7 */ ND_NULL,
        /* d8 */ ND_NULL,
        /* d9 */ ND_NULL,
        /* da */ ND_NULL,
        /* db */ ND_NULL,
        /* dc */ ND_NULL,
        /* dd */ ND_NULL,
        /* de */ ND_NULL,
        /* df */ ND_NULL,
        /* e0 */ ND_NULL,
        /* e1 */ ND_NULL,
        /* e2 */ ND_NULL,
        /* e3 */ ND_NULL,
        /* e4 */ ND_NULL,
        /* e5 */ ND_NULL,
        /* e6 */ ND_NULL,
        /* e7 */ ND_NULL,
        /* e8 */ ND_NULL,
        /* e9 */ ND_NULL,
        /* ea */ ND_NULL,
        /* eb */ ND_NULL,
        /* ec */ (const void *)&gXopTable_root_08_ec_leaf,
        /* ed */ (const void *)&gXopTable_root_08_ed_leaf,
        /* ee */ (const void *)&gXopTable_root_08_ee_leaf,
        /* ef */ (const void *)&gXopTable_root_08_ef_leaf,
        /* f0 */ ND_NULL,
        /* f1 */ ND_NULL,
        /* f2 */ ND_NULL,
        /* f3 */ ND_NULL,
        /* f4 */ ND_NULL,
        /* f5 */ ND_NULL,
        /* f6 */ ND_NULL,
        /* f7 */ ND_NULL,
        /* f8 */ ND_NULL,
        /* f9 */ ND_NULL,
        /* fa */ ND_NULL,
        /* fb */ ND_NULL,
        /* fc */ ND_NULL,
        /* fd */ ND_NULL,
        /* fe */ ND_NULL,
        /* ff */ ND_NULL,
    }
};

const ND_TABLE_VEX_MMMMM gXopTable_root_mmmmm = 
{
    ND_ILUT_VEX_MMMMM,
    { 
        /* 00 */ ND_NULL,
        /* 01 */ ND_NULL,
        /* 02 */ ND_NULL,
        /* 03 */ ND_NULL,
        /* 04 */ ND_NULL,
        /* 05 */ ND_NULL,
        /* 06 */ ND_NULL,
        /* 07 */ ND_NULL,
        /* 08 */ (const void *)&gXopTable_root_08_opcode,
        /* 09 */ (const void *)&gXopTable_root_09_opcode,
        /* 0a */ (const void *)&gXopTable_root_0a_opcode,
        /* 0b */ ND_NULL,
        /* 0c */ ND_NULL,
        /* 0d */ ND_NULL,
        /* 0e */ ND_NULL,
        /* 0f */ ND_NULL,
        /* 10 */ ND_NULL,
        /* 11 */ ND_NULL,
        /* 12 */ ND_NULL,
        /* 13 */ ND_NULL,
        /* 14 */ ND_NULL,
        /* 15 */ ND_NULL,
        /* 16 */ ND_NULL,
        /* 17 */ ND_NULL,
        /* 18 */ ND_NULL,
        /* 19 */ ND_NULL,
        /* 1a */ ND_NULL,
        /* 1b */ ND_NULL,
        /* 1c */ ND_NULL,
        /* 1d */ ND_NULL,
        /* 1e */ ND_NULL,
        /* 1f */ ND_NULL,
    }
};

const PND_TABLE gXopTable = (const PND_TABLE)&gXopTable_root_mmmmm;


#endif

