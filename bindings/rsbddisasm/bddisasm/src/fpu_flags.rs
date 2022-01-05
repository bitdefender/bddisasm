/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Offers information about how an instructions accesses the FPU status registers.

use super::decode_error::DecodeError;

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

#[doc(hidden)]
impl FpuFlagsAccess {
    pub(crate) fn from_raw(value: u8) -> Result<Self, DecodeError> {
        let value = u32::from(value);
        match value {
            ffi::ND_FPU_FLAG_SET_0 => Ok(FpuFlagsAccess::Cleared),
            ffi::ND_FPU_FLAG_SET_1 => Ok(FpuFlagsAccess::Set),
            ffi::ND_FPU_FLAG_MODIFIED => Ok(FpuFlagsAccess::Modified),
            ffi::ND_FPU_FLAG_UNDEFINED => Ok(FpuFlagsAccess::Undefined),
            _ => Err(DecodeError::InternalError(value.into())),
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
impl FpuFlags {
    pub(crate) fn from_raw(flags: ffi::ND_FPU_FLAGS) -> Result<Self, DecodeError> {
        Ok(Self {
            c0: FpuFlagsAccess::from_raw(flags.C0())?,
            c1: FpuFlagsAccess::from_raw(flags.C1())?,
            c2: FpuFlagsAccess::from_raw(flags.C2())?,
            c3: FpuFlagsAccess::from_raw(flags.C3())?,
        })
    }
}
