/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef BDDISASM_H
#define BDDISASM_H

#include "disasmstatus.h"
#include "registers.h"
#include "constants.h"
#include "cpuidflags.h"
#include "version.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4214) // Bitfield in type other than int.
#pragma warning(disable: 4201) // Nonstandard extension used: nameless struct/union.
#endif

//
// Preferred vendor; the disassembler will try to be smart and disassemble as much as it can, but if there are
// encoding conflicts, than an alternate vendor can be selected. Note that this has effect only on conflicting
// encodings.
//
#define ND_VEND_ANY                 0   // Generic decode, include any vendor.
#define ND_VEND_INTEL               1   // Prefer Intel.
#define ND_VEND_AMD                 2   // Prefer AMD.
#define ND_VEND_GEODE               3   // Prefer Geode.
#define ND_VEND_CYRIX               4   // Prefer Cyrix.

//
// These control what instructions should be decoded if they map onto the wide NOP space (0F 1A and 0F 1B). Those are
// tricky, because they might be NOP if the feature is disabled, but might be something else (even #UD) if the feature
// is enabled. Ergo, we allow the user to select whether said feature is on or off, so that he controls whether he
// sees the NOPs or the MPX/CET/CLDEMOTE/etc. instructions instead.
//
#define ND_FEAT_NONE                0x00    // No feature/mode enabled.
#define ND_FEAT_MPX                 0x01    // MPX support enabled.
#define ND_FEAT_CET                 0x02    // CET support enabled.
#define ND_FEAT_CLDEMOTE            0x04    // CLDEMOTE support enabled.
#define ND_FEAT_ALL                 0xFF    // Decode as if all features are enabled. This is default.

//
// Code type
//
#define ND_CODE_16                  0   // 16 bit decode mode.
#define ND_CODE_32                  1   // 32 bit decode mode.
#define ND_CODE_64                  2   // 64 bit decode mode.

//
// Data type
//
#define ND_DATA_16                  0   // 16 bit data size.
#define ND_DATA_32                  1   // 32 bit data size.
#define ND_DATA_64                  2   // 64 bit data size.

//
// Stack type
//
#define ND_STACK_16                 0   // 16 bit stack size.
#define ND_STACK_32                 1   // 32 bit stack size.
#define ND_STACK_64                 2   // 64 bit stack size.

//
// Addressing mode
//
#define ND_ADDR_16                  0   // 16 bit addressing.
#define ND_ADDR_32                  1   // 32 bit addressing.
#define ND_ADDR_64                  2   // 64 bit addressing.

//
// Operand mode/size
//
#define ND_OPSZ_16                  0   // 16 bit operand size.
#define ND_OPSZ_32                  1   // 32 bit operand size.
#define ND_OPSZ_64                  2   // 64 bit operand size.

//
// Vector mode/size
//
#define ND_VECM_128                 0   // 128 bit vector size.
#define ND_VECM_256                 1   // 256 bit vector size.
#define ND_VECM_512                 2   // 512 bit vector size.

//
// Encoding mode
//
#define ND_ENCM_LEGACY              0   // Legacy encoded instruction.
#define ND_ENCM_XOP                 1   // XOP encoded instruction.
#define ND_ENCM_VEX                 2   // VEX (bot 2B or 3B) encoded instruction.
#define ND_ENCM_EVEX                3   // EVEX encoded instruction.

//
// VEX prefixes
//
#define ND_VEXM_2B                  0   // 2B VEX prefix (0xC5).
#define ND_VEXM_3B                  1   // 3B VEX prefix (0xC4).


//
// Size definitions
//
#define ND_SIZE_8BIT                1           // 1 byte.
#define ND_SIZE_16BIT               2           // 1 word or 2 bytes.
#define ND_SIZE_32BIT               4           // 1 double word or 4 bytes.
#define ND_SIZE_48BIT               6           // 1 fword or 6 bytes.
#define ND_SIZE_64BIT               8           // 1 qword or 8 bytes.
#define ND_SIZE_80BIT               10          // 1 fpu word or 10 bytes.
#define ND_SIZE_112BIT              14          // FPU environment, 14 bytes.
#define ND_SIZE_128BIT              16          // 1 xmm word or 16 bytes.
#define ND_SIZE_224BIT              28          // FPU environment, 28 bytes.
#define ND_SIZE_256BIT              32          // 1 ymm word or 32 bytes.
#define ND_SIZE_384BIT              48          // 48 bytes, used for Key Locker handles.
#define ND_SIZE_512BIT              64          // 1 zmm word or 64 bytes. Used also for Key Locker handles.
#define ND_SIZE_752BIT              94          // FPU state, 94 bytes.
#define ND_SIZE_864BIT              108         // FPU state, 108 bytes.
#define ND_SIZE_4096BIT             512         // Extended state, 512 bytes.
#define ND_SIZE_1KB                 1024        // Tile register, 1KB.
#define ND_SIZE_CACHE_LINE          0xFFFFFFFE  // The size of a cache line.
#define ND_SIZE_UNKNOWN             0xFFFFFFFF  // Unknown/invalid size.


typedef uint32_t ND_OPERAND_SIZE;

typedef uint32_t ND_REG_SIZE;


//
// Prefix definitions
//
#define ND_PREFIX_G0_LOCK           0xF0    // LOCK prefix.
#define ND_PREFIX_G1_REPNE_REPNZ    0xF2    // REPNE/REPNZ prefix.
#define ND_PREFIX_G1_XACQUIRE       0xF2    // XACQUIRE prefix.
#define ND_PREFIX_G1_REPE_REPZ      0xF3    // REPE/REPZ prefix.
#define ND_PREFIX_G1_XRELEASE       0xF3    // XRELEASE prefix.
#define ND_PREFIX_G1_BND            0xF2    // BND prefix.
#define ND_PREFIX_G2_SEG_CS         0x2E    // CS segment override.
#define ND_PREFIX_G2_SEG_SS         0x36    // SS segment override.
#define ND_PREFIX_G2_SEG_DS         0x3E    // DS segment override.
#define ND_PREFIX_G2_SEG_ES         0x26    // ES segment override.
#define ND_PREFIX_G2_SEG_FS         0x64    // FS segment override.
#define ND_PREFIX_G2_SEG_GS         0x65    // GS segment override.
#define ND_PREFIX_G2_BR_NOT_TAKEN   0x2E    // Branch not taken hint.
#define ND_PREFIX_G2_BR_TAKEN       0x3E    // Branch taken hint.
#define ND_PREFIX_G2_BR_ALT         0x64    // Alternating branch hint.
#define ND_PREFIX_G2_NO_TRACK       0x3E    // Do not track prefix.
#define ND_PREFIX_G3_OPERAND_SIZE   0x66    // Operand size override.
#define ND_PREFIX_G4_ADDR_SIZE      0x67    // Address size override.

#define ND_PREFIX_REX_MIN           0x40    // First REX prefix.
#define ND_PREFIX_REX_MAX           0x4F    // Last REX prefix.
#define ND_PREFIX_VEX_2B            0xC5    // 2B VEX prefix.
#define ND_PREFIX_VEX_3B            0xC4    // 3B VEX prefix.
#define ND_PREFIX_XOP               0x8F    // XOP prefix.
#define ND_PREFIX_EVEX              0x62    // EVEX prefix.


//
// Instruction flags
//
#define ND_FLAG_MODRM               0x00000001  // The instruction has modrm.
#define ND_FLAG_F64                 0x00000002  // The operand is forced to 64 bit. Size changing prefix 0x66 is ignored.
#define ND_FLAG_D64                 0x00000004  // The default operand size is 64 bit. Size changing prefix 0x66
                                                // changes the size to 16 bit. No 32 bit version can be encoded.
#define ND_FLAG_O64                 0x00000008  // The instruction is available only in 64 bit mode.
#define ND_FLAG_I64                 0x00000010  // The instruction is invalid in 64 bit mode.
#define ND_FLAG_COND                0x00000020  // The instruction has a condition code encoded in low 4 bits of the opcode.
#define ND_FLAG_SSE_CONDB           0x00000040  // The instruction has a SSE condition byte.
#define ND_FLAG_VSIB                0x00000080  // The instruction uses VSIB addressing mode.
#define ND_FLAG_MIB                 0x00000100  // The instruction used MIB addressing mode.
#define ND_FLAG_LIG                 0x00000200  // L flag inside XOP/VEX/EVEX is ignored.
#define ND_FLAG_WIG                 0x00000400  // W flag inside XOP/VEX/EVEX is ignored.
#define ND_FLAG_3DNOW               0x00000800  // The instruction is 3DNow!. The opcode is the last byte.
#define ND_FLAG_LOCK_SPECIAL        0x00001000  // MOV to/from CR in 16/32 bit, on AMD, extend the access to
                                                // high 8 CRs via the LOCK prefix.
#define ND_FLAG_MMASK               0x00002000  // Mandatory mask. The instruction does not accept k0 as mask.
#define ND_FLAG_NOMZ                0x00004000  // Zeroing is not permitted when accessing memory.
#define ND_FLAG_NOL0                0x00008000  // SSE 128 encoding is not permitted for the instruction.
#define ND_FLAG_NOA16               0x00010000  // The instruction does not support 16 bit addressing.
#define ND_FLAG_MFR                 0x00020000  // The Mod inside Mod R/M is forced to reg. No SIB/disp present.
#define ND_FLAG_VECTOR              0x00040000  // The instruction is a SIMD instruction that operates on vector regs.
#define ND_FLAG_S66                 0x00080000  // Special flag for mandatory 0x66 prefix that actually changes
                                                // the default op length.
