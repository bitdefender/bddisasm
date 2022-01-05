/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Offers information about the CPU modes in which an instruction is supported.
//!
//! # Examples
//!
//! ```
//! # use bddisasm::DecodeError;
//! #
//! # fn main() -> Result<(), DecodeError> {
//! use bddisasm::{DecodedInstruction, DecodeMode, Mnemonic};
//!
//! // `VMXON     qword ptr [rax]`
//! let ins = DecodedInstruction::decode(&[0xf3, 0x0f, 0xc7, 0x30], DecodeMode::Bits64)?;
//! let modes = ins.valid_cpu_modes();
//!
//! // Check if the instruction is available from user mode
//! if modes.privilege_level.ring3 {
//!     println!("Available in user mode");
//! } else {
//!     println!("Not available in user mode");
//! }
//!
//! # Ok(())
//! # }

// TODO: maybe use something like the `bitflags` crate and have all these as flags?

/// Privilege levels (rings) in which an instruction is supported.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct PrivilegeLevel {
    /// Instruction is valid in ring 0.
    pub ring0: bool,

    /// Instruction is valid in ring 1.
    pub ring1: bool,

    /// Instruction is valid in ring 2.
    pub ring2: bool,

    /// Instruction is valid in ring 3.
    pub ring3: bool,
}

/// Operating modes in which an instruction is supported.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct OperatingMode {
    /// The instruction is valid in real mode.
    pub real: bool,

    /// The instruction is valid in Virtual 8086 mode.
    pub v8086: bool,

    /// The instruction is valid in protected mode (32 bit).
    pub protected: bool,

    /// The instruction is valid in compatibility mode (32 bit in 64 bit).
    pub compat: bool,

    /// The instruction is valid in long mode.
    pub long: bool,
}

/// Special modes - these may be active inside other modes (example: `TSX` in `Long mode`).
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct SpecialModes {
    /// The instruction is valid in System Management Mode.
    pub smm: bool,

    /// The instruction is valid outside System Management Mode.
    pub smm_off: bool,

    /// The instruction is valid in SGX mode.
    pub sgx: bool,

    /// The instruction is valid outside SGX mode.
    pub sgx_off: bool,

    /// The instruction is valid in transactional regions.
    pub tsx: bool,

    /// The instruction is valid in transactional regions.
    pub tsx_off: bool,
}

/// VMX mode - they engulf all the other modes.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct VmxMode {
    /// The instruction is valid in VMX root mode.
    pub root: bool,

    /// The instruction is valid in VMX non root mode.
    pub non_root: bool,

    /// The instruction is valid in VMX root SEAM.
    pub root_seam: bool,

    /// The instruction is valid in VMX non-root SEAM.
    pub non_root_seam: bool,

    /// The instruction is valid outside VMX operation.
    pub off: bool,
}

/// Indicates in which modes the instruction is valid in
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct CpuModes {
    pub privilege_level: PrivilegeLevel,
    pub operating_mode: OperatingMode,
    pub special_modes: SpecialModes,
    pub vmx: VmxMode,
}

#[doc(hidden)]
impl CpuModes {
    pub(crate) fn from_raw(raw: ffi::ND_VALID_MODES) -> Self {
        let raw = unsafe { raw.__bindgen_anon_1 };

        Self {
            privilege_level: PrivilegeLevel {
                ring0: raw.Ring0() != 0,
                ring1: raw.Ring1() != 0,
                ring2: raw.Ring2() != 0,
                ring3: raw.Ring3() != 0,
            },
            operating_mode: OperatingMode {
                real: raw.Real() != 0,
                v8086: raw.V8086() != 0,
                protected: raw.Protected() != 0,
                compat: raw.Compat() != 0,
                long: raw.Long() != 0,
            },
            special_modes: SpecialModes {
                smm: raw.Smm() != 0,
                smm_off: raw.SmmOff() != 0,
                sgx: raw.Sgx() != 0,
                sgx_off: raw.SgxOff() != 0,
                tsx: raw.Tsx() != 0,
                tsx_off: raw.TsxOff() != 0,
            },
            vmx: VmxMode {
                root: raw.VmxRoot() != 0,
                non_root: raw.VmxNonRoot() != 0,
                root_seam: raw.VmxRootSeam() != 0,
                non_root_seam: raw.VmxNonRootSeam() != 0,
                off: raw.VmxOff() != 0,
            },
        }
    }
}
