#!/usr/bin/env python3
#
# Copyright (c) 2024 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import os
import sys
import re
import glob


# Instruction attributes. These values go with the 'a' metadata key.
valid_attributes = {
    # Misc instruction properties.
    'MODRM',        # Mod r/m is present.
    '3DNOW',        # Instruction uses 3dnow encoding.
    'COND',         # Instruction has condition encoded in lower 4 bit of the opcode.
    'LOCKSP',       # Special lock - MOV CR on amd can use LOCK to access CR8 in 32 bit mode.
    'VECTOR',       # Vector instruction.
    'S66',          # 0x66 prefix changes length even if it is in special map (66, f2, f3).
    'SHS',          # Instruction accesses the shadow stack.
    'MFR',          # The Mod inside Mod R/M is forced to register. No SIB/disp present.
    'CETT',         # Instruction is CET tracked.
    'SERIAL',       # Instruction is serializing.
    'SCALABLE',     # EVEX.pp prefix can be 0 or 1, simulating the presence of 0x66 prefix.

    # 64-bit modifiers.
    'I64',          # Instruction invalid in 64 bit mode.
    'O64',          # Instruction valid only in 64 bit mode.
    'F64',          # Operand size forced to 64 bit.
    'D64',          # Operand size defaults to 64 bit.

    # Per operand indicators.
    'OP1DEF',       # Operand 1 is default (implicit).
    'OP2DEF',       # Operand 2 is default (implicit).
    'OP2SIGNEXO1',  # Operand 2 is sign-extended to the size of the first operand.
    'OP3SIGNEXO1',  # Operand 3 is sign-extended to the size of the first operand.
    'OP1SIGNEXDW',  # Operand 1 is sign-extended to the size of the default word.

    # Addressing flags.
    'AG',           # Instruction uses address generation, no memory access.
    'BITBASE',      # Instruction uses bitbase addressing.
    'VSIB',         # Instruction uses VSIB addressing.
    'MIB',          # Instruction uses MIB addressing.
    'SIBMEM',       # Instruction uses sibmem addressing (AMX instructions).

    # Ignored fields.
    'LIG',          # *vex.L is ignored.
    'WIG',          # *vex.W is ignored.
    'I67',          # Ignore the address size override (0x67) prefix in 64 bit mode.
    'IER',          # Ignore embedded rounding for the instruction.
    'IWO64',        # The VEX/EVEX.W field is ignored outside 64 bit mode, and behaves as if it's 0.

    # Restrictions - these fields being present leads to #UD.
    'MMASK',        # Instruction must have mask specified (mask cannot be k0).
    'NOMZ',         # Zeroing not allowed with memory addressing.
    'NOL0',         # Vector length 128 not supported.
    'NOA16',        # 16 bit addressing not supported.
    'NORIPREL',     # RIP relative addressing not supported.
    'NO66',         # 0x66 prefix causes #UD.
    'NO67',         # 0x67 prefix causes #UD.
    'NOREP',        # REP prefixes will cause #UD.
    'NOREX2',       # REX2 prefix will cause #UD.
    'NOV',          # XOP/VEX/EVEX.v will cause #UD if not logical 0.
    'NOVP',         # EVEX.v' will cause #UD if not logical 0.
}

# Explicit operands types.
valid_optypes = [
    'A',            # Direct addressing. Used by direct branches.
    'B',            # The vvvv field inside VEX/EVEX encodes a general purpose registr.
    'C',            # The reg  field inside Mod R/M encodes a control register.
    'D',            # The reg  field inside Mod R/M encodes a debug register.
    'E',            # The rm   field inside Mod R/M encodes a general purpose register or memory.
    'F',            # Implicit flags register.
    'G',            # The reg  field inside Mod R/M encodes a general purpose register.
    'H',            # The vvvv field inside VEX/EVEX encodes a SIMD register.
    'I',            # Immediate encoded in instruction bytes.
    'J',            # Relative offset encoded in instruction bytes.
    'K',            # The operand is the stack.
    'L',            # The upper 4-bit of an immediate encode a SIMD register.
    'M',            # The rm   field inside Mod R/M encodes memory.
    'N',            # The rm   field inside Mod R/M encodes a MMX register.
    'O',            # Moffset addressing.
    'P',            # The reg  field inside Mod R/M encodes a MMX register.
    'Q',            # The rm   field inside Mod R/M encodes a MMX register or memory.
    'R',            # The rm   field inside Mod R/M encodes a general purpose register.
    'S',            # The reg  field inside Mod R/M emcodes a segment register.
    'T',            # The reg  field inside Mod R/M encodes a test register.
    'U',            # The rm   field inside Mod R/M encodes a SIMD register.
    'V',            # The reg  field inside Mod R/M encodes a SIMD register.
    'W',            # The rm   field inside Mod R/M enocdes a SIMD register or memory.
    'X',            # DS:rSI addressing.
    'Y',            # ES:rDI addressing.
    'Z',            # The low 3 bits inside the opcode encode a general purpose register. 
    'rB',           # The reg  field inside Mod R/M enocdes a bound register.
    'mB',           # The rm   field inside Mod R/M enocdes a bound register or memory.
    'rK',           # The reg  field inside Mod R/M enocdes a mask register.
    'vK',           # The vvvv field inside VEX/EVEX encodes a mask register.
    'mK',           # The rm   field inside Mod R/M encodes a mask register.
    'aK',           # The aaa  field inside EVEX encodes a mask register.
    'rM',           # The reg  field inside Mod R/M encodes the base address of a memory operand. 
                    # Default segment is ES.
    'mM',           # The rm   field inside Mod R/M encodes the base address of a memory operand.
                    # Default segment is DS.
    'rT',           # The reg  field inside Mod R/M encodes a tile register (AMX extension).
    'mT',           # The rm   field inside Mod R/M encodes a tile register (AMX extension).
    'vT',           # The vvvv field inside VEX/EVEX encodes a tile register (AMX extension).
    'm2zI',         # Bits [1,0] of the immediate byte which encodes the fourth register.
    'dfv',          # The EVEX.VVVV encodes a default flags value (OF,SF,ZF,CF).
]


