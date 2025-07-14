/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Decodes instructions.

use ffi::NdGetOperandMini;

use crate::cpu_modes::CpuModes;
use crate::cpuid::Cpuid;
use crate::decode_error::{status_to_error, DecodeError};
use crate::fpu_flags::FpuFlags;
use crate::instruction_category::Category;
use crate::isa_set::IsaSet;
use crate::mnemonic::Mnemonic;
use crate::operand::{OpAccess, Operand};
use crate::rflags::flags_raw;
use crate::simd_exceptions::SimdExceptions;
use crate::tuple::Tuple;

use core::convert::TryFrom;
use core::fmt;
use core::mem::{self, MaybeUninit};

/// Represents a succesfull instruction decoding, or failure.
pub type DecodeResult = Result<DecodedInstruction, DecodeError>;

/// The decoding mode to use.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum DecodeMode {
    /// 16-bit.
    Bits16,
    /// 32-bit.
    Bits32,
    /// 64-bit.
    Bits64,
}

#[doc(hidden)]
impl From<DecodeMode> for u8 {
    fn from(mode: DecodeMode) -> Self {
        match mode {
            DecodeMode::Bits16 => ffi::ND_CODE_16 as u8,
            DecodeMode::Bits32 => ffi::ND_CODE_32 as u8,
            DecodeMode::Bits64 => ffi::ND_CODE_64 as u8,
        }
    }
}

/// Encoding mode.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum EncodingMode {
    /// Legacy encoded instruction.
    Legacy,

    /// XOP encoded instruction.
    Xop,

    /// VEX encoded instruction.
    Vex,

    /// EVEX encoded instruction.
    Evex,
}

#[doc(hidden)]
impl EncodingMode {
    pub(crate) fn from_raw(value: u32) -> Result<Self, DecodeError> {
        match value {
            ffi::ND_ENCM_LEGACY => Ok(EncodingMode::Legacy),
            ffi::ND_ENCM_XOP => Ok(EncodingMode::Xop),
            ffi::ND_ENCM_VEX => Ok(EncodingMode::Vex),
            ffi::ND_ENCM_EVEX => Ok(EncodingMode::Evex),
            _ => Err(DecodeError::InternalError(value.into())),
        }
    }
}

/// VEX mode.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum VexMode {
    /// 2B VEX prefix (0xC5).
    Vex2b,

    /// 3B VEX prefix (0xC4).
    Vex3b,
}

#[doc(hidden)]
impl VexMode {
    pub(crate) fn from_raw(value: u32) -> Result<Self, DecodeError> {
        match value {
            ffi::ND_VEXM_2B => Ok(VexMode::Vex2b),
            ffi::ND_VEXM_3B => Ok(VexMode::Vex3b),
            _ => Err(DecodeError::InternalError(value.into())),
        }
    }
}

/// Addressing mode.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum AddressingMode {
    /// 16 bit addressing.
    Addr16,

    /// 32 bit addressing.
    Addr32,

    /// 64 bit addressing.
    Addr64,
}

#[doc(hidden)]
impl AddressingMode {
    pub(crate) fn from_raw(value: u32) -> Result<Self, DecodeError> {
        match value {
            ffi::ND_ADDR_16 => Ok(AddressingMode::Addr16),
            ffi::ND_ADDR_32 => Ok(AddressingMode::Addr32),
            ffi::ND_ADDR_64 => Ok(AddressingMode::Addr64),
            _ => Err(DecodeError::InternalError(value.into())),
        }
    }
}

/// Operand mode/size.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum OperandSize {
    /// 16 bit operand size.
    OpSize16,

    /// 32 bit operand size.
    OpSize32,

    /// 64 bit operand size.
    OpSize64,
}

#[doc(hidden)]
impl OperandSize {
    pub(crate) fn from_raw(value: u32) -> Result<Self, DecodeError> {
        match value {
            ffi::ND_OPSZ_16 => Ok(OperandSize::OpSize16),
            ffi::ND_OPSZ_32 => Ok(OperandSize::OpSize32),
            ffi::ND_OPSZ_64 => Ok(OperandSize::OpSize64),
            _ => Err(DecodeError::InternalError(value.into())),
        }
    }
}

/// Operand mode/size.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum VectorSize {
    /// 128 bit vector size.
    VecSize128,
    /// 256 bit vector size.
    VecSize256,
    /// 512 bit vector size.
    VecSize512,
}

#[doc(hidden)]
impl VectorSize {
    pub(crate) fn from_raw(value: u32) -> Result<Self, DecodeError> {
        match value {
            ffi::ND_VECM_128 => Ok(VectorSize::VecSize128),
            ffi::ND_VECM_256 => Ok(VectorSize::VecSize256),
            ffi::ND_VECM_512 => Ok(VectorSize::VecSize512),
            _ => Err(DecodeError::InternalError(value.into())),
        }
    }
}

/// Describes the way an instruction accesses the flags register.
///
/// Individual bits can be checked using the [rflags](crate::rflags) module.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct FlagsAccess {
    /// RFLAGS access mode, as per the entire register.
    pub mode: OpAccess,

    /// Tested flags.
    pub tested: u32,

    /// Modified (according to the result) flags.
    pub modified: u32,

    /// Flags that are always set to 1.
    pub set: u32,

    /// Flags that are always cleared to 0.
    pub cleared: u32,

    /// Undefined flags.
    pub undefined: u32,
}

/// EVEX rounding mode.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum EvexRounding {
    /// Round to nearest equal.
    NearestEqual,
    /// Round down.
    Down,
    /// Round up.
    Up,
    /// round to zero.
    Zero,
}

#[doc(hidden)]
impl EvexRounding {
    pub(crate) fn from_raw(value: u8) -> Result<Self, DecodeError> {
        if value == ffi::_ND_ROUNDING::ND_RND_RNE as u8 {
            Ok(EvexRounding::NearestEqual)
        } else if value == ffi::_ND_ROUNDING::ND_RND_RD as u8 {
            Ok(EvexRounding::Down)
        } else if value == ffi::_ND_ROUNDING::ND_RND_RU as u8 {
            Ok(EvexRounding::Up)
        } else if value == ffi::_ND_ROUNDING::ND_RND_RZ as u8 {
            Ok(EvexRounding::Zero)
        } else {
            Err(DecodeError::InternalError(value.into()))
        }
    }
}

