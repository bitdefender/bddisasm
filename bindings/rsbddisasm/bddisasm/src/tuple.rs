/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Instruction tuple type.

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
    Ovm,
    /// M128, 128 bit.
    M128,
    /// DUP (VMOVDDUP).
    Dup,
    /// 4 x 32 bit Memory Elements are referenced.
    T14X,
}

#[doc(hidden)]
impl Tuple {
    pub(crate) fn from_raw(value: ffi::_ND_TUPLE) -> Self {
        match value {
            ffi::_ND_TUPLE::ND_TUPLE_None => Tuple::None,
            ffi::_ND_TUPLE::ND_TUPLE_FV => Tuple::Fv,
            ffi::_ND_TUPLE::ND_TUPLE_HV => Tuple::Hv,
            ffi::_ND_TUPLE::ND_TUPLE_QV => Tuple::Qv,
            ffi::_ND_TUPLE::ND_TUPLE_T1S8 => Tuple::T1S8,
            ffi::_ND_TUPLE::ND_TUPLE_T1S16 => Tuple::T1S16,
            ffi::_ND_TUPLE::ND_TUPLE_T1S => Tuple::T1S,
            ffi::_ND_TUPLE::ND_TUPLE_T1F => Tuple::T1F,
            ffi::_ND_TUPLE::ND_TUPLE_T2 => Tuple::T2,
            ffi::_ND_TUPLE::ND_TUPLE_T4 => Tuple::T4,
            ffi::_ND_TUPLE::ND_TUPLE_T8 => Tuple::T8,
            ffi::_ND_TUPLE::ND_TUPLE_FVM => Tuple::Fvm,
            ffi::_ND_TUPLE::ND_TUPLE_HVM => Tuple::Hvm,
            ffi::_ND_TUPLE::ND_TUPLE_QVM => Tuple::Qvm,
            ffi::_ND_TUPLE::ND_TUPLE_OVM => Tuple::Ovm,
            ffi::_ND_TUPLE::ND_TUPLE_M128 => Tuple::M128,
            ffi::_ND_TUPLE::ND_TUPLE_DUP => Tuple::Dup,
            ffi::_ND_TUPLE::ND_TUPLE_T1_4X => Tuple::T14X,
        }
    }
}
