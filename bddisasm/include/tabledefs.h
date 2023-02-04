/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef TABLEDEFS_H
#define TABLEDEFS_H

#include "../inc/disasmtypes.h"

//
// Types of tables.
//
typedef enum _ND_ILUT_TYPE
{
    ND_ILUT_INSTRUCTION = 0,// Table contains one entry that directly points to an instruction.
    ND_ILUT_OPCODE,         // Table contains 256 entries. Next entry is selected using an opcode.
    ND_ILUT_OPCODE_3DNOW,   // Table contains 256 entries. Next entry is selected using an opcode, but the 
                            // opcode follows the instruction.
    ND_ILUT_MODRM_MOD,      // Table contains 2 entries. Next entry is selected using modrm.mod (0 - mem, 1 - reg)
    ND_ILUT_MODRM_REG,      // Table contains 8 entries. Next entry is selected using modrm.reg.
    ND_ILUT_MODRM_RM,       // Table contains 8 entries. Next entry is selected using modrm.rm.
    ND_ILUT_MAN_PREFIX,     // Table contains 4 entries. Next entry is 0 (no prefix), 1 (0x66), 2 (0xF3), 3 (0xF2).
    ND_ILUT_MODE,           // Table contains 4 entries. Next entry is 0 (16 bit mode), 1 (32 bit mode), 2 (64 bit mode).
    ND_ILUT_DSIZE,          // Table contains 4 entries. Next entry is 0 (16 bit size), 1 (32 bit size), 2 (64 bit size).
                            // This DOES NOT take into consideration forced/default sizes.
    ND_ILUT_ASIZE,          // Default addressing mode is used to transition
    ND_ILUT_AUXILIARY,      // Table contains 8 entries. Next entry is 0 (no prefix), 1 (rex), 2 (rex.w), 3 (64 bit),
                            // 4 (default op size 16), 5 (default op size 32), 6 (default op size 64),
    ND_ILUT_VENDOR,         // Preferred vendor is used to transition. Default is entry 0. Otherwise, preferred
                            // vendor selects an alternate entry.
    ND_ILUT_FEATURE,        // Some instructions are mapped onto wide NOP space. They will be decoded only if the
                            // associated feature is set during decoding.
    ND_ILUT_VEX_MMMMM,      // Table contains 32 entries. Next entry is vex/xop/evex.mmmmm
    ND_ILUT_VEX_PP,         // Table contains 4 entries. Next entry is vex/xop/evex.pp
    ND_ILUT_VEX_L,          // Table contains 4 entries. Next entry is vex/xop.l or evex.l'l
    ND_ILUT_VEX_W,          // Table contains 2 entries. Next entry is vex/xop/evex.w
    ND_ILUT_VEX_WI,         // Table contains 2 entries. Next entry is vex/xop/evex.w. If not in 64 bit, next entry is 0.
} ND_ILUT_TYPE;


// Index definitions
#define ND_ILUT_INDEX_MOD_MEM           0
#define ND_ILUT_INDEX_MOD_REG           1

// Mandatory prefixes.
#define ND_ILUT_INDEX_MAN_PREF_NONE     0
#define ND_ILUT_INDEX_MAN_PREF_66       1
#define ND_ILUT_INDEX_MAN_PREF_F3       2
#define ND_ILUT_INDEX_MAN_PREF_F2       3

// Operating mode.
#define ND_ILUT_INDEX_MODE_NONE         0
#define ND_ILUT_INDEX_MODE_16           1
#define ND_ILUT_INDEX_MODE_32           2
#define ND_ILUT_INDEX_MODE_64           3

// Operand size.
#define ND_ILUT_INDEX_DSIZE_NONE        0
#define ND_ILUT_INDEX_DSIZE_16          1
#define ND_ILUT_INDEX_DSIZE_32          2
#define ND_ILUT_INDEX_DSIZE_64          3
#define ND_ILUT_INDEX_DSIZE_DEF64       4
#define ND_ILUT_INDEX_DSIZE_F64         5

// Address size.
#define ND_ILUT_INDEX_ASIZE_NONE        0
#define ND_ILUT_INDEX_ASIZE_16          1
#define ND_ILUT_INDEX_ASIZE_32          2
#define ND_ILUT_INDEX_ASIZE_64          3