/// Indicates which prefixes are valid for an instruction.
#[allow(clippy::struct_excessive_bools)]
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct ValidPrefixes {
    /// The instruction supports REP prefix.
    pub rep: bool,
    /// The instruction supports REPZ/REPNZ prefixes.
    pub rep_cond: bool,
    /// The instruction supports LOCK prefix.
    pub lock: bool,
    /// The instruction supports XACQUIRE/XRELEASE prefixes.
    pub hle: bool,
    /// The instruction supports only XACQUIRE.
    pub xacquire: bool,
    /// The instruction supports only XRELEASE.
    pub xrelease: bool,
    /// The instruction supports BND prefix.
    pub bnd: bool,
    /// The instruction supports branch hints.
    pub bhint: bool,
    /// HLE prefix is accepted without LOCK.
    pub hle_no_lock: bool,
    /// The instruction supports the DNT (Do Not Track) CET prefix.
    pub dnt: bool,
}

#[doc(hidden)]
impl ValidPrefixes {
    pub(crate) fn from_raw(raw: ffi::ND_VALID_PREFIXES) -> Self {
        let raw = unsafe { raw.__bindgen_anon_1 };
        Self {
            rep: raw.Rep() != 0,
            rep_cond: raw.RepCond() != 0,
            lock: raw.Lock() != 0,
            hle: raw.Hle() != 0,
            xacquire: raw.Xacquire() != 0,
            xrelease: raw.Xrelease() != 0,
            bnd: raw.Bnd() != 0,
            bhint: raw.Bhint() != 0,
            hle_no_lock: raw.HleNoLock() != 0,
            dnt: raw.Dnt() != 0,
        }
    }
}

/// Indicates which decorators are valid for an instruction.
#[allow(clippy::struct_excessive_bools)]
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct ValidDecorators {
    /// The instruction supports embedded rounding mode.
    pub er: bool,
    /// The instruction supports suppress all exceptions mode.
    pub sae: bool,
    /// The instruction supports zeroing.
    pub zero: bool,
    /// The instruction supports mask registers.
    pub mask: bool,
    /// The instruction supports broadcast.
    pub broadcast: bool,
}

#[doc(hidden)]
impl ValidDecorators {
    pub(crate) fn from_raw(raw: ffi::ND_VALID_DECORATORS) -> Self {
        let raw = unsafe { raw.__bindgen_anon_1 };
        Self {
            er: raw.Er() != 0,
            sae: raw.Sae() != 0,
            zero: raw.Zero() != 0,
            mask: raw.Mask() != 0,
            broadcast: raw.Broadcast() != 0,
        }
    }
}

/// A decoded instruction.
#[derive(Copy, Clone, Debug)]
pub struct DecodedInstruction {
    inner: ffi::INSTRUX_MINI,
    ip: u64,
    instruction: Mnemonic,
    length: usize,
}

impl DecodedInstruction {
    /// Decodes an array of bytes.
    ///
    /// # Arguments
    ///
    /// * `code` - An [`u8`] slice that holds the code to be decoded. Note that decoding is attempted only from offset
    ///     0 inside this code chunk.
    /// * `mode` - The mode in which to decode the instruction.
    /// * `ip` - The instruction pointer value to use when formatting the decoded instruction. Does not affect the
    ///     decoding process in any way. If not needed, use [decode](DecodedInstruction::decode) instead.
    ///
    /// # Errors
    ///
    /// Wil return `Err` if the given bytes do not encode a valid instruction in the given mode.
    ///
    /// # Examples
    ///
    /// ```
    /// # use bddisasm::DecodeError;
    /// #
    /// # fn main() -> Result<(), DecodeError> {
    /// use bddisasm::{DecodedInstruction, DecodeMode, Mnemonic};
    ///
    /// let code = vec![0x48, 0x8b, 0x05, 0xf9, 0xff, 0xff, 0xff];
    ///
    /// let instruction = DecodedInstruction::decode_with_ip(&code, DecodeMode::Bits64, 0)?;
    /// assert_eq!(format!("{}", instruction), "MOV       rax, qword ptr [rel 0x0]");
    ///
    /// let instruction = DecodedInstruction::decode_with_ip(&code, DecodeMode::Bits64, 0x100)?;
    /// assert_eq!(format!("{}", instruction), "MOV       rax, qword ptr [rel 0x100]");
    /// # Ok(())
    /// # }
    /// ```
    pub fn decode_with_ip(code: &[u8], mode: DecodeMode, ip: u64) -> DecodeResult {
        let mut instrux: mem::MaybeUninit<ffi::INSTRUX_MINI> = mem::MaybeUninit::uninit();
        let instrux = instrux.as_mut_ptr();

        let status =
            unsafe { ffi::NdDecodeMini(instrux, code.as_ptr(), code.len() as u64, mode.into()) };

        status_to_error(status)?;

        let instrux = unsafe { *instrux };
        Ok(DecodedInstruction {
            inner: instrux,
            ip,
            instruction: Mnemonic::try_from(instrux.Instruction).unwrap(),
            length: instrux.Length as usize,
        })
    }

    /// Decodes an array of bytes.
    ///
    /// This function is a thin wrapper over [`decode_with_ip`](DecodedInstruction::decode_with_ip) and behaves exactly
    /// the same. It sets `ip` to 0.
    ///
    /// # Errors
    ///
    /// Wil return `Err` if the given bytes do not encode a valid instruction in the given mode.
    pub fn decode(code: &[u8], mode: DecodeMode) -> DecodeResult {
        Self::decode_with_ip(code, mode, 0)
    }

    /// Get the mnemonic of the instruction.
    #[inline]
    #[must_use]
    pub fn mnemonic(&self) -> Mnemonic {
        self.instruction
    }

    /// Get the instruction length, in bytes.
    ///
    /// It is guaranteed that no instruction will exceed a length of 15 bytes.
    #[inline]
    #[must_use]
    pub fn length(&self) -> usize {
        self.length
    }

    /// Get the operand with the given index.
    ///
    /// # Examples
    ///
    /// See [`Operand`] for more in-depth examples on how to work with instruction operands.
    ///
    /// ```
    /// # use bddisasm::DecodeError;
    /// #
    /// # fn main() -> Result<(), DecodeError> {
    /// use bddisasm::{DecodedInstruction, DecodeMode, Mnemonic};
    ///
    /// // `MOV       ebx, dword ptr [ecx+edi]`
    /// let instruction = DecodedInstruction::decode(b"\x8b\x1c\x39", DecodeMode::Bits32)?;
    /// // The first operand is the destination
    /// let dst = instruction.operand(0)?;
    /// // The second operand is the source
    /// let src = instruction.operand(1)?;
    ///
    /// // Print information about the operands
    /// println!("{:#?} {:#?}", dst.info, src.info);
    /// # Ok(())
    /// # }
    /// ```
    #[inline]
    pub fn operand(&self, index: u8) -> Result<Operand, DecodeError> {
        let mut op: MaybeUninit<ffi::_ND_OPERAND> = MaybeUninit::uninit();
        let op = op.as_mut_ptr();

        let status =
            unsafe { NdGetOperandMini(&self.inner as *const ffi::INSTRUX_MINI, index, op) };
        status_to_error(status)?;

        Operand::from_raw(unsafe { *op })
    }

