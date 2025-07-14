#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import re

from isg_x86.common import *
from isg_x86 import operand
from isg_x86 import attributes
from isg_x86 import prefix_map
from isg_x86 import cpu_modes
from isg_x86 import operand_access
from isg_x86 import flags_access
from isg_x86 import fpu_access
from isg_x86 import exception_type
from isg_x86 import simd_exceptions
from isg_x86 import evex_tuple
from isg_x86 import evex_mode
from isg_x86 import decorators


class Instruction:
    def __init__(self, mnemonic: str, expops: list[str], impops: list[str], encoding: str, meta: list[str]):
        # Fill in raw instruction information
        self.Mnemonic   = mnemonic
        self.RawExpOps  = expops
        self.RawImpOps  = impops
        self.RawEnc     = encoding
        self.RawMeta    = meta
        
        self.Class      = self.Mnemonic
        self.Id         = 'UNKNOWN'
        self.Set        = 'UNKNOWN'
        self.Category   = 'UNKNOWN'
        self.Attributes = attributes.Attributes()
        self.Prefmap    = prefix_map.PrefixMap()
        self.Rwm        = operand_access.OperandAccessMap()
        self.Tuple      = evex_tuple.EvexTuple()
        self.ExType     = exception_type.ExceptionType()
        self.EvexMode   = evex_mode.EvexMode()
        self.Rflags     = flags_access.FlagsAccess()
        self.FpuFlags   = fpu_access.FpuFlagsAccess()
        self.SimdExc    = simd_exceptions.SimdExceptions()
        self.Modes      = cpu_modes.CpuModes()
        self.Decorators = decorators.Decorators()

        # Instruction payload bytes. This is a value that tells the decoder what kind (and how many bytes) of subsequent
        # bytes the instruction contains. This includes immediates, relative offsets, moffset, absolute address or SSE
        # immediates. This allows us to decode an instruction without parsing each operand.
        self.Ipb        = None

        # Pre-decoded information about access.
        self.CsAccess   = []
        self.RipAccess  = []
        self.RflAccess  = []
        self.Mem1Access = []
        self.Mem2Access = []
        self.StkAccess  = []
        self.StkWords   = 0

        # Assume no XOP/VEX/EVEX (i.e., legacy encoding)
        self.Xop = self.Vex = self.Evex = False

        # Pre-process the mnemonic, which may contain decorators.
        self.pre_process_mnemonic()
        
        # Pre-process the explicit operands. The mask register is contained as a decorator, but put it as a direct
        # operand as well. The access flag is already present in rwm.
        self.pre_process_operands()

        # Parse meta-information.
        self.process_meta()

        # Parse the instruction encoding.
        self.process_encoding()

        # Parse the explicit instruction operands.
        self.ExpOps = self.process_operands(self.RawExpOps, False)

        # Parse the implicit instruction operands.
        self.ImpOps = self.process_operands(self.RawImpOps, True)

        # Post-process the operands. We fill up the flags with additional info based on the operands.
        self.post_process_operands()

        # VEX, XOP, EVEX instructions are not valid in real or v8086 modes.
        self.post_process_modes()

        # Augment the mnemonic with NF, ZU, conditions, etc.
        self.post_process_mnemonic()

        # Process the instruction filters. These are essentially #UD conditions that are checked during lookup.
        self.process_filters()

        # Fill operand (CS, RIP, flags, stack, memory) access info.
        self.fill_operand_access_info()


    def process_meta(self):
        """
        Extract all the meta-data from the instruction spec, and fill in class attributes.
        """
        for y in self.RawMeta:
            token, value = y.strip().split(':')
            
            match token:
                case 'c': self.Class = value
                case 't': self.Category = value
                case 's': self.Set = self.Id = value
                case 'i': self.Id = value
                case 'a': self.Attributes.add(value)
                case 'p': self.Prefmap.add(value)
                case 'w': self.Rwm.add(value)
                case 'l': self.Tuple.add(value)
                case 'v': self.EvexMode.add(value)
                case 'e': self.ExType.add(value)
                case 'f': self.Rflags.add(value)
                case 'u': self.FpuFlags.add(value)
                case 'm': self.Modes.add(value)
                case 'x': self.SimdExc.add(value)
                case  _ : raise InvalidSpecificationException("Unknown token specified: %s" % token)
                
        # The read/write map must have the same size as the number of operands.
        if self.Rwm.count() < len(self.RawExpOps) + len(self.RawImpOps):
            raise InvalidSpecificationException(
                "Invalid number of operand access specifiers: provided %d, expecting %d" % 
                (self.Rwm.count, len(self.RawExpOps) + len(self.RawImpOps)))


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
            'lpdf'        : [], # Combined pp, l, nd & nf fields.
        }

        # Initially, we don't have any filters (#UD condition).
        self.Filters = [
        ]

        had_modrm = False

        # Now parse each info chunk and extract the actual data
        for t in self.RawEnc.split(' '):
            match t:
                case '': 
                    continue # Skip empty tokens
                case 'xop': 
                    self.Xop = True
                case 'vex': 
                    self.Vex = True
                case 'evex': 
                    self.Evex = True
                case 'NP' | '0x66' | '0xF3' | '0xF2' if not self.Encoding['opcode'] and not (self.Xop or self.Vex or self.Evex):
                    # Mandatory prefixes.
                    self.Encoding['prefix'].append('P' + t) # Prefix with P, so we don't confuse this with an opcode.
                case 'repz' | 'mo64' | 'rexw' | 'rexb' | 'rep' | 'riprel' | 'rex2' | 'rex2w':
                    # Auxiliary conditions.
                    self.Encoding['auxiliary'].append(t)
                case 'ds16' | 'ds32' | 'ds64' | 'dd64' | 'df64':
                    # Data size.
                    self.Encoding['dsize'].append(t)
                case 'as16' | 'as32' | 'as64':
                    # Address size.
                    self.Encoding['asize'].append(t)
                case 'intel' | 'amd':
                    # Vendor.
                    self.Encoding['vendor'].append(t)
                case 'mpx' | 'cet' | 'cldm' | 'piti' | 'movrs' | 'bhi':
                    # Feature.
                    self.Encoding['feature'].append(t)
                case 'vsib' | 'mib' | 'bitbase' | 'sibmem':
                    # Misc encoding & addressing
                    self.Attributes.add(t.upper())
                    if t in ['vsib', 'sibmem']:
                        self.Encoding['modrmrm'].append('4')
                case 'ib' | 'iw' | 'iz' | 'iv' | 'id' | 'cb' | 'cz' | 'cv' | 'cp' | 'cq' | 'is4':
                    # Misc immediates. Not used for now, but they must be specified, for a complete 
                    # instruction encoding specification.
                    pass
                case _ if t.startswith('/'):
                    # ModR/M.
                    had_modrm = True
                    self.Attributes.add('MODRM')
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
                        raise InvalidSpecificationException(
                            "Invalid encoding: modrm specification is invalid: '%s'!" % t)
                case _ if m := re.match(r'^rm:([0-7])$', t):
                    # Modrm.rm value.
                    self.Encoding['modrmrm'].append(m[1])
                case _ if m := re.match(r'^m:([0-9A-C])$', t): 
                    # Map field inside XOP/VEX/EVEX.
                    self.Encoding['mmmmm'].append(m[1])
                case _ if m := re.match(r'^p:([0-3])$', t): 
                    # Compressed prefix field inside XOP/VEX/EVEX.
                    self.Encoding['pp'].append(m[1])
                case _ if m := re.match(r'^l:([x|i|n|0|1|2|3])$', t):
                    # Vector length field inside XOP/VEX/EVEX.
                    match m[1]:
                        case 'x': pass
                        case 'i': self.Attributes.add('LIG')
                        case 'n': self.Attributes.add('NOL0')
                        case  _ : self.Encoding['l'].append(m[1])
                case _ if m := re.match(r'^w:(x|i|0|1)$', t):
                    # W field inside XOP/VEX/EVEX/REX2.
                    match m[1]:
                        case 'x': pass
                        case 'i': self.Attributes.add('WIG')
                        case _ if 'IWO64' in self.Attributes.Names: self.Encoding['wi'].append(m[1])
                        case _: self.Encoding['w'].append(m[1])
                case _ if m := re.match(r'^nd:(0|1)$', t): 
                    # New Data Destination field inside EVEX.
                    self.Encoding['nd'].append(m[1])
                case _ if m := re.match(r'^nf:(0|1)$', t): 
                    # No Flags field inside EVEX.
                    self.Encoding['nf'].append(m[1])
                case _ if m := re.match(r'^sc:([0-9A-F]{1})$', t): 
                    # Standard Condition Code field inside EVEX.
                    self.Encoding['sc'].append(m[1])
                case _ if re.match(r'^0x[0-9a-fA-F]{2}$', t):
                    # Opcode.
                    if had_modrm:
                        self.Encoding['opcode_last'].append(t)
                    else:
                        self.Encoding['opcode'].append(t)
                case _:
                    raise InvalidSpecificationException(f"Invalid encoding: Unknown token: {t}")

        # Compress l, pp, nd & nf into a single entry, if possible.
        if self.Encoding['pp'] and self.Encoding['l'] and self.Encoding['nd'] and self.Encoding['nf']:
            l = int(self.Encoding['l'][0],  16)
            p = int(self.Encoding['pp'][0], 16)
            d = int(self.Encoding['nd'][0], 16)
            f = int(self.Encoding['nf'][0], 16)
            lpdf = (f << 0) | (d << 1) | (p << 2) | (l << 4)
            self.Encoding['lpdf'] = ['%x' % lpdf]
            self.Encoding['l']  = []
            self.Encoding['pp'] = []
            self.Encoding['nd'] = []
            self.Encoding['nf'] = []


    def process_operands(self, ops: list[str], imp: bool = False) -> list[operand.Operand]:
        """
        Process the provided operands list, and generate a list of Operand objects.
        """
        p, res = 1, []
        for op in ops:
            if op in absent_op:
                break
            flags = []
            if not imp:
                for f in self.Attributes.Names:
                    if f.startswith('OP%d' % p): 
                        flags.append('OP' + f[3:])
                        self.Attributes.Names.remove(f)
            else:
                flags.append('OPDEF')
            if not imp:
                res.append(operand.Operand(op, self.Rwm.Names[p - 1], flags, imp))
            else:
                res.append(operand.Operand(op, self.Rwm.Names[len(self.ExpOps) + p - 1], flags, imp))
            p += 1
        return res
        
    def pre_process_mnemonic(self):
        for vd in decorators.valid_decorators:
            if vd in self.Mnemonic:
                self.Decorators.add(decorators.deco_flags[vd])
                self.Mnemonic = self.Mnemonic.replace(vd, '')
        # Make sure to change the class, if the mnemonic has changed.
        self.Class = self.Mnemonic
        
    def post_process_mnemonic(self):
        sufix = ''

        # Create sufix.
        if 'NF' in self.Decorators.Names:
            # Add the NF suffix, if {NF} is present.
            sufix += 'NF'
        if 'ZU' in self.Decorators.Names:
            # Add the ZU suffix, if {ZU} is present.
            sufix += 'ZU'

        if self.Mnemonic.find('cc') > 0:
            # Add the condition, if needed.
            cond = [
                'O', 'NO', 'C', 'NC', 'Z', 'NZ', 'BE', 'NBE',
                'S', 'NS', 'P', 'NP', 'L', 'NL', 'LE', 'NLE'
            ][int(self.Encoding["opcode"][-1][-1], 16)]

            self.Mnemonic = self.Mnemonic.replace('cc', sufix + cond)
        elif self.Mnemonic.find('sc') > 0:
            # Add the standard condition (EVEX), if needed.
            cond = [
                'O', 'NO', 'C', 'NC', 'Z', 'NZ', 'BE', 'NBE',
                'S', 'NS', 'T', 'F',  'L', 'NL', 'LE', 'NLE'
            ][int(self.Encoding["sc"][0], 16)]

            self.Mnemonic = self.Mnemonic.replace('sc', sufix + cond)
        else:
            self.Mnemonic = self.Mnemonic + sufix


    def pre_process_operands(self):
        if len(self.RawExpOps) >= 1 and self.RawExpOps[0].find('{K') > 0:
            self.RawExpOps.insert(1, 'aKq')

    def post_process_operands(self):
        usesv = usesvp = False
        immt = None
        imms = []
        for op in self.ExpOps:
            for deco in op.Decorators:
                self.Decorators.add(decorators.deco_flags[deco])
            if op.Type in ['U', 'V', 'W', 'H', 'L']:
                self.Attributes.add('VECTOR')
            if op.Type in ['B', 'H', 'vK', 'vT', 'dfv']:
                usesv = True
            if self.Evex:
                if op.Type in ['B', 'H']:
                    usesvp = True
                if 'VSIB' in self.Attributes.Names:
                    usesvp = True
                if self.Encoding["sc"]:
                    usesvp = True
            if op.Type in ['I', 'I1', 'I2']:
                immt = 'I'
                imms.append(op.Size)
            elif op.Type in ['J', 'A']:
                immt = op.Type
                imms.append(op.Size)
            elif op.Type in ['O']:
                immt = op.Type
                imms.append('a')
            elif op.Type in ['L']:
                immt = op.Type
                imms.append('b')
        if immt:
            self.Ipb = immt + '_' + ''.join(imms)
        if self.Xop or self.Vex or self.Evex:
            if not usesv:
                self.Attributes.add('NOV')
            if self.Evex and not usesvp:
                self.Attributes.add('NOVP')

    def fill_operand_access_info(self):
        # Fill information based on operands. This is static information, so there's no point of computing
        # it during decoding phase, as it's fixed for a given instruction.
        for op in self.ExpOps + self.ImpOps:
            match op.Type:
                case 'CS':
                    self.CsAccess  = [op.Access]
                case 'rIP':
                     self.RipAccess = [op.Access]
                case 'F':
                    self.RflAccess = [op.Access]
                case 'K': 
                    self.StkAccess = [op.Access]
                    match op.Size:
                        case 'v' : self.StkWords = 1
                        case 'v2': self.StkWords = 2
                        case 'v3': self.StkWords = 3
                        case 'v4': self.StkWords = 4
                        case 'v5': self.StkWords = 5
                        case 'v8': self.StkWords = 8
                        case _   : raise InvalidSpecificationException("Unknown stack size specified: %s" % op.Size)
                    if op.Access not in self.Mem2Access: self.Mem2Access.append(op.Access)
                case _ if op.is_memory_modrm():
                    # Memory access only when ModRm.mod is mem.
                    self.Mem1Access = [op.Access]
                case _ if op.is_memory_default():
                    # Memory access, unconditional.
                    if op.Access not in self.Mem2Access: self.Mem2Access.append(op.Access)
                case _ if op.is_memory_implicit():
                    # Implicit memory access.
                    if op.Access not in self.Mem2Access: self.Mem2Access.append(op.Access)

    def post_process_modes(self):
        if self.Vex or self.Xop or self.Evex:
            if self.Modes.contains('real'):
                self.Modes.remove('real')
            if self.Modes.contains('v8086'):
                self.Modes.remove('v8086')
        if not self.Modes.contains('long') and not self.Attributes.contains('I64'):
            self.Attributes.add('I64')
        if self.Modes.contains('long') and not self.Modes.contains('prot') and not self.Attributes.contains('O64'):
            self.Attributes.add('O64')

    def process_filters(self):
        # Add encoding based filters.
        for attr in self.Attributes.Names:
            match attr:
                case 'I64': self.Filters.append('no64')
                case 'O64': self.Filters.append('no1632')
                case 'NORIPREL': self.Filters.append('noriprel')
                case 'NOA16': self.Filters.append('noa16')
                case 'NO66': self.Filters.append('no66')
                case 'NO67': self.Filters.append('no67')
                case 'NOREP': self.Filters.append('norep')
                # Special case for XSAVE* & XRSTOR* which, even though under an opcode group that supports REX2, 
                # they do not support it. For all other instruction, a REX2 table is generated for fast lookup.
                case 'NOREX2' if self.Mnemonic.startswith('XSAVE') or self.Mnemonic.startswith('XRSTOR'): 
                    self.Filters.append('norex2')
                case 'NOL0': self.Filters.append('nol0')
                case 'NOV': self.Filters.append('nov')
                case 'NOVP': self.Filters.append('novp')

        # If both VVVV and V' must be 0, combine them in a single filter. This allows us to do a single lookup 
        # instead of 2.
        if 'nov' in self.Filters and 'novp' in self.Filters:
            self.Filters.remove('nov')
            self.Filters.remove('novp')
            self.Filters.append('novvp')

        # Add register specific filters. Some registers cannot be encoded and are invalid. We check those only when
        # decoding an instruction that uses such registers.
        for op in self.ExpOps:
            match op.Type:
                case 'G' if self.Evex and self.EvexMode.Name is None: self.Filters.append('rrlt16')
                case 'B' if self.Evex and self.EvexMode.Name is None: self.Filters.append('rvlt16')
                case 'S' if 'W' in op.Access: self.Filters.append('srin02345')
                case 'S':  self.Filters.append('srin012345')
                case 'C':  self.Filters.append('crin02348')
                case 'D':  self.Filters.append('drlt8')
                case 'T':  self.Filters.append('qrlt8')
                case 'rB': self.Filters.append('brlt4')
                case 'mB': self.Filters.append('bmlt4')
                case 'rK': self.Filters.append('krlt8')
                case 'vK': self.Filters.append('kvlt8')
                case 'rT': self.Filters.append('trlt8')
                case 'mT': self.Filters.append('tmlt8')
                case 'vT': self.Filters.append('tvlt8')

        # Add instruction specific filters.
        if self.Attributes.contains('VSIB') and self.Category == 'AVX2GATHER':
            if self.ExType.Name in ['12']:
                self.Filters.append('vxnevr_vxnevv_vrnevv')
            elif self.ExType.Name in ['E12']:
                self.Filters.append('vxnevr')
        elif self.ExType.Name in ['AMX_E4', 'AMX_E10']:
            self.Filters.append('trnetm_trnetv_tvnetm')
        elif self.ExType.Name in ['E4S', 'E10S']:
            self.Filters.append('vrnevv_vrnevm')
        elif self.ExType.Name in ['APX_EVEX_PP2']:
            self.Filters.append('rvne4_rmne4')
            if self.Class == 'POP2':
                self.Filters.append('rvnerm')


    def __str__(self):
        ops = ''
        for o in self.ExpOps:
            ops += o.__str__() + ','
        ops = ops[:-1]

        # Return a text reprezentation of the encoding
        return (self.Mnemonic + ' ' + ops).strip()
