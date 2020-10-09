#
# Copyright (c) 2020 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import os
import sys
import re
import glob


valid_attributes = {
    'MODRM',        # Mod r/m is present.
    'II64',         # Instruction invalid in 64 bit mode.
    'F64',          # Operand size forced to 64 bit.
    'D64',          # Operand size defaults to 64 bit.
    'O64',          # Instruction valid only in 64 bit mode.
    'SSECONDB',     # Instruction has condition byte.
    'COND',         # Instruction has predicated encoded in lower 4 bit of the opcode.
    'VSIB',         # Instruction uses VSIB addressing.
    'MIB',          # Instruction uses MIB addressing.
    'LIG',          # *vex.L is ignored.
    'WIG',          # *vex.W is ignored.
    '3DNOW',        # Instruction uses 3dnow encoding.
    'MMASK',        # Instruction must have mask specified (mask cannot be k0).
    'NOMZ',         # Zeroing not allowed with memory addressing.
    'LOCKSP',       # Special lock - MOV CR on amd can use LOCK to access CR8 in 32 bit mode.
    'NOL0',         # Vector length 128 not supported.
    'NOA16',        # 16 bit addressing not supported.
    'NO66',         # 0x66 prefix causes #UD.
    'NORIPREL',     # RIP relative addressing not supported.
    'VECT',         # Vector instruction.
    'S66',          # 0x66 prefix changes length even if it is in special map (66, f2, f3).
    'BITBASE',      # Instruction uses bitbase addressing.
    'AG',           # Instruction uses address generation, no memory access.
    'SHS',          # Instruction accesses the shadow stack.
    'MFR',          # The Mod inside Mod R/M is forced to register. No SIB/disp present.
    'CETT',         # Instruction is CET tracked.
    'OP1DEF',       # Operand 1 is default (implicit).
    'OP2DEF',       # Operand 2 is default (implicit).
    'OP2SEXO1',     # Operand 2 is sign-extended to the size of the first operand.
    'OP3SEXO1',     # Operand 3 is sign-extended to the size of the first operand.
    'OP1SEXDW',     # Operand 1 is sign-extended to the size of the default word.
    'PREFIX',       # Prefix.
    'SERIAL',       # Instruction is serializing.
    'SIBMEM',       # Instruction uses sibmem addressing (AMX instructions).
    'I67',          # Ignore the address size override (0x67) prefix in 64 bit mode.
    'IER',          # Ignore embedded rounding for the instruction.
}

#
# Explicit operands types.
#
valid_optype = [
    'A',            # Direct address: the instruction has no ModR/M byte; the address of the 
                    # operand is encoded in the instruction. No base register, index register, 
                    # or scaling factor can be applied (for example, far JMP (EA)).
    'B',            # The VEX.vvvv field of the VEX prefix selects a general purpose register.
    'C',            # The reg field of the ModR/M byte selects a control register (for example,
                    # MOV (0F20, 0F22)).
    'D',            # The reg field of the ModR/M byte selects a debug register (for example,
                    # MOV (0F21,0F23)).
    'E',            # A ModR/M byte follows the opcode and specifies the operand. The operand 
                    # is either a general-purpose register or a memory address. If it is a
                    # memory address, the address is computed from a segment register and any
                    # of the following values: a base register, an index register, a scaling 
                    # factor, a displacement.
    'F',            # EFLAGS/RFLAGS Register.
    'G',            # The reg field of the ModR/M byte selects a general register (for example,
                    # AX (000)).
    'H',            # The VEX.vvvv field of the VEX prefix selects a 128-bit XMM register or a 
                    # 256-bit YMM register, determined by operand type. For legacy SSE 
                    # encodings this operand does not exist, changing the instruction to
                    # destructive form. Addition: 512 bit ZMM register may also be selected in 
                    # EVEX encodings.
    'I',            # Immediate data: the operand value is encoded in subsequent bytes of the 
                    # instruction.
    'J',            # The instruction contains a relative offset to be added to the instruction 
                    # pointer register (for example, JMP (0E9), LOOP).
    'K',            # The operand is the stack.
    'L',            # The upper 4 bits of the 8-bit immediate selects a 128-bit XMM register 
                    # or a 256-bit YMM register, determined by operand type. (the MSB is 
                    # ignored in 32-bit mode). Addition: a 512 bit ZMM register may also be 
                    # selected using EVEX encoding.
    'M',            # The ModR/M byte may refer only to memory (for example, BOUND, LES, LDS, 
                    # LSS, LFS, LGS, CMPXCHG8B).
    'N',            # The R/M field of the ModR/M byte selects a packed-quadword, MMX 
                    # technology register. 
    'O',            # The instruction has no ModR/M byte. The offset of the operand is coded 
                    # as a word or double word (depending on address size attribute) in the 
                    # instruction. No base register, index register, or scaling factor can be 
                    # applied (for example, MOV (A0-A3)).
    'P',            # The reg field of the ModR/M byte selects a packed quadword MMX technology
                    # register.
    'Q',            # A ModR/M byte follows the opcode and specifies the operand. The operand 
                    # is either an MMX technology register or a memory address. If it is a 
                    # memory address, the address is computed from a segment register and any 
                    # of the following values: a base register, an index register, a scaling 
                    # factor, and a displacement.
    'R',            # The R/M field of the ModR/M byte may refer only to a general register 
                    # (for example, MOV (0F20-0F23)).
    'S',            # The reg field of the ModR/M byte selects a segment register (for example, MOV (8C,8E)).
    'T',            # The reg field of the ModR/M byte selects a test register (for example, MOV (0F24, 0F26)).
    'U',            # The R/M field of the ModR/M byte selects a 128-bit XMM register or a 256-bit YMM register, 
                    # determined by operand type. Addition: a 512-bit ZMM register may also be selected using EVEX 
                    # encodings.
    'V',            # The reg field of the ModR/M byte selects a 128-bit XMM register or a 256-bit YMM register, 
                    # determined by operand type. Addition: a 512-bit ZMM register may also be selected using 
                    # EVEX encodings.
    'W',            # A ModR/M byte follows the opcode and specifies the operand. The operand is either a 128-bit 
                    # XMM register, a 256-bit YMM register (determined by operand type), or a memory address. If it is 
                    # a memory address, the address is computed from a segment register and any of the following values:
                    # a base register, an index register, a scaling factor, and a displacement. Addition:a 512-bit ZMM 
                    # register may also be selected # using EVEX encodings.
    'X',            # Memory addressed by the DS:rSI register pair (for example, MOVS, CMPS, OUTS, or LODS).
    'Y',            # Memory addressed by the ES:rDI register pair (for example, MOVS, CMPS, INS, STOS, or SCAS).
    'Z',            # The low 3 bits inside the opcode select a general purpose register. R field inside REX may
                    # extend it.
    'rB',           # The reg field selects a BND register.
    'mB',           # The rm field selects A BND register or a memory location.
    'rK',           # The reg field selects a mask register.
    'vK',           # The vvvv field of the VEX prefix selects a mask register.
    'mK',           # The rm field selects e mask register.
    'aK',           # The aaa field inside evex selects a mask register which is used for masking of a destination 
                    # operand.
    'rM',           # The reg field inside modrm encodes the base address of a memory operand. Default segment is ES.
    'mM',           # The rm  field inside modrm encodes the base address of a memory operand, iregardless of the mod 
                    # fields. Default segment is DS.
    'rT',           # The reg field inside modrm encodes a TMM register (AMX extension).
    'mT',           # The rm field inside modrm encodes a TMM register (AMX extension).
    'vT',           # The v field inside vex encodes a TMM register (AMX extension).
    'm2zI',         # Bits [1,0] of the immediate byte which selects the fourth register.
]