    /// Returns the CPUID support flag.
    ///
    /// If [`None`], the instruction is supported on any CPU, and no CPUID flag exists.
    ///
    /// # Examples
    ///
    /// See [cpuid](crate::cpuid) for more in-depth examples on how to use the CPUID information.
    ///
    /// ```
    /// # use bddisasm::DecodeError;
    /// #
    /// # fn main() -> Result<(), DecodeError> {
    /// use bddisasm::{DecodedInstruction, DecodeMode, Mnemonic};
    ///
    /// // `RDMSR`
    /// let instruction = DecodedInstruction::decode(b"\x0f\x32", DecodeMode::Bits64)?;
    /// let cpuid = instruction.cpuid();
    /// assert!(cpuid.is_some());
    ///
    /// let cpuid = cpuid.unwrap();
    /// assert_eq!(cpuid.leaf, 1);
    /// assert_eq!(cpuid.sub_leaf, None);
    /// assert_eq!(cpuid.register, 2);
    /// assert_eq!(cpuid.bit, 5);
    ///
    /// # Ok(())
    /// # }
    /// ```
    #[must_use]
    pub fn cpuid(&self) -> Option<Cpuid> {
        let mut cpuid_flags: MaybeUninit<ffi::_ND_CPUID_FLAG> = MaybeUninit::uninit();
        let cpuid_flags = cpuid_flags.as_mut_ptr();

        let status = unsafe {
            ffi::NdGetCpuidFlagMini(&self.inner as *const ffi::INSTRUX_MINI, cpuid_flags)
        };
        // Note: the only errors that can be returned from `NdGetCpuidFlagMini` are for invalid `INSTRUX_MINI`
        // instances, which can not happen in our context.
        status_to_error(status).ok()?;

        let cpuid = unsafe { *cpuid_flags };
        let cpuid = unsafe { cpuid.__bindgen_anon_1 };
        let leaf = cpuid.Leaf;
        if leaf == ffi::ND_CFF_NO_LEAF {
            None
        } else {
            let sub_leaf = cpuid.SubLeaf();
            let sub_leaf = if sub_leaf == ffi::ND_CFF_NO_SUBLEAF {
                None
            } else {
                Some(sub_leaf)
            };

            let register = cpuid.Reg() as u8;
            let bit = u64::from(cpuid.Bit());

            Some(Cpuid {
                leaf,
                sub_leaf,
                register,
                bit,
            })
        }
    }

    /// Get the encoding mode used.
    ///
    /// # Panics
    ///
    /// This function will panic if the encoding mode is unrecognized. This can not happen under normal circumstances.
    #[inline]
    #[must_use]
    pub fn encoding_mode(&self) -> EncodingMode {
        EncodingMode::from_raw(u32::from(self.inner.EncMode())).unwrap()
    }

    /// Get the VEX mode, if any.
    ///
    /// # Panics
    ///
    /// This function will panic if the VEX mode is unrecognized. This can not happen under normal circumstances.
    #[inline]
    #[must_use]
    pub fn vex_mode(&self) -> Option<VexMode> {
        if self.has_vex() {
            Some(VexMode::from_raw(u32::from(self.inner.VexMode())).unwrap())
        } else {
            None
        }
    }

    /// Get the addressing mode.
    ///
    /// # Panics
    ///
    /// This function will panic if the addressing mode is unrecognized. This can not happen under normal circumstances.
    #[inline]
    #[must_use]
    pub fn addr_mode(&self) -> AddressingMode {
        AddressingMode::from_raw(u32::from(self.inner.AddrMode())).unwrap()
    }

    /// Get the operand mode/size.
    ///
    /// This is computed based on the passed-in [`DecodeMode`] and instruction prefixes.
    ///
    /// # Remarks
    ///
    /// The effective mode can be different (see [`effective_op_mode`](DecodedInstruction::effective_op_mode)).
    ///
    /// # Examples
    ///
    /// Using [`DecodeMode::Bits64`], `0x50` encodes a `PUSH rax` instruction with an operand size of
    /// 32 because it has no prefix that promotes it, but the effective size is 64 because the instruction always
    /// operates on 64 bits.
    ///
    /// ```
    /// # use bddisasm::DecodeError;
    /// #
    /// # fn main() -> Result<(), DecodeError> {
    /// use bddisasm::{DecodedInstruction, DecodeMode, Mnemonic, OperandSize};
    ///
    /// let ins =
    ///     DecodedInstruction::decode(&[0x50], DecodeMode::Bits64)?;
    /// assert_eq!(ins.mnemonic(), Mnemonic::PUSH);
    /// assert_eq!(ins.op_mode(), OperandSize::OpSize32);
    /// assert_eq!(ins.effective_op_mode(), OperandSize::OpSize64);
    ///
    /// let ins =
    ///     DecodedInstruction::decode(&[0x48, 0x50], DecodeMode::Bits64)?;
    /// assert_eq!(ins.mnemonic(), Mnemonic::PUSH);
    /// assert_eq!(ins.op_mode(), OperandSize::OpSize64);
    /// assert_eq!(ins.effective_op_mode(), OperandSize::OpSize64);
    /// # Ok(())
    /// # }
    /// ```
    ///
    /// # Panics
    ///
    /// This function will panic if the operand size is unrecognized. This can not happen under normal circumstances.
    #[inline]
    #[must_use]
    pub fn op_mode(&self) -> OperandSize {
        OperandSize::from_raw(u32::from(self.inner.OpMode())).unwrap()
    }

    /// Get the effective operand mode/size.
    ///
    /// Unlike [`op_mode`](DecodedInstruction::op_mode), this takes into account the actual instruction.
    ///
    /// # Panics
    ///
    /// This function will panic if the operand size is unrecognized. This can not happen under normal circumstances.
    #[inline]
    #[must_use]
    pub fn effective_op_mode(&self) -> OperandSize {
        OperandSize::from_raw(u32::from(self.inner.EfOpMode())).unwrap()
    }

    /// Get the Vector mode/size, if any.
    ///
    /// This is computed based on the passed-in [`DecodeMode`] and instruction prefixes.
    ///
    /// # Remarks
    ///
    /// The effective mode can be different (see [`effective_vec_mode`](DecodedInstruction::effective_vec_mode)).
    ///
    /// # Panics
    ///
    /// This function will panic if the vector mode is unrecognized. This can not happen under normal circumstances.
    #[inline]
    #[must_use]
    pub fn vec_mode(&self) -> Option<VectorSize> {
        if self.has_vector() {
            Some(VectorSize::from_raw(u32::from(self.inner.VecMode())).unwrap())
        } else {
            None
        }
    }