# Operand sizes.
# Unless otherwise stated, where multiple sizes are given, the correct size is selected by the
# operand size or vector length as follows: 
# - the first size if operand size is 16-bit or vector length is 128-bit
# - the second size if operand size is 32-bit or vector length is 256-bit
# - the third size of the operand size is 64-bit or vector length is 512-bit. 
# If only two sizes are given, only 16-bit and 32-bit operand sizes are considered, unles otherwise
# indicated.
# If only a size is given, that is available in all modes and with all operand sizes.
valid_opsizes = [
    'a',            # 2 x 16 bits (16-bit opsize) or 2 x 32 bits (32-bit opsize).

    # Fixed integer sizes.
    'b',            # 8 bits.
    'w',            # 16 bits.
    'd',            # 32 bits.
    'q',            # 64 bits.

    # Variable integer sizes.
    'z',            # 16 bits (16-bit opsize) or 32 bits (32 or 64-bit opsize).
    'v',            # 16, 32 or 64 bits.
    'y',            # 64 bits (64-bit opsize), 32 bits othwerwise.
    'yf',           # 64 bits (64-bit mode), 32 bits (16, 32-bit opsize).
    's',            # 48 or 80 bits descriptor.
    'p',            # 32, 48 or 80 bits pointer.
    'l',            # 64 (16 or 32-bit opsize) or 128 bits (64-bit opsize).

    # FPU sizes.
    'fa',           # 80 bits packed BCD.
    'fw',           # 16 bits real number.
    'fd',           # 32 bits real number.
    'fq',           # 64 bits real number.
    'ft',           # 80 bits real number.
    'fe',           # 14 bytes or 28 bytes FPU environment.
    'fs',           # 94 bytes or 108 bytes FPU state.

    # SIMD sizes.
    'dq',           # 128 bits.
    'qq',           # 256 bits.
    'oq',           # 512 bits.
    'ev',           # 1/8 of vlen: 16, 32 or 64 bits.
    'qv',           # 1/4 of vlen: 32, 64 or 128 bits.
    'hv',           # 1/2 of vlen: 64, 128 or 256 bits.
    'x',            # 128 bits (128-bit vlen) or 256 bits (256-bit vlen).
    'uv',           # 256 bits (256-bit vlen) or 512 bits (512-bit vlen).
    'fv',           # 128, 256 or 512 bits.

    'pd',           # 128 or 256 bits.
    'ps',           # 128 or 256 bits.
    'ph',           # Packed FP16 values.
    'sd',           # 128 bits scalar element (double precision).
    'ss',           # 128 bits scalar element (single precision).
    'sh',           # FP16 Scalar element.

    # VSIB addressing.
    'vm32x',        # VSIB addressing, using DWORD indices in XMM register, select 32/64 bit.
    'vm32y',        # VSIB addressing, using DWORD indices in YMM register, select 32/64 bit.
    'vm32z',        # VSIB addressing, using DWORD indices in ZMM register, select 32/64 bit.
    'vm32h',        # VSIB addressing, using DWORD indices in half register, select 32/64 bit.
    'vm32n',        # VSIB addressing, using DWORD indices in normal register, select 32/64 bit.
    'vm64x',        # VSIB addressing, using QWORD indices in XMM register, select 32/64 bit.
    'vm64y',        # VSIB addressing, using QWORD indices in YMM register, select 32/64 bit.
    'vm64z',        # VSIB addressing, using QWORD indices in ZMM register, select 32/64 bit.
    'vm64h',        # VSIB addressing, using QWORD indices in half register, select 32/64 bit.
    'vm64n',        # VSIB addressing, using QWORD indices in normal register, select 32/64 bit.
    
    # MIB addressing.
    'mib',          # MIB addressing, the base & the index are used to form a pointer.
    
    # Stack sizes and partial access.
    'v2',           # Two stack words.
    'v3',           # Three stack words.
    'v4',           # Four stack words.
    'v5',           # Five stack words.
    'v8',           # Eight stack words.

    # Misc and special sizes.
    '?',            # Unknown operand size. Depends on many factors (for example, XSAVE).
    '0',            # Used for instructions that do not actually access any memory.
    'asz',          # The size of the operand is given by the current addressing mode.
    'ssz',          # The size of the operand is given by the current stack mode.
    'rx',           # 512 bytes extended state.
    'cl',           # 32/64/128 bytes - the size of one cache line.
    '12',           # 4 bytes (0) + 8 bytes (old SSP), used by SAVEPREVSSP.
    't',            # A tile register. The size varies depending on execution environment, but can be as high as 1K.
    '384',          # 384 bits representing a Key Locker handle.
    '512',          # 512 bits representing a Key Locker handle.
    '4096',         # 4096 bits representing an MSR address/value table.
]