#define ND_FLAG_BITBASE             0x00100000  // The instruction uses bitbase addressing mode.
#define ND_FLAG_AG                  0x00200000  // The instruction is an address generator; no actual memory access.
#define ND_FLAG_SHS                 0x00400000  // The instruction does a shadow stack access.
#define ND_FLAG_CETT                0x00800000  // The instruction is CET tracked.
#define ND_FLAG_SERIAL              0x01000000  // The instruction is serializing.
#define ND_FLAG_NO_RIP_REL          0x02000000  // The instruction doesn't work with RIP relative addressing.
#define ND_FLAG_NO66                0x04000000  // The 0x66 prefix is not accepted by the instruction.
#define ND_FLAG_SIBMEM              0x08000000  // sibmem addressing is used (Intel AMX instructions).
#define ND_FLAG_I67                 0x10000000  // Ignore the 0x67 prefix in 64 bit mode (Intel MPX instructions).
#define ND_FLAG_IER                 0x20000000  // Ignore EVEX embedded rounding.


//
// Accepted prefixes map
//
#define ND_PREF_REP                 0x0001      // The instruction supports REP prefix.
#define ND_PREF_REPC                0x0002      // The instruction supports REPZ/REPNZ prefixes.
#define ND_PREF_LOCK                0x0004      // The instruction supports LOCK prefix.
#define ND_PREF_HLE                 0x0008      // The instruction supports XACQUIRE/XRELEASE prefixes.
#define ND_PREF_XACQUIRE            0x0010      // The instruction supports only XACQUIRE.
#define ND_PREF_XRELEASE            0x0020      // The instruction supports only XRELEASE.
#define ND_PREF_BND                 0x0040      // The instruction supports BND prefix.
#define ND_PREF_BHINT               0x0080      // The instruction supports branch hints.
#define ND_PREF_HLE_WO_LOCK         0x0100      // HLE prefix is accepted without LOCK.
#define ND_PREF_DNT                 0x0200      // The instruction supports the DNT (Do Not Track) CET prefix.


//
// Accepted decorators map. These are stored per-instruction. There are also per-operand indicators for
// each decorator.
//
#define ND_DECO_ER                  0x01        // Embedded rounding is accepted.
#define ND_DECO_SAE                 0x02        // Suppress all Exceptions is accepted.
#define ND_DECO_ZERO                0x04        // Zeroing is accepted.
#define ND_DECO_MASK                0x08        // Masking is accepted.
#define ND_DECO_BROADCAST           0x10        // Memory broadcast is accepted.


//
// Operand access flags.
//
#define ND_ACCESS_NONE              0x00        // The operand is not accessed.
#define ND_ACCESS_READ              0x01        // The operand is read.
#define ND_ACCESS_WRITE             0x02        // The operand is written.
#define ND_ACCESS_COND_READ         0x04        // The operand is read only if some conditions are met.
#define ND_ACCESS_COND_WRITE        0x08        // The operand is written only if some conditions are met (ie: CMOVcc).
#define ND_ACCESS_ANY_READ          (ND_ACCESS_READ | ND_ACCESS_COND_READ)      // Any read mask.
#define ND_ACCESS_ANY_WRITE         (ND_ACCESS_WRITE | ND_ACCESS_COND_WRITE)    // Any write mask.
#define ND_ACCESS_PREFETCH          0x10        // The operand is prefetched.


//
// Predicate/condition definitions.
//
#define ND_COND_OVERFLOW            0x0         // OF
#define ND_COND_CARRY               0x2         // CF
#define ND_COND_BELOW               0x2         // CF
#define ND_COND_NOT_ABOVE_OR_EQUAL  0x2         // CF
#define ND_COND_ZERO                0x4         // ZF
#define ND_COND_EQUAL               0x4         // ZF
#define ND_COND_BELOW_OR_EQUAL      0x6         // CF | ZF
#define ND_COND_NOT_ABOVE           0x6         // CF | ZF
#define ND_COND_SIGN                0x8         // SF
#define ND_COND_PARITY              0xA         // PF
#define ND_COND_LESS                0xC         // SF ^ OF
#define ND_COND_LESS_OR_EQUAL       0xE         // (SF ^ OF) | ZF
#define ND_COND_NOT(p)              ((p) | 0x1) // Negates the predicate.

// Kept for backwards compatibility - renamed to ND_COND*.
#define ND_PRED_OVERFLOW            0x0         // OF
#define ND_PRED_CARRY               0x2         // CF
#define ND_PRED_BELOW               0x2         // CF
#define ND_PRED_NOT_ABOVE_OR_EQUAL  0x2         // CF
#define ND_PRED_ZERO                0x4         // ZF
#define ND_PRED_EQUAL               0x4         // ZF
#define ND_PRED_BELOW_OR_EQUAL      0x6         // CF | ZF
#define ND_PRED_NOT_ABOVE           0x6         // CF | ZF
#define ND_PRED_SIGN                0x8         // SF
#define ND_PRED_PARITY              0xA         // PF
#define ND_PRED_LESS                0xC         // SF ^ OF
#define ND_PRED_LESS_OR_EQUAL       0xE         // (SF ^ OF) | ZF
#define ND_PRED_NOT(p)              ((p) | 0x1) // Negates the predicate.


//
// Condition code definitions. These apply to condition codes encoded in instruction bytes as used by some SSE/AVX
// instructions.
//
#define ND_SSE_COND_EQ              0x00        // Equal.
#define ND_SSE_COND_LT              0x01        // Less-than.
#define ND_SSE_COND_LE              0x02        // Less-than or equal.
#define ND_SSE_COND_UNORD           0x03        // Unordered.
#define ND_SSE_COND_FALSE1          0x03        // False.
#define ND_SSE_COND_NEQ             0x04        // Not equal.
#define ND_SSE_COND_NLT             0x05        // Not less-than.
#define ND_SSE_COND_NLE             0x06        // Not less-than or equal.
#define ND_SSE_COND_ORD             0x07        // Ordered.
#define ND_SSE_COND_TRUE1           0x07        // True.
#define ND_SSE_COND_EQ_UQ           0x08        // Equal.
#define ND_SSE_COND_NGE             0x09        // Not greater-than or equal.
#define ND_SSE_COND_NGT             0x0A        // Not greater-than.
#define ND_SSE_COND_FALSE           0x0B        // False.
#define ND_SSE_COND_NEQ_OQ          0x0C        // Not equal.
#define ND_SSE_COND_GE              0x0D        // Greater-than or equal.
#define ND_SSE_COND_GT              0x0E        // Greater-than.
#define ND_SSE_COND_TRUE            0x0F        // True.
#define ND_SSE_COND_EQ_OS           0x10        // Equal.
#define ND_SSE_COND_LT_OQ           0x11        // Less-than.
#define ND_SSE_COND_LE_OQ           0x12        // Less-than or equal.
#define ND_SSE_COND_UNORD_S         0x13        // Unordered.
#define ND_SSE_COND_NEQ_US          0x14        // Not equal.
#define ND_SSE_COND_NLT_UQ          0x15        // Not less-than.
#define ND_SSE_COND_NLE_UQ          0x16        // Not less-than or equal.
#define ND_SSE_COND_ORD_S           0x17        // Ordered.
#define ND_SSE_COND_EQ_US           0x18        // Equal.
#define ND_SSE_COND_NGE_UQ          0x19        // Not greater-than or equal.
#define ND_SSE_COND_NGT_UQ          0x1A        // Not greater-than.
#define ND_SSE_COND_FALSE_OS        0x1B        // False.
#define ND_SSE_COND_NEQ_OS          0x1C        // Not equal.
#define ND_SSE_COND_GE_OQ           0x1D        // Greater-than or equal.
#define ND_SSE_COND_GT_OQ           0x1E        // Greater-than.
#define ND_SSE_COND_TRUE_US         0x1F        // True.


//
// Valid CPU modes.
//
// Group 1: ring
#define ND_MOD_R0                   0x00000001  // Instruction valid in ring 0.
#define ND_MOD_R1                   0x00000002  // Instruction valid in ring 1.
#define ND_MOD_R2                   0x00000004  // Instruction valid in ring 2.
#define ND_MOD_R3                   0x00000008  // Instruction valid in ring 3.

// Group 2: operating mode.
#define ND_MOD_REAL                 0x00000010  // Instruction valid in real mode.
#define ND_MOD_V8086                0x00000020  // Instruction valid in virtual 8086 mode.
#define ND_MOD_PROT                 0x00000040  // Instruction valid in protected mode.
#define ND_MOD_COMPAT               0x00000080  // Instruction valid in compatibility mode.
#define ND_MOD_LONG                 0x00000100  // Instruction valid in long mode.

// Group 3: misc modes.
#define ND_MOD_SMM                  0x00001000  // Instruction valid in System-Management Mode.
#define ND_MOD_SMM_OFF              0x00002000  // Instruction valid outside SMM.
#define ND_MOD_SGX                  0x00004000  // Instruction valid in SGX enclaves.
#define ND_MOD_SGX_OFF              0x00008000  // Instruction valid outside SGX enclaves.
#define ND_MOD_TSX                  0x00010000  // Instruction valid in TSX transactional regions.
#define ND_MOD_TSX_OFF              0x00020000  // Instruction valid outside TSX.


// Group 4: VMX
#define ND_MOD_VMXR                 0x00040000  // Instruction valid in VMX Root mode.
#define ND_MOD_VMXN                 0x00080000  // Instruction valid in VMX non-root mode.
#define ND_MOD_VMXR_SEAM            0x00100000  // Instruction valid in VMX root Secure Arbitration Mode.
#define ND_MOD_VMXN_SEAM            0x00200000  // Instruction valid in VMX non-root Secure Arbitration Mode.
#define ND_MOD_VMX_OFF              0x00400000  // Instruction valid outside VMX operation.

