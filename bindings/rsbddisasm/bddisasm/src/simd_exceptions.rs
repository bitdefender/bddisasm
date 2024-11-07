/*
 * Copyright (c) 2024 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Offers information about the SIMD exceptions that can be triggered by an instruction.

#![allow(clippy::module_name_repetitions)]

// TODO: maybe use something like the `bitflags` crate and have all these as flags?

/// SIMD Floating-Point Exceptions.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct SimdExceptions {
    /// Invalid Operation Exception.
    pub invalid_operation: bool,

    /// Denormal Exception.
    pub denormal: bool,

    /// Divide-by-Zero Exception.
    pub divide_by_zero: bool,

    /// Overflow Exception.
    pub overflow: bool,

    /// Underflow Exception.
    pub underflow: bool,

    /// Precision Exception.
    pub precision: bool,
}

#[doc(hidden)]
impl SimdExceptions {
    pub(crate) fn from_raw(value: u8) -> Self {
        let value = u32::from(value);
        Self {
            invalid_operation: (value & ffi::ND_SIMD_EXC_IE) != 0,
            denormal: (value & ffi::ND_SIMD_EXC_DE) != 0,
            divide_by_zero: (value & ffi::ND_SIMD_EXC_ZE) != 0,
            overflow: (value & ffi::ND_SIMD_EXC_OE) != 0,
            underflow: (value & ffi::ND_SIMD_EXC_UE) != 0,
            precision: (value & ffi::ND_SIMD_EXC_PE) != 0,
        }
    }
}
