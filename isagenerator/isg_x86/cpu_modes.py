#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from isg_x86.common import *


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

# Valid operating modes for instructions. Please use these templates and not inline definitions. If a template does
# not exist already, create a new one.
# Syntax: NAME : group1:mode11[+mode12[|group2:!mode21+!mode22]] etc.
#
# NAME cam be anything and can be used as a placeholder when defining the instruction.
# group - can be one of the 4 operating groups: 
#         ring - specify the needed privilege level for the instruction
#         mode - specify the needed operating mode 
#         vmx  - specify the VMX operation mode
#         other- specify other misc operating modes/special cases
#
# ring  - can be any of:
#         r0 - instruction is valid in ring 0
#         r1 - instruction is valid in ring 1
#         r2 - instruction is valid in ring 2
#         r3 - instruction is valid in ring 3
#
# mode  - can be any of: 
#         real  - instruction valid in real mode (16 bit)
#         v8086 - instruction valid in virtual 8086 mode (16 bit inside 32 bit mode)
#         prot  - instruction valid in protected mode (32 bit)
#         compat- instruction valid in compatibility mode (32 inside 64 bit mode)
#         long  - instruction valid in long mode (64 bit) 
#
# other - can be any of:
#         smm  - instruction valid in System Management Mode
#         smm_off - instruction valid outside SMM
#         sgx  - instruction is valid inside a Software Guard Extensions enclave
#         sgx_off - instruction valid outside SGX
#         tsx  - instruction is valid inside a Transactional Syncronization Extensions region
#         tsx_off - instruction valid outside TSX
#
# vmx  - can be any of:
#         vmxr - instruction valid in VMX root mode
#         vmxn - instruction valid in VMX non-root mode
#         vmxr_seam - instruction is valid inside VMX root Secure Arbitration Mode
#         vmxn_seam - instruction is valid inside VMX non-root Secure Arbitration Mode
#         vmx_off - instruction valid outside VMX operation

#
# If a group is not specified, all modes for that group will be considered valid.
# If only some modes are specified for a group, only they will be considered valid, and all the others invalid.
# If only some modes are specified negated (!), the instruction will be considered valid in all but the specified modes.
#
# For SGX and TSX, the most restrictive rule is applied: an instruction is listed as valid in any of these modes if
# it will always execute successfully; instructions that may be valid or invalid depending on microarchitectural
# design/conditions are listed as invalid in these modes.
# 
# SMM, SGX, TSX, VMX, SEAM are special operating modes for bddisasm, so validty of an instruction must be specified both
# inside and outside each of these modes. This is because an instruction may be valid in both modes (both SMM/SGX/TSX/SEAM
# on or off), but others may be valid only if the respective operating mode is on or off.
#
template_modes = {
    # Instruction that are valid ONLY in VMX root operation; this also implies ring0, and only protected or long mode.
    "VMXROOT" : "ring=r0|vmx=vmxr+vmxr_seam|mode=prot+long|other=sgx_off+tsx_off+smm_off",

    # Instructions valid only in VMX operation, be it VMX root or VMX non-root.
    "VMX"     : "vmx=!vmx_off",

    # Instruction valid only in VMX-non root
    "VMXNROOT" : "vmx=vmxn+vmxn_seam",

    # Instructions valid only in r0. SGX enclaves exist only in r3, so a 0 instruction will not be valid in SGX mode.
    "KERNEL"  : "ring=r0|other=!sgx",

    # Instructions valid only in r3.
    "USER"    : "ring=r3",

    # Instructions invalid in real mode. If an instruction is invalid in real mode, it will be invalid in v8086 as well.
    "NOREAL"  : "mode=!real+!v8086",

    # Instructions invalid in v8086 mode. They may be, however, valid in real mode.
    "NOV86"   : "mode=!v8086",

    # Instructions valid only in long mode (64 bit).
    "O64"     : "mode=long",

    # Instructions invalid in long mode.
    "NO64"    : "mode=!long",

    # Instructions invalid in transactional regions.
    "NOTSX"   : "other=!tsx",

    # Instructions invalid in SGX mode.
    "NOSGX"   : "other=!sgx",

    # Instructions valid only in SMM. Note that while in SMM, one can transition in protected/long mode.
    "SMM"     : "other=!smm_off",

    # Instructions invalid inside SMM, real-mode or V8086.
    "NOSMM"   : "mode=!real+!v8086|other=!smm",

    # Instruction valid inside SEAM VXM root.
    "SEAMR"   : "ring=r0|mode=long|vmx=vmxr_seam",

    # Instruction valid in SEAM non-root mode or VMX root. Not valid in SMM.
    "SEAMN"   : "mode=long|vmx=vmxr+vmxn+vmxn_seam|other=!smm",
}


class CpuModes:
    """
    Describes CPU operation modes. Each instruction is valid or invalid in certain operating modes.

    Attributes
    ----------
    Names: list[str]
        A list of valid CPU modes. Each entry must be one listed in valid_cpu_modes.
    """
    def __init__(self):
        # By default, assume valid in every mode.
        self.Names = valid_cpu_modes.copy()

    def add(self, value: str):
        # Example: m:ring=0,1,2,3|vmx=root,nonroot|mode=real,v8086,smm,prot,compat,long|other=sgx,tsx
        # Note: any group that is not specified is considered entirely valid
        # Note: any group that is specified overrides all the other fields in the group; example:
        #       mode=real   - this means the instruction is valid ONLY in real mode.
        #       mode=!v8086 - this means the instructiom is valid is ANY mode except for V8086
        # value can be one of the templates in template_modes.
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
                    raise InvalidSpecificationException(
                        "Unknown CPU mode group specified: %s" % m)
                if vx not in valid_mode_map[m]:
                    raise InvalidSpecificationException(
                        "Mode %s is not valid for mode group %s; it can be one of [%s]" % 
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
                raise InvalidSpecificationException(
                    "Unknown CPU mode specifier '%s', expecting one of [%s]" % 
                    (m, ','.join(valid_cpu_modes)))

        self.Names = modes
        
    def remove(self, value: str):
        if value not in valid_cpu_modes:
            raise InvalidSpecificationException(
                "Unknown CPU mode specifier '%s', expecting one of [%s]" % 
                (value, ','.join(valid_cpu_modes)))
        self.Names.remove(value)
        
    def contains(self, value: str) -> bool:
        if value not in valid_cpu_modes:
            raise InvalidSpecificationException(
                "Unknown CPU mode specifier '%s', expecting one of [%s]" % 
                (value, ','.join(valid_cpu_modes)))
        return value in self.Names
        
    def contains_all_modes(self) -> bool:
        for m in valid_cpu_modes:
            if m not in self.Names:
                return False
        return True