#define ND_MOD_RING_MASK            0x0000000F  // Valid ring mask.
#define ND_MOD_MODE_MASK            0x000001F0  // Valid mode mask.
#define ND_MOD_OTHER_MASK           0x0003F000  // Misc mask.
#define ND_MOD_VMX_MASK             0x007C0000  // VMX mask.

// For instructions valid in any operating mode.
#define ND_MOD_ANY                  0xFFFFFFFF  // Instruction valid in any mode.


//
// Misc constants
//
#define ND_MAX_INSTRUCTION_LENGTH   15          // 15 bytes is the maximum instruction length supported by the x86 arch.
#define ND_MAX_MNEMONIC_LENGTH      32          // Should do for now.
#define ND_MIN_BUF_SIZE             128         // Textual disassembly minimal buffer size.
#define ND_MAX_OPERAND              10          // No more than 10 operands/instruction, but I'm generous.
#define ND_MAX_REGISTER_SIZE        64          // Maximum register size - 64 bytes.

#define ND_MAX_GPR_REGS             16          // Max number of GPRs.
#define ND_MAX_SEG_REGS             8           // Max number of segment registers.
#define ND_MAX_FPU_REGS             8           // Max number of FPU registers.
#define ND_MAX_MMX_REGS             8           // Max number of MMX registers.
#define ND_MAX_SSE_REGS             32          // Max number of SSE registers.
#define ND_MAX_CR_REGS              16          // Max number of control registers.
#define ND_MAX_DR_REGS              16          // Max number of debug registers.
#define ND_MAX_TR_REGS              16          // Max number of test registers.
#define ND_MAX_MSK_REGS             8           // Max number of mask registers.
#define ND_MAX_BND_REGS             4           // Max number of bound registers.
#define ND_MAX_SYS_REGS             8           // Max number of system registers.
#define ND_MAX_X87_REGS             8           // Max number of x87 state/control registers registers.
#define ND_MAX_TILE_REGS            8           // Max number of tile registers.



//
// Misc macros.
//

// Sign extend 8 bit to 64 bit.
#define ND_SIGN_EX_8(x)             (((x) & 0x00000080) ? (0xFFFFFFFFFFFFFF00 | (x)) : (x))
// Sign extend 16 bit to 64 bit.
#define ND_SIGN_EX_16(x)            (((x) & 0x00008000) ? (0xFFFFFFFFFFFF0000 | (x)) : (x))
// Sign extend 32 bit to 64 bit.
#define ND_SIGN_EX_32(x)            (((x) & 0x80000000) ? (0xFFFFFFFF00000000 | (x)) : (x))
// Wrapper for for ND_SIGN_EX_8/ND_SIGN_EX_16/ND_SIGN_EX_32. Sign extend sz bytes to 64 bits.
#define ND_SIGN_EX(sz, x)           ((sz) == 1 ? ND_SIGN_EX_8(x) : (sz) == 2 ? ND_SIGN_EX_16(x) :                   \
                                     (sz) == 4 ? ND_SIGN_EX_32(x) : (x))
// Trim 64 bits to sz bytes.
#define ND_TRIM(sz, x)              ((sz) == 1 ? (x) & 0xFF : (sz) == 2 ? (x) & 0xFFFF :                            \
                                     (sz) == 4 ? (x) & 0xFFFFFFFF : (x))
// Returns most significant bit, given size in bytes sz.
#define ND_MSB(sz, x)               ((sz) == 1 ? ((x) >> 7) & 1 : (sz) == 2 ? ((x) >> 15) & 1 :                     \
                                     (sz) == 4 ? ((x) >> 31) & 1 : ((x) >> 63) & 1)
// Returns least significant bit.
#define ND_LSB(sz, x)               ((x) & 1)
// Convert a size in bytes to a bitmask.
#define ND_SIZE_TO_MASK(sz)         (((sz) < 8) ? ((1ULL << ((sz) * 8)) - 1) : (0xFFFFFFFFFFFFFFFF))
// Get bit at position bit from x.
#define ND_GET_BIT(bit, x)          (((x) >> (bit)) & 1)
// Return the sign of sz bytes long value x.
#define ND_GET_SIGN(sz, x)          ND_MSB(sz, x)
// Sets the sign of the sz bytes long value x.
#define ND_SET_SIGN(sz, x)          ND_SIGN_EX(sz, x)

#ifdef BIG_ENDIAN
#define ND_FETCH_64(b)              ((uint64_t)FETCH_uint32_t((char *)b) | ((uint64_t)FETCH_uint32_t((char *)b + 4) << 32))
#define ND_FETCH_32(b)              ((uint32_t)FETCH_WORD((char *)b)  | ((uint32_t)FETCH_WORD((char *)b + 2)  << 16))
#define ND_FETCH_16(b)              ((((char *)b)[0]) | (((char *)b)[1] << 8))
#define ND_FETCH_8(b)               (*((char *)b))
#else
#define ND_FETCH_64(b)              (*((uint64_t *)(b)))
#define ND_FETCH_32(b)              (*((uint32_t *)(b)))
#define ND_FETCH_16(b)              (*((uint16_t *)(b)))
#define ND_FETCH_8(b)               (*((uint8_t *)(b)))
#endif


//
// Helper macros which simply test the presence of various ND_FLAG_* in the instruction attributes.
//
#define ND_IS_3DNOW(ix)             (!!((ix)->Attributes & ND_FLAG_3DNOW))
#define ND_HAS_PREDICATE(ix)        (!!((ix)->Attributes & ND_FLAG_COND))
#define ND_HAS_CONDITION(ix)        (!!((ix)->Attributes & ND_FLAG_COND))
#define ND_HAS_SSE_CONDITION(ix)    (!!((ix)->Attributes & ND_FLAG_SSE_CONDB))
#define ND_HAS_MODRM(ix)            (!!((ix)->Attributes & ND_FLAG_MODRM))
#define ND_HAS_VSIB(ix)             (!!((ix)->Attributes & ND_FLAG_VSIB))
#define ND_HAS_MIB(ix)              (!!((ix)->Attributes & ND_FLAG_MIB))
#define ND_HAS_VECTOR(ix)           (!!((ix)->Attributes & ND_FLAG_VECTOR))
#define ND_HAS_BITBASE(ix)          (!!((ix)->Attributes & ND_FLAG_BITBASE))
#define ND_HAS_AG(ix)               (!!((ix)->Attributes & ND_FLAG_AG))
#define ND_HAS_SIBMEM(ix)           (!!((ix)->Attributes & ND_FLAG_SIBMEM))
#define ND_HAS_SHS(ix)              (!!((ix)->Attributes & ND_FLAG_SHS))
#define ND_HAS_CETT(ix)             (!!((ix)->Attributes & ND_FLAG_CETT))

//
// Supported prefixes macros.
//
#define ND_REP_SUPPORT(ix)          (!!((ix)->ValidPrefixes.Rep))
#define ND_REPC_SUPPORT(ix)         (!!((ix)->ValidPrefixes.RepCond))
#define ND_LOCK_SUPPORT(ix)         (!!((ix)->ValidPrefixes.Lock))
#define ND_HLE_SUPPORT(ix)          (!!((ix)->ValidPrefixes.Hle))
#define ND_XACQUIRE_SUPPORT(ix)     (!!((ix)->ValidPrefixes.Xacquire))
#define ND_XRELEASE_SUPPORT(ix)     (!!((ix)->ValidPrefixes.Xrelease))
#define ND_BND_SUPPORT(ix)          (!!((ix)->ValidPrefixes.Bnd))
#define ND_BHINT_SUPPORT(ix)        (!!((ix)->ValidPrefixes.Bhint))
#define ND_DNT_SUPPORT(ix)          (!!((ix)->ValidPrefixes.Dnt))

//
// Decorators map macros.
//
#define ND_DECORATOR_SUPPORT(ix)    ((ix)->ValidDecorators.Raw != 0)
#define ND_MASK_SUPPORT(ix)         (!!((ix)->ValidDecorators.Mask))
#define ND_ZERO_SUPPORT(ix)         (!!((ix)->ValidDecorators.Zero))
#define ND_ER_SUPPORT(ix)           (!!((ix)->ValidDecorators.Er))
#define ND_SAE_SUPPORT(ix)          (!!((ix)->ValidDecorators.Sae))
#define ND_BROADCAST_SUPPORT(ix)    (!!((ix)->ValidDecorators.Broadcast))

// Generates a unique ID per register type, size and reg.
#define ND_OP_REG_ID(op)            (((op)->Type << 24) | ((op)->Info.Register.Type << 16) | \
                                     ((op)->Info.Register.Size << 8) | ((op)->Info.Register.Reg))

// Example: ND_IS_OP_REG(op, ND_REG_GPR, 4, REG_ESP)
// Example: ND_IS_OP_REG(op, ND_REG_CR,  8, REG_CR3)
// Example: ND_IS_OP_REG(op, ND_REG_RIP, 8, 0)

// Checks if the indicated operand op is a register of type t, with size s and index r.
#define ND_IS_OP_REG(op, t, s, r)   ((uint32_t)(ND_OP_REG_ID(op)) == (uint32_t)((ND_OP_REG << 24)|(t << 16)|(s << 8)|(r)))

// Checjs if the indicated operand is the stack.
#define ND_IS_OP_STACK(op)          ((op)->Type == ND_OP_MEM && (op)->Info.Memory.IsStack)


//
// Operand types.
//
typedef enum _ND_OPERAND_TYPE
{
    ND_OP_NOT_PRESENT,      // Indicates the absence of any operand.
    ND_OP_REG,              // The operand is a register.
    ND_OP_MEM,              // The operand is located in memory.
    ND_OP_IMM,              // The operand is an immediate.
    ND_OP_OFFS,             // The operand is a relative offset.
    ND_OP_ADDR,             // The operand is an absolute address, in the form seg:offset.
    ND_OP_CONST,            // The operand is an implicit constant.
    ND_OP_BANK,             // An entire bank/set of registers are being accessed. Used in PUSHA/POPA/XSAVE/LOADALL.
} ND_OPERAND_TYPE;


