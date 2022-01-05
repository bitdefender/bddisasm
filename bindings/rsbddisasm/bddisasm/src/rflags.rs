/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Describes the bits in the FLAGS register.

/// Carry flag.
pub const CF: u32 = ffi::NDR_RFLAG_CF;
/// Parity flag.
pub const PF: u32 = ffi::NDR_RFLAG_PF;
/// Auxiliary flag.
pub const AF: u32 = ffi::NDR_RFLAG_AF;
/// Zero flag.
pub const ZF: u32 = ffi::NDR_RFLAG_ZF;
/// Sign flag.
pub const SF: u32 = ffi::NDR_RFLAG_SF;
/// Trap flag.
pub const TF: u32 = ffi::NDR_RFLAG_TF;
/// Interrupt flag.
pub const IF: u32 = ffi::NDR_RFLAG_IF;
/// Direction flag.
pub const DF: u32 = ffi::NDR_RFLAG_DF;
/// Overflow flag.
pub const OF: u32 = ffi::NDR_RFLAG_OF;
/// I/O privilege level flag.
pub const IOPL: u32 = ffi::NDR_RFLAG_IOPL;
/// Nested task flag.
pub const NT: u32 = ffi::NDR_RFLAG_NT;
/// Resume flag.
pub const RF: u32 = ffi::NDR_RFLAG_RF;
/// Virtual mode flag.
pub const VM: u32 = ffi::NDR_RFLAG_VM;
/// Alignment check flag.
pub const AC: u32 = ffi::NDR_RFLAG_AC;
/// Virtual interrupts flag.
pub const VIF: u32 = ffi::NDR_RFLAG_VIF;
/// Virtual interrupt pending flag.
pub const VIP: u32 = ffi::NDR_RFLAG_VIP;
/// CPUID identification flag.
pub const ID: u32 = ffi::NDR_RFLAG_ID;

pub(crate) fn flags_raw(flags: ffi::ND_RFLAGS) -> u32 {
    unsafe { flags.Raw }
}