# Implicit/fixed operands.
valid_impops = {# register      size
    'AH'       : ('AH',         'b'),   # AH register.
    'AL'       : ('rAX',        'b'),   # AL register.
    'AX'       : ('rAX',        'w'),   # AX register.
    'EAX'      : ('rAX',        'd'),   # EAX register.
    'RAX'      : ('rAX',        'q'),   # RAX register.
    'eAX'      : ('rAX',        'z'),   # AX or EAX register, depending on op size.
    'rAX'      : ('rAX',        'v'),   # AX, EAX or RAX register, depending on op size.
    'yAX'      : ('rAX',        'y'),   # EAX or RAX register, depending on op size.
    'CL'       : ('rCX',        'b'),   # CL register.
    'ECX'      : ('rCX',        'd'),   # ECX register.
    'RCX'      : ('rCX',        'q'),   # RCX register.
    'eCX'      : ('rCX',        'z'),   # CX or ECX register.
    'rCX'      : ('rCX',        'v'),   # CX, ECX or RCX register, depending on op size.
    'yCX'      : ('rCX',        'y'),   # ECX or RCX register, depending on op size.
    'aCX'      : ('rCX',        'asz'), # CX, ECX or RCX register, depedning on address size.
    'DX'       : ('rDX',        'w'),   # DX register.
    'EDX'      : ('rDX',        'd'),   # EDX register.
    'RDX'      : ('rDX',        'q'),   # RDX register.
    'eDX'      : ('rDX',        'z'),   # DX or EDX register, depending on op size.
    'rDX'      : ('rDX',        'v'),   # DX, EDX or RDX register, depending on op size.
    'yDX'      : ('rDX',        'y'),   # EDX or RDX register, depending on op size.
    'EBX'      : ('rBX',        'd'),   # EBX register.
    'RBX'      : ('rBX',        'q'),   # RBX register.
    'rBX'      : ('rBX',        'v'),   # BX, EBX or RBX register, depending on op size.
    'yBX'      : ('rBX',        'y'),   # EBX or RBX register, depending on op size.
    'rBP'      : ('rBP',        'v'),   # BP, EBP or RBP register, depending on op size.
    'sBP'      : ('rBP',        'ssz'), # BP, EBP or RBP register, depending on stack size.
    'rSP'      : ('rSP',        'v'),   # SP, ESP or RSP register, depending on op size.
    'sSP'      : ('rSP',        'ssz'), # SP, ESP or RSP register, depending on stack size.
    'aSI'      : ('rSI',        'asz'), # SI, ESI, or RSI register, depending on address size.
    'aDI'      : ('rDI',        'asz'), # DI, EDI, or RDI register, depending on address size.
    'R8'       : ('rR8',        'q'),   # R8 register.
    'R9'       : ('rR9',        'q'),   # R9 register.
    'R11'      : ('rR11',       'q'),   # R11 register.
    'rIP'      : ('rIP',        'v'),   # IP, EIP or RIP, depending on op size.
    'yIP'      : ('rIP',        'yf'),  # EIP in 16/32 bit mode, or RIP in 64 bit mode.
    '1'        : ('1',          'b'),   # Constant 1.
    'XMM0'     : ('XMM0',       'dq'),  # XMM0 register.
    'XMM1'     : ('XMM1',       'dq'),  # XMM1 register.
    'XMM2'     : ('XMM2',       'dq'),  # XMM2 register.
    'XMM3'     : ('XMM3',       'dq'),  # XMM3 register.
    'XMM4'     : ('XMM4',       'dq'),  # XMM4 register.
    'XMM5'     : ('XMM5',       'dq'),  # XMM5 register.
    'XMM6'     : ('XMM6',       'dq'),  # XMM6 register.
    'XMM7'     : ('XMM7',       'dq'),  # XMM7 register.
    'ST(0)'    : ('ST0',        'ft'),  # ST(0) register.
    'ST(i)'    : ('STi',        'ft'),  # ST(1) register.
    'CS'       : ('CS',         'v'),   # CS register.
    'SS'       : ('SS',         'v'),   # SS register.
    'DS'       : ('DS',         'v'),   # DS register.
    'ES'       : ('ES',         'v'),   # ES register.
    'FS'       : ('FS',         'v'),   # FS register.
    'GS'       : ('GS',         'v'),   # GS register.
    'CR0'      : ('CR0',        'yf'),  # CR0 register.
    'XCR'      : ('XCR',        'q'),   # An XCR register.
    'XCR0'     : ('XCR0',       'q'),   # XCR0 register.
    'MSR'      : ('MSR',        'q'),   # A MSR.
    'TSC'      : ('TSC',        'q'),   # TSC register.
    'TSCAUX'   : ('TSCAUX',     'q'),   # TSXAUX register.
    'SCS'      : ('SCS',        'q'),   # IA32_SYSNETER_CS register.
    'SEIP'     : ('SEIP',       'q'),   # IA32_SYSENTER_EIP register.
    'SESP'     : ('SESP',       'q'),   # IA32_SYSENTER_ESP register.
    'FSBASE'   : ('FSBASE',     'q'),   # IA32_FS_BASE register.
    'GSBASE'   : ('GSBASE',     'q'),   # IA32_GS_BASE register.
    'KGSBASE'  : ('KGSBASE',    'q'),   # IA32_KERNEL_GS_BASE register.
    'STAR'     : ('STAR',       'q'),   # IA32_STAR register.
    'LSTAR'    : ('LSTAR',      'q'),   # IA32_LSTAR register.
    'FMASK'    : ('FMASK',      'q'),   # IA32_FMASK register.
    'GDTR'     : ('GDTR',       's'),   # GDT register.
    'IDTR'     : ('IDTR',       's'),   # IDT register.
    'LDTR'     : ('LDTR',       'w'),   # LDT register.
    'TR'       : ('TR',         'w'),   # Task register.
    'BANK'     : ('BANK',       '?'),   # A register bank.
    'X87CONTROL':('X87CONTROL', 'w'),   # X87 control register.
    'X87TAG'   : ('X87TAG',     'w'),   # X87 tag register.
    'X87STATUS': ('X87STATUS',  'w'),   # X87 status register.
    'MXCSR'    : ('MXCSR',      'd'),   # MXCSR register.
    'PKRU'     : ('PKRU',       'd'),   # PKRU register.
    'SSP'      : ('SSP',        'yf'),  # Shadow stack pointer. 32 bit in protected/compat mode, 64 in long mode.
    'SMT'      : ('SMT',        '4096'),# Source MSR table, encododed in [RSI], up to 4096 bits long (64 entries x 64 bits per entry).
    'DMT'      : ('DMT',        '4096'),# Value MSR table, encododed in [RDI], up to 4096 bits long (64 entries x 64 bits per entry).

    # Implicit memory operands.
    'pAXb'     : ('pAX',        'b'),   # Implicit byte [rAX], used by MONITOR and MONITORX. Can be overriden.
    'pCXdq'    : ('pCX',        'dq'),  # Implicit xmmword [rCX], used by RMPADJUST. Can be overriden.
    'pBXALb'   : ('pBXAL',      'b'),   # Implicit [RBX + AL], as used by XLAT.
    'pDIq'     : ('pDI',        'q'),   # Implicit qword [RDI].
    'pDIdq'    : ('pDI',        'dq'),  # Implicit xmmword [RDI].
    'pBP'      : ('pBP',        'v'),   # Implicit operand size loaded from [RBP]. RBP is subject to stack address size.

    # Implicit shadow stack accesses.
    'SHS'      : ('SHS',        'q'),   # Shadow stack (SSP) implicit access, 1 qword (use by CET instructions).
    'SHS0'     : ('SHS0',       'q'),   # Shadow stack (IA32_PL0_SSP) implicit access, 1 qword (use by CET instructions).
    'SHSI'     : ('SHS',        'v2'),  # Shadow stack load & discard, 2 elements (INCCSPD/INCSSPQ).
    'SHSS'     : ('SHS',        '12'),  # Shadow stack read & store 4 + 8 bytes (SAVEPREVSSP).
    'SHS1'     : ('SHSP',       'v'),   # Shadow stack push/pop, 1 word.
    'SHS2'     : ('SHSP',       'v2'),  # Shadow stack push/pop, 2 words.
    'SHS3'     : ('SHSP',       'v3'),  # Shadow stack push/pop, 3 words.
    'SHS4'     : ('SHSP',       'v4'),  # Shadow stack push/pop, 4 words.

    # User Interrupt Flag.
    'UIF'      : ('UIF',        'b'),   # User Interrupt Flag, stored with size of 1 byte, although it is 1 bit.
}

# If an operand type is not present here, than that operand is implicit & it's not encoded inside the instruction.
operand_encoding = {
    'A'        : 'D',   # Immediate, encoded directly in the instruction bytes.
    'B'        : 'V',   # VEX/EVEX.vvvv encoded general purpose register.
    'C'        : 'R',   # Modrm.reg encoded control register.
    'D'        : 'R',   # Modrm.reg encoded debug register.
    'E'        : 'M',   # Modrm.rm encoded general purpose register or memory.
    'G'        : 'R',   # Modrm.reg encoded general purpose register.
    'H'        : 'V',   # VEX/EVEX.vvvv encoded vector register.
    'I'        : 'I',   # Immediate, encoded directly in the instruction bytes.
    'J'        : 'D',   # Relative offset, encoded directly in the instruction bytes.
    'L'        : 'L',   # Register encoded in an immediate.
    'M'        : 'M',   # Modrm.rm encoded memory.
    'N'        : 'M',   # Modrm.rm encoded MMX register.
    'O'        : 'D',   # Absolute memory encoded directly in the instruction.
    'P'        : 'R',   # Modrm.reg encoded MMX register.
    'Q'        : 'M',   # Modrm.rm encoded MMX register or memory.
    'R'        : 'M',   # Modrm.rm encoded general purpose register.
    'S'        : 'R',   # Modrm.reg encoded segment register.
    'T'        : 'R',   # Modrm.reg encoded test register.
    'U'        : 'M',   # Modrm.rm encoded vector register.
    'V'        : 'R',   # Modrm.reg encoded vector register.
    'W'        : 'M',   # Modrm.rm encoded vector register or memory.
    'Z'        : 'O',   # General purpose register encoded in opcode low 3 bit.
    'rB'       : 'R',   # Modrm.reg encoded bound register.
    'mB'       : 'M',   # Modrm.rm encoded bound register or memory.
    'rK'       : 'R',   # Modrm.reg encoded mask register.
    'vK'       : 'V',   # VEX/EVEX.vvvv encoded mask register.
    'mK'       : 'M',   # Modrm.rm encoded mask register or memory.
    'aK'       : 'A',   # EVEX.aaa encoded mask register.
    'mR'       : 'R',   # Modrm.reg encoded memory.
    'mM'       : 'M',   # Modrm.rm encoded memory (always).
    '1'        : '1',   # Constant 1.
    'CL'       : 'C',   # CL register.
    'ST(i)'    : 'M',   # Modrm.rm encoded FPU register.
    'm2zI'     : 'L',   # Immediate encodes a register.
    'dfv'      : 'V',   # EVEX.VVVV encodes a default flags value.
}