//
// Register types.
//
typedef enum _ND_REG_TYPE
{
    ND_REG_NOT_PRESENT,
    ND_REG_GPR,             // The register is a 8/16/32/64 bit general purpose register.
    ND_REG_SEG,             // The register is a segment register.
    ND_REG_FPU,             // The register is a 80-bit FPU register.
    ND_REG_MMX,             // The register is a 64-bit MMX register.
    ND_REG_SSE,             // The register is a 128/256/512 bit SSE vector register.
    ND_REG_CR,              // The register is a control register.
    ND_REG_DR,              // The register is a debug register.
    ND_REG_TR,              // The register is a test register.
    ND_REG_BND,             // The register is a bound register.
    ND_REG_MSK,             // The register is a mask register.
    ND_REG_TILE,            // The register is a tile register.
    ND_REG_MSR,             // The register is a model specific register.
    ND_REG_XCR,             // The register is a extended control register.
    ND_REG_SYS,             // The register is a system register.
    ND_REG_X87,             // The register is a x87 status/control register.
    ND_REG_MXCSR,           // The register is the MXCSR register.
    ND_REG_PKRU,            // The register is the PKRU register.
    ND_REG_SSP,             // The register is the SSP (Shadow Stack Pointer) register.
    ND_REG_FLG,             // The register is the FLAGS register.
    ND_REG_RIP,             // The register is the instruction pointer register.
    ND_REG_UIF,             // The register is the User Interrupt Flag.
} ND_REG_TYPE;


//
// Operand encoding types.
//
typedef enum _ND_OPERAND_ENCODING
{
    ND_OPE_NP,              // No encoding present.
    ND_OPE_R,               // Operand encoded in modrm.reg.
    ND_OPE_M,               // Operand encoded in modrm.rm.
    ND_OPE_V,               // Operand encoded in Xop/Vex/Evex/Mvex.(v')vvvv
    ND_OPE_D,               // Operand is encoded inside subsequent instruction bytes.
    ND_OPE_O,               // Operand is encoded in low 3 bit of the opcode.
    ND_OPE_I,               // Operand is an immediate.
    ND_OPE_C,               // Operand is CL.
    ND_OPE_1,               // Operand is 1.
    ND_OPE_L,               // Operand is reg encoded in immediate.
    ND_OPE_A,               // Operand is encoded in Evex.aaa.
    ND_OPE_E,               // Operand is a MSR or XCR encoded in ECX register.
    ND_OPE_S,               // Operand is implicit/suppressed. Not encoded anywhere.
} ND_OPERAND_ENCODING;


//
// Instruction tuple type; used to determine compressed displacement size for disp8 EVEX instructions. Note that
// most of the EVEX encoded instructions use the compressed displacement addressing scheme.
//
typedef enum _ND_TUPLE
{
    ND_TUPLE_None,
    ND_TUPLE_FV,            // Full Vector
    ND_TUPLE_HV,            // Half Vector
    ND_TUPLE_T1S8,          // Tuple1 scalar, size 8 bit
    ND_TUPLE_T1S16,         // Tuple1 scalar, size 16 bit
    ND_TUPLE_T1S,           // Tuple1 scalar, size 32/64 bit
    ND_TUPLE_T1F,           // Tuple1 float, size 32/64 bit
    ND_TUPLE_T2,            // Tuple2, 64/128 bit
    ND_TUPLE_T4,            // Tuple4, 128/256 bit
    ND_TUPLE_T8,            // Tuple8, 256 bit
    ND_TUPLE_FVM,           // Full Vector Memory
    ND_TUPLE_HVM,           // Half Vector Memory
    ND_TUPLE_QVM,           // Quarter Vector Memory
    ND_TUPLE_OVM,           // Oct Vector Memory
    ND_TUPLE_M128,          // M128, 128 bit
    ND_TUPLE_DUP,           // DUP (VMOVDDUP)
    ND_TUPLE_T1_4X,         // 4 x 32 bit Memory Elements are referenced
} ND_TUPLE;


//
// EVEX rounding control.
//
typedef enum _ND_ROUNDING
{
    ND_RND_RNE,             // Round to nearest equal.
    ND_RND_RD,              // Round down.
    ND_RND_RU,              // Round up.
    ND_RND_RZ,              // round to zero.
} ND_ROUNDING;


//
// Exception classes. Different instruction sets or encodings are covered by different exception classes.
//
typedef enum _ND_EX_CLASS
{
    ND_EXC_None,
    ND_EXC_SSE_AVX,         // SSE/AVX exception class (for legacy encoded SSE instructions and VEX instructions).
    ND_EXC_EVEX,            // EVEX exception class (for EVEX encoded AVX* instructions).
    ND_EXC_OPMASK,          // Opmask instructions exception class.
    ND_EXC_AMX,             // AMX exception class type (for VEX encoded AMX instructions).
} ND_EX_CLASS;


//
// SSE/AVX exception types. Covers VEX instructions as well, except for AMX instructions.
//
typedef enum _ND_EX_TYPE_SSE_AVX
{
    ND_EXT_SSE_AVX_None,
    ND_EXT_1, 
    ND_EXT_2, 
    ND_EXT_3, 
    ND_EXT_4, 
    ND_EXT_5, 
    ND_EXT_6, 
    ND_EXT_7, 
    ND_EXT_8, 
    ND_EXT_9, 
    ND_EXT_10,
    ND_EXT_11,
    ND_EXT_12,
    ND_EXT_13,
} ND_EX_TYPE_SSE_AVX;


//
// EVEX exception types.
//
typedef enum _ND_EX_TYPE_EVEX
{
    ND_EXT_EVEX_None,
    ND_EXT_E1,
    ND_EXT_E1NF,
    ND_EXT_E2,
    ND_EXT_E3,
    ND_EXT_E3NF,
    ND_EXT_E4,
    ND_EXT_E4nb,
    ND_EXT_E4NF,
    ND_EXT_E4NFnb,
    ND_EXT_E5,
    ND_EXT_E5NF,
    ND_EXT_E6,
    ND_EXT_E6NF,
    ND_EXT_E7NM,
    ND_EXT_E9,
    ND_EXT_E9NF,
    ND_EXT_E10,
    ND_EXT_E10NF,
    ND_EXT_E11,
    ND_EXT_E12,
    ND_EXT_E12NP,
} ND_EX_TYPE_EVEX;


//
// Opmask exception types.
//
typedef enum _ND_EX_TYPE_OPMASK
{
    ND_EXT_OPMASK_None,
    ND_EXT_K20,
    ND_EXT_K21,
} ND_EX_TYPE_OPMASK;


//
// AMX exception types.
//
typedef enum _ND_EX_TYPE_AMX
{
    ND_EXT_AMX_None,
    ND_EXT_AMX_E1,
    ND_EXT_AMX_E2,
    ND_EXT_AMX_E3,
    ND_EXT_AMX_E4,
    ND_EXT_AMX_E5,
    ND_EXT_AMX_E6,
} ND_EX_TYPE_AMX;


//
// Operands access map. Contains every register except for MSR & XCR, includes memory, flags, RIP, stack.
// Use NdGetFullAccessMap to populate this structure.
//
typedef struct _ND_ACCESS_MAP
{
    uint8_t         RipAccess;
    uint8_t         FlagsAccess;
    uint8_t         StackAccess;
    uint8_t         MemAccess;
    uint8_t         MxcsrAccess;
    uint8_t         PkruAccess;
    uint8_t         SspAccess;
    uint8_t         GprAccess[ND_MAX_GPR_REGS];
    uint8_t         SegAccess[ND_MAX_SEG_REGS];
    uint8_t         FpuAccess[ND_MAX_FPU_REGS];
    uint8_t         MmxAccess[ND_MAX_MMX_REGS];
    uint8_t         SseAccess[ND_MAX_SSE_REGS];
    uint8_t         CrAccess [ND_MAX_CR_REGS ];
    uint8_t         DrAccess [ND_MAX_DR_REGS ];
    uint8_t         TrAccess [ND_MAX_TR_REGS ];
    uint8_t         BndAccess[ND_MAX_BND_REGS];
    uint8_t         MskAccess[ND_MAX_MSK_REGS];
    uint8_t         SysAccess[ND_MAX_SYS_REGS];
    uint8_t         X87Access[ND_MAX_X87_REGS];
} ND_ACCESS_MAP, *PND_ACCESS_MAP;



//
// Operand access mode.
//
typedef union _ND_OPERAND_ACCESS
{
    uint8_t             Access;
    struct
    {
        uint8_t         Read : 1;       // The operand is read.
        uint8_t         Write : 1;      // The operand is written.
        uint8_t         CondRead : 1;   // The operand is read only under some conditions.
        uint8_t         CondWrite : 1;  // The operand is written only under some conditions.
        uint8_t         Prefetch : 1;   // The operand is prefetched.
    };
} ND_OPERAND_ACCESS;


//
// Operand flags.
//
typedef union _ND_OPERAND_FLAGS
{
    uint8_t             Flags;
    struct
    {
        uint8_t         IsDefault : 1;        // 1 if the operand is default. This also applies to implicit ops.
        uint8_t         SignExtendedOp1 : 1;  // 1 if the operand is sign extended to the first operands' size.
        uint8_t         SignExtendedDws : 1;  // 1 if the operand is sign extended to the default word size.
    };
} ND_OPERAND_FLAGS;