// Misc conditions.
#define ND_ILUT_INDEX_AUX_NONE          0
#define ND_ILUT_INDEX_AUX_REXB          1
#define ND_ILUT_INDEX_AUX_REXW          2
#define ND_ILUT_INDEX_AUX_O64           3
#define ND_ILUT_INDEX_AUX_F3            4
#define ND_ILUT_INDEX_AUX_REP           5
#define ND_ILUT_INDEX_AUX_RIPREL        6

// Specific features for instructions that map on the wide NOP space.
#define ND_ILUT_FEATURE_NONE            0
#define ND_ILUT_FEATURE_MPX             1
#define ND_ILUT_FEATURE_CET             2
#define ND_ILUT_FEATURE_CLDEMOTE        3
#define ND_ILUT_FEATURE_PITI            4





typedef struct _ND_TABLE
{
    ND_UINT32       Type;
    const void      *Table[1];
} ND_TABLE, *PND_TABLE;

typedef struct _ND_TABLE_INSTRUCTION
{
    ND_UINT32       Type;
    const void      *Instruction;
} ND_TABLE_INSTRUCTION, *PND_TABLE_INSTRUCTION;

typedef struct _ND_TABLE_OPCODE
{
    ND_UINT32       Type;
    const void      *Table[256];
} ND_TABLE_OPCODE, *PND_TABLE_OPCODE;

typedef struct _ND_TABLE_MODRM_MOD
{
    ND_UINT32       Type;
    const void      *Table[2];
} ND_TABLE_MODRM_MOD, *PND_TABLE_MODRM_MOD;

typedef struct _ND_TABLE_MODRM_REG
{
    ND_UINT32       Type;
    const void      *Table[8];
} ND_TABLE_MODRM_REG, *PND_TABLE_MODRM_REG;

typedef struct _ND_TABLE_MODRM_RM
{
    ND_UINT32       Type;
    const void      *Table[8];
} ND_TABLE_MODRM_RM, *PND_TABLE_MODRM_RM;

typedef struct _ND_TABLE_MPREFIX
{
    ND_UINT32       Type;
    const void      *Table[4];
} ND_TABLE_MPREFIX, *PND_TABLE_MPREFIX;


typedef struct _ND_TABLE_AUXILIARY
{
    ND_UINT32       Type;
    const void      *Table[8];
} ND_TABLE_AUXILIARY, *PND_TABLE_AUXILIARY;

typedef struct _ND_TABLE_VENDOR
{
    ND_UINT32       Type;
    const void      *Table[6];
} ND_TABLE_VENDOR;

typedef struct _ND_TABLE_FEATURE
{
    ND_UINT32       Type;
    const void      *Table[8];
} ND_TABLE_FEATURE;

typedef struct _ND_TABLE_DSIZE
{
    ND_UINT32       Type;
    const void      *Table[6];
} ND_TABLE_DSIZE, *PND_TABLE_DSIZE;

typedef struct _ND_TABLE_ASIZE
{
    ND_UINT32       Type;
    const void      *Table[4];
} ND_TABLE_ASIZE, *PND_TABLE_ASIZE;

typedef struct _ND_TABLE_MODE
{
    ND_UINT32       Type;
    const void      *Table[4];
} ND_TABLE_MODE, *PND_TABLE_MODE;

typedef struct _ND_TABLE_VEX_MMMMM
{
    ND_UINT32       Type;
    const void      *Table[32];
} ND_TABLE_VEX_MMMMM, *PND_TABLE_VEX_MMMMM;

typedef struct _ND_TABLE_VEX_PP
{
    ND_UINT32       Type;
    const void      *Table[4];
} ND_TABLE_VEX_PP, *PND_TABLE_VEX_PP;

typedef struct _ND_TABLE_VEX_L
{
    ND_UINT32       Type;
    const void      *Table[4];
} ND_TABLE_VEX_L, *PND_TABLE_VEX_L;

typedef struct _ND_TABLE_VEX_W
{
    ND_UINT32       Type;
    const void      *Table[8];
} ND_TABLE_VEX_W, *PND_TABLE_VEX_W;