    /// Get the Vector mode/size, if any.
    ///
    /// Unlike [`vec_mode`](DecodedInstruction::vec_mode), this takes into account the actual instruction.
    ///
    /// # Panics
    ///
    /// This function will panic if the vector mode is unrecognized. This can not happen under normal circumstances.
    #[inline]
    #[must_use]
    pub fn effective_vec_mode(&self) -> Option<VectorSize> {
        if self.has_vector() {
            Some(VectorSize::from_raw(u32::from(self.inner.EfVecMode())).unwrap())
        } else {
            None
        }
    }

    /// `true` if REX is present.
    #[inline]
    #[must_use]
    pub fn has_rex(&self) -> bool {
        self.inner.HasRex() != 0
    }

    /// `true` if VEX is present.
    #[inline]
    #[must_use]
    pub fn has_vex(&self) -> bool {
        self.inner.HasVex() != 0
    }

    /// `true` if XOP is present.
    #[inline]
    #[must_use]
    pub fn has_xop(&self) -> bool {
        self.inner.HasXop() != 0
    }

    /// `true` if EVEX is present.
    #[inline]
    #[must_use]
    pub fn has_evex(&self) -> bool {
        self.inner.HasEvex() != 0
    }

    /// `true` if 0x66 is present.
    #[inline]
    #[must_use]
    pub fn has_op_size(&self) -> bool {
        self.inner.HasOpSize() != 0
    }

    /// `true` if 0x67 is present.
    #[inline]
    #[must_use]
    pub fn has_addr_size(&self) -> bool {
        self.inner.HasAddrSize() != 0
    }

    /// `true` if 0xF0 is present.
    #[inline]
    #[must_use]
    pub fn has_lock(&self) -> bool {
        self.inner.HasLock() != 0
    }

    /// `true` if 0xF2 is present.
    #[inline]
    #[must_use]
    pub fn has_repnz_xacquire_bnd(&self) -> bool {
        self.inner.HasRepnzXacquireBnd() != 0
    }

    /// `true` if 0xF3 is present.
    #[inline]
    #[must_use]
    pub fn has_rep_repz_xrelease(&self) -> bool {
        self.inner.HasRepRepzXrelease() != 0
    }

    /// `true` if segment override is present.
    #[inline]
    #[must_use]
    pub fn has_seg(&self) -> bool {
        self.inner.HasSeg() != 0
    }

    /// `true` if the instruction is repeated up to `RCX` times.
    #[inline]
    #[must_use]
    pub fn is_repeated(&self) -> bool {
        self.inner.IsRepeated() != 0
    }

    /// `true` if the instruction is XACQUIRE enabled.
    #[inline]
    #[must_use]
    pub fn is_xacquire_enabled(&self) -> bool {
        self.inner.IsXacquireEnabled() != 0
    }

    /// `true` if the instruction is XRELEASE enabled.
    #[inline]
    #[must_use]
    pub fn is_xrelease_enabled(&self) -> bool {
        self.inner.IsXreleaseEnabled() != 0
    }

    /// `true` if the instruction uses RIP relative addressing.
    #[inline]
    #[must_use]
    pub fn is_rip_relative(&self) -> bool {
        self.inner.IsRipRelative() != 0
    }

    /// `true` if this is an indirect CALL/JMP that is CET tracked.
    #[inline]
    #[must_use]
    pub fn is_cet_tracked(&self) -> bool {
        self.inner.IsCetTracked() != 0
    }

    /// `true` if we have valid MODRM.
    #[inline]
    #[must_use]
    pub fn has_mod_rm(&self) -> bool {
        self.inner.HasModRm() != 0
    }

    /// `true` if we have valid SIB.
    #[inline]
    #[must_use]
    pub fn has_sib(&self) -> bool {
        self.inner.HasSib() != 0
    }

    /// `true` if the instruction has displacement.
    #[inline]
    #[must_use]
    pub fn has_disp(&self) -> bool {
        self.inner.HasDisp() != 0
    }

    /// `true` if the instruction contains a direct address (ie, `CALL far 0x9A`).
    #[inline]
    #[must_use]
    pub fn has_addr(&self) -> bool {
        self.inner.HasAddr() != 0
    }

    /// `true` if the instruction contains a moffset (ie, `MOV al, [mem], 0xA0`).
    #[inline]
    #[must_use]
    pub fn has_moffset(&self) -> bool {
        self.inner.HasMoffset() != 0
    }

    /// `true` if immediate is present.
    #[inline]
    #[must_use]
    pub fn has_imm1(&self) -> bool {
        self.inner.HasImm1() != 0
    }

    /// `true` if second immediate is present.
    #[inline]
    #[must_use]
    pub fn has_imm2(&self) -> bool {
        self.inner.HasImm2() != 0
    }

    /// `true` if the instruction contains a relative offset (ie, `Jcc 0x7x`).
    #[inline]
    #[must_use]
    pub fn has_rel_offs(&self) -> bool {
        self.inner.HasRelOffs() != 0
    }

    /// `true` if SSE immediate that encodes additional registers is present.
    #[inline]
    #[must_use]
    pub fn has_sse_imm(&self) -> bool {
        self.inner.HasSseImm() != 0
    }

    /// `true` if the instruction uses compressed displacement.
    #[inline]
    #[must_use]
    pub fn has_comp_disp(&self) -> bool {
        self.inner.HasCompDisp() != 0
    }

    /// `true` if the instruction uses broadcast addressing.
    #[inline]
    #[must_use]
    pub fn has_broadcast(&self) -> bool {
        self.inner.HasBroadcast() != 0
    }

    /// `true` if the instruction has mask.
    #[inline]
    #[must_use]
    pub fn has_mask(&self) -> bool {
        self.inner.HasMask() != 0
    }

    /// `true` if the instruction uses zeroing.
    #[inline]
    #[must_use]
    pub fn has_zero(&self) -> bool {
        self.inner.HasZero() != 0
    }

    /// `true` if the instruction has embedded rounding.
    #[inline]
    #[must_use]
    pub fn has_er(&self) -> bool {
        self.inner.HasEr() != 0
    }

    /// `true` if the instruction has SAE.
    #[inline]
    #[must_use]
    pub fn has_sae(&self) -> bool {
        self.inner.HasSae() != 0
    }

    /// `true` if the instruction ignores embedded rounding.
    #[inline]
    #[must_use]
    pub fn has_ign_er(&self) -> bool {
        self.inner.HasIgnEr() != 0
    }