//
// Constant operand.
//
typedef struct _ND_OPDESC_CONSTANT
{
    uint64_t           Const;   // Instruction constant, ie ROL reg, 1.
} ND_OPDESC_CONSTANT;


//
// Immediate operand.
//
typedef struct _ND_OPDESC_IMMEDIATE
{
    uint64_t           Imm;     // Immediate. Only Size bytes are valid. The rest are undefined.
} ND_OPDESC_IMMEDIATE;


//
// Relative offset operand.
//
typedef struct _ND_OPDESC_REL_OFFSET
{
    uint64_t           Rel;     // Relative offset (relative to the current RIP). Sign extended.
} ND_OPDESC_RELOFFSET;


//
// Describes a register operand. Count registers are used starting with Reg.
//
typedef struct _ND_OPDESC_REGISTER
{
    ND_REG_TYPE     Type;       // The register type. See enum ND_REG_TYPE.
    ND_REG_SIZE     Size;       // Indicates the register size. This may not be equal to the Size
                                // field, as a smaller amount of data may be processed from a
                                // register (especially if we have a SSE register or a mask register).
                                // Also note that as of now, 64 bytes is the maximum register size.
    uint32_t        Reg;        // The register number/ID.
    uint32_t        Count;      // The number of registers accessed, starting with Reg.

    bool            IsHigh8:1;  // TRUE if this is AH, CH, DH or BH register.
    bool            IsBlock:1;  // TRUE if this is a block register addressing.
} ND_OPDESC_REGISTER;


//
// Describes an address operand.
//
typedef struct _ND_OPDESC_ADDRESS
{
    // Size is the size of the address. Usually 4 (16 bit mode) or 6 (32 bit mode) or 10 (64 bit).
    uint16_t    BaseSeg;        // Base segment selector of the address.
    uint64_t    Offset;         // Offset inside the segment.
} ND_OPDESC_ADDRESS;


//
// Shadow stack access types.
//
typedef enum _ND_SHSTK_ACCESS
{
    ND_SHSTK_NONE = 0,
    ND_SHSTK_EXPLICIT,      // Explicit memory operand accessed as shadow stack.
    ND_SHSTK_SSP_LD_ST,     // Shadow Stack Pointer (SSP) used as base for addressing using conventional load/store.
    ND_SHSTK_SSP_PUSH_POP,  // Shadow Stack Pointer (SSP) used as base for addressing using push/pop.
    ND_SHSTK_PL0_SSP,       // Privilege 0 SSP (IA32_PL0_SSP) used (SETSSBSY).
} ND_SHSTK_ACCESS;


//
// Describes a memory operand.
//
typedef struct _ND_OPDESC_MEMORY
{
    bool            HasSeg:1;           // TRUE if segment is present & used.
    bool            HasBase:1;          // TRUE if base register is present.
    bool            HasIndex:1;         // TRUE if index & scale are present.
    bool            HasDisp:1;          // TRUE if displacement is present.
    bool            HasCompDisp:1;      // TRUE if compressed disp8 is used (EVEX instructions).
    bool            HasBroadcast:1;     // TRUE if the memory operand is a broadcast operand.

    bool            IsRipRel:1;         // TRUE if this is a rip-relative addressing. Base, Index, Scale are
                                        // all ignored.
    bool            IsStack:1;          // TRUE if this is a stack op. Note that explicit stack accesses are not
                                        // included (eg: mov eax, [rsp] will NOT set IsStack).
    bool            IsString:1;         // TRUE for [RSI] and [RDI] operands inside string operations.
    bool            IsShadowStack:1;    // TRUE if this is a shadow stack access. Check out ShStkType for more info.
    bool            IsDirect:1;         // TRUE if direct addressing (MOV [...], EAX, 0xA3).
    bool            IsBitbase:1;        // TRUE if this is a bit base. Used for BT* instructions. The bitbase
                                        // stored in the second operand must be added to the linear address.
    bool            IsAG:1;             // TRUE if the memory operand is address generation and no mem access is
                                        // made.
    bool            IsMib:1;            // TRUE if MIB addressing is used (MPX instructions).
    bool            IsVsib:1;           // TRUE if the index register selects a vector register.
    bool            IsSibMem:1;         // TRUE if the addressing uses sibmem (AMX instructions).


    ND_REG_SIZE     BaseSize;           // Base size, in bytes. Max 8 bytes.
    ND_REG_SIZE     IndexSize;          // Ditto for index size. Max 8 bytes.
    uint8_t         DispSize;           // Displacement size. Max 4 bytes.
    uint8_t         CompDispSize;       // Compressed displacement size - 1, 2, 4, 8, 16, 32, 64.

    uint8_t         ShStkType;          // Shadow stack access type. Check out ND_SHSTK_ACCESS.

    struct
    {
        uint8_t     IndexSize;          // VSIB index size.
        uint8_t     ElemSize;           // VSIB element size.
        uint8_t     ElemCount;          // Number of elements scattered/gathered/prefetched.
    } Vsib;

    uint8_t         Seg;                // Base segment used to address the memory. 0 = es, 1 = cs, etc.
    uint8_t         Base;               // Base register. Can only be a GPR.
    uint8_t         Index;              // Index register. Can be a vector reg (ZMM0-ZMM31).
    uint8_t         Scale;              // Scale: 1, 2, 4 or 8. Always present if Index is present.

    uint64_t        Disp;               // Sign extended displacement.

} ND_OPDESC_MEMORY;


//
// Describes a decorator.
//
typedef struct _ND_OPERAND_DECORATOR
{
    bool                HasMask:1;      // TRUE if mask is present, 0 otherwise.
    bool                HasZero:1;      // TRUE if zeroing will be made, 0 if merging will be made.
    bool                HasBroadcast:1; // TRUE if broadcasting is being made. Valid only for memory operands.

    // These are used only to indicate where the SAE and ER decorators should be placed in the disassembly.
    // Otherwise, SAE and ER are global, per instruction, and don't apply to a single operand.
    bool                HasSae:1;       // TRUE if SAE is present.
    bool                HasEr:1;        // TRUE if ER is present. 

    // Mask register specifier.
    struct
    {
        uint8_t         Msk;            // Mask register used. Only k0-k7 can be encoded.
    } Mask;

    // Broadcast specifier
    struct
    {
        uint8_t         Count;          // Number of times to broadcast the element.
        uint8_t         Size;           // Size of one element.
    } Broadcast;

} ND_OPERAND_DECORATOR;


//
// Extended operand information.
//
typedef struct _ND_OPERAND
{
    ND_OPERAND_TYPE         Type;       // Operand type. One of ND_OPERAND_TYPE enumerations.
    ND_OPERAND_ENCODING     Encoding;   // Where is the operand encoded - modrm.reg, modrm.rm, etc.
    ND_OPERAND_SIZE         Size;       // Operand size in bytes. This should be used when operating with
                                        // the operand. It includes sign-extension or zero-extensions.
                                        // Note that the Size field indicates the actual amount of data
                                        // used for processing. If the operand type is a register, it MAY NOT
                                        // indicate the register size. Use the Info.Register.Size
                                        // field to get the actual register size.
    ND_OPERAND_SIZE         RawSize;    // Raw size inside the instruction. This will usually be identical
                                        // to Size; however, some instructions force the actual size of their
                                        // operands to 64 bit; PUSH/POP or branches are good examples; although
                                        // the raw size of the relative offset or the immediate will be RawSize,
                                        // internally, the CPU will use Size (usually sign-extended).
    ND_OPERAND_ACCESS       Access;     // Access mode (read, write, read-write, etc.)
    ND_OPERAND_FLAGS        Flags;      // Misc operand flags.

    // Depending in the Type field, one of these subfields contains information about the operand.
    union
    {
        ND_OPDESC_CONSTANT  Constant;   // Constant, if operand type is ND_OP_CONST.
        ND_OPDESC_IMMEDIATE Immediate;  // Immediate, if operand type is ND_OP_IMM.
        ND_OPDESC_REGISTER  Register;   // Register, if operand type if ND_OP_REG.
        ND_OPDESC_RELOFFSET RelativeOffset; // Relative offset, if operand type is ND_OP_REL_OFFS.
        ND_OPDESC_ADDRESS   Address;    // Address, if operand type is ND_OP_ADDR.
        ND_OPDESC_MEMORY    Memory;     // Memory, if operand type is ND_OP_MEM.
    } Info;

    ND_OPERAND_DECORATOR    Decorator;  // Decorator information.

} ND_OPERAND, *PND_OPERAND;



//
// REX prefix.
//
typedef union _ND_REX
{
    uint8_t         Rex;
    struct
    {
        uint8_t     b : 1;      // b (rm or low opcode) extension field.
        uint8_t     x : 1;      // x (index) extension field.
        uint8_t     r : 1;      // r (reg) extension field.
        uint8_t     w : 1;      // w (size) extension field. Promotes to 64 bit.
    };
} ND_REX;


//
// Mod R/M byte.
//
typedef union _ND_MODRM
{
    uint8_t         ModRm;
    struct
    {
        uint8_t     rm : 3;     // rm field.
        uint8_t     reg : 3;    // reg field.
        uint8_t     mod : 2;    // mod field. Indicates memory access (0, 1 or 2), or register access (3).
    };
} ND_MODRM;


//
// SIB byte.
//
typedef union _ND_SIB
{
    uint8_t         Sib;
    struct
    {
        uint8_t     base : 3;   // Base register.
        uint8_t     index : 3;  // Index register.
        uint8_t     scale : 2;  // Scale.
    };
} ND_SIB;


