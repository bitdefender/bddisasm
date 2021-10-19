/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Offers information about how an instructions accesses the FPU status registers.

extern crate bddisasm_sys as ffi;

/// The mode in which a FPU status flag is accessed.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum FpuFlagsAccess {
    /// The FPU status flag is cleared to 0.
    Cleared,
    /// The FPU status flag is set to 1.
    Set,
    /// The FPU status flag is modified according to a result.
    Modified,
    /// The FPU status flag is undefined or unaffected.
    Undefined,
}

impl From<u8> for FpuFlagsAccess {
    fn from(value: u8) -> FpuFlagsAccess {
        let value = value as u32;
        match value {
            ffi::ND_FPU_FLAG_SET_0 => FpuFlagsAccess::Cleared,
            ffi::ND_FPU_FLAG_SET_1 => FpuFlagsAccess::Set,
            ffi::ND_FPU_FLAG_MODIFIED => FpuFlagsAccess::Modified,
            ffi::ND_FPU_FLAG_UNDEFINED => FpuFlagsAccess::Undefined,
            _ => panic!("Unexpected FPU flag access: {}", value),
        }
    }
}

/// Describes the way an instruction accesses the FPU flags registers.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct FpuFlags {
    /// C0 flag access mode.
    pub c0: FpuFlagsAccess,
    /// C1 flag access mode.
    pub c1: FpuFlagsAccess,
    /// C2 flag access mode.
    pub c2: FpuFlagsAccess,
    /// C3 flag access mode.
    pub c3: FpuFlagsAccess,
}

#[doc(hidden)]
impl From<ffi::ND_FPU_FLAGS> for FpuFlags {
    fn from(flags: ffi::ND_FPU_FLAGS) -> FpuFlags {
        FpuFlags {
            c0: FpuFlagsAccess::from(flags.C0()),
            c1: FpuFlagsAccess::from(flags.C1()),
            c2: FpuFlagsAccess::from(flags.C2()),
            c3: FpuFlagsAccess::from(flags.C3()),
        }
    }
}