    /// `true` if changing prefix.
    #[inline]
    #[must_use]
    pub fn has_mandatory_66(&self) -> bool {
        self.inner.HasMandatory66() != 0
    }

    /// 0x66 is mandatory prefix. Does not behave as REP prefix.
    #[inline]
    #[must_use]
    pub fn has_mandatory_f2(&self) -> bool {
        self.inner.HasMandatoryF2() != 0
    }

    /// 0x66 is mandatory prefix. Does not behave as REP prefix.
    #[inline]
    #[must_use]
    pub fn has_mandatory_f3(&self) -> bool {
        self.inner.HasMandatoryF3() != 0
    }

    /// The length of the instruction word. 2, 4 or 8.
    #[inline]
    #[must_use]
    pub fn word_length(&self) -> usize {
        self.inner.WordLength() as usize
    }

    /// The total number of bytes consumed by prefixes.
    ///
    /// This will also be the offset to the first opcode. The primary opcode will always be the last one.
    #[inline]
    #[must_use]
    pub fn pref_length(&self) -> usize {
        self.inner.PrefLength() as usize
    }

    /// Number of opcode bytes. Max 3.
    #[inline]
    #[must_use]
    pub fn op_length(&self) -> usize {
        self.inner.OpLength() as usize
    }

    /// Displacement length, in bytes. Maximum 4.
    #[inline]
    #[must_use]
    pub fn disp_length(&self) -> usize {
        self.inner.DispLength() as usize
    }

    /// Absolute address length, in bytes. Maximum 8 bytes.
    #[inline]
    #[must_use]
    pub fn addr_length(&self) -> usize {
        self.inner.AddrLength() as usize
    }

    /// Memory offset length, in bytes. Maximum 8 bytes.
    #[inline]
    #[must_use]
    pub fn moffset_length(&self) -> usize {
        self.inner.MoffsetLength() as usize
    }

    /// First immediate length, in bytes. Maximum 8 bytes.
    #[inline]
    #[must_use]
    pub fn imm1_length(&self) -> usize {
        self.inner.Imm1Length() as usize
    }

    /// Second immediate length, in bytes. Maximum 8 bytes.
    #[inline]
    #[must_use]
    pub fn imm2_length(&self) -> usize {
        self.inner.Imm2Length() as usize
    }

    /// Relative offset length, in bytes. Maximum 4 bytes.
    #[inline]
    #[must_use]
    pub fn rel_offs_length(&self) -> usize {
        self.inner.RelOffsLength() as usize
    }

    /// The offset of the first opcode, inside the instruction.
    #[inline]
    #[must_use]
    pub fn op_offset(&self) -> usize {
        self.inner.OpOffset() as usize
    }

    /// The offset of the nominal opcode, inside the instruction.
    #[inline]
    #[must_use]
    pub fn main_op_offset(&self) -> usize {
        self.inner.MainOpOffset() as usize
    }