# Operand sizes.
valid_opsize = [
    'a',            # Two one-word operands in memory or two double-word operands in memory, 
                    # depending on operand-size attribute (used only by the BOUND instruction).
    'b',            # Byte, regardless of operand-size attribute.
    'c',            # Byte or word, depending on operand-size attribute.
    'd',            # Doubleword, regardless of operand-size attribute.
    
    'dq',           # Double-quadword, regardless of operand-size attribute (XMM register or 
                    # 128 bit memory location). A smaller quantity from the 128 bit register may be accessed.

    'e',            # eighth = word or dword or qword.
    'f',            # fourth = dword or qword or oword.
    'h',            # half = qword or oword or yword.
    'n',            # normal = 128, 256 or 512 bits, depending on vector length.
    'u',            # 256 or 512 bit, depending on vector length.

    # VSIB addressing
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
    
    # MIB addressing
    'mib',          # MIB addressing, the base & the index are used to form a pointer.
    
    # Stack sizes and partial access
    'v2',           # Two stack words.
    'v3',           # Three stack words.
    'v4',           # Four stack words.
    'v8',           # Eight stack words.

    # These are aliased over 'dq.*' encodings.
    'o',            # Always 128 bits/2 QWORDs. Same as 'dq'.
    'oq',           # 512 bit regardless the operand size/vector length.
    'p',            # 32, 48 or 80 bits pointer, depending on operand size.
    'pd',           # 128 bit or 256 bit double-precision fp data.
    'ps',           # 128 bit or 256 bit single-prevision fp data.
    'q',            # Always 1 QWORD.
    'qq',           # Always 4 QWORDs.
    's',            # 6-byte or 10-byte pseudo-descriptor.
    'sd',           # Scalar element of 128 bit double-precision fp data.
    'ss',           # Scalar element of 128 bit single-precision fp data.
    'v',            # WORD, DWORD or QWORD, depending on operand size.
    'w',            # Always WORD.
    'x',            # 128 bit, 256 bit, depending on operand size.
    'y',            # DWORD or QWORD, depending on operand size.
    'yf',           # Always QWORD in 64 bit mode and DWORD in 16/32 bit mode.
    'z',            # WORD for 16 bit op size, DWORD for 32 & 64 bit operand size.
    '?',            # Unknown operand size. Depends on many factors (for example, XSAVE).
    '0',            # Used for instructions that do not actually access any memory.
    'asz',          # The size of the operand is given by the current addressing mode.
    'ssz',          # The size of the operand is given by the current stack mode.
    'fa',           # FPU integer binary coded decimal.
    'fw',           # FPU real word.
    'fd',           # FPU real dword.
    'fq',           # FPU real qword.
    'ft',           # FPU real extended.
    'fe',           # FPU environment.
    'fs',           # FPU state.
    'l',            # Either a 64 bit or a 128 bit operand size (used by BNDMOV).
    'rx',           # 512 bytes extended state.
    'cl',           # 32/64/128 bytes - the size of one cache line.
    '12',           # 4 bytes (0) + 8 bytes (old SSP), used by SAVEPREVSSP.
    't',            # A tile register. The size varies dependning on execution environment, but can be as high as 1K.

    '384',          # 384 bits representing a Key Locker handle.
    '512',          # 512 bits representing a Key Locker handle.
]