# Accepted prefixes. These values go with the 'p' metadata key.
valid_prefixes = [
    'REP',          # Rep prefix is accepted.
    'REPC',         # Conditional rep prefix is accepted.
    'HLE',          # Hardware Lock Elision accepted.
    'BND',          # Bound prefix accepted (MPX).
    'LOCK',         # Lock prefix accepted.
    'BH',           # Branch hints accepted.
    'XACQUIRE',     # Xacquire prefix accepted.
    'XRELEASE',     # Xrelease prefix accepted.
    'HLEWOL',       # HLE prefix is accepted without lock - used by MOV instructions.
    'DNT',          # Do Not Track prefix accepted (CET).
]

# Accepted access types. These values go with the 'w' metadata key (operand access map).
valid_access = [
    'N',            # No access.
    'P',            # Prefetch access.
    'R',            # Read.
    'W',            # Write.
    'CR',           # Conditional read.
    'CW',           # Conditional write.
    'RW',           # Read-Write.
    'CRW',          # Conditional Read-Write.
    'RCW',          # Read-Conditional Write.
    'CRCW',         # Conditional Read-Conditional Write.
]

# Valid flags. These values go with the 'f' metadata key.
valid_flags = [
    'CF',           # Carry.
    'PF',           # Parity.
    'AF',           # Auxiliary.
    'ZF',           # Zero.
    'SF',           # Sign.
    'TF',           # Trap.
    'IF',           # Interrupt.
    'DF',           # Direction.
    'OF',           # Overflow.
    'IOPL',         # I/O privilege level.
    'NT',           # Nested Task.
    'RF',           # Resume Flag.
    'VM',           # V8086 mmode.
    'AC',           # Alignment Check.
    'VIF',          # Virtual IF.
    'VIP',          # Virtual IP.
    'ID'            # CPUID ID flag.
]

# Valid flags access. These values go with the 'f' metadata key.
valid_flag_op = [
    'm',            # modified.
    't',            # tested.
    '0',            # cleared.
    '1',            # set.
    'u',            # undefined.
    'n',            # not accessed.
]

# Valid CPU modes. These values go with the 'm' metadata key.
valid_cpu_modes = [
    'r0',           # Ring 0.
    'r1',           # Ring 1.
    'r2',           # Ring 2.
    'r3',           # Ring 3.
    'real',         # Real mode.
    'v8086',        # V8086 mode.
    'prot',         # Protected mode.
    'compat',       # Compatibility mode.
    'long',         # Long mode.
    'smm',          # System Management Mode.
    'smm_off',      # Outside SMM.
    'sgx',          # Software Guard Extensions SGX enclave.
    'sgx_off',      # Outside SGX.
    'tsx',          # Transactional Synchronization Extensions.
    'tsx_off',      # Outside TSX.
    'vmxr',         # VMX root.
    'vmxn',         # VMX non-root.
    'vmxr_seam',    # VMX root SEAM.
    'vmxn_seam',    # VMX non-root SEAM.
    'vmx_off',      # Outside VMX operation.
]

valid_mode_groups = [
    'ring',
    'mode',
    'vmx',
    'other',
]

valid_ring_modes = [
    'r0', 
    'r1', 
    'r2', 
    'r3',
]

valid_mode_modes = [
    'real',  
    'v8086', 
    'prot',  
    'compat',
    'long',  
]

valid_vmx_modes = [
    'vmxr',
    'vmxn',
    'vmxr_seam',
    'vmxn_seam',
    'vmx_off',
]

valid_other_modes = [
    'smm',
    'smm_off',
    'sgx',
    'sgx_off',
    'tsx',
    'tsx_off',
]

valid_mode_map = {
    'ring'  : valid_ring_modes,
    'mode'  : valid_mode_modes,
    'vmx'   : valid_vmx_modes,
    'other' : valid_other_modes,
}

# Valid decoratoras that can be present in either mnemonic or operand.
valid_decorators = [
    '{K}',          # Masking support.
    '{z}',          # Zeroing support.
    '{sae}',        # Surpress All Exceptions.
    '{er}',         # Embedded Rounding.
    '|B32',         # Broadcast 32.
    '|B64',         # Broadcast 64.
    '|B16',         # Broadcast 16.
    '{ND}',         # New data destination.
    '{NF}',         # No flags.
    '{ZU}',         # Zero-upper.
]

# Maps decorator to global flags.
deco_flags = {
    '{K}'   : 'MASK', 
    '{z}'   : 'ZERO', 
    '{sae}' : 'SAE', 
    '{er}'  : 'ER', 
    '|B32'  : 'BROADCAST', 
    '|B64'  : 'BROADCAST', 
    '|B16'  : 'BROADCAST',
    '{ND}'  : 'ND',
    '{ZU}'  : 'ZU',
    '{NF}'  : 'NF',
}

# Maps decorator to operand flags.
deco_op_flags = {
    '{K}'   : 'MASK', 
    '{z}'   : 'ZERO', 
    '{sae}' : 'SAE',
    '{er}'  : 'ER',
    '|B32'  : 'B32', 
    '|B64'  : 'B64', 
    '|B16'  : 'B16',
}

# Valid EVEX tuples. These values go with the 'l' metadata key.
valid_tuples = [
    'fv',           # Full Vector, Load+Op (Full Vector Dword/Qword).
    'hv',           # Half Vector, Load+Op (Half Vector).
    'qv',           # Quarter vector, Load+op (Quarter Vector, FP16)
    'fvm',          # Full Vector Memory, Load/store or subDword full vector.
    'hvm',          # Half Vector Memory, SubQword Conversion.
    'qvm',          # Quarter Vector Memory, SubDword Conversion.
    'ovm',          # Oct Vector Memory, SubWord Conversion.
    'dup',          # Dup, VMOVDDUP.
    'm128',         # Mem 128, Shift count from memory.
    't1s8',         # Tuple 1 Scalar, 8 bit, 1Tuple less than Full Vector.
    't1s16',        # Tuple 1 Scalar, 16 bit, 1Tuple less than Full Vector.
    't1s',          # Tuple 1 Scalar, 32/64 bit, 1Tuple less than Full Vector.
    't1f',          # Tuple 1 Fixed, 1 Tuple memsize not affected by EVEX.W.
    't2',           # Tuple 2, Broadcast (2 elements).
    't4',           # Tuple 4, Broadcast (4 elements).
    't8',           # Tuple 8, Broadcast (8 elements).
    't1_4x',
]

# Valid EVEX modes. These values go with the 'v' metadata key.
valid_evex_mode = [
    'none',         # Legacy EVEX
    'vex',          # EVEX extension for VEX
    'legacy',       # EVEX extension for legacy instructions
    'cond',         # EVEX extension for conditional instructons
]


# Use one of these value to indicate absent operands.
absent_op = ['n/a', '']

# Global templates, extracted from template files.
template_flags = {}
template_cpuid = {}
template_modes = {}


def my_str(x):
    if x is None:
        return x
    else:
        return str(x)