//
// DREX byte. Exactly as AMD defined it. No actual instructions exist that use DREX encoding.
//
typedef union _ND_DREX
{
    uint8_t         Drex;
    struct
    {
        uint8_t     b : 1;
        uint8_t     x : 1;
        uint8_t     r : 1;
        uint8_t     oc0 : 1;
        uint8_t     vd : 3;
        uint8_t     d : 1;
    };
} ND_DREX;


//
// 2-bytes VEX. Exactly as Intel defined it.
//
typedef union _ND_VEX2
{
    uint8_t        Vex[2];
    struct
    {
        uint8_t    op;          // 0xC5

        uint8_t    p : 2;       // p0, p1
        uint8_t    l : 1;       // L
        uint8_t    v : 4;       // ~v0, ~v1, ~v2, ~v3
        uint8_t    r : 1;       // ~R
    };
} ND_VEX2;


//
// 3-bytes VEX. Exactly as Intel defined it.
//
typedef union _ND_VEX3
{
    uint8_t         Vex[3];
    struct
    {
        uint8_t     op;         // 0xC4

        uint8_t     m : 5;      // m0, m1, m2, m3, m4
        uint8_t     b : 1;      // ~B
        uint8_t     x : 1;      // ~X
        uint8_t     r : 1;      // ~R

        uint8_t     p : 2;      // p0, p1
        uint8_t     l : 1;      // L
        uint8_t     v : 4;      // ~v0, ~v1, ~v2, ~v3
        uint8_t     w : 1;      // W
    };
} ND_VEX3;


//
// XOP. Exactly as AMD defined it.
//
typedef union _ND_XOP
{
    uint8_t         Xop[3];
    struct
    {
        uint8_t     op;         // 0x8F

        uint8_t     m : 5;      // m0, m1, m2, m3, m4
        uint8_t     b : 1;      // ~B
        uint8_t     x : 1;      // ~X
        uint8_t     r : 1;      // ~R

        uint8_t     p : 2;      // p0, p1
        uint8_t     l : 1;      // L
        uint8_t     v : 4;      // ~v0, ~v1, ~v2, ~v3
        uint8_t     w : 1;      // W
    };
} ND_XOP;


//
// EVEX prefix. Exactly as Intel defined it.
//
typedef union _ND_EVEX
{
    uint8_t         Evex[4];
    struct
    {
        uint8_t     op;         // 0x62

        uint8_t     m : 2;      // m0, m1
        uint8_t     zero : 2;   // 00
        uint8_t     rp : 1;     // ~R'
        uint8_t     b : 1;      // ~B
        uint8_t     x : 1;      // ~X
        uint8_t     r : 1;      // ~R

        uint8_t     p : 2;      // p0, p1
        uint8_t     one : 1;    // 1
        uint8_t     v : 4;      // ~v0, ~v1, ~v2, ~v3
        uint8_t     w : 1;      // W

        uint8_t     a : 3;      // a0, a1, a2
        uint8_t     vp : 1;     // ~V'
        uint8_t     bm : 1;     // b
        uint8_t     l : 2;      // L'L
        uint8_t     z : 1;      // z
    };
} ND_EVEX;


//
// Describes the CPUID leaf, sub-leaf, register & bit that indicate whether an instruction is supported or not.
// If Leaf == ND_CFF_NO_LEAF, the instruction is supported on any CPU, and no CPUID flag exists.
// If SubLeaf == ND_CFF_NO_SUBLEAF, there is no subleaf to check.
//
typedef union _ND_CPUID_FLAG
{
    uint64_t       Flag;
    struct
    {
        uint32_t   Leaf;          // CPUID leaf. ND_CFF_NO_LEAF if not applicable.
        uint32_t   SubLeaf : 24;  // CPUID sub-leaf. ND_CFF_NO_SUBLEAF if not applicable.
        uint32_t   Reg : 3;       // The register that contains info regarding the instruction.
        uint32_t   Bit : 5;       // Bit inside the register that indicates whether the instruction is present.
    };
} ND_CPUID_FLAG;


//
// Each instruction may accept one or more prefixes. This structure indicates which prefixes are valid for the 
// given instruction.
//
typedef union _ND_VALID_PREFIXES
{
    uint16_t     Raw;
    struct
    {
        uint16_t Rep : 1;        // The instruction supports REP prefix.
        uint16_t RepCond : 1;    // The instruction supports REPZ/REPNZ prefixes.
        uint16_t Lock : 1;       // The instruction supports LOCK prefix.
        uint16_t Hle : 1;        // The instruction supports XACQUIRE/XRELEASE prefixes.
        uint16_t Xacquire : 1;   // The instruction supports only XACQUIRE.
        uint16_t Xrelease : 1;   // The instruction supports only XRELEASE.
        uint16_t Bnd : 1;        // The instruction supports BND prefix.
        uint16_t Bhint : 1;      // The instruction supports branch hints.
        uint16_t HleNoLock : 1;  // HLE prefix is accepted without LOCK.
        uint16_t Dnt : 1;        // The instruction supports the DNT (Do Not Track) CET prefix.
    };
} ND_VALID_PREFIXES, *PND_VALID_PREFIXES;


//
// Each instruction may accept several decorators. This instruction indicates which decorators are valid for the 
// given instruction.
//
typedef union _ND_VALID_DECORATORS
{
    uint8_t     Raw;
    struct
    {
        uint8_t Er : 1;         // The instruction supports embedded rounding mode.
        uint8_t Sae : 1;        // The instruction supports suppress all exceptions mode.
        uint8_t Zero : 1;       // The instruction supports zeroing.
        uint8_t Mask : 1;       // The instruction supports mask registers.
        uint8_t Broadcast : 1;  // The instruction supports broadcast.
    };
} ND_VALID_DECORATORS, *PND_VALID_DECORATORS;


//
// Each instruction is valid or invalid in any certain mode. This indicates which modes the instruction is valid in.
// If the bit is set, the isntruction is valid in that given mode.
//
typedef union _ND_VALID_MODES
{
    uint32_t     Raw;
    struct
    {
        // Group 1: privilege level.
        uint32_t Ring0 : 1;     // The instruction is valid in ring 0.
        uint32_t Ring1 : 1;     // The instruction is valid in ring 1.
        uint32_t Ring2 : 1;     // The instruction is valid in ring 2.
        uint32_t Ring3 : 1;     // The instruction is valid in ring 3.

        // Group 2: operating mode - the CPU can be on only one of these modes at any moment.
        uint32_t Real : 1;      // The instruction is valid in real mode.
        uint32_t V8086 : 1;     // The instruction is valid in Virtual 8086 mode.
        uint32_t Protected : 1; // The instruction is valid in protected mode (32 bit).
        uint32_t Compat : 1;    // The instruction is valid in compatibility mode (32 bit in 64 bit).
        uint32_t Long : 1;      // The instruction is valid in long mode.

        uint32_t Reserved : 3;  // Reserved for padding/future use.

        // Group 3: special modes - these may be active inside other modes (example: TSX in Long mode).
        uint32_t Smm : 1;       // The instruction is valid in System Management Mode.
        uint32_t SmmOff : 1;    // The instruction is valid outside SMM.
        uint32_t Sgx : 1;       // The instruction is valid in SGX mode.
        uint32_t SgxOff : 1;    // The instruction is valid outside SGX.
        uint32_t Tsx : 1;       // The instruction is valid in transactional regions.
        uint32_t TsxOff : 1;    // The instruction is valid outside TSX.

        // Group 4: VMX mode - they engulf all the other modes.
        uint32_t VmxRoot : 1;   // The instruction is valid in VMX root mode.
        uint32_t VmxNonRoot : 1;// The instruction is valid in VMX non root mode.
        uint32_t VmxRootSeam : 1;   // The instruction is valid in VMX root SEAM.
        uint32_t VmxNonRootSeam : 1;// The instruction is valid in VMX non-root SEAM.
        uint32_t VmxOff : 1;    // The instruction is valid outside VMX operation.
        
    };
} ND_VALID_MODES, *PND_VALID_MODES;


//
// RFLAGS register. This structure reflects the actual position of each flag insdide the RFLAGS register, so it can
// be used for direct processing.
//
typedef union _ND_RFLAGS
{
    uint32_t        Raw;
    struct
    {
        uint32_t    CF : 1;         // Carry flag.
        uint32_t    Reserved1 : 1;  // Reserved, must be 1.
        uint32_t    PF : 1;         // Parity flag.
        uint32_t    Reserved2 : 1;  // Reserved.
        uint32_t    AF : 1;         // Auxiliary flag.
        uint32_t    Reserved3 : 1;  // Reserved.
        uint32_t    ZF : 1;         // Zero flag.
        uint32_t    SF : 1;         // Sign flag.
        uint32_t    TF : 1;         // Trap flag.
        uint32_t    IF : 1;         // Interrupt flag.
        uint32_t    DF : 1;         // Direction flag.
        uint32_t    OF : 1;         // Overflow flag.
        uint32_t    IOPL : 2;       // I/O privilege level flag.
        uint32_t    NT : 1;         // Nested task flag.
        uint32_t    Reserved4 : 1;  // Reserved.
        uint32_t    RF : 1;         // Resume flag.
        uint32_t    VM : 1;         // Virtual mode flag.
        uint32_t    AC : 1;         // Alignment check flag.
        uint32_t    VIF : 1;        // Virtual interrupts flag.
        uint32_t    VIP : 1;        // Virtual interrupt pending flag.
        uint32_t    ID : 1;         // CPUID identification flag.
    };
} ND_RFLAGS, *PND_RFLAGS;


#define ND_FPU_FLAG_SET_0           0   // The FPU status flag is cleared to 0.
#define ND_FPU_FLAG_SET_1           1   // The FPU status flag is set to 1.
#define ND_FPU_FLAG_MODIFIED        2   // The FPU status flag is modified according to a result.
#define ND_FPU_FLAG_UNDEFINED       3   // The FPU status flag is undefined or unaffected.

