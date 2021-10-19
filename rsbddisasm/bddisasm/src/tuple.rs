/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Instruction tuple type.

extern crate bddisasm_sys as ffi;

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
impl From<ffi::ND_TUPLE> for Tuple {
    fn from(value: ffi::ND_TUPLE) -> Tuple {
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
            ffi::_ND_TUPLE::ND_TUPLE_OVM => Tuple::OVm,
            ffi::_ND_TUPLE::ND_TUPLE_M128 => Tuple::M128,
            ffi::_ND_TUPLE::ND_TUPLE_DUP => Tuple::Dup,
            ffi::_ND_TUPLE::ND_TUPLE_T1_4X => Tuple::T14X,
            // NOTE: when updating this take care to also update the `From<u32>` implementation!
            // TODO: any way of keeping these in sync automagically?
        }
    }
}

impl From<u32> for Tuple {
    fn from(value: u32) -> Tuple {
        if value == ffi::_ND_TUPLE::ND_TUPLE_None as u32 {
            Tuple::None
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_FV as u32 {
            Tuple::Fv
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_HV as u32 {
            Tuple::Hv
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_QV as u32 {
            Tuple::Qv
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T1S8 as u32 {
            Tuple::T1S8
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T1S16 as u32 {
            Tuple::T1S16
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T1S as u32 {
            Tuple::T1S
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T1F as u32 {
            Tuple::T1F
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T2 as u32 {
            Tuple::T2
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T4 as u32 {
            Tuple::T4
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T8 as u32 {
            Tuple::T8
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_FVM as u32 {
            Tuple::Fvm
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_HVM as u32 {
            Tuple::Hvm
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_QVM as u32 {
            Tuple::Qvm
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_OVM as u32 {
            Tuple::OVm
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_M128 as u32 {
            Tuple::M128
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_DUP as u32 {
            Tuple::Dup
        } else if value == ffi::_ND_TUPLE::ND_TUPLE_T1_4X as u32 {
            Tuple::T14X
        } else {
            panic!("Unknown tuple: {}", value)
        }
    }
}