class InvalidSpecificationException(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr(self.value)


class CpuidFeatureFlag():
    """
    Describes a CPUID feature flag.

    Attributes
    ----------
    Name: str
        Feature name.
    Leaf: str
        Leaf number (the EAX input value).
    SubLeaf: str
        SubLeaf number (the ECX input value. '0xFFFFFFFF' if no subleaf present).
    Reg: str
        The output register that contains the feature indication.
    Bit: str
        The bit inside Reg that indicates whether the feature is present or not.
    """
    def __init__(self, finfo):
        self.Name = finfo['name']
        self.Leaf = finfo['leaf']
        self.SubLeaf = finfo['subleaf']
        self.Reg = finfo['reg']
        self.Bit = finfo['bit']

    def __str__(self):
        return '%s: %s, %s, %s, %s' % (self.Name, self.Leaf, self.SubLeaf, self.Reg, self.Bit)


class Operand():
    """
    Describes an x86 instruction operand.

    Attributes
    ----------
    Raw: str
        Raw operand description (examples: 'Gv', or 'EAX').
    Type: str
        Operand type. One of valid_optypes (example: 'G').
    Size: str
        Operand size. One of valid_opsizes (example: 'v').
    Flags: list[str]
        Operand flags (example: ['OPDEF']).
    Decorators: list[str]
        Operand decorators (example: ['MASK', 'ZERO']). Entries from deco_op_flags.
    Access: str
        Operand access mode (example: 'RW'). One of valid_access.
    Block: int
        Operand block register access. Usually 0.
    Encoding: str
        Operand encoding (example: 'R'). One of operand_encoding values.
    Implicit: bool
        Whether the operand is implicit or explicit.
    """
    def __init__(self, op, access, flags, imp = False):
        self.Raw = op
        self.Type = 0
        self.Size = 0
        self.Flags = flags
        self.Decorators = []
        self.Access = []
        self.Block = 0
        self.Encoding = 'S'
        self.Implicit = imp
        orig = op

        # Handle block registers.
        if op.endswith('+3'):
            self.Block = 4
            op = op.replace('+3', '')
        elif op.endswith('+1'):
            self.Block = 2
            op = op.replace('+1', '')
        else:
            m = re.match(r'XMM(\d)-(\d)', op)
            if m:
                start = m.group(1)
                end = m.group(2)
                self.Block = int(end) - int(start) + 1
                op = 'XMM' + start

        # Handle the decorators.
        for dec in valid_decorators:
            if -1 != op.find(dec):
                # Found decorator.
                self.Decorators.append(dec)
                # Remove it from the opstring.
                op = op.replace(dec, '')

        # Handle hard-coded operators - those that are implicit/are not encoded anywhere.
        if op in valid_impops:
            self.Type, self.Size = valid_impops[op][0], valid_impops[op][1]
        # Now handle explicit operators.
        else:
            # Attempt a match inside the explicit operands map.
            for opt in valid_optypes:
                if op.startswith(opt):
                    self.Type = opt
                    op = op.replace(opt, '')
                    break

            # Now the operand size. After parsing the decorator and the operand type, we should be left with
            # the operand size only.
            if self.Type in ['rK', 'mK', 'vK', 'aK'] and not op in valid_opsizes:
                self.Size = 'q'
            elif self.Type in ['dfv']:
                # No size for default flags value.
                self.Size = '0'
            elif op in valid_opsizes:
                self.Size = op
            else:
                raise InvalidSpecificationException("Invalid operand size specified: " + orig)

        if self.Type in operand_encoding:
            self.Encoding = operand_encoding[self.Type]
        elif self.Raw in operand_encoding:
            self.Encoding = operand_encoding[self.Raw]

        if imp and 'OPDEF' not in self.Flags:
            self.Flags.append('OPDEF')

        self.Access = access

    def __str__(self):
        return self.Raw


class Instruction():
    """
    Describes an x86 instruction.

    Attributes
    ----------
    Mnemonic: str
        The instruction mnemonic. Decorators are not included.
    RawExpOps: list[str]
        Raw explicit operands, as supplied in the specification file (example: ['Gv', 'Ev']).
    RawImpOps: list[str]
        Raw implicit operands, as supplied in the specification file (example: ['Fv', 'rIP']).
    RawEnc: str
        Raw encoding, as supplied in the specification file (example: '0xBD /r').
    RawMeta: list[str]
        Raw meta-data, as supplied in the specification file (example: ['a:MODRM', 'p:LOCK', 's:ABC']).
    DecoFlags: list[str]
        Decorator flags, extracted from mnemonic & operands. Indicates what decorators are accepted
        by the instruction.
    Xop: bool
        True if instruction uses XOP encoding.
    Vex: bool
        True if instruction uses VEX encoding.
    Evex: bool
        True if instruction uses EVEX encoding.
    ExpOps: list[Operand]
        Processed explicit operands. A list of Operand objects.
    ImpOps: list[Operand]
        Processed implicit operands. A list of Operand objects.
    Attributes: list[str]
        A list of instruction attributes (example: ['MODRM', 'I64']). Each attriubute in this list
        must be one of the values listed in valid_attributes.
    Prefmap: list[str]
        A list of accepted prefixes (example: ['LOCK', 'HLE']). Each prefix in this list must be one
        of the values listed in valid_prefixes.
    Set: str
        Instruction set (example: 'ABCD').
    Category: str
        Instruction category (example: 'XYZQ').
    Class: str
        Instruction class. Usually the same as the mnemonic. Can be overridden, if it is explicitly 
        specified.
    Rwm: list[str]
        Operand access map (example: ['RW', 'W']). Length must be equal to len(ExpOps) + len(ImpOps).
        Inside the Rwm list, the entry at position x indicates the access mode for operand x.
    Id: str
        CPUID feature flag, if any (example: 'AVX512VBMI2').
    Tuple: str
        EVEX tuple, if any (example: 'fvm').
    ExType: str
        Exception type, if any (example: '1', or 'EVEX_PP2').
    Rflags: dict
        Flags access mode (example: {'m': ['CF'], 't': ['CF'], 'u': ['ZF'], '0': ['AF'], '1': ['OF']}).
        A dictionary with keys 'm' (modified), 't' (tested), 'u' (undefined), '0' (cleared), '1' (set),
        and each key pointing to a list of flags which are accessed in the indicated mode.
    FpuFlags: list[str]
        FPU flags access mode (example: ['u', 'u', 'u', 'u']). A list of 4 str elements, each one 
        indicating the access mode for flag Cx, where x is the position in the list.
    Modes: list[str]
        Valid operating modes for the indicated instruction.
    Encoding: dict
        A dictionary containing all the encoding components.
    """
    def __init__(self, mnemonic, expops, impops, encoding, meta):
        # Fill in raw instruction information
        self.Mnemonic   = mnemonic
        self.RawExpOps  = expops
        self.RawImpOps  = impops
        self.RawEnc     = encoding
        self.RawMeta    = meta
        self.DecoFlags  = []

        # Assume no XOP/VEX/EVEX.
        self.Xop = self.Vex = self.Evex = False

        # Pre-process the mnemonic, which may contain decorators.
        for vd in valid_decorators:
            if vd in self.Mnemonic:
                self.DecoFlags.append(deco_flags[vd])
                self.Mnemonic = self.Mnemonic.replace(vd, '')

        # Parse meta-information.
        if self.RawMeta:
            self.process_meta()

        # Parse the instruction encoding.
        if self.RawEnc:
            self.process_encoding()
        else:
            raise InvalidSpecificationException("Instruction with no encoding specified: " + self.Mnemonic)

        # Pre-process the explicit operands. The mask register is contained as a decorator, but put it as a direct
        # operand as well. The access flag is already present in rwm.
        if len(self.RawExpOps) >= 1 and self.RawExpOps[0].find('{K') > 0:
            self.RawExpOps.insert(1, 'aKq')

        # Parse the explicit instruction operands.
        self.ExpOps = self.process_operands(self.RawExpOps, False)

        # Parse the implicit instruction operands.
        self.ImpOps = self.process_operands(self.RawImpOps, True)

        # Post-process the operands. We fill up the flags with additional info based on the operands.
        usesv = usesvp = False
        for op in self.ExpOps:
            for deco in op.Decorators:
                self.DecoFlags.append(deco_flags[deco])
            if op.Type in ['U', 'V', 'W', 'H', 'L']:
                self.add_attribute('VECTOR')
            if op.Type in ['B', 'H', 'vK', 'vT', 'dfv']:
                usesv = True
            if self.Evex:
                if op.Type in ['B', 'H']:
                    usesvp = True
                if 'VSIB' in self.Attributes:
                    usesvp = True
                if self.Encoding["sc"]:
                    usesvp = True
        if self.Xop or self.Vex or self.Evex:
            if not usesv: 
                self.add_attribute('NOV')
            if self.Evex and not usesvp:
                self.add_attribute('NOVP')


        # VEX, XOP, EVEX instructions are not valid in real or v8086 modes.
        if self.Vex or self.Xop or self.Evex:
            if 'real' in self.Modes:
                self.Modes.remove('real')
            if 'v8086' in self.Modes:
                self.Modes.remove('v8086')
        if 'long' not in self.Modes and 'I64' not in self.Attributes:
            self.Attributes.append('I64')
        if 'long' in self.Modes and 'prot' not in self.Modes and 'O64' not in self.Attributes:
            self.Attributes.append('O64')


    def process_meta(self):
        """
        Extract all the meta-data from the instruction spec, and fill in class attributes.
        """
        self.Attributes = []
        self.Prefmap    = []
        self.Set        = 'UNKNOWN'
        self.Category   = 'UNKNOWN'
        self.Class      = self.Mnemonic
        self.Rwm        = None
        self.Id         = self.Set
        self.Tuple      = None
        self.ExType     = None
        self.EvexMode   = None
        self.Rflags     = {'m': [], 't': [], '0': [], '1': [], 'u': []}
        self.FpuFlags   = ['u', 'u', 'u', 'u']
        self.Modes      = valid_cpu_modes.copy()

        for y in self.RawMeta:
            token, value = y.strip().split(':')

            if token == 'a':
                # Instruction attributes.
                self.Attributes = value.split('|')
                for a in self.Attributes:
                    if a not in valid_attributes:
                        raise InvalidSpecificationException(
                            "Unknown attribute specifier '%s', expecting one of [%s]" % (a, ','.join(valid_attributes)))
            elif token == 'p':
                # Accepted prefixes.
                self.Prefmap = value.split('|')
                for p in self.Prefmap:
                    if p not in valid_prefixes:
                        raise InvalidSpecificationException(
                            "Unknown prefix specifier '%s', expecting one of [%s]" % (p, ','.join(valid_prefixes)))
            elif token == 's':
                # Instruction set.
                self.Set = self.Id = value
            elif token == 't':
                # Instruction type.
                self.Category = value
            elif token == 'c':
                # Instruction class.
                self.Class = value
            elif token == 'w':
                # Operand access.
                self.Rwm = value.split('|')
                # The read/write map must have the same size as the number of operands.
                if len(self.Rwm) < len(self.RawExpOps) + len(self.RawImpOps):
                    raise InvalidSpecificationException(
                        "Invalid number of operand access specifiers: provided %d, expecting %d" % 
                        (len(self.Rwm), len(self.RawExpOps) + len(self.RawImpOps)))
                for r in self.Rwm:
                    if r not in valid_access:
                        raise InvalidSpecificationException(
                            "Unknown operand access specifier '%s', expecting one of [%s]" % (r, ','.join(valid_access)))
            elif token == 'i':
                # CPUID flag.
                self.Id = value
            elif token == 'l':
                # EVEX tuple.
                self.Tuple = value
                if self.Tuple not in valid_tuples:
                    raise InvalidSpecificationException(
                        "Unknown tuple specifier '%s', expecting one of [%s]" % (self.Tuple, ','.join(valid_tuples)))
            elif token == 'v':
                # EVEX mode.
                self.EvexMode = value
                if self.EvexMode not in valid_evex_mode:
                    raise InvalidSpecificationException("Unknown evex extension type '%s', expecting one of [%s]" % 
                                             (self.EvexMode, ','.join(valid_evex_mode)))
            elif token == 'e':
                # VEX/EVEX exception type.
                self.ExType = value
            elif token == 'f':
                # RFLAGS access.
                flgaccess = []
                for v in value.split('|'):
                    if v in template_flags:
                        flgaccess += template_flags[v].split('|')
                    else:
                        flgaccess.append(v)
                revflg = {}
                for m in valid_flag_op:
                    revflg[m] = []
                for flg in flgaccess:
                    f, m = flg.split('=')
                    if m not in valid_flag_op:
                        raise InvalidSpecificationException(
                            "Unknown flag access specifier '%s', expecting one of [%s]" % (m, ','.join(valid_flag_op)))
                    if f not in valid_flags:
                        raise InvalidSpecificationException(
                            "Unknown flag specifier '%s', expecting one of [%s]" % (f, ','.join(valid_flags)))
                    revflg[m].append(f)
                flgaccess = revflg
                self.Rflags = flgaccess
            elif token == 'u':
                # FPU flags access.
                self.FpuFlags = ['u', 'u', 'u', 'u'] # each one is undefined.
                for v in value.split('|'):
                    flg, acc = v.split('=')
                    if flg not in ['C0', 'C1', 'C2', 'C3']:
                        raise InvalidSpecificationException("Unknown FPU flag: %s" % flg)
                    if acc not in ['0', '1', 'm', 'u']:
                        raise InvalidSpecificationException("Unknown FPU flag access: %s" % acc)
                    self.FpuFlags[int(flg[1])] = acc
            elif token == 'm':
                # Example: m:ring=0,1,2,3|vmx=root,nonroot|mode=real,v8086,smm,prot,compat,long|other=sgx,tsx
                # Note: any group that is not specified is considered entirely valid
                # Note: any group that is specified overrides all the other fields in the group; example:
                #       mode=real   - this means the instruction is valid ONLY in real mode.
                #       mode=!v8086 - this means the instructiom is valid is ANY mode except for V8086
                tmodes = []
                for t in value.split('|'):
                    if t in template_modes:
                        tmodes += template_modes[t].split('|')
                    else:
                        tmodes.append(t)

                modes = []
                groups = {}
                for g in valid_mode_groups:
                    groups[g] = {}
                    groups[g]['negated'] = False
                    groups[g]['specified'] = False
                    groups[g]['modes'] = []

                for tm in tmodes:
                    m, v = tm.split('=')
                    for vx in v.split('+'):
                        if vx.startswith('!'):
                            vx = vx[1:]
                            groups[m]['negated'] = True
                        if m not in valid_mode_groups:
                            raise InvalidSpecificationException("Unknown CPU mode group specified: %s" % m)
                        if vx not in valid_mode_map[m]:
                            raise InvalidSpecificationException("Mode %s is not valid for mode group %s; it can be one of [%s]" % 
                                                        (vx, m, ','.join(valid_mode_map[m])))
                        groups[m]['specified'] = True
                        groups[m]['modes'].append(vx)

                for g in groups:
                    if not groups[g]['specified']:
                        modes += valid_mode_map[g]
                    elif not groups[g]['negated']:
                        modes += groups[g]['modes']
                    else:
                        modes += [x for x in valid_mode_map[g] if x not in groups[g]['modes']]

                for m in modes:
                    if m.startswith('!'): 
                        m = m[1:]
                    if m not in valid_cpu_modes:
                        raise InvalidSpecificationException("Unknown CPU mode specifier '%s', expecting one of [%s]" % 
                                                    (m, ','.join(valid_cpu_modes)))

                self.Modes = modes
            else:
                raise InvalidSpecificationException("Unknown token specified: %s" % token)


    def process_encoding(self):
        """
        Extract all the encoding fields from the instruction spec, and fill in the Encoding dictionary.
        Once instruction grouping has been performed, the Encoding dictionary may be left with empty values.
        This function can be called an arbitrary number of times to regenerate the Encoding dictionary.
        """
        # The order of the entries in Encoding does not matter. When grouping instructions, whoemever does the grouping,
        # can choose whatever order they wish.
        self.Encoding = {
            'opcode'      : [], # Opcode group
            'opcode_last' : [], # Opcode group, but opcode comes after ModR/M (3DNow! instructions)
            'vendor'      : [], # Vendor grouping
            'feature'     : [], # Feature grouping; generally useful for instructions which act like NOP if feature is off
            'prefix'      : [], # Mandatory prefix grouping: NP, 0x66, 0xF3, 0xF2
            'modrmreg'    : [], # ModR/M.reg group
            'modrmmod'    : [], # ModR/M.mod group (mem or reg)
            'modrmrm'     : [], # ModR/M.rm group
            'mode'        : [], # Operating mode group (16, 32, 64)
            'dsize'       : [], # Data size group
            'asize'       : [], # Address size group
            'auxiliary'   : [], # Other grouping criteria
            'mmmmm'       : [], # XOP/VEX/EVEX.map
            'pp'          : [], # XOP/VEX/EVEX.pp, compressed prefix
            'l'           : [], # XOP/VEX/EVEX.l, vector length
            'w'           : [], # XOP/VEX/EVEX.w, width
            'wi'          : [], # XOP/VEX/EVEX.w, width, but ignored if outside 64-bit
            'nd'          : [], # EVEX.nd, new data destination
            'nf'          : [], # EVEX.nf, no flags
            'sc'          : [], # EVEX.sc, standard condition code
        }

        had_modrm = False

        # Now parse each info chunk and extract the actual data
        for t in self.RawEnc.split(' '):
            if not t: 
                continue # Skip empty tokens
            if   t == 'xop': 
                self.Xop = True
            elif t == 'vex': 
                self.Vex = True
            elif t == 'evex': 
                self.Evex = True
            # Mandatory prefixes.
            elif t in ['NP', '0x66', '0xF3', '0xF2'] and not self.Encoding['opcode'] and not (self.Xop or self.Vex or self.Evex):
                self.Encoding['prefix'].append('P' + t) # Prefix with P, so we don't confuse this with an opcode.
            # Auxiliary conditions.
            elif t in ['repz', 'mo64', 'rexw', 'rexb', 'rep', 'riprel', 'rex2', 'rex2w']:
                self.Encoding['auxiliary'].append(t)
            # Data size.
            elif t in ['ds16', 'ds32', 'ds64', 'dds64', 'fds64']:
                self.Encoding['dsize'].append(t)
            # Address size.
            elif t in ['as16', 'as32', 'as64']:
                self.Encoding['asize'].append(t)
            # Vendor.
            elif t in ['intel', 'amd']:
                self.Encoding['vendor'].append(t)
            # Feature.
            elif t in ['mpx', 'cet', 'cldm', 'piti']:
                self.Encoding['feature'].append(t)
            # ModR/M.
            elif t.startswith('/'):
                had_modrm = True
                self.add_attribute('MODRM')
                if m := re.match(r'^/r$', t):
                    pass
                elif m := re.match(r'^/r:(reg|mem)$', t):
                    # Modrm mod
                    self.Encoding['modrmmod'].append(m[1])
                elif m := re.match(r'^/([0-7])$', t):
                    # Modrm reg
                    self.Encoding['modrmreg'].append(str(m[1]))
                elif m := re.match(r'^/([0-7]):(reg|mem)$', t):
                    # Modrm reg & mod
                    self.Encoding['modrmreg'].append(str(m[1]))
                    self.Encoding['modrmmod'].append(m[2])
                # Handle opcode-like ModR/M
                elif m := re.match(r'^/(0x[0-9a-fA-F]{2})$', t):
                    mrm = int(m[1], 16)
                    if 0xC0 == (mrm & 0xC0):
                        self.Encoding['modrmmod'].append('reg')
                    else:
                        self.Encoding['modrmmod'].append('mem')
                    self.Encoding['modrmrm'].append(str(mrm & 7))
                    self.Encoding['modrmreg'].append(str((mrm >> 3) & 7))
                else:
                    raise InvalidSpecificationException("Invalid encoding: modrm specification is invalid: '%s'!" % t)
            # Modrm.rm value.
            elif m := re.match(r'^rm:([0-7])$', t): 
                self.Encoding['modrmrm'].append(m[1])
            # Map field inside XOP/VEX/EVEX.
            elif m := re.match(r'^m:([0-9A-C])$', t): 
                self.Encoding['mmmmm'].append(m[1])
            # Compressed prefix field inside XOP/VEX/EVEX.
            elif m := re.match(r'^p:([0-3])$', t): 
                self.Encoding['pp'].append(m[1])
            # Vector length field inside XOP/VEX/EVEX.
            elif m := re.match(r'^l:([x|i|0|1|2|3])$', t):
                if m[1] == 'x': 
                    pass
                elif m[1] == 'i':
                    self.add_attribute('LIG')
                else:
                    self.Encoding['l'].append(m[1])
            # W field inside XOP/VEX/EVEX/REX2.
            elif m := re.match(r'^w:(x|i|0|1)$', t):
                if m[1] == 'x':
                    pass
                elif m[1] == 'i':
                    self.add_attribute('WIG')
                else:
                    if 'IWO64' in self.Attributes:
                        self.Encoding['wi'].append(m[1])
                    else:
                        self.Encoding['w'].append(m[1])
            # New Data Destination field inside EVEX.
            elif m := re.match(r'^nd:(0|1)$', t): 
                self.Encoding['nd'].append(m[1])
            # No Flags field inside EVEX.
            elif m := re.match(r'^nf:(0|1)$', t): 
                self.Encoding['nf'].append(m[1])
            # Standard Condition Code field inside EVEX.
            elif m := re.match(r'^sc:([0-9A-F]{1})$', t): 
                self.Encoding['sc'].append(m[1])
            # Opcode.
            elif re.match(r'^0x[0-9a-fA-F]{2}$', t):
                if had_modrm:
                    self.Encoding['opcode_last'].append(t)
                else:
                    self.Encoding['opcode'].append(t)
            # Misc encoding & addressing
            elif 'vsib' == t: 
                self.add_attribute('VSIB')
            elif 'mib' == t: 
                self.add_attribute('MIB')
            elif 'bitbase' == t: 
                self.add_attribute('BITBASE')
            elif 'sibmem' == t: 
                self.add_attribute('SIBMEM')
            # Misc immediates
            elif t in ['ib', 'iw', 'iz', 'iv', 'id', 'cb', 'cz', 'cv', 'cp', 'cq', 'is4']:
                # Not used for now, but they must be specified, for a complete instruction encoding specification.
                pass
            else:
                raise InvalidSpecificationException(f"Invalid encoding: Unknown token: {t}")


    def process_operands(self, ops, imp = False):
        """
        Process the provided operands list, and generate a list of Operand objects.
        """
        p, res = 1, []
        for op in ops:
            if op in absent_op:
                break
            flags = []
            if not imp:
                for f in self.Attributes:
                    if f.startswith('OP%d' % p): 
                        flags.append('OP' + f[3:])
                        self.Attributes.remove(f)
            else:
                flags.append('OPDEF')
            if not imp:
                res.append(Operand(op, self.Rwm[p - 1], flags, imp))
            else:
                res.append(Operand(op, self.Rwm[len(self.ExpOps) + p - 1], flags, imp))
            p += 1
        return res


    def add_attribute(self, attribute):
        if attribute not in self.Attributes:
            self.Attributes.append(attribute)

    def __str__(self):
        ops = ''
        for o in self.ExpOps:
            ops += o.__str__() + ','
        ops = ops[:-1]

        # Return a text reprezentation of the encoding
        return (self.Mnemonic + ' ' + ops).strip()


def parse_entry(
    entry: str
    ) -> Instruction:
    """
    Parse one instruction specification line, and return an Instruction object.
    For examples on how an entry must look, check any entry inside table_* files.
    An instruction specification must contain 5 elements:
        1. The mnemonic
        2. The explicit operands (n/a or empty if none is present)
        3. The implicit operands (n/a or empty if none is present)
        4. The encoding (mandatory, cannot be empty)
        5. Metadata
    The instruction metadata includes several 'key:value' pairs; 'key' can be one of the following:
        1.  'a': instruction attributes
        2.  's': instruction set
        3.  'c': instruction class
        4.  'p': accepted prefixes
        5.  'w': operand access map
        6.  'i': CPUID feature flag; by default, the instruction set is used to look up a potential CPUID flag
        7.  'l': EVEX tuple type
        8.  'e': VEX/EVEX exception type
        9.  'v': EVEX mode (normal EVEX, extended VEX, extended legacy, conditional)
        10. 'f': flags access ('m': modified, 't': tested, 'u': undefined, '0': cleared, '1': set to 1)
        11. 'u': FPU flags access
        12. 'm': valid operating modes

    Raises
    ------
    InvalidSpecificationException
        - If the specification is incomplete
        - If an unknwon operand type or size is used
        - If an invalid encoding is specified
        - If an unknown metadata key is specified
        - If any metadata value is invalid/unknown

    Parameters
    ----------
    entry: str
        The instruction specification.

    Returns
    -------
    An object representing the instruction.
    """
    # Make sure this is not a comment. Skip comments.
    if entry.startswith('#') or len(entry) < 4: 
        return None

    # Preprocess: remove comments, CR/LF
    entry = entry.replace('\x0D', '').replace('\x0A', '')
    com = entry.find('#')
    if -1 != com: x = entry[:com]

    # Space can't be the first character.
    if entry[0] == ' ': 
        raise InvalidSpecificationException("Space cannot be the first character!")

    components = entry.split(';')
    if len(components) != 5:
        raise InvalidSpecificationException("Expected 5 components per line, but found %d (missing semicolon?)!" % len(components))

    mnemonic = components[0].strip()
    expops   = components[1].strip().split(',')
    impops   = components[2].strip().split(',')
    encoding = components[3].strip()
    meta     = components[4].strip().split(',')

    if len(mnemonic) < 1:
        raise InvalidSpecificationException("Mnemonic cannot be empty!")

    if len(expops) == 1 and expops[0] in absent_op:
        expops = []

    if len(impops) == 1 and impops[0] in absent_op:
        impops = []

    return Instruction(mnemonic, expops, impops, encoding, meta)


def parse_ins_file(
    fpath: str
    ) -> list[Instruction]:
    """
    Parse an entire instruction specification file, and return a list of Instruction objects.

    Parameters
    ----------
    fpath: str
        The file containing multiple instruction specifications.

    Returns
    -------
    A list containing Instruction objects generated from the specs inside the provided file.
    """
    instructions = []
    lcount = 0
    for line in open(fpath, 'rt'):
        lcount += 1
        try:
            ins = parse_entry(line)
            if ins: instructions.append(ins)
        except Exception as e:
            print('ERROR: Parsing failed at %s:%d: %s' % (fpath, lcount, e))
            raise
    return instructions


def parse_cff_file(
    fpath: str
    ) -> list[CpuidFeatureFlag]:
    """
    Parse the CPUID feature flags file, and return a list of CpuidFeatureFlag objects.

    Parameters
    ----------
    fpath: str
        The file containing multiple CPUID feature specifications.

    Returns
    -------
    A list containing CpuidFeatureFlag objects generated from the specs inside the provided file.
    """
    global template_cpuid
    features = []
    for line in open(fpath, 'rt'):
        if line.startswith('#'):
            continue
        res = re.findall(r'([^\s]+)\s+:\s+(0x[0-9a-fA-F]+),\s+(0x[0-9a-fA-F]+),\s+(EAX|ECX|EDX|EBX),\s+(\d+)', line)
        if not res:
            continue
        res = res[0]
        cffi = {}
        cffi['name'] = res[0]
        cffi['leaf'] = res[1]
        cffi['subleaf'] = res[2]
        cffi['reg'] = res[3]
        cffi['bit'] = res[4]
        features.append(CpuidFeatureFlag(cffi))
    template_cpuid = features
    return features


def parse_flags_file(
    fpath: str
    ) -> dict:
    """
    Parse the flags access file, and return a list of flags access templates.

    Parameters
    ----------
    fpath: str
        The file containing multiple flags access templates.

    Returns
    -------
    A dictionary containing all the flags access templates in the provided file.
    """
    global template_flags
    flags = {}
    for line in open(fpath, 'rt'):
        if line.startswith('#'):
            continue
        res = re.findall(r'([^\s]+)\s+:([^$]+)', line)
        if not res:
            continue
        res = res[0]
        flags[res[0]] = res[1].strip('\n\r ')
    template_flags = flags
    return flags


def parse_modess_file(
    fpath: str
    ) -> dict:
    """
    Parse the valid modes file, and return a dictionary of existing valid modes templates.

    Parameters
    ----------
    fpath: str
        The file containing multiple valid modes specifications.

    Returns
    -------
    A dictionary containing all the valid modes templates in the provided file.
    """
    global template_modes
    modes = {}
    for line in open(fpath, 'rt'):
        if line.startswith('#'):
            continue
        res = re.findall(r'([^\s]+)\s+:([^$]+)', line)
        if not res:
            continue
        res = res[0]
        modes[res[0]] = res[1].strip('\n\r ')
    template_modes = modes
    return modes


#
# =============================================================================
# Main
# =============================================================================
#
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print('Usage: %s defs-file' % os.path.basename(sys.argv[0]))
        sys.exit(-1)

    # Parse the flags file.
    template_flags = parse_flags_file('%s/flags.dat' % sys.argv[1])

    # Parse the cpuid feature flags and extract each feature
    template_cpuid = parse_cff_file('%s/cpuid.dat' % sys.argv[1])

    # Parse the modes file.
    template_modes = parse_modess_file('%s/modes.dat' % sys.argv[1])

    # Parse the instruction file and extract the instructions
    instructions = []
    for fn in glob.glob('%s/table*.dat' % sys.argv[1]):
        instructions += parse_ins_file(fn)

    # Sort the instructions.
    instructions = sorted(instructions, key = lambda x: x.Mnemonic)
    for i in range(0, len(instructions)):
        print(instructions[i])

    features = sorted(template_cpuid, key = lambda x: x.Name)
    for i in range(0, len(features)):
        print(features[i])