//
// One instruction entry. This structure should be maintained bellow 2 cache lines in size (128 bytes).
//
#ifdef _MSC_VER
__declspec(align(128))
#pragma warning(push)
#pragma warning(disable: 4214)
#else
__attribute__((aligned(128)))
#endif
typedef struct _ND_INSTRUCTION
{
    ND_UINT16       Instruction;    // Instruction identifier. Check ND_INS_CLASS definitions.
    ND_UINT8        Category;       // Instruction category. Check ND_INS_TYPE.
    ND_UINT8        IsaSet;         // Instruction set. Check ND_INS_SET.
    ND_UINT16       Mnemonic;       // Mnemonic (index inside the global mnemonic table).

    ND_UINT16       ValidPrefixes;  // Accepted prefixes.
    ND_UINT32       ValidModes;     // Valid operating modes for the instruction.
    ND_UINT8        ValidDecorators;// Accepted decorators (valid for EVEX instructions).

    ND_UINT8        OpsCount;       // Low 4 bits: explicit operands count; high 4 bits: implicit ops count.

    ND_UINT8        TupleType;      // EVEX tuple type.
    ND_UINT8        ExcType : 5;    // SSE/VEX/EVEX/OPMASK/AMX exception type.
    ND_UINT8        ExcClass : 3;   // Indicates the exception class (SSE/AVX, EVEX, OPMASK or AMX).
    
    ND_UINT8        FpuFlags;       // FPU status word C0, C1, C2 & C3 access type.

    ND_UINT8        Reserved2;
    ND_UINT16       Reserved3;

    ND_UINT32       Attributes;     // Instruction attributes.
    ND_UINT64       CpuidFlag;      // Required CPUID feature flag.

    // Per-flag access. Undefined flags will have their bit set in both the "Set" and "Cleared" mask, since a flag
    // cannot be both cleared and set.
    ND_UINT32       TestedFlags;    // Tested flags.
    ND_UINT32       ModifiedFlags;  // Modified flags.
    ND_UINT32       SetFlags;       // Flags that are always set to 1.
    ND_UINT32       ClearedFlags;   // Flags that are always cleared.

    ND_UINT64       Operands[10];
} ND_INSTRUCTION, *PND_INSTRUCTION;
#ifdef _MSC_VER
#pragma warning(pop)
#endif

//
// The following definitions are per-operand specific.
//
// Packed operand structure:
// Byte 0: operand type
// Byte 1: operand size
// Byte 2: operand flags
// Byte 3: operand access
// Byte 4: operand decorators
// Byte 5: operand block addressing mode - 0 if not block addressing
//
#define ND_OP(type, size, flags, acc, dec, block)       (((ND_UINT64)((type) & 0xFF) << 0)   |    \
                                                         ((ND_UINT64)((size) & 0xFF) << 8)   |    \
                                                         ((ND_UINT64)((flags) & 0xFF) << 16) |    \
                                                         ((ND_UINT64)((acc) & 0xFF) << 24)   |    \
                                                         ((ND_UINT64)((dec) & 0xFF) << 32)   |    \
                                                         ((ND_UINT64)((block) & 0xFF) << 40))

#define OP ND_OP

#define ND_OPS_CNT(expo, impo)              ((expo) | ((impo) << 4))
#define ND_EXP_OPS_CNT(cnt)                 ((cnt) & 0xF)
#define ND_IMP_OPS_CNT(cnt)                 ((cnt) >> 4)

#define ND_OP_TYPE(op)                      ((op) & 0xFF)
#define ND_OP_SIZE(op)                      (((op) >> 8) & 0xFF)
#define ND_OP_FLAGS(op)                     (((op) >> 16) & 0xFF)
#define ND_OP_ACCESS(op)                    (((op) >> 24) & 0xFF)
#define ND_OP_DECORATORS(op)                (((op) >> 32) & 0xFF)
#define ND_OP_BLOCK(op)                     (((op) >> 40) & 0xFF)