# Implicit/fixed operands. Self explanatory.
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
    'ST(0)'    : ('ST(0)',      'ft'),  # ST(0) register.
    'ST(i)'    : ('ST(i)',      'ft'),  # ST(1) register.
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

    # Implicit memory operands.
    'pBXALb'   : ('pBXAL',      'b'),   # Implicit [RBX + AL], as used by XLAT.
    'pDIq'     : ('pDI',        'q'),   # Implicit qword [RDI].
    'pDIdq'    : ('pDI',        'dq'),  # Implicit xmmword [RDI].
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
}

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

valid_flag_op = [
    'm',            # modified.
    't',            # tested.
    '0',            # cleared.
    '1',            # set.
    'u',            # undefined.
    'n',            # not accessed.
]

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
    "ring",
    "mode",
    "vmx",
    "other",
]

valid_ring_modes = [
    "r0", 
    "r1", 
    "r2", 
    "r3",
]

valid_mode_modes = [
    "real",  
    "v8086", 
    "prot",  
    "compat",
    "long",  
]

valid_vmx_modes = [
    "vmxr",
    "vmxn",
    "vmxr_seam",
    "vmxn_seam",
    "vmx_off",
]

valid_other_modes = [
    "smm",
    "smm_off",
    "sgx",
    "sgx_off",
    "tsx",
    "tsx_off",
]

valid_mode_map = {
    "ring"  : valid_ring_modes,
    "mode"  : valid_mode_modes,
    "vmx"   : valid_vmx_modes,
    "other" : valid_other_modes,
}


valid_decorators = [
    '{K}',          # Masking support.
    '{z}',          # Zeroing support.
    '{sae}',        # Surpress All Exceptions.
    '{er}',         # Embedded Rounding.
    '|B32',         # Broadcast 32.
    '|B64',         # Broadcast 64.
]