    /// The offset of the displacement, inside the instruction.
    #[inline]
    #[must_use]
    pub fn disp_offset(&self) -> Option<usize> {
        let value = self.inner.DispOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the hard-coded address.
    #[inline]
    #[must_use]
    pub fn addr_offset(&self) -> Option<usize> {
        let value = self.inner.AddrOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the absolute address, inside the instruction.
    #[inline]
    #[must_use]
    pub fn moffset_offset(&self) -> Option<usize> {
        let value = self.inner.MoffsetOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the immediate, inside the instruction.
    #[inline]
    #[must_use]
    pub fn imm1_offset(&self) -> Option<usize> {
        let value = self.inner.Imm1Offset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the second immediate, if any, inside the instruction.
    #[inline]
    #[must_use]
    pub fn imm2_offset(&self) -> Option<usize> {
        let value = self.inner.Imm2Offset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the relative offset used in instruction.
    #[inline]
    #[must_use]
    pub fn rel_offs_offset(&self) -> Option<usize> {
        let value = self.inner.RelOffsOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the SSE immediate, if any, inside the instruction.
    #[inline]
    #[must_use]
    pub fn sse_imm_offset(&self) -> Option<usize> {
        let value = self.inner.SseImmOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the mod rm byte inside the instruction, if any.
    #[inline]
    #[must_use]
    pub fn mod_rm_offset(&self) -> Option<usize> {
        let value = self.inner.ModRmOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// Number of words accessed on/from the stack.
    #[inline]
    pub fn stack_words(&self) -> Result<usize, DecodeError> {
        let mut stack_words: u8 = 0;
        let status = unsafe {
            ffi::NdGetStackWordsMini(&self.inner as *const ffi::INSTRUX_MINI, &mut stack_words)
        };
        status_to_error(status)?;

        Ok(stack_words as usize)
    }

    /// The last rep/repz/repnz prefix. if any.
    #[inline]
    #[must_use]
    pub fn rep(&self) -> Option<u8> {
        let value = self.inner.Rep;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The last segment override prefix. if none. `FS`/`GS` if 64 bit.
    #[inline]
    #[must_use]
    pub fn seg(&self) -> Option<u8> {
        let value = self.inner.Seg;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// Get the `ModRM` byte.
    #[inline]
    #[must_use]
    pub fn mod_rm(&self) -> Option<u8> {
        if self.has_mod_rm() {
            Some(unsafe { self.inner.ModRm.ModRm })
        } else {
            None
        }
    }

    /// Get the `SIB` byte.
    #[inline]
    #[must_use]
    pub fn sib(&self) -> Option<u8> {
        if self.has_sib() {
            Some(unsafe { self.inner.Sib.Sib })
        } else {
            None
        }
    }

    /// Get the 2-bytes `VEX` prefix.
    #[inline]
    #[must_use]
    pub fn vex2(&self) -> Option<(u8, u8)> {
        if matches!(self.vex_mode(), Some(VexMode::Vex2b)) {
            let vex2 = self.inner.__bindgen_anon_1;
            let vex2 = unsafe { vex2.Vex2.Vex };

            Some((vex2[0], vex2[1]))
        } else {
            None
        }
    }

    /// Get the 3-bytes `VEX` prefix.
    #[inline]
    #[must_use]
    pub fn vex3(&self) -> Option<(u8, u8, u8)> {
        if matches!(self.vex_mode(), Some(VexMode::Vex3b)) {
            let vex3 = self.inner.__bindgen_anon_1;
            let vex3 = unsafe { vex3.Vex3.Vex };

            Some((vex3[0], vex3[1], vex3[2]))
        } else {
            None
        }
    }

    /// Get the `XOP` bytes.
    #[inline]
    #[must_use]
    pub fn xop(&self) -> Option<(u8, u8, u8)> {
        if self.has_xop() {
            let xop = self.inner.__bindgen_anon_1;
            let xop = unsafe { xop.Xop.Xop };

            Some((xop[0], xop[1], xop[2]))
        } else {
            None
        }
    }

    /// Get the `EVEX` bytes.
    #[inline]
    #[must_use]
    pub fn evex(&self) -> Option<(u8, u8, u8, u8)> {
        if self.has_evex() {
            let evex = self.inner.__bindgen_anon_1;
            let evex = unsafe { evex.Evex.Evex };

            Some((evex[0], evex[1], evex[2], evex[3]))
        } else {
            None
        }
    }

    /// Get the absolute offset, if any.
    #[inline]
    #[must_use]
    pub fn moffset(&self) -> Option<u64> {
        if self.has_moffset() {
            Some(unsafe { self.inner.__bindgen_anon_2.Moffset })
        } else {
            None
        }
    }

    /// Get the displacement. Max 4 bytes. Used in `ModRM` instructions.
    #[inline]
    #[must_use]
    pub fn disp(&self) -> Option<u32> {
        if self.has_disp() {
            Some(unsafe { self.inner.__bindgen_anon_2.Displacement })
        } else {
            None
        }
    }

    /// Get the relative offset, used for branches. Max 4 bytes.
    #[inline]
    #[must_use]
    pub fn rel_offset(&self) -> Option<u32> {
        if self.has_rel_offs() {
            Some(unsafe { self.inner.__bindgen_anon_2.RelativeOffset })
        } else {
            None
        }
    }

    /// Get the first immediate.
    #[inline]
    #[must_use]
    pub fn immediate1(&self) -> Option<u64> {
        if self.has_imm1() {
            Some(self.inner.Immediate1)
        } else {
            None
        }
    }

    /// Get the second immediate. Used mainly for [`Mnemonic::ENTER`].
    #[inline]
    #[must_use]
    pub fn immediate2(&self) -> Option<u8> {
        if self.has_imm2() {
            Some(unsafe { self.inner.__bindgen_anon_3.Immediate2 })
        } else {
            None
        }
    }

    /// Get the SSE immediate. It is used to select a register.
    #[inline]
    #[must_use]
    pub fn sse_immediate(&self) -> Option<u8> {
        if self.has_sse_imm() {
            Some(unsafe { self.inner.__bindgen_anon_3.SseImmediate })
        } else {
            None
        }
    }

    /// Get the raw instruction attributes.
    #[inline]
    #[must_use]
    pub fn get_attributes(&self) -> u64 {
        let mut attrs: u64 = 0;
        unsafe { ffi::NdGetAttributesMini(&self.inner as *const ffi::INSTRUX_MINI, &mut attrs) };

        attrs
    }

    /// Get the condition byte.
    #[inline]
    #[must_use]
    pub fn cond(&self) -> Option<u8> {
        let attrs = self.get_attributes();

        if (attrs & ffi::ND_FLAG_COND as u64) != 0 {
            Some(self.inner.__bindgen_anon_4.Condition())
        } else {
            None
        }
    }

    /// `true` if the instruction is 3DNow!.
    ///
    /// The opcode is the last byte.
    #[inline]
    #[must_use]
    pub fn is_3d_now(&self) -> bool {
        let attrs = self.get_attributes();
        (attrs & ffi::ND_FLAG_3DNOW as u64) != 0
    }

    /// Get the number of operands.
    #[inline]
    #[must_use]
    pub fn operands_count(&self) -> usize {
        self.inner.OperandsCount() as usize
    }

    /// Number of explicit operands.
    ///
    /// Use this if you want to ignore implicit operands such as stack, flags, etc.
    #[inline]
    #[must_use]
    pub fn exp_operands_count(&self) -> usize {
        self.inner.ExpOperandsCount() as usize
    }

    /// Get the `CS` access mode.
    #[inline]
    #[must_use]
    pub fn cs_access(&self) -> OpAccess {
        let mut acc: MaybeUninit<ffi::_ND_OPERAND_ACCESS> = MaybeUninit::uninit();
        let acc = acc.as_mut_ptr();

        unsafe { ffi::NdGetCsAccessMini(&self.inner as *const ffi::INSTRUX_MINI, acc) };

        OpAccess::from_raw(unsafe { *acc })
    }

    /// Get the `RIP` access mode.
    #[inline]
    #[must_use]
    pub fn rip_access(&self) -> OpAccess {
        let mut acc: MaybeUninit<ffi::_ND_OPERAND_ACCESS> = MaybeUninit::uninit();
        let acc = acc.as_mut_ptr();

        unsafe { ffi::NdGetRipAccessMini(&self.inner as *const ffi::INSTRUX_MINI, acc) };

        OpAccess::from_raw(unsafe { *acc })
    }

    /// Get the stack access mode.
    #[inline]
    #[must_use]
    pub fn stack_access(&self) -> OpAccess {
        let mut acc: MaybeUninit<ffi::_ND_OPERAND_ACCESS> = MaybeUninit::uninit();
        let acc = acc.as_mut_ptr();

        unsafe { ffi::NdGetStackAccessMini(&self.inner as *const ffi::INSTRUX_MINI, acc) };

        OpAccess::from_raw(unsafe { *acc })
    }

    /// Get the RFLAGS access mode.
    #[inline]
    #[must_use]
    pub fn rflags_access(&self) -> OpAccess {
        let mut acc: MaybeUninit<ffi::_ND_OPERAND_ACCESS> = MaybeUninit::uninit();
        let acc = acc.as_mut_ptr();

        unsafe { ffi::NdGetRflagsAccessMini(&self.inner as *const ffi::INSTRUX_MINI, acc) };

        OpAccess::from_raw(unsafe { *acc })
    }

    /// Get the memory access mode.
    ///
    /// This includes the stack or shadow stack access.
    #[inline]
    #[must_use]
    pub fn memory_access(&self) -> OpAccess {
        let mut acc: MaybeUninit<ffi::_ND_OPERAND_ACCESS> = MaybeUninit::uninit();
        let acc = acc.as_mut_ptr();

        unsafe { ffi::NdGetMemoryAccessMini(&self.inner as *const ffi::INSTRUX_MINI, acc) };

        OpAccess::from_raw(unsafe { *acc })
    }

    /// `true` if the instruction is a branch.
    #[inline]
    #[must_use]
    pub fn is_branch(&self) -> bool {
        let rip_acc = self.rip_access();
        rip_acc.write || rip_acc.cond_write
    }

    /// `true` if the instruction is a conditional branch.
    #[inline]
    #[must_use]
    pub fn is_conditional_branch(&self) -> bool {
        let rip_acc = self.rip_access();
        rip_acc.cond_write
    }

    /// `true` if the instruction is a indirect branch.
    #[inline]
    #[must_use]
    pub fn is_indirect_branch(&self) -> bool {
        self.is_branch() && self.inner.HasModRm() != 0
    }

    /// `true` if the instruction is a far branch.
    #[inline]
    #[must_use]
    pub fn is_far_branch(&self) -> bool {
        if self.is_branch() {
            let mut acc: MaybeUninit<ffi::_ND_OPERAND_ACCESS> = MaybeUninit::uninit();
            let acc = acc.as_mut_ptr();

            unsafe { ffi::NdGetCsAccessMini(&self.inner as *const ffi::INSTRUX_MINI, acc) };

            let acc = unsafe { *acc };
            unsafe { acc.Access != 0 }
        } else {
            false
        }
    }

    /// Get the rflags access.
    #[must_use]
    pub fn flags_access(&self) -> FlagsAccess {
        let mut tested: MaybeUninit<ffi::_ND_RFLAGS> = MaybeUninit::uninit();
        let mut modified: MaybeUninit<ffi::_ND_RFLAGS> = MaybeUninit::uninit();
        let mut set: MaybeUninit<ffi::_ND_RFLAGS> = MaybeUninit::uninit();
        let mut cleared: MaybeUninit<ffi::_ND_RFLAGS> = MaybeUninit::uninit();
        let mut undefined: MaybeUninit<ffi::_ND_RFLAGS> = MaybeUninit::uninit();

        unsafe {
            ffi::NdGetFlagsAccessMini(
                &self.inner as *const ffi::INSTRUX_MINI,
                tested.as_mut_ptr(),
                modified.as_mut_ptr(),
                set.as_mut_ptr(),
                cleared.as_mut_ptr(),
                undefined.as_mut_ptr(),
            )
        };

        let mode = self.rflags_access();

        FlagsAccess {
            mode,
            tested: flags_raw(unsafe { *tested.as_ptr() }),
            modified: flags_raw(unsafe { *modified.as_ptr() }),
            set: flags_raw(unsafe { *set.as_ptr() }),
            cleared: flags_raw(unsafe { *cleared.as_ptr() }),
            undefined: flags_raw(unsafe { *undefined.as_ptr() }),
        }
    }

    /// `FPU` status word C0-C3 bits access.
    ///
    /// # Panics
    ///
    /// This function will panic if the access mode is unrecognized. This can not happen under normal circumstances.
    #[inline]
    #[must_use]
    pub fn fpu_flags_access(&self) -> FpuFlags {
        let mut fpu_flags: MaybeUninit<ffi::_ND_FPU_FLAGS> = MaybeUninit::uninit();
        let fpu_flags = fpu_flags.as_mut_ptr();

        unsafe { ffi::NdGetFpuFlagsAccessMini(&self.inner as *const ffi::INSTRUX_MINI, fpu_flags) };

        FpuFlags::from_raw(unsafe { *fpu_flags }).unwrap()
    }

    /// SIMD Floating-Point Exceptions.
    #[inline]
    #[must_use]
    pub fn simd_exceptions(&self) -> SimdExceptions {
        let mut simd_exceptions: MaybeUninit<ffi::_ND_SIMD_EXCEPTIONS> = MaybeUninit::uninit();
        let simd_exceptions = simd_exceptions.as_mut_ptr();

        unsafe {
            ffi::NdGetSimdExceptionsMini(&self.inner as *const ffi::INSTRUX_MINI, simd_exceptions)
        };
        let simd_exceptions = unsafe { *simd_exceptions };

        SimdExceptions::from_raw(unsafe { simd_exceptions.Raw })
    }

    /// `EVEX` tuple type.
    #[inline]
    #[must_use]
    pub fn evex_tuple(&self) -> Option<Tuple> {
        let mut tuple: MaybeUninit<ffi::_ND_TUPLE> = MaybeUninit::uninit();
        let tuple = tuple.as_mut_ptr();

        unsafe { ffi::NdGetTupleTypeMini(&self.inner as *const ffi::INSTRUX_MINI, tuple) };
        let tuple = unsafe { *tuple };

        if self.has_evex() {
            Some(Tuple::from_raw(tuple))
        } else {
            None
        }
    }

    /// `EVEX` rounding mode.
    ///
    /// # Panics
    ///
    /// This function will panic if the EVEX rounding mode is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    #[must_use]
    pub fn evex_rounding(&self) -> Option<EvexRounding> {
        if self.has_er() {
            Some(EvexRounding::from_raw(self.inner.RoundingMode()).unwrap())
        } else {
            None
        }
    }

    /// Get the instruction category.
    #[inline]
    #[must_use]
    pub fn category(&self) -> Option<Category> {
        let mut cat = ffi::ND_INS_CATEGORY::ND_CAT_INVALID;
        unsafe {
            ffi::NdGetInstructionCategoryMini(&self.inner as *const ffi::INSTRUX_MINI, &mut cat)
        };

        Category::from_raw(cat)
    }

    /// Get the ISA set.
    #[inline]
    #[must_use]
    pub fn isa_set(&self) -> Option<IsaSet> {
        let mut set = ffi::ND_INS_SET::ND_SET_INVALID;
        unsafe { ffi::NdGetInstructionSetMini(&self.inner as *const ffi::INSTRUX_MINI, &mut set) };

        IsaSet::from_raw(set)
    }

    /// Get the CPU modes in which the instruction is valid.
    ///
    /// # Examples
    ///
    /// See [`cpu_modes`](crate::cpu_modes) for examples.
    #[inline]
    #[must_use]
    pub fn valid_cpu_modes(&self) -> CpuModes {
        let mut modes: MaybeUninit<ffi::_ND_VALID_MODES> = MaybeUninit::uninit();
        let modes = modes.as_mut_ptr();

        unsafe { ffi::NdGetValidModesMini(&self.inner as *const ffi::INSTRUX_MINI, modes) };

        CpuModes::from_raw(unsafe { *modes })
    }

    /// Get the valid prefixes for this instruction.
    #[inline]
    #[must_use]
    pub fn valid_prefixes(&self) -> ValidPrefixes {
        let mut prefixes: MaybeUninit<ffi::_ND_VALID_PREFIXES> = MaybeUninit::uninit();
        let prefixes = prefixes.as_mut_ptr();

        unsafe { ffi::NdGetValidPrefixesMini(&self.inner as *const ffi::INSTRUX_MINI, prefixes) };

        ValidPrefixes::from_raw(unsafe { *prefixes })
    }

    /// Get the decorators accepted by the instruction.
    #[inline]
    #[must_use]
    pub fn valid_decorators(&self) -> ValidDecorators {
        let mut decorators: MaybeUninit<ffi::_ND_VALID_DECORATORS> = MaybeUninit::uninit();
        let decorators = decorators.as_mut_ptr();

        unsafe {
            ffi::NdGetValidDecoratorsMini(&self.inner as *const ffi::INSTRUX_MINI, decorators)
        };

        ValidDecorators::from_raw(unsafe { *decorators })
    }

    /// Get the main/nominal opcode.
    #[inline]
    #[must_use]
    pub fn primary_op_code(&self) -> u8 {
        unsafe { self.inner.__bindgen_anon_4.PrimaryOpCode }
    }

    /// `true` if the instruction is a SIMD instruction that operates on vector regs.
    #[inline]
    #[must_use]
    pub fn has_vector(&self) -> bool {
        let attrs = self.get_attributes();
        attrs & ffi::ND_FLAG_VECTOR as u64 != 0
    }
}

impl fmt::Display for DecodedInstruction {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let mut buffer: [u8; ffi::ND_MIN_BUF_SIZE as usize] = [0; ffi::ND_MIN_BUF_SIZE as usize];
        let status = unsafe {
            ffi::NdToTextMini(
                &self.inner as *const ffi::INSTRUX_MINI,
                self.ip,
                buffer.len() as u32,
                buffer.as_mut_ptr().cast::<i8>(),
            )
        };
        match status_to_error(status) {
            Ok(_) => {
                let text = core::str::from_utf8(&buffer).unwrap();
                write!(f, "{}", text.trim_matches(char::from(0)))
            }
            Err(_) => Err(fmt::Error),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::operand;

    #[test]
    fn decode() {
        let code = vec![0xb8, 0x00, 0x00, 0x00, 0x00];
        let ins = DecodedInstruction::decode(&code, DecodeMode::Bits32).expect("Unable to decode");
        assert_eq!(ins.instruction, Mnemonic::MOV);
        assert_eq!(format!("{}", ins), "MOV       eax, 0x00000000");
    }

    #[test]
    fn decode_with_ip() {
        let code = b"\x48\x8b\x05\xf9\xff\xff\xff";
        let ins = DecodedInstruction::decode_with_ip(code, DecodeMode::Bits64, 0x100)
            .expect("Unable to decode");
        assert_eq!(ins.instruction, Mnemonic::MOV);
        assert_eq!(format!("{}", ins), "MOV       rax, qword ptr [rel 0x100]");
    }

    fn get_tokens(line: &str, index: usize) -> u32 {
        let tokens = line.split(" ").collect::<Vec<&str>>();
        let tokens = tokens
            .into_iter()
            .filter(|s| !s.is_empty())
            .collect::<Vec<&str>>();

        let token = tokens[index];
        if token.starts_with("0x") {
            u32::from_str_radix(token.trim_start_matches("0x"), 16).unwrap()
        } else {
            token.parse().unwrap()
        }
    }

    #[test]
    fn constants() {
        // These will probably never change, so it is not really worth it to generate the enums and the `From`
        // implementations at every build, but these tests should be enough to catch the unlikely situation in which
        // a new constant is added.
        let bindings = include_str!("../../bddisasm-sys/csrc/inc/bddisasm.h");
        let mut shadow_stack_count: u8 = 0;
        let mut evex_rounding: u8 = 0;
        for line in bindings.lines() {
            if line.starts_with("#define ND_ENCM_") {
                assert!(EncodingMode::from_raw(get_tokens(line, 2)).is_ok());
            } else if line.starts_with("#define ND_VEXM_") {
                assert!(VexMode::from_raw(get_tokens(line, 2)).is_ok());
            } else if line.starts_with("#define ND_ADDR_") {
                assert!(AddressingMode::from_raw(get_tokens(line, 2)).is_ok());
            } else if line.starts_with("#define ND_OPSZ_") {
                assert!(OperandSize::from_raw(get_tokens(line, 2)).is_ok());
            } else if line.starts_with("#define ND_VECM_") {
                assert!(VectorSize::from_raw(get_tokens(line, 2)).is_ok());
            } else if line.starts_with("#define ND_SIZE_")
                && !line.starts_with("#define ND_SIZE_TO_MASK(sz)")
            {
                // TODO: this test should be in `operand.rs`, but since we are already parsing `bddisasm.h` here it
                // felt wastefull to also parse it there.
                assert!(operand::OpSize::from_raw(get_tokens(line, 2)).is_ok());
            } else if line.starts_with("    ND_SHSTK_") {
                // TODO: this test should be in `operand.rs`, but since we are already parsing `bddisasm.h` here it
                // felt wastefull to also parse it there.
                assert!(operand::ShadowStackAccess::from_raw(shadow_stack_count).is_ok());
                shadow_stack_count += 1;
            } else if line.starts_with("#define ND_FPU_FLAG_") {
                // TODO: this test should be in `fpu_flags.rs`, but since we are already parsing `bddisasm.h` here it
                // felt wastefull to also parse it there.
                assert!(
                    crate::fpu_flags::FpuFlagsAccess::from_raw(get_tokens(line, 2) as u8).is_ok()
                );
            } else if line.starts_with("    ND_RND_") {
                // TODO: this test should be in `operand.rs`, but since we are already parsing `bddisasm.h` here it
                // felt wastefull to also parse it there.
                assert!(EvexRounding::from_raw(evex_rounding).is_ok());
                evex_rounding += 1;
            }
        }
    }

    #[test]
    fn status() {
        let status = include_str!("../../bddisasm-sys/csrc/inc/bddisasm_status.h");
        for line in status.lines() {
            if line.starts_with("#define ND_STATUS_SUCCESS")
                || line.starts_with("#define ND_STATUS_HINT_OPERAND_NOT_USED")
                || line.starts_with("#define ND_STATUS_HINT_OPERAND_NOT_PRESENT")
            {
                assert!(status_to_error(get_tokens(line, 2)).is_ok());
            } else if line.starts_with("#define ND_STATUS_") {
                assert!(status_to_error(get_tokens(line, 2)).is_err());
            }
        }
    }

    #[test]
    fn check_all_evex_roundings() {
        // This is a really contrieved way of making sure that we check all variants of `ffi::_ND_ROUNDING`. If a new
        // one is added, this will fail to build. We do this vecause `EvexRounding::from_raw` takes an `u8`.
        // NOTE: When a new variant is added, `EvexRounding::from_raw` must be updated.
        match ffi::_ND_ROUNDING::ND_RND_RNE {
            ffi::_ND_ROUNDING::ND_RND_RNE => {}
            ffi::_ND_ROUNDING::ND_RND_RD => {}
            ffi::_ND_ROUNDING::ND_RND_RU => {}
            ffi::_ND_ROUNDING::ND_RND_RZ => {}
        }
    }
}