//
// FPU status flags. Each status flag can be one of ND_FPU_FLAG*.
//
typedef struct _ND_FPU_FLAGS
{
    uint8_t         C0 : 2;     // C0 flag access mode. See ND_FPU_FLAG_*.
    uint8_t         C1 : 2;     // C1 flag access mode. See ND_FPU_FLAG_*.
    uint8_t         C2 : 2;     // C2 flag access mode. See ND_FPU_FLAG_*.
    uint8_t         C3 : 2;     // C3 flag access mode. See ND_FPU_FLAG_*.
} ND_FPU_FLAGS, *PND_FPU_FLAGS;


//
// Describes a decoded instruction. All the possible information about the instruction is contained in this structure.
// You don't have to call any other APIs to gather any more info about it.
//
typedef struct _INSTRUX
{
    uint8_t             DefCode:4;                  // ND_CODE_*. Indicates disassembly mode.
    uint8_t             DefData:4;                  // ND_DATA_*. Indicates default data size.
    uint8_t             DefStack:4;                 // ND_STACK_*. Indicates default stack pointer width.
    uint8_t             VendMode:4;                 // ND_VEND_*. Indicates vendor mode.
    uint8_t             FeatMode;                   // ND_FEAT_*. Indicates which features are enabled.
    uint8_t             EncMode:4;                  // ND_ENCM_*. Indicates encoding mode.
    uint8_t             VexMode:4;                  // ND_VEX_*.  Indicates the VEX mode, if any.
    uint8_t             AddrMode:4;                 // ND_ADDR_*. Indicates addressing mode.
    uint8_t             OpMode:4;                   // ND_OPSZ_*. Indicates operand mode/size.
    uint8_t             EfOpMode:4;                 // ND_OPSZ_*. Indicates effective operand mode/size.
    uint8_t             VecMode:4;                  // ND_VECM_*. Indicates vector length.
    uint8_t             EfVecMode:4;                // ND_VECM_*. Indicates effective vector length.
    
    // Prefixes.
    bool                HasRex:1;                   // TRUE - REX is present.
    bool                HasVex:1;                   // TRUE - VEX is present.
    bool                HasXop:1;                   // TRUE - XOP is present.
    bool                HasEvex:1;                  // TRUE - EVEX is present.
    bool                HasMvex:1;                  // TRUE - MVEX is present.
    bool                HasOpSize:1;                // TRUE - 0x66 present.
    bool                HasAddrSize:1;              // TRUE - 0x67 present.
    bool                HasLock:1;                  // TRUE - 0xF0 present.
    bool                HasRepnzXacquireBnd:1;      // TRUE - 0xF2 present.
    bool                HasRepRepzXrelease:1;       // TRUE - 0xF3 present.
    bool                HasSeg:1;                   // TRUE - segment override present.

    // Indicators for prefix activation.
    bool                IsRepeated:1;               // TRUE - the instruction is REPed up to RCX times.
    bool                IsXacquireEnabled:1;        // TRUE - the instruction is XACQUIRE enabled.
    bool                IsXreleaseEnabled:1;        // TRUE - the instruction is XRELEASE enabled.
    bool                IsRipRelative:1;            // TRUE - the instruction uses RIP relative addressing.
    bool                IsCetTracked:1;             // TRUE - this is an indirect CALL/JMP that is CET tracked.

    // Instruction chunks.
    bool                HasModRm:1;                 // TRUE - we have valid MODRM.
    bool                HasSib:1;                   // TRUE - we have valid SIB.
    bool                HasDrex:1;                  // TRUE - we have valid DREX.
    bool                HasDisp:1;                  // TRUE - the instruction has displacement.
    bool                HasAddr:1;                  // TRUE - the instruction contains a direct address (ie, CALL far 0x9A)
    bool                HasMoffset:1;               // TRUE - the instruction contains a moffset (ie, MOV al, [mem], 0xA0)
    bool                HasImm1:1;                  // TRUE - immediate present.
    bool                HasImm2:1;                  // TRUE - second immediate present.
    bool                HasImm3:1;                  // TRUE - third immediate present.
    bool                HasRelOffs:1;               // TRUE - the instruction contains a relative offset (ie, Jcc 0x7x).
    bool                HasSseImm:1;                // TRUE - SSE immediate that encodes additional registers is present.
    bool                HasCompDisp:1;              // TRUE - the instruction uses compressed displacement
    bool                HasBroadcast:1;             // TRUE - the instruction uses broadcast addressing
    bool                HasMask:1;                  // TRUE - the instruction has mask.
    bool                HasZero:1;                  // TRUE - the instruction uses zeroing.
    bool                HasEr:1;                    // TRUE - the instruction has embedded rounding.
    bool                HasSae:1;                   // TRUE - the instruction has SAE.

    bool                SignDisp:1;                 // Displacement sign. 0 is positive, 1 is negative.

    // Encoding specifics.
    bool                HasMandatory66:1;           // 0x66 is mandatory prefix. Does not behave as size-changing prefix.
    bool                HasMandatoryF2:1;           // 0x66 is mandatory prefix. Does not behave as REP prefix.
    bool                HasMandatoryF3:1;           // 0x66 is mandatory prefix. Does not behave as REP prefix.

    // Instruction components lengths. Will be 0 if the given field is not present.
    uint8_t             Length;                     // 1-15 valid. Instructions longer than 15 bytes will cause #GP.

    uint8_t             WordLength:4;               // The length of the instruction word. 2, 4 or 8.
    uint8_t             PrefLength:4;               // The total number of bytes consumed by prefixes. This will also be 
                                                    // the offset to the first opcode. The primary opcode will always be 
                                                    // the last one, so at offset PrefixesLength + OpcodeLength - 1
    uint8_t             OpLength:4;                 // Number of opcode bytes. Max 3.
    uint8_t             DispLength:4;               // Displacement length, in bytes. Maximum 4.
    uint8_t             AddrLength:4;               // Absolute address length, in bytes. Maximum 8 bytes.
    uint8_t             MoffsetLength:4;            // Memory offset length, in bytes. Maximum 8 bytes.
    uint8_t             Imm1Length:4;               // First immediate length, in bytes. Maximum 8 bytes.
    uint8_t             Imm2Length:4;               // Second immediate length, in bytes. Maximum 8 bytes.
    uint8_t             Imm3Length:4;               // Third immediate length, in bytes. Maximum 8 bytes.
    uint8_t             RelOffsLength:4;            // Relative offset length, in bytes. Maximum 4 bytes.

    // Instruction components offsets. Will be 0 if the given field is not present. Prefixes ALWAYS start at offset 0.
    uint8_t             OpOffset:4;                 // The offset of the first opcode, inside the instruction.
    uint8_t             MainOpOffset:4;             // The offset of the nominal opcode, inside the instruction.
    uint8_t             DispOffset:4;               // The offset of the displacement, inside the instruction
    uint8_t             AddrOffset:4;               // The offset of the hard-coded address.
    uint8_t             MoffsetOffset:4;            // The offset of the absolute address, inside the instruction
    uint8_t             Imm1Offset:4;               // The offset of the immediate, inside the instruction
    uint8_t             Imm2Offset:4;               // The offset of the second immediate, if any, inside the instruction
    uint8_t             Imm3Offset:4;               // The offset of the third immediate, if any, inside the instruction
    uint8_t             RelOffsOffset:4;            // The offset of the relative offset used in instruction.
    uint8_t             SseImmOffset:4;             // The offset of the SSE immediate, if any, inside the instruction.
    uint8_t             ModRmOffset:4;              // The offset of the mod rm byte inside the instruction, if any.
                                                    // If SIB is also present, it will always be at ModRmOffset + 1.

    uint8_t             StackWords;                 // Number of words accessed on/from the stack.

    uint8_t             Rep;                        // The last rep/repz/repnz prefix. 0 if none.
    uint8_t             Seg;                        // The last segment override prefix. 0 if none. FS/GS if 64 bit.
    uint8_t             Bhint;                      // The last segment override indicating a branch hint.
    ND_REX              Rex;                        // REX prefix.
    ND_MODRM            ModRm;                      // ModRM byte.
    ND_SIB              Sib;                        // SIB byte.
    ND_DREX             Drex;                       // DREX byte. Part of AMD SSE 5. Never shipped.

    union
    {
        ND_VEX2         Vex2;                       // VEX 2 prefix.
        ND_VEX3         Vex3;                       // VEX 3 prefix.
        ND_XOP          Xop;                        // XOP prefix.
        ND_EVEX         Evex;                       // EVEX prefix.
    };

    // This structures contains the fields extracted from either REX, XOP, VEX, EVEX or MVEX fields. They're globally
    // placed here, in order to avoid testing for each kind of prefix each time. Instead, one can use the different
    // fields directly from here, regardless the actual encoding mode.
    struct
    {
        uint32_t        w:1;                        // REX/XOP/VEX/EVEX/MVEX.W field
        uint32_t        r:1;                        // REX/XOP/VEX/EVEX/MVEX.R field (reg extension)
        uint32_t        x:1;                        // REX/XOP/VEX/EVEX/MVEX.X field (index extension)
        uint32_t        b:1;                        // REX/XOP/VEX/EVEX/MVEX.B field (base extension)
        uint32_t        rp:1;                       // EVEX/MVEX.R' (reg extension)
        uint32_t        p:2;                        // XOP/VEX/EVEX/MVEX.pp (embedded prefix)
        uint32_t        m:5;                        // XOP/VEX/EVEX/MVEX.mmmmm (decoding table)
        uint32_t        l:2;                        // XOP/VEX.L or EVEX.L'L (vector length)
        uint32_t        v:4;                        // XOP/VEX.VVVV or EVEX/MVEX.VVVV (additional operand)
        uint32_t        vp:1;                       // EVEX/MVEX.V' (vvvv extension)
        uint32_t        bm:1;                       // EVEX.b (broadcast)
        uint32_t        e:1;                        // MVEX.e (eviction hint)
        uint32_t        z:1;                        // EVEX.z (zero)
        uint32_t        k:3;                        // EVEX.aaa/MVEX.kkk (mask registers)
        uint32_t        s:3;                        // MVEX.sss (swizzle)
    } Exs;

    union
    {
        struct
        {
            uint32_t    Ip;
            uint16_t    Cs;
        };
    } Address;                                      // seg:offset address.

    uint64_t            Moffset;                    // Offset. Used by 'O' operands. It's an absolute address.
    uint32_t            Displacement;               // Displacement. Max 4 bytes. Used in ModRM instructions.
    uint32_t            RelativeOffset;             // Relative offset, used for branches. Max 4 bytes.
    uint64_t            Immediate1;                 // Can be 8 bytes on x64
    uint8_t             Immediate2;                 // For enter, mainly. Can only be 1 byte.
    uint8_t             Immediate3;                 // Third additional immediate. Limited to 1 byte for now.
    uint8_t             SseImmediate;               // This immediate actually selects a register.
    uint8_t             SseCondition;               // Condition code encoded in additional byte.
    uint8_t             Condition;                  // Condition code encoded in low 4 bit of the opcode.
    uint8_t             Predicate;                  // Same as Condition, kept for backwards compatibility.

    uint8_t             OperandsCount;              // Number of operands.
    uint8_t             ExpOperandsCount;           // Number of explicit operands. Use this if you want to ignore
                                                    // implicit operands such as stack, flags, etc.
    uint16_t            OperandsEncodingMap;        // What parts of the instruction encode operands.
    ND_OPERAND          Operands[ND_MAX_OPERAND];   // Instruction operands.

    // As extracted from the operands themselves.
    uint8_t             RipAccess;                  // RIP access mode (read/write).
    uint8_t             StackAccess;                // Stack access mode (push/pop).
    uint8_t             MemoryAccess;               // Memory access mode (read/write, including stack or shadow stack).

    struct
    {
        uint8_t         RegAccess;                  // RFLAGS access mode (read/write), as per the entire register.
        ND_RFLAGS       Tested;                     // Tested flags.
        ND_RFLAGS       Modified;                   // Modified (according to the result) flags.
        ND_RFLAGS       Set;                        // Flags that are always set to 1.
        ND_RFLAGS       Cleared;                    // Flags that are always cleared to 0.
        ND_RFLAGS       Undefined;                  // Undefined flags.
    } FlagsAccess;

    ND_FPU_FLAGS        FpuFlagsAccess;             // FPU status word C0-C3 bits access. Valid only for FPU instructions!

    uint8_t             ExceptionClass;             // ND_EX_CLASS_TYPE, indicates the exception class type.
    uint8_t             ExceptionType;              // Exception type. Depends on ExceptionClass.
    uint8_t             TupleType;                  // EVEX tuple type, if EVEX. Check out ND_TUPLE.
    uint8_t             RoundingMode;               // EVEX rounding mode, if present. Check out ND_ROUNDING.

    // Stored inside the instruction entry as well. These are specific for an instruction and do not depend on
    // encoding. Use the flags definitions (ND_FLAG_*, ND_PREF_*, ND_DECO_*, ND_EXOP_*) to access specific bits.
    uint32_t            Attributes;                 // Instruction attributes/flags. A collection of ND_FLAG_*.
    union
    {
        ND_INS_CLASS    Instruction;                // One of the ND_INS_*
        ND_INS_CLASS    Iclass;                     // Same as Instruction.
    };
    ND_INS_CATEGORY     Category;                   // One of the ND_CAT_*
    ND_INS_SET          IsaSet;                     // One of the ND_SET_*
    ND_CPUID_FLAG       CpuidFlag;                  // CPUID support flag.
    ND_VALID_MODES      ValidModes;                 // Valid CPU modes for the instruction.
    ND_VALID_PREFIXES   ValidPrefixes;              // Indicates which prefixes are valid for this instruction.
    ND_VALID_DECORATORS ValidDecorators;            // What decorators are accepted by the instruction.
    uint8_t             Reserved1[3];               // Padding purpose. Aligns the mnemonic to 8 bytes.
    char                Mnemonic[ND_MAX_MNEMONIC_LENGTH];   // Instruction mnemonic.
    uint8_t             OpCodeBytes[3];             // Opcode bytes - escape codes and main op code
    uint8_t             PrimaryOpCode;              // Main/nominal opcode
    uint32_t            Reserved2;                  // Padding purpose. Aligns the InstructionBytes to 16 bytes.
    uint8_t             InstructionBytes[16];       // The entire instruction.

} INSTRUX, *PINSTRUX;