valid_tuples = [
    'fv',           # Full Vector, Load+Op (Full Vector Dword/Qword).
    'hv',           # Half Vector, Load+Op (Half Vector).
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

class InvalidEncodingException(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr(self.value)

class ParseLineException(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr(self.value)


def reverse_dict(d):
    r = {}
    for k in d:
        r[d[k]] = k
    return r

def my_str(x):
    if x is None:
        return x
    else:
        return str(x)


#
# CPUID feature flags.
#
class CpuidFeatureFlag():
    def __init__(self, finfo):
        self.Name = finfo["name"]
        self.Leaf = finfo["leaf"]
        self.SubLeaf = finfo["subleaf"]
        self.Reg = finfo["reg"]
        self.Bit = finfo["bit"]

    def __str__(self):
        return "%s: %s, %s, %s, %s" % (self.Name, self.Leaf, self.SubLeaf, self.Reg, self.Bit)


#
# Operand description
#
class Operand():
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
                op = op.replace(dec, "")

        # Handle hard-coded operators - those that are implicit/are not encoded anywhere.
        if op in valid_impops:
            self.Type, self.Size = valid_impops[op][0], valid_impops[op][1]
        # Now handle explicit operators.
        else:
            # Attempt a match inside the explicit operands map.
            for opt in valid_optype:
                if op.startswith(opt):
                    self.Type = opt
                    op = op.replace(opt, "")
                    break

            # Now the operand size. After parsing the decorator and the operand type, we should be left with
            # the operand size only.
            if self.Type in ['rK', 'mK', 'vK', 'aK'] and not op in valid_opsize:
                self.Size = 'q'
            elif op in valid_opsize:
                self.Size = op
            else:
                raise InvalidEncodingException('Invalid operand size specified: ' + orig)

        if self.Type in operand_encoding:
            self.Encoding = operand_encoding[self.Type]
        elif self.Raw in operand_encoding:
            self.Encoding = operand_encoding[self.Raw]

        if imp and 'OPDEF' not in self.Flags:
            self.Flags.append('OPDEF')

        self.Access = access

    def __str__(self):
        if True:
            return self.Raw


#
# Prefixes.
#
class Prefix():
    def __init__(self, prefix):
        self.Mnemonic = prefix["mnemonic"]
        self.Encoding = prefix["encoding"]

    def __str__(self):
        return self.Mnemonic


#
# Instructions.
#
class Instruction():
    def __init__(self, iinfo):
        # Fill in raw instruction information
        self.Mnemonic = iinfo["mnemonic"]
        self.RawEnc   = iinfo["encoding"]
        self.Flags    = iinfo["flags"]
        self.Prefmap  = iinfo["prefixes"]
        self.Set      = iinfo["set"]
        self.Category = iinfo["cat"]
        self.Class    = iinfo["class"]
        self.Rwm      = iinfo["rwm"]
        self.Id       = iinfo["cff"] or self.Set
        self.Tuple    = iinfo["tuple"]
        self.ExClass  = iinfo["exclass"]
        self.RevFlagsAccess = iinfo["flgaccess"]
        self.Modes    = iinfo["modes"]
        self.FpuFlags = iinfo["fpuflg"]

        # First redirecton class: opcodes
        self.Opcodes = []
        self.Prefixes = []
        self.DecoFlags = []
        # Second redirection class: Modrm
        self.HasModrm = self.ModrmRedirAfterMpref = False
        self.Mod = self.Reg = self.Rm = None
        # Third redirection class: mandatory prefix.
        self.Np = self.MustHave66 = self.MustHaveF2 = self.MustHaveF3 = False
        # Fourth redirection class: operating mode
        self.RedM16 = self.RedM32 = self.RedM64 = False
        # Fifth redirection class: default operand size
        self.RedDs16 = self.RedDs32 = self.RedDs64 = self.RedDDs64 = self.RedFDs64 = False
        # Sixth redirection class: default address size
        self.RedAs16 = self.RedAs32 = self.RedAs64 = False
        # Seventh redirecton class: rex, rex.w, rep, repz
        self.RedRexB = self.RedRexW = self.RedRep = self.Red64 = self.RedF3 = False
        # Misc - vendor
        self.Vendor = None
        # Misc - feature.
        self.Feature = None

        # XOP, VEX and EVEX classes.
        self.Vex = self.Xop = self.Evex = self.Mvex = False
        self.M = self.P = self.L = self.W = None

        # Now parse each info chunk and extract the actual data
        for t in iinfo["encoding"].split(' '):
            if '0x66' == t and not self.Opcodes and not (self.Xop or self.Vex or self.Evex):
                self.Prefixes.append(0x66)
                self.MustHave66 = True
            elif '0xF3' == t and not self.Opcodes and not (self.Xop or self.Vex or self.Evex):
                self.Prefixes.append(0xF3)
                self.MustHaveF3 = True
            elif '0xF2' == t and not self.Opcodes and not (self.Xop or self.Vex or self.Evex):
                self.Prefixes.append(0xF2)
                self.MustHaveF2 = True
            elif 'NP' == t:
                self.Np = True
            elif 'a0xF3' == t:
                self.Prefixes.append(0xF3)
                self.RedF3 = True
            elif 'o64' == t: 
                self.Red64 = True
            elif 'rexw' == t: 
                self.RedRexW = True
            elif 'rexb' == t: 
                self.RedRexB = True
            elif 'rep' == t: 
                self.RedRep = True
            elif 'ds16' == t: 
                self.RedDs16 = True
            elif 'ds32' == t: 
                self.RedDs32 = True
            elif 'ds64' == t: 
                self.RedDs64 = True
            elif 'dds64' == t: 
                self.RedDDs64 = True
            elif 'fds64' == t: 
                self.RedFDs64 = True
            elif 'as16' == t: 
                self.RedAs16 = True
            elif 'as32' == t: 
                self.RedAs32 = True
            elif 'as64' == t: 
                self.RedAs64 = True
            elif t.startswith('/'):
                self.HasModrm = True
                self.Flags.append('MODRM')
                if t.endswith(':mem'):
                    self.Mod = 'mem'
                if t.endswith('reg'):
                    self.Mod = 'reg'
                t = t.replace(':mem', '').replace(':reg', '')
                for i in range(0, 8):
                    if '/%d' % i == t:
                        self.Reg = i
                if re.match(r'0x[0-9a-fA-F]{2}', t[1:]):
                    mrm = int(t[1:], 16)
                    if 0xC0 == (mrm & 0xC0):
                        self.Mod = 'reg'
                    else:
                        self.Mod = 'mem'
                    self.Rm = mrm & 7
                    self.Reg = (mrm >> 3) & 7
            elif 'modrm' == t:
                self.HasModrm = True
                self.Flags.append('MODRM')
            elif t.startswith('mod:'):
                self.Mod = t[4:]
                if self.Mod not in ['mem', 'reg']:
                    raise InvalidEncodingException('Invalid encoding: illegal "mod" modifier')
            elif t.startswith('reg:'):
                self.Reg = t[4:]
                if self.Reg not in ['0', '1', '2', '3', '4', '5', '6', '7']:
                    raise InvalidEncodingException('Invalid encoding: illegal "reg" value')
                self.Reg = int(self.Reg)
            elif t.startswith('rm:'):
                self.Rm = t[3:]
                if self.Rm not in ['0', '1', '2', '3', '4', '5', '6', '7']:
                    raise InvalidEncodingException('Invalid encoding: illegal "rm" value')
                self.Rm = int(self.Rm)
            elif t.startswith('modrmpmp'):
                self.ModrmRedirAfterMpref = True
            elif t == 'xop': 
                self.Xop = True
            elif t == 'vex': 
                self.Vex = True
            elif t == 'evex': 
                self.Evex = True
            elif t == 'mvex': 
                self.Mvex = True
            elif t.startswith('m:'): 
                self.M = t[2:]
                if self.M not in ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C']:
                    raise InvalidEncodingException('Invalid encoding: illegal "mmmmm" value')
                self.M = int(self.M, 16)
            elif t.startswith('p:'):
                self.P = t[2:]
                if not self.P in ['0', '1', '2', '3']:
                    raise InvalidEncodingException('Invalid encoding: illegal "pp" value!')
                self.P = int(self.P)
            elif t.startswith('l:'):
                self.L = t[2:]
                if self.L == '128': 
                    self.L = 0
                elif self.L == '256': 
                    self.L = 1
                elif self.L == '512': 
                    self.L = 2
                elif self.L == 'x':
                    self.L = None
                elif self.L == 'i':
                    self.L = None
                    if 'LIG' not in self.Flags:
                        self.Flags.append('LIG')
                elif self.L in ['0', '1', '2', '3']: 
                    self.L = int(self.L)
                else:
                    raise InvalidEncodingException('Invalid encoding: illegal "l" value!')
            elif t.startswith('w:'):
                self.W = self.RawW = t[2:]
                if self.W == 'x':
                    self.W = None
                elif self.W == 'i':
                    self.W = None
                    if 'WIG' not in self.Flags:
                        self.Flags.append('WIG')
                elif self.W in ['0', '1']:
                    self.W = int(self.W)
                else:
                    raise InvalidEncodingException('Invalid encoding: illegal "w" value!')
            elif re.match(r'0x[0-9a-fA-F]{2}', t):
                self.Opcodes.append(int(t, 16))
            elif t in ['intel', 'amd', 'via', 'cyrix']:
                self.Vendor = t
            elif t in ['mpx', 'cet', 'cldm']:
                self.Feature = t
            elif 'vsib' == t:
                self.HasVsib = True
                if 'VSIB' not in self.Flags:
                    self.Flags.append('VSIB')
            elif 'mib' == t:
                self.HasMib = True
                if 'MIB' not in self.Flags:
                    self.Flags.append('MIB')
            elif 'bitbase' == t:
                self.HasBitbase = True
                if 'BITBASE' not in self.Flags:
                    self.Flags.append('BITBASE')
            elif 'sibmem' == t:
                self.HasSibMem = True
                if 'SIBMEM' not in self.Flags:
                    self.Flags.append('SIBMEM')
            elif t in ['ib', 'iw', 'iz', 'iv', 'id', 'cb', 'cz', 'cv', 'cp', 'is4']:
                # Not used for now, but they must be specified, for a complete instruction encoding specification.
                pass
            elif t.startswith('evex.'):
                tokens2 = t.split('.')
                self.Evex = True
                self.M = self.P = self.L = self.W = 0
                for t2 in tokens2[1:]:
                    # Handle the L specifier
                    if t2 == 'LIG':
                        self.L = None
                        if 'LIG' not in self.Flags:
                            self.Flags.append('LIG')
                    elif t2 == 'LANY':
                        self.L = None
                    elif t2 == '128' or t2 == 'LZ' or t2 == 'L0':
                        self.L = 0
                    elif t2 == '256' or t2 == 'L1':
                        self.L = 1
                    elif t2 == '512' or t2 == 'L2':
                        self.L = 2
                    # Handle the W specifier
                    elif t2 == 'WIG':
                        self.W = None
                        if 'WIG' not in self.Flags:
                            self.Flags.append('WIG')
                    elif t2 == 'WANY':
                        self.W = None
                    elif t2 == 'W0':
                        self.W = 0
                    elif t2 == 'W1':
                        self.W = 1
                    # Handle compressed prefix
                    elif t2 == '66':
                        self.P = 1
                    elif t2 == 'F3':
                        self.P = 2
                    elif t2 == 'F2':
                        self.P = 3
                    # Handle opcode map
                    elif t2 == '0F':
                        self.M = 1
                    elif t2 == '0F38':
                        self.M = 2
                    elif t2 == '0F3A':
                        self.M = 3
                    elif t2 in ['NDS', 'NDD', 'DDS']:
                        pass
                    else:
                        raise InvalidEncodingException('Unknwon new evex token: %s/%s' % (t, t2))
            else:
                raise InvalidEncodingException('Unknown token: %s' % t)

        # Pre-process the explicit operands. The mask register is contained as a decorator, but put it as a direct
        # operand as well. The access flag is already present in rwm.
        if len(iinfo["expops"]) >= 1 and iinfo["expops"][0].find("{K") > 0:
            iinfo["expops"].insert(1, 'aKq')

        # Parse the explicit instruction operands.
        self.ExpOps = self.process_operands(iinfo["expops"], False)

        # Parse the implicit instruction operands.
        self.ImpOps = self.process_operands(iinfo["impops"], True)

        # Post-process the operands. We fill up the flags with additional info based on the operands.
        for op in self.ExpOps:
            for deco in op.Decorators:
                self.DecoFlags.append({'{K}':'MASK', '{z}':'ZERO', '{sae}':'SAE', '{er}':'ER', '|B32':'BROADCAST', '|B64':'BROADCAST'}[deco])
            if op.Type in ['U', 'V', 'W', 'H', 'L'] and 'VECT' not in self.Flags:
                self.Flags.append('VECT')

        # VEX, XOP, EVEX and MVEX instructions are not valid in real or v8086 modes.
        if self.Vex or self.Xop or self.Evex or self.Mvex:
            if 'real' in self.Modes:
                self.Modes.remove('real')
            if 'v8086' in self.Modes:
                self.Modes.remove('v8086')
        if 'long' not in self.Modes and 'II64' not in self.Flags:
            self.Flags.append('II64')
        if 'long' in self.Modes and 'prot' not in self.Modes and 'O64' not in self.Flags:
            self.Flags.append('O64')

        # Split the instruction into encoding entities.
        e = self.split_encoding()
        if self.Vex or self.Xop or self.Evex:
            self.Spec = { "mmmmm" : e[0], "opcodes" : e[1], "modrm" : e[2], "pp" : e[3], "l" : e[4], "w" : e[5] }
        else:
            self.Spec = { "opcodes" : e[0], "modrm" : e[1], "mpre" : e[2], "mode" : e[3], "dsize" : e[4], \
                          "asize" : e[5], "opre" : e[6], "vendor" : e[7], "feature": e[8] }

    def process_operands(self, ops, imp = False):
        p = 1
        res = []
        for op in ops:
            if op == "nil":
                break
            flags = []
            if not imp:
                for f in self.Flags:
                    if f.startswith('OP%d' % p): 
                        flags.append('OP' + f[3:])
                        self.Flags.remove(f)
            else:
                flags.append('OPDEF')
            if not imp:
                res.append(Operand(op, self.Rwm[p - 1], flags, imp))
            else:
                res.append(Operand(op, self.Rwm[len(self.ExpOps) + p - 1], flags, imp))
            p += 1
        return res

    def split_encoding(self):
        if self.Vex or self.Xop or self.Evex or self.Mvex:
            return self.split_encoding_vex()
        else:
            return self.split_encoding_legacy()
            
    def split_encoding_vex(self):
        # First, get the 'mmmmm' - VEX decoding table.
        mmmmm = '%x' % self.M
        # Now get the opcode. Should be only one.
        opcodes = ['%02x' % x for x in self.Opcodes]
        # Get the modrm redirections.
        modrm = { "mod": self.Mod, "reg": my_str(self.Reg), "rm": my_str(self.Rm), "modpost": None }
        # Get the pp, if any.
        pp = my_str(self.P)
        # Get the l, if any.
        l = my_str(self.L)
        # Get the w, if any.
        w = my_str(self.W)
        return (mmmmm, opcodes, modrm, pp, l, w)

    def split_encoding_legacy(self):
        # First redirection class, the opcode.
        opcodes = ['%02x' % x for x in self.Opcodes]
        # Second redirection class, modrm
        modrm = { "mod": self.Mod, "reg": my_str(self.Reg), "rm": my_str(self.Rm), "modpost": None }
        # Third redirection class, mandatory prefixes
        mprefixes = []
        if self.MustHaveF2: 
            mprefixes.append('F2')
        if self.MustHaveF3: 
            mprefixes.append('F3')
        if self.MustHave66: 
            mprefixes.append('66')
        if self.Np:
            mprefixes.append('NP')
        if len(mprefixes) == 0 and (not (self.Xop or self.Vex or self.Evex or self.Mvex)) and\
                                   (self.Opcodes[0] == 0x0F and self.Opcodes[1] in [0x3A, 0x38]):
            mprefixes.append(None)
        # Fourth redirection class, operating mode.
        mode = []
        if self.RedM16: 
            mode.append('m16')
        elif self.RedM32: 
            mode.append('m32')
        elif self.RedM64: 
            mode.append('m64')
        # Fifth redirection class, default operand size.
        dsize = []
        if self.RedDs16: 
            dsize.append('ds16')
        elif self.RedDs32: 
            dsize.append('ds32')
        elif self.RedDs64: 
            dsize.append('ds64')
        elif self.RedDDs64: 
            dsize.append('dds64')
        elif self.RedFDs64: 
            dsize.append('fds64')
        # Sixth redirection class, default address size.
        asize = []
        if self.RedAs16: 
            asize.append('as16')
        elif self.RedAs32: 
            asize.append('as32')
        elif self.RedAs64: 
            asize.append('as64')
        # Seventh redirection class, REX prefix, REX.W, 64 bit mode, 0xF3, SIB. The important aspect here is that unlike
        # the other classes, this is not exhaustive - if an instruction does not fit in any of the entries, it 
        # will default to index 0 (and it will not return invalid encoding, unless entry 0 is invalid).
        oprefixes = []
        if self.RedRexB: 
            oprefixes.append('rexb')
        if self.RedRexW: 
            oprefixes.append('rexw')
        if self.Red64: 
            oprefixes.append('64')
        if self.RedF3: 
            oprefixes.append('aF3')
        if self.RedRep: 
            oprefixes.append('rep')
        # Vendor redirection, if any.
        return (opcodes, modrm, mprefixes, mode, dsize, asize, oprefixes, self.Vendor, self.Feature)


    def __str__(self):
        # Get the operands
        ops = ''
        for o in self.ExpOps:
            ops += o.__str__() + ','
        ops = ops[:-1]

        # Return a text reprezentation of the encoding
        return (self.Mnemonic + ' ' + ops).strip()



def parse_entry(entry, template_flags = {}, template_cpuid = {}, template_modes = {}):
    # make sure this is not a comment. Skip comments.
    if entry.startswith('#') or len(entry) < 4: return None

    try:
        # Preprocess: remove comments, CR/LF
        com = entry.find('#')
        x = entry.replace('\x0D', '').replace('\x0A', '')
        if -1 != com: x = entry[:com]
                
        # Space can't be the first character.
        if x[0] == ' ': 
            raise ParseLineException('Space cannot be the first character!')
    
        # Extract the mnemonic
        mnemonic = x[0:x.find(' ')].strip()

        # Extract the explicit operands
        x = x[x.find(' '):].strip()
        expops = x[:x.find(' ')].split(',')
        if len(expops) == 1 and expops[0] == 'nil': expops = []
            
        # Extract the implicit operands
        x = x[x.find(' '):].strip()
        impops = x[:x.find(' ')].split(',')
        if len(impops) == 1 and impops[0] == 'nil': impops = []

        # Extract the encoding
        x = x[x.find('[')+1:]
        encoding = x[:x.find(']')].strip()

        # Extract the flags, class, set, category, encoding, prefmap
        attributes = prefmap = isaset = category = iclass = adop = rwm = None
        cff = tuple = flgaccess = modes = exclass = fpuflg = None

        x = x[x.find(']')+1:].strip()

        while x:
            start = x.find(':')
            end   = x.find(',')
            if start == -1: 
                break
            if end == -1: 
                end = len(x)
            token = x[:start].strip()
            value = x[start+1:end].strip()

            # parse token
            if token == 'a':                    # Instruction attributes.
                attributes = value.split('|')
            elif token == 'p':                  # Accepted prefixes.
                prefmap = value.split('|')
            elif token == 's':                  # Instruction set
                isaset = value
            elif token == 't':                  # Instruction type
                category = value
            elif token == 'c':                  # Instruction class. Defaults to the mnemonic if not specified.
                iclass = value
            elif token == 'w':                  # Read/write map
                rwm = value.split('|')
            elif token == 'i':                  # CPUID.
                cff = value
            elif token == 'l':                  # tuple
                tuple = value
            elif token == 'e':
                exclass = value
            elif token == 'f':                  # Flags access
                flgaccess = []
                for v in value.split('|'):
                    if v in template_flags:
                        flgaccess += template_flags[v].split('|')
                    else:
                        flgaccess.append(v)
            elif token == 'u':
                fpuflg = ['u', 'u', 'u', 'u'] # each one is undefined.
                for v in value.split('|'):
                    flg, acc = v.split('=')
                    if flg not in ['C0', 'C1', 'C2', 'C3']:
                        raise ParseLineException('Unknown FPU flag: %s' % flg)
                    if acc not in ['0', '1', 'm', 'u']:
                        raise ParseLineException('Unknown FPU flag access: %s' % acc)
                    fpuflg[int(flg[1])] = acc
            elif token == 'm':                  # CPU modes.
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
                    groups[g]["negated"] = False
                    groups[g]["specified"] = False
                    groups[g]["modes"] = []

                for tm in tmodes:
                    m, v = tm.split('=')
                    for vx in v.split('+'):
                        negated = False
                        if vx.startswith('!'):
                            vx = vx[1:]
                            groups[m]["negated"] = True
                        if m not in valid_mode_groups:
                            raise ParseLineException('Unknown CPU mode group specified: %s' % m)
                        if vx not in valid_mode_map[m]:
                            raise ParseLineException('Mode %s is not valid for mode group %s; it can be one of [%s]' % 
                                                     (vx, m, ','.join(valid_mode_map[m])))
                        groups[m]["specified"] = True
                        groups[m]["modes"].append(vx)

                for g in groups:
                    if not groups[g]["specified"]:
                        modes += valid_mode_map[g]
                    elif not groups[g]["negated"]:
                        modes += groups[g]["modes"]
                    else:
                        modes += [x for x in valid_mode_map[g] if x not in groups[g]["modes"]]
            else:
                raise ParseLineException('Unknown token specified: %s' % token)

            # Advance
            if -1 == x.find(','):
                x = ''
            else:
                x = x[x.find(',')+1:].strip()

        if attributes is None:
            attributes = []
        if prefmap is None:
            prefmap = []
        if isaset is None:
            isaset = 'UNKNOWN'
        if category is None:
            category = 'UNKNOWN'
        if iclass is None:
            iclass = mnemonic
        if rwm is None:
            rwm = []
        if cff is None:
            cff = None
        if modes is None:
            # No mode specified, assume validity in all modes.
            modes = []
            modes += valid_cpu_modes
        if flgaccess is None:
            flgaccess = []
        if fpuflg is None:
            # fpuflg[x] is for Cx (fpuflg[0] = C0, fpuflg[1] = C1, etc.)
            # u = undefined, m = modified, 0 = cleared to 0, 1 = set to 1.
            fpuflg = ['u', 'u', 'u', 'u']

        # Validate the tokens.
        # The set can be anything.
        # The type can be anything.
        # The iclass can be missing, it will default to the mnemonic.

        # The read/write map must have the same size as the number of operands.
        if len(rwm) < len(expops) + len(impops):
            raise ParseLineException('Invalid number of operand access specifiers: provided %d, expecting at least %d' % 
                                     (len(rwm), len(expops) + len(impops)))
        for r in rwm:
            if r not in valid_access:
                raise ParseLineException('Unknown access specifier "%s", expecting one of [%s]' % 
                                         (r, ','.join(valid_access)))
        # The CPUID can be anything, even if it doesn't match something specified in cpuid.dat.

        # The modes must be one of the valid modes.
        for m in modes:
            if m.startswith('!'): 
                m = m[1:]
            if m not in valid_cpu_modes:
                raise ParseLineException('Unknown CPU mode specifier "%s", expecting one of [%s]' % 
                                         (m, ','.join(valid_cpu_modes)))

        # Validate the prefixes.
        for p in prefmap:
            if p not in valid_prefixes:
                raise ParseLineException('Unknown prefix specifier "%s", expecting one of [%s]' % 
                                         (p, ','.join(valid_prefixes)))

        # Validate the tuples.
        if tuple and tuple not in valid_tuples:
            raise ParseLineException('Unknown tuple specifier "%s", expecting one of [%s]' % 
                                     (tuple, ','.join(valid_tuples)))

        # Validate the attributes.
        for a in attributes:
            if a not in valid_attributes:
                raise ParseLineException('Unknown attribute specifier "%s", expecting one of [%s]' % 
                                         (a, ','.join(valid_attributes)))

        # Validate the flags.
        revflg = {}
        for m in valid_flag_op:
            revflg[m] = []
        for flg in flgaccess:
            f, m = flg.split('=')
            if m not in valid_flag_op:
                raise ParseLineException('Unknow flag access specifier "%s", expecting one of [%s]' % 
                                         (m, ','.join(valid_flag_op)))
            if f not in valid_flags:
                raise ParseLineException('Unknow flag specifier "%s", expecting one of [%s]' % 
                                         (f, ','.join(valid_flas)))
            revflg[m].append(f)
        flgaccess = revflg

        iinfo = {
            "mnemonic"  : mnemonic,     # Mnemonic
            "expops"    : expops,       # Explicit operands
            "impops"    : impops,       # Implicit operands
            "encoding"  : encoding,     # Encoding
            "flags"     : attributes,   # Instruction attributes
            "prefixes"  : prefmap,      # Accepted prefixes
            "set"       : isaset,       # Instruction set
            "cat"       : category,     # Instruction category
            "class"     : iclass,       # Instruction class
            "rwm"       : rwm,          # Read/write operands map 
            "cff"       : cff,          # CPUID feature flag
            "tuple"     : tuple,        # Tuple type, for EVEX instruxtions
            "exclass"   : exclass,      # Exception class, for SSE/VEX/EVEX instructions
            "flgaccess" : flgaccess,    # RFLAGS access
            "modes"     : modes,        # Valid operating modes
            "fpuflg"    : fpuflg,       # FPU flags access (C0, C1, C2, C3), valid for x87 instructions only
        }

        if 'PREFIX' in attributes: 
            return None

        try:
            ins  = Instruction(iinfo)
        except:
            raise

    except Exception as e:
        raise

    return ins



def parse_ins_file(fpath, template_flags = {}, template_cpuid = {}, template_modes = {}):
    instructions = []
    lcount = 0
    for line in open(fpath, 'rt'):
        lcount += 1
        try:
            ins = parse_entry(line, template_flags, template_cpuid, template_modes)
            if ins: instructions.append(ins)
        except Exception as e:
            print('ERROR: Parsing failed at %s:%d: %s' % (fpath, lcount, e))
            raise
    return instructions



def parse_pre_file(fpath):
    prefixes = []
    for line in open(fpath, 'rt'):
        # Ignore comments.
        if line.startswith('#'):
            continue
        res = re.findall(r'([^\s]+)\s*\[\s*(0x[0-9a-fA-F]+)\]', line)
        if not res:
            continue
        res = res[0]
        pref = {}
        pref["mnemonic"] = res[0]
        pref["encoding"] = res[1]
        prefixes.append(Prefix(pref))
    return prefixes



def parse_cff_file(fpath):
    features = []
    for line in open(fpath, 'rt'):
        if line.startswith('#'):
            continue
        res = re.findall(r'([^\s]+)\s+:\s+(0x[0-9a-fA-F]+),\s+(0x[0-9a-fA-F]+),\s+(EAX|ECX|EDX|EBX),\s+(\d+)', line)
        if not res:
            continue
        res = res[0]
        cffi = {}
        cffi["name"] = res[0]
        cffi["leaf"] = res[1]
        cffi["subleaf"] = res[2]
        cffi["reg"] = res[3]
        cffi["bit"] = res[4]
        features.append(CpuidFeatureFlag(cffi))
    return features


def parse_flags_file(fpath):
    flags = {}
    for line in open(fpath, 'rt'):
        if line.startswith('#'):
            continue
        res = re.findall(r'([^\s]+)\s+:([^$]+)', line)
        if not res:
            continue
        res = res[0]
        flags[res[0]] = res[1].strip('\n\r ')
    return flags


def parse_modess_file(fpath):
    modes = {}
    for line in open(fpath, 'rt'):
        if line.startswith('#'):
            continue
        res = re.findall(r'([^\s]+)\s+:([^$]+)', line)
        if not res:
            continue
        res = res[0]
        modes[res[0]] = res[1].strip('\n\r ')
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
    flags = parse_flags_file('%s/flags.dat' % sys.argv[1])

    # Parse the prefixes
    prefixes = parse_pre_file('%s/prefixes.dat' % sys.argv[1])

    # Parse the cpuid feature flags and extract each feature
    features = parse_cff_file('%s/cpuid.dat' % sys.argv[1])

    # Parse the modes file.
    modes = parse_modess_file('%s/modes.dat' % sys.argv[1])

    # Parse the instruction file and extract the instructions
    instructions = []
    for fn in glob.glob('%s/table*.dat' % sys.argv[1]):
        instructions += parse_ins_file(fn, flags, features)

    # Sort the instructions.
    instructions = sorted(instructions, key = lambda x: x.Mnemonic)
    for i in range(0, len(instructions)):
        print(instructions[i])

    for i in range(0, len(prefixes)):
        print(prefixes[i])

    features = sorted(features, key = lambda x: x.Name)
    for i in range(0, len(features)):
        print(features[i])