//
// Operand sizes descriptors. These are internal and stored inside the specifier. They must be interpreted
// accordingly to extract the actual size used in the instruction. These should not be used by anyone using
// the disassembler. Use the "Size" and "RawSize" fields inside the operand instead.
//
// For info on what each enum means, check out the valid_opsize field inside disasmlib.py.
//
typedef enum _ND_OPERAND_SIZE_SPEC
{
    ND_OPS_none = 0,
    ND_OPS_0,
    ND_OPS_asz,
    ND_OPS_ssz,
    ND_OPS_a,
    ND_OPS_c,
    ND_OPS_b,
    ND_OPS_w,
    ND_OPS_d,
    ND_OPS_q,
    ND_OPS_dq,
    ND_OPS_qq,
    ND_OPS_oq,
    ND_OPS_v,
    ND_OPS_y,
    ND_OPS_yf,
    ND_OPS_z,
    ND_OPS_s,
    ND_OPS_p,
    ND_OPS_fa,
    ND_OPS_fw,
    ND_OPS_fd,
    ND_OPS_fq,
    ND_OPS_ft,
    ND_OPS_fe,
    ND_OPS_fs,
    ND_OPS_l,
    ND_OPS_rx,
    ND_OPS_cl,
    ND_OPS_sd,
    ND_OPS_ss,
    ND_OPS_sh,
    ND_OPS_ps,
    ND_OPS_pd,
    ND_OPS_ph,
    ND_OPS_ev,
    ND_OPS_qv,
    ND_OPS_hv,
    ND_OPS_x,
    ND_OPS_uv,
    ND_OPS_fv,
    ND_OPS_t,       // Tile register size, can be up to 1K.
    ND_OPS_384,     // 384 bit Key Locker handle.
    ND_OPS_512,     // 512 bit Key Locker handle.
    ND_OPS_4096,    // 4096 bit MSR address/value table, used by RDMSRLIST/WRMSRLIST.
    // Stack sizes - indicates number of words. Also, hybrid sizes - sizes where from a large register (say 32 bit GPR)
    // only a smaller amount of data is used (for example, 8 bit).
    ND_OPS_v2,
    ND_OPS_v3,
    ND_OPS_v4,
    ND_OPS_v5,
    ND_OPS_v8,
    // 4 + 8 bytes accessed on the shadow stack by the SAVPREVSSP instruction.
    ND_OPS_12,
    // MIB
    ND_OPS_mib,
    // VSIB sizes (for both the index and the accessed data).
    ND_OPS_vm32x,
    ND_OPS_vm32y,
    ND_OPS_vm32z,
    ND_OPS_vm32h,
    ND_OPS_vm32n,
    ND_OPS_vm64x,
    ND_OPS_vm64y,
    ND_OPS_vm64z,
    ND_OPS_vm64h,
    ND_OPS_vm64n,
    // Used for arbitrary sized buffers.
    ND_OPS_unknown,

} ND_OPERAND_SIZE_SPEC;


//
// Operand types. These definitions are internal and have to be interpreted in order to find out what each
// operand represents. These should not be used by anyone using the disassembler. Use the "Type" field inside
// the operand structure instead.
//
// For more info on what each operand type means, please check out the valid_optype field inside disasmlib.py.
//
typedef enum _ND_OPERAND_TYPE_SPEC
{
    ND_OPT_A,
    ND_OPT_B,
    ND_OPT_C,
    ND_OPT_D,
    ND_OPT_E,
    ND_OPT_F,
    ND_OPT_G,
    ND_OPT_H,
    ND_OPT_I,
    ND_OPT_J,
    ND_OPT_K,

    ND_OPT_L,
    ND_OPT_M,
    ND_OPT_N,
    ND_OPT_O,
    ND_OPT_P,
    ND_OPT_Q,
    ND_OPT_R,
    ND_OPT_S,
    ND_OPT_T,
    ND_OPT_U,

    ND_OPT_V,
    ND_OPT_W,
    ND_OPT_X,
    ND_OPT_Y,
    ND_OPT_Z,
    ND_OPT_rB,
    ND_OPT_mB,
    ND_OPT_rK,
    ND_OPT_vK,
    ND_OPT_mK,
    ND_OPT_aK,
    ND_OPT_rM,
    ND_OPT_mM,
    ND_OPT_rT,
    ND_OPT_mT,
    ND_OPT_vT,
    ND_OPT_CONST_1,

    // These are implicit arguments inside instructions.

    // Special registers.
    ND_OPT_RIP,
    ND_OPT_MXCSR,
    ND_OPT_PKRU,
    ND_OPT_SSP,
    ND_OPT_UIF,

    // General Purpose REgisters.
    ND_OPT_GPR_AH,
    ND_OPT_GPR_rAX,
    ND_OPT_GPR_rCX,
    ND_OPT_GPR_rDX,
    ND_OPT_GPR_rBX,
    ND_OPT_GPR_rSP,
    ND_OPT_GPR_rBP,
    ND_OPT_GPR_rSI,
    ND_OPT_GPR_rDI,
    ND_OPT_GPR_rR8,
    ND_OPT_GPR_rR9,
    ND_OPT_GPR_rR11,

    // Segment registers.
    ND_OPT_SEG_CS,
    ND_OPT_SEG_SS,
    ND_OPT_SEG_DS,
    ND_OPT_SEG_ES,
    ND_OPT_SEG_FS,
    ND_OPT_SEG_GS,

    // FPU registers.
    ND_OPT_FPU_ST0,
    ND_OPT_FPU_STX,

    // SSE registers.
    ND_OPT_SSE_XMM0,
    ND_OPT_SSE_XMM1,
    ND_OPT_SSE_XMM2,
    ND_OPT_SSE_XMM3,
    ND_OPT_SSE_XMM4,
    ND_OPT_SSE_XMM5,
    ND_OPT_SSE_XMM6,
    ND_OPT_SSE_XMM7,

    // Implicit memory operands.
    ND_OPT_MEM_rAX,     // [rAX]
    ND_OPT_MEM_rCX,     // [rCX]
    ND_OPT_MEM_rBX_AL,  // [rBX + AL]
    ND_OPT_MEM_rDI,     // [rDI]
    ND_OPT_MEM_SHS,     // Shadow stack.
    ND_OPT_MEM_SHSP,    // Shadow stack pointed by the SSP.
    ND_OPT_MEM_SHS0,
    ND_OPT_MEM_SMSRT,   // Source MSR table, encoded in [RSI].
    ND_OPT_MEM_DMSRT,   // Destination MSR table, encoded in [RDI].

    // Special immediates.
    ND_OPT_Im2z,

    // Misc CR/XCR/MSR/SYS registers.
    ND_OPT_CR_0,
    ND_OPT_SYS_IDTR,
    ND_OPT_SYS_GDTR,
    ND_OPT_SYS_LDTR,
    ND_OPT_SYS_TR,
    ND_OPT_X87_CONTROL,
    ND_OPT_X87_TAG,
    ND_OPT_X87_STATUS,
    ND_OPT_MSR,
    ND_OPT_XCR,
    ND_OPT_MSR_TSC,
    ND_OPT_MSR_TSCAUX,
    ND_OPT_MSR_SEIP,
    ND_OPT_MSR_SESP,
    ND_OPT_MSR_SCS,
    ND_OPT_MSR_STAR,
    ND_OPT_MSR_LSTAR,
    ND_OPT_MSR_FMASK,
    ND_OPT_MSR_FSBASE,
    ND_OPT_MSR_GSBASE,
    ND_OPT_MSR_KGSBASE,
    ND_OPT_XCR_0,
    ND_OPT_REG_BANK,

} ND_OPERAND_TYPE_SPEC;