//
// Decoder context. Such a structure must be passed to the NdDecodeWithContext API. This structure must be initialized
// only once, and then it can be re-used across NdDecodeWithContext calls.
//
typedef struct _ND_CONTEXT
{
    uint64_t DefCode : 4;       // Decode mode - one of the ND_CODE_* values.
    uint64_t DefData : 4;       // Data mode - one of the ND_DATA_* values.
    uint64_t DefStack : 4;      // Stack mode - one of the ND_STACK_* values.
    uint64_t VendMode : 4;      // Prefered vendor - one of the ND_VEND_* values.
    uint64_t FeatMode : 8;      // Supported features mask. A combination of ND_FEAT_* values.
    uint64_t Reserved : 40;     // Reserved for future use.
} ND_CONTEXT;


//
// Returns the bddisasm version.
//
void
NdGetVersion(
    uint32_t *Major,
    uint32_t *Minor,
    uint32_t *Revision,
    char **BuildDate,
    char **BuildTime
    );

//
// Decode one instruction. Note that this is equivalent to: 
// NdDecodeEx(Instrux, Code, ND_MAX_INSTRUCTION_LEN, DefCode, DefData).
// This version should be used if the caller doesn't care about the length of the buffer. Otherwise, use the other
// decode API.
// 
NDSTATUS
NdDecode(
    INSTRUX *Instrux,       // Output decoded instruction.
    const uint8_t *Code,    // Buffer containing the instruction bytes.
    uint8_t DefCode,        // Decode mode - one of the ND_CODE_* values.
    uint8_t DefData         // Data mode - one of the ND_DATA_* value.
    );

//
// Decode one instruction. Note that this is equivalent to: 
// NdDecodeEx2(Instrux, Code, Size, DefCode, DefData, DefCode, ND_VEND_ANY).
// By default, the used vendor will be ND_VEND_ANY, so all instructions will be decoded.
// By default, the feature mode will be ND_FEAT_ALL, so all instructions will be decoded (but may yield error where
// otherwise a NOP would be encoded - use ND_FEAT_NONE in that case).
// 
NDSTATUS
NdDecodeEx(
    INSTRUX *Instrux,       // Output decoded instruction.
    const uint8_t *Code,    // Buffer containing the instruction bytes.
    size_t Size,            // Maximum size of the Code buffer.
    uint8_t DefCode,        // Decode mode - one of the ND_CODE_* values.
    uint8_t DefData         // Data mode - one of the ND_DATA_* value.
    );

//
// Fills a ND_CONTEXT structure, and calls NdDecodeWithContext. The feature mode will be ND_FEAT_ALL by default.
//
NDSTATUS
NdDecodeEx2(
    INSTRUX *Instrux,       // Output decoded instruction.
    const uint8_t *Code,    // Buffer containing the instruction bytes.
    size_t Size,            // Maximum size of the Code buffer.
    uint8_t DefCode,        // Decode mode - one of the ND_CODE_* values.
    uint8_t DefData,        // Data mode - one of the ND_DATA_* value.
    uint8_t DefStack,       // Stack mode - one of the ND_STACK_* values.
    uint8_t PreferedVendor  // Preferred vendor - one of the ND_VEND_* values.
    );

//
// This API received a decode context, where it expects DefCode, DefData, DefStack, VendMode and FeatMode to be 
// already initialized. The Context will not be modified by the decoder, so it can be reused across decode calls.
// The Context should initially be initialized using NdInitContext. This will ensure backwards compatibility
// by setting new fields to default values.
// Note that this is the base decoding API, and this ends up being called by all the other decoding APIs, after 
// providing default arguments and filling them in the Context structure. For maximum speed, use this instead of
// the others.
//
NDSTATUS
NdDecodeWithContext(
    INSTRUX *Instrux,       // Output decoded instruction.
    const uint8_t *Code,    // Buffer containing the instruction bytes.
    size_t Size,            // Maximum size of the Code buffer.
    ND_CONTEXT *Context     // Context describing decode mode, vendor mode and supported features.
    );

//
// Convert the given instruction into textual representation (Intel syntax).
//
NDSTATUS
NdToText(
    const INSTRUX *Instrux,
    uint64_t Rip,
    uint32_t BufferSize,
    char *Buffer
    );

//
// Returns true if the instruction is RIP relative. Note that this function is kept for backwards compatibility, since
// there already is a IsRipRelative field inside INSTRUX.
//
bool
NdIsInstruxRipRelative(
    const INSTRUX *Instrux
    );

//
// Returns an access map that contains the access for each register.
//
NDSTATUS
NdGetFullAccessMap(
    const INSTRUX *Instrux,
    ND_ACCESS_MAP *AccessMap
    );

//
// Initialize the decoder context.
//
void
NdInitContext(
    ND_CONTEXT *Context
    );

// #pragma warning(default: 4214) // Bitfield in type other than int.
// #pragma warning(default: 4201) // Nonstandard extension used: nameless struct/union.
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // BDDISASM_H
