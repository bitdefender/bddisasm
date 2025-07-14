#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import re

from isg_x86.common import *
from isg_x86 import decorators


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
    'I1',           # Immediate encoded in instruction bytes. This is the first of two immediates.
    'I2',           # Immediate encoded in instruction bytes. This is the second of two immediates.
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


class Operand:
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
    def __init__(self, op: str, access: str, flags: list[str], imp: bool = False):
        self.Raw = op
        self.Type = 0
        self.Size = 0
        self.Flags = flags
        self.Decorators = []
        self.Access = []
        self.Block = 0
        self.Encoding = 'S'
        self.Implicit = imp

        # Handle block registers.
        op = self.process_block_regs(op)
        
        # Handle the decorators.
        op = self.process_decorators(op)

        # Handle type & size.
        op = self.process_size_type(op)

        if self.Type in operand_encoding:
            self.Encoding = operand_encoding[self.Type]
        elif self.Raw in operand_encoding:
            self.Encoding = operand_encoding[self.Raw]

        if imp and 'OPDEF' not in self.Flags:
            self.Flags.append('OPDEF')

        self.Access = access
        
    def process_block_regs(self, op: str) -> str:
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
        return op
        
    def process_decorators(self, op: str) -> str:
        for dec in decorators.valid_decorators:
            if -1 != op.find(dec):
                # Found decorator.
                self.Decorators.append(dec)
                # Remove it from the opstring.
                op = op.replace(dec, '')
        return op
        
    def process_size_type(self, op: str) -> str:
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
                raise InvalidSpecificationException(
                    "Invalid operand size specified: " + self.Raw)
        return op

    def is_memory_modrm(self):
        # Operand is memory only if ModRm.mod is 0, 1 or 2.
        return self.Type in ['M', 'E', 'Q', 'W', 'mB', 'mK']

    def is_memory_default(self):
        # Operand is memory without ModRm, or, if ModRm present, irregardless of ModRm.mod.
        return self.Type in ['O', 'X', 'Y', 'K', 'rM', 'mM']

    def is_memory_implicit(self):
        # Operand is implicit memory access.
        return self.Type in ['pDI', 'pBXAL', 'pAX', 'pCX', 'pBP', 'SHS', 'SHS0', 'SHSP', 'SMT', 'DMT']

    def is_cs(self):
        return self.Type in ['CS']

    def is_rip(self):
        return self.Type in ['rIP']

    def is_stack(self):
        return self.Type in ['K']

    def __str__(self):
        return self.Raw