//
// Operand flags.
//
#define ND_OPF_DEFAULT              0x01    // The operand is default, no need to show it in disassembly.
#define ND_OPF_SEX_OP1              0x02    // The operand is sign-extended to the first operands' size.
#define ND_OPF_SEX_DWS              0x04    // The operand is sign-extended to the default word size.


//
// Operand access.
//
#define ND_OPA_N                    0x00    // The operand is not accessed.
#define ND_OPA_R                    0x01    // The operand is read.
#define ND_OPA_W                    0x02    // The operand is written.
#define ND_OPA_CR                   0x04    // The operand is read conditionally.
#define ND_OPA_CW                   0x08    // The operand is written conditionally.
#define ND_OPA_RW                   0x03    // Read-Write access.
#define ND_OPA_RCW                  0x09    // Read-Conditional Write access.
#define ND_OPA_CRW                  0X06    // Conditional Read-Write access.
#define ND_OPA_CRCW                 0x0C    // Conditional Read-Conditional Write access.
#define ND_OPA_P                    0x10    // The operand is memory, and it is prefetched.


//
// Operand decorator flags.
//
#define ND_OPD_MASK                 0x01    // Mask accepted.
#define ND_OPD_Z                    0x02    // Zeroing accepted.
#define ND_OPD_B32                  0x04    // 32 bit broadcast supported.
#define ND_OPD_B64                  0x08    // 64 bit broadcast supported.
#define ND_OPD_SAE                  0x10    // Suppress all exceptions supported.
#define ND_OPD_ER                   0x20    // Embedded rounding supported.
#define ND_OPD_B16                  0x40    // 16 bit broadcast supported.

#define ND_OPD_BCAST                (ND_OPD_B16 | ND_OPD_B32 | ND_OPD_B64)


//
// Include auto-generated stuff.
//
#include "mnemonics.h"
#include "../../inc/constants.h"
#include "instructions.h"
#include "prefixes.h"
#include "table_root.h"
#include "table_xop.h"
#include "table_vex.h"
#include "table_evex.h"

#endif // TABLEDEFS_H
