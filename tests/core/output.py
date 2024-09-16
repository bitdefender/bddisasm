#
# Copyright (c) 2024 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import re

from core import objects


class OutputParser(object):
    def __init__(self, data: str):
        self._data = data
        self._output = list()

    def read(self) -> list:
        """
        Return a list containing the parsed output.
        """
        return self._output


class OutputDecode(OutputParser):
    def __init__(self, data: str):
        OutputParser.__init__(self, data)

        self.rd(self._data)

    def rd(self, data) -> type(None):
        """
        Used for decoder. Example:
        0000000000000000 00d8                            ADD       al, bl
                DSIZE: 32, ASIZE: 64, VLEN: -
                ISA Set: I86, Ins cat: ARITH, CET tracked: no
                FLAGS access
                        CF: m, PF: m, AF: m, ZF: m, SF: m, OF: m,
                Valid modes
                        R0: yes, R1: yes, R2: yes, R3: yes
                        Real: yes, V8086: yes, Prot: yes, Compat: yes, Long: yes
                        SMM on: yes, SMM off: yes, SGX on: yes, SGX off: yes, TSX on: yes, TSX off: yes
                        VMXRoot: yes, VMXNonRoot: yes, VMXRoot SEAM: yes, VMXNonRoot SEAM: yes, VMX off: yes
                Valid prefixes
                        REP: no, REPcc: no, LOCK: yes
                        HLE: yes, XACQUIRE only: no, XRELEASE only: no
                        BND: no, BHINT: no, DNT: no
                Operand: 0, Acc:  RW,  Type:   Register, Size:  1, RawSize:  1, Encoding: M, RegType:  General Purpose, RegSize:  1, RegId: 0, RegCount: 1
                Operand: 1, Acc:  R-,  Type:   Register, Size:  1, RawSize:  1, Encoding: R, RegType:  General Purpose, RegSize:  1, RegId: 3, RegCount: 1
                Operand: 2, Acc:  -W,  Type:   Register, Size:  4, RawSize:  4, Encoding: S, RegType:            Flags, RegSize:  4, RegId: 0, RegCount: 1

        Reads the given text and parses it to obtain dictionaries.
        """
        output = list()
        ostr = list()
        local = ""
        for line in data.split("\n"):
            if re.search("^[0-9A-F]{16}", line):
                if local:
                    local = local.replace("\r", "")
                    ostr.append(local)
                local = ""
            local += line
            local += "\n"

        ostr.append(local)

        for elem in ostr:
            if not elem:
                break
            instr = objects.DecodeInstrux(elem)
            output.append(instr)

        self._output.append(output)


class OutputShemu(OutputParser):
    def __init__(self, data: str):
        OutputParser.__init__(self, data)

        self.rd(self._data)
        self.rdem(self._data)

    def rd(self, data) -> type(None):
        """
        Used for shemu instructions. Example:
        IP: 0x000000000020005f  90                              NOP
            RAX = 0x0000000000000000 RCX = 0x0000000000000000 RDX = 0x0000000000000000 RBX = 0x0000000000000000
            RSP = 0x00000000001fe000 RBP = 0x0000000000000000 RSI = 0x0000000000000000 RDI = 0x0000000000000000
            R8  = 0x0000000000000000 R9  = 0x0000000000000000 R10 = 0x0000000000000000 R11 = 0x0000000000000000
            R12 = 0x0000000000000000 R13 = 0x0000000000000000 R14 = 0x0000000000000000 R15 = 0x0000000000000000
            R16 = 0x0000000000000000 R17 = 0x0000000000000000 R18 = 0x0000000000000000 R19 = 0x0000000000000000
            R20 = 0x0000000000000000 R21 = 0x0000000000000000 R22 = 0x0000000000000000 R23 = 0x0000000000000000
            R24 = 0x0000000000000000 R25 = 0x0000000000000000 R26 = 0x0000000000000000 R27 = 0x0000000000000000
            R28 = 0x0000000000000000 R29 = 0x0000000000000000 R30 = 0x0000000000000000 R31 = 0x0000000000000000
            RIP = 0x0000000000200060 RFLAGS = 0x0000000000000247   CF:1 PF:1 AF:0 ZF:1 SF:0 TF:0 IF:1 DF:0 OF

        Reads the given text and parses it to obtain dictionaries.
        """
        output = list()
        ostr = list()
        local = ""
        for line in data.split("\n"):
            if line.startswith("Emulation ") or "SHEMU" in line:
                continue
            if re.search("^PC:", line) or re.search("^IP:", line):
                if local:
                    local = local.replace("\r", "")
                    ostr.append(local)
                local = ""
            local += line
            local += "\n"

        ostr.append(local)

        for elem in ostr:
            if not elem:
                break
            instr = objects.ShemuInstrux(elem)
            output.append(instr)

        self._output.append(output)

    def rdem(self, data) -> type(None):
        """
        Used for shemu result. Example:
        Emulation terminated with status 0x0000000a, flags: 0x1000000, 1 NOPs, 0 NULLs, 62 total instructions, 61 unique instructions
            SHEMU_FLAG_OBFUS_NEUTRAL_IMM

        Reads the given text and parse it to obtain dictionaries.
        """
        ems = ""
        for line in data.split("\n"):
            if line.startswith("Emulation ") or "SHEMU" in line:
                line = line.replace("\r", "")
                ems += "\n"
                ems += line

        em = objects.ShemuResult(ems)

        self._output.append([em])
