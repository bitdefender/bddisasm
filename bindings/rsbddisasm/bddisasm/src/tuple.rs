/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Instruction tuple type.

use super::decode_error::DecodeError;

/// Instruction tuple type.
///
/// Used to determine compressed displacement size for `disp8 EVEX` instructions. Note that most of the `EVEX` encoded
/// instructions use the compressed displacement addressing scheme.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum Tuple {
    None,
    /// Full Vector.
    Fv,
    /// Half Vector.
    Hv,
    /// Quarter Vector.
    Qv,
    /// Tuple1 scalar, size 8 bit.
    T1S8,
    /// Tuple1 scalar, size 16 bit.
    T1S16,
    /// Tuple1 scalar, size 32/64 bit.
    T1S,
    /// Tuple1 float, size 32/64 bit.
    T1F,
    /// Tuple2, 64/128 bit.
    T2,
    /// Tuple4, 128/256 bit.
    T4,
    /// Tuple8, 256 bit.
    T8,
    /// Full Vector Memory.
    Fvm,
    /// Half Vector Memory.
    Hvm,
    /// Quarter Vector Memory.
    Qvm,
    /// Oct Vector Memory.
    OVm,
    /// M128, 128 bit.
    M128,
    /// DUP (VMOVDDUP).
    Dup,
    /// 4 x 32 bit Memory Elements are referenced.
    T14X,
}

#[doc(hidden)]
impl Tuple {
    pub(crate) fn from_raw(value: u32) -> Result<Self, DecodeError> {
        if value == ffi::_ND_TUPLE::ND_TUPLE_None as u32 {
            Ok(Tuple::None)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_FV as u32 {
            Ok(Tuple::Fv)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_HV as u32 {
            Ok(Tuple::Hv)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_QV as u32 {
            Ok(Tuple::Qv)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T1S8 as u32 {
            Ok(Tuple::T1S8)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T1S16 as u32 {
            Ok(Tuple::T1S16)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T1S as u32 {
            Ok(Tuple::T1S)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T1F as u32 {
            Ok(Tuple::T1F)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T2 as u32 {
            Ok(Tuple::T2)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T4 as u32 {
            Ok(Tuple::T4)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T8 as u32 {
            Ok(Tuple::T8)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_FVM as u32 {
            Ok(Tuple::Fvm)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_HVM as u32 {
            Ok(Tuple::Hvm)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_QVM as u32 {
            Ok(Tuple::Qvm)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_OVM as u32 {
            Ok(Tuple::OVm)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_M128 as u32 {
            Ok(Tuple::M128)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_DUP as u32 {
            Ok(Tuple::Dup)
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T1_4X as u32 {
            Ok(Tuple::T14X)
        } else {
            Err(DecodeError::InternalError(value.into()))
        }
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn check_all_tuples() {
        // This is a really contrieved way of making sure that we check all variants of `ffi::_ND_TUPLE`. If a new
        // one is added, this will fail to build. We do this because `Tuple::from_raw` takes an `u32`.
        // NOTE: When a new variant is added, `Tuple::from_raw` must be updated.
        match ffi::_ND_TUPLE::ND_TUPLE_None {
            ffi::_ND_TUPLE::ND_TUPLE_None => {}
            ffi::_ND_TUPLE::ND_TUPLE_FV => {}
            ffi::_ND_TUPLE::ND_TUPLE_HV => {}
            ffi::_ND_TUPLE::ND_TUPLE_QV => {}
            ffi::_ND_TUPLE::ND_TUPLE_T1S8 => {}
            ffi::_ND_TUPLE::ND_TUPLE_T1S16 => {}
            ffi::_ND_TUPLE::ND_TUPLE_T1S => {}
            ffi::_ND_TUPLE::ND_TUPLE_T1F => {}
            ffi::_ND_TUPLE::ND_TUPLE_T2 => {}
            ffi::_ND_TUPLE::ND_TUPLE_T4 => {}
            ffi::_ND_TUPLE::ND_TUPLE_T8 => {}
            ffi::_ND_TUPLE::ND_TUPLE_FVM => {}
            ffi::_ND_TUPLE::ND_TUPLE_HVM => {}
            ffi::_ND_TUPLE::ND_TUPLE_QVM => {}
            ffi::_ND_TUPLE::ND_TUPLE_OVM => {}
            ffi::_ND_TUPLE::ND_TUPLE_M128 => {}
            ffi::_ND_TUPLE::ND_TUPLE_DUP => {}
            ffi::_ND_TUPLE::ND_TUPLE_T1_4X => {}
        }
    }
}
