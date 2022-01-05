/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Decodes instructions.

use crate::cpu_modes::CpuModes;
use crate::cpuid::Cpuid;
use crate::decode_error::{status_to_error, DecodeError};
use crate::fpu_flags::FpuFlags;
use crate::instruction_category::Category;
use crate::isa_set::IsaSet;
use crate::mnemonic::Mnemonic;
use crate::operand;
use crate::operand::{OpAccess, OpAddr, Operands, OperandsLookup};
use crate::rflags::flags_raw;
use crate::tuple::Tuple;

use core::convert::TryFrom;
use core::fmt;
use core::mem;

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
impl From<DecodeMode> for (u8, u8) {
    fn from(mode: DecodeMode) -> Self {
        match mode {
            DecodeMode::Bits16 => (ffi::ND_CODE_16 as u8, ffi::ND_DATA_16 as u8),
            DecodeMode::Bits32 => (ffi::ND_CODE_32 as u8, ffi::ND_DATA_32 as u8),
            DecodeMode::Bits64 => (ffi::ND_CODE_64 as u8, ffi::ND_DATA_64 as u8),
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

/// Exception classes.
///
/// Different instruction sets or encodings are covered by different exception classes.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum ExceptionClass {
    None,

    /// SSE/AVX exception class (for legacy encoded SSE instructions and VEX instructions).
    SseAvx,

    /// EVEX exception class (for EVEX encoded AVX* instructions).
    Evex,

    /// Opmask instructions exception class.
    Opmask,

    /// AMX exception class type (for VEX encoded AMX instructions).
    Amx,
}

#[doc(hidden)]
impl ExceptionClass {
    pub(crate) fn from_raw(value: u8) -> Result<Self, DecodeError> {
        if value == ffi::_ND_EX_CLASS::ND_EXC_None as u8 {
            Ok(ExceptionClass::None)
        } else if value == ffi::_ND_EX_CLASS::ND_EXC_SSE_AVX as u8 {
            Ok(ExceptionClass::SseAvx)
        } else if value == ffi::_ND_EX_CLASS::ND_EXC_EVEX as u8 {
            Ok(ExceptionClass::Evex)
        } else if value == ffi::_ND_EX_CLASS::ND_EXC_OPMASK as u8 {
            Ok(ExceptionClass::Opmask)
        } else if value == ffi::_ND_EX_CLASS::ND_EXC_AMX as u8 {
            Ok(ExceptionClass::Amx)
        } else {
            Err(DecodeError::InternalError(value.into()))
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
    inner: ffi::INSTRUX,
    ip: u64,
    instruction: Mnemonic,
    length: usize,
}

impl DecodedInstruction {
    /// Decodes an array of bytes.
    ///
    /// # Arguments
    ///
    /// * `code` - An [u8](u8) slice that holds the code to be decoded. Note that decoding is attempted only from offset
    /// 0 inside this code chunk.
    /// * `mode` - The mode in which to decode the instruction.
    /// * `ip` - The instruction pointer value to use when formatting the decoded instruction. Does not affect the
    /// decoding process in any way. If not needed, use [decode](DecodedInstruction::decode) instead.
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
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    pub fn decode_with_ip(code: &[u8], mode: DecodeMode, ip: u64) -> DecodeResult {
        let mut instrux: mem::MaybeUninit<ffi::INSTRUX> = mem::MaybeUninit::uninit();
        let instrux = instrux.as_mut_ptr();

        let (code_def, data_def) = mode.into();

        let status = unsafe {
            ffi::NdDecodeEx(
                instrux,
                code.as_ptr(),
                code.len() as u64,
                code_def,
                data_def,
            )
        };

        status_to_error(status)?;

        let instrux = unsafe { *instrux };
        Ok(DecodedInstruction {
            inner: instrux,
            ip,
            instruction: Mnemonic::try_from(unsafe { instrux.__bindgen_anon_2.Instruction })
                .unwrap(),
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
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    pub fn decode(code: &[u8], mode: DecodeMode) -> DecodeResult {
        Self::decode_with_ip(code, mode, 0)
    }

    /// Get the mnemonic of the instruction.
    #[inline]
    pub fn mnemonic(&self) -> Mnemonic {
        self.instruction
    }

    /// Get the instruction length, in bytes.
    ///
    /// It is guaranteed that no instruction will exceed a length of 15 bytes.
    #[inline]
    pub fn length(&self) -> usize {
        self.length
    }

    /// Get the instruction operands.
    ///
    /// For working with specific operands (like the source or destination),
    /// [`operand_lookup()`](DecodedInstruction::operand_lookup) might be a better choice.
    ///
    /// The number of elements in the returned vector will always be equal to
    /// [`operands_count`](DecodedInstruction::operands_count).
    ///
    /// # Examples
    ///
    /// See [`operand`] for more in-depth examples on how to work with instruction operands.
    ///
    /// ```
    /// # use bddisasm::DecodeError;
    /// #
    /// # fn main() -> Result<(), DecodeError> {
    /// use bddisasm::{DecodedInstruction, DecodeMode, Mnemonic};
    ///
    /// // `MOV       ebx, dword ptr [ecx+edi]`
    /// let instruction = DecodedInstruction::decode(b"\x8b\x1c\x39", DecodeMode::Bits32)?;    ///
    /// let operands = instruction.operands();
    /// // The first operand is the destination
    /// let dst = operands[0];
    /// // The second operand is the source
    /// let src = operands[1];
    ///
    /// // Print information about the operands
    /// println!("{:#?} {:#?}", dst.info, src.info);
    /// # Ok(())
    /// # }
    /// ```
    ///
    /// # Panics
    ///
    /// This function will panic if the operand returned by the C library is invalid. This can not happen under normal
    /// circumstances.
    pub fn operands(&self) -> Operands {
        let mut operands = Operands::default();

        for op_index in 0..self.inner.OperandsCount {
            operands.operands[op_index as usize] =
                operand::Operand::from_raw(self.inner.Operands[op_index as usize]).unwrap();
        }

        operands.actual_count = self.inner.OperandsCount as usize;

        operands
    }

    /// Returns the CPUID support flag.
    ///
    /// If [None](None), the instruction is supported on any CPU, and no CPUID flag exists.
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
    pub fn cpuid(&self) -> Option<Cpuid> {
        let cpuid = unsafe { self.inner.CpuidFlag.__bindgen_anon_1 };
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
    pub fn encoding_mode(&self) -> EncodingMode {
        EncodingMode::from_raw(u32::from(self.inner.EncMode())).unwrap()
    }

    /// Get the VEX mode, if any.
    ///
    /// # Panics
    ///
    /// This function will panic if the VEX mode is unrecognized. This can not happen under normal circumstances.
    #[inline]
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
    pub fn addr_mode(&self) -> AddressingMode {
        AddressingMode::from_raw(u32::from(self.inner.AddrMode())).unwrap()
    }

    /// Get the operand mode/size.
    ///
    /// This is computed based on the passed-in [DecodeMode](DecodeMode) and instruction prefixes.
    ///
    /// # Remarks
    ///
    /// The effective mode can be different (see [`effective_op_mode`](DecodedInstruction::effective_op_mode)).
    ///
    /// # Examples
    ///
    /// Using [DecodeMode::Bits64](DecodeMode::Bits64), `0x50` encodes a `PUSH rax` instruction with an operand size of
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
    /// assert_eq!(ins.mnemonic(), Mnemonic::Push);
    /// assert_eq!(ins.op_mode(), OperandSize::OpSize32);
    /// assert_eq!(ins.effective_op_mode(), OperandSize::OpSize64);
    ///
    /// let ins =
    ///     DecodedInstruction::decode(&[0x48, 0x50], DecodeMode::Bits64)?;
    /// assert_eq!(ins.mnemonic(), Mnemonic::Push);
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
    pub fn effective_op_mode(&self) -> OperandSize {
        OperandSize::from_raw(u32::from(self.inner.EfOpMode())).unwrap()
    }

    /// Get the Vector mode/size, if any.
    ///
    /// This is computed based on the passed-in [DecodeMode](DecodeMode) and instruction prefixes.
    ///
    /// # Remarks
    ///
    /// The effective mode can be different (see [`effective_vec_mode`](DecodedInstruction::effective_vec_mode)).
    ///
    /// # Panics
    ///
    /// This function will panic if the vector mode is unrecognized. This can not happen under normal circumstances.
    #[inline]
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
    pub fn effective_vec_mode(&self) -> Option<VectorSize> {
        if self.has_vector() {
            Some(VectorSize::from_raw(u32::from(self.inner.EfVecMode())).unwrap())
        } else {
            None
        }
    }

    /// `true` if REX is present.
    #[inline]
    pub fn has_rex(&self) -> bool {
        self.inner.HasRex() != 0
    }

    /// `true` if VEX is present.
    #[inline]
    pub fn has_vex(&self) -> bool {
        self.inner.HasVex() != 0
    }

    /// `true` if XOP is present.
    #[inline]
    pub fn has_xop(&self) -> bool {
        self.inner.HasXop() != 0
    }

    /// `true` if EVEX is present.
    #[inline]
    pub fn has_evex(&self) -> bool {
        self.inner.HasEvex() != 0
    }

    /// `true` if MVEX is present.
    #[inline]
    pub fn has_mvex(&self) -> bool {
        self.inner.HasMvex() != 0
    }

    /// `true` if 0x66 is present.
    #[inline]
    pub fn has_op_size(&self) -> bool {
        self.inner.HasOpSize() != 0
    }

    /// `true` if 0x67 is present.
    #[inline]
    pub fn has_addr_size(&self) -> bool {
        self.inner.HasAddrSize() != 0
    }

    /// `true` if 0xF0 is present.
    #[inline]
    pub fn has_lock(&self) -> bool {
        self.inner.HasLock() != 0
    }

    /// `true` if 0xF2 is present.
    #[inline]
    pub fn has_repnz_xacquire_bnd(&self) -> bool {
        self.inner.HasRepnzXacquireBnd() != 0
    }

    /// `true` if 0xF3 is present.
    #[inline]
    pub fn has_rep_repz_xrelease(&self) -> bool {
        self.inner.HasRepRepzXrelease() != 0
    }

    /// `true` if segment override is present.
    #[inline]
    pub fn has_seg(&self) -> bool {
        self.inner.HasSeg() != 0
    }

    /// `true` if the instruction is repeated up to `RCX` times.
    #[inline]
    pub fn is_repeated(&self) -> bool {
        self.inner.IsRepeated() != 0
    }

    /// `true` if the instruction is XACQUIRE enabled.
    #[inline]
    pub fn is_xacquire_enabled(&self) -> bool {
        self.inner.IsXacquireEnabled() != 0
    }

    /// `true` if the instruction is XRELEASE enabled.
    #[inline]
    pub fn is_xrelease_enabled(&self) -> bool {
        self.inner.IsXreleaseEnabled() != 0
    }

    /// `true` if the instruction uses RIP relative addressing.
    #[inline]
    pub fn is_rip_relative(&self) -> bool {
        self.inner.IsRipRelative() != 0
    }

    /// `true` if this is an indirect CALL/JMP that is CET tracked.
    #[inline]
    pub fn is_cet_tracked(&self) -> bool {
        self.inner.IsCetTracked() != 0
    }

    /// `true` if we have valid MODRM.
    #[inline]
    pub fn has_mod_rm(&self) -> bool {
        self.inner.HasModRm() != 0
    }

    /// `true` if we have valid SIB.
    #[inline]
    pub fn has_sib(&self) -> bool {
        self.inner.HasSib() != 0
    }

    /// `true` if we have valid DREX.
    #[inline]
    pub fn has_drex(&self) -> bool {
        self.inner.HasDrex() != 0
    }

    /// `true` if the instruction has displacement.
    #[inline]
    pub fn has_disp(&self) -> bool {
        self.inner.HasDisp() != 0
    }

    /// `true` if the instruction contains a direct address (ie, `CALL far 0x9A`).
    #[inline]
    pub fn has_addr(&self) -> bool {
        self.inner.HasAddr() != 0
    }

    /// `true` if the instruction contains a moffset (ie, `MOV al, [mem], 0xA0`).
    #[inline]
    pub fn has_moffset(&self) -> bool {
        self.inner.HasMoffset() != 0
    }

    /// `true` if immediate is present.
    #[inline]
    pub fn has_imm1(&self) -> bool {
        self.inner.HasImm1() != 0
    }

    /// `true` if second immediate is present.
    #[inline]
    pub fn has_imm2(&self) -> bool {
        self.inner.HasImm2() != 0
    }

    /// `true` if third immediate is present.
    #[inline]
    pub fn has_imm3(&self) -> bool {
        self.inner.HasImm3() != 0
    }

    /// `true` if the instruction contains a relative offset (ie, `Jcc 0x7x`).
    #[inline]
    pub fn has_rel_offs(&self) -> bool {
        self.inner.HasRelOffs() != 0
    }

    /// `true` if SSE immediate that encodes additional registers is present.
    #[inline]
    pub fn has_sse_imm(&self) -> bool {
        self.inner.HasSseImm() != 0
    }

    /// `true` if the instruction uses compressed displacement.
    #[inline]
    pub fn has_comp_disp(&self) -> bool {
        self.inner.HasCompDisp() != 0
    }

    /// `true` if the instruction uses broadcast addressing.
    #[inline]
    pub fn has_broadcast(&self) -> bool {
        self.inner.HasBroadcast() != 0
    }

    /// `true` if the instruction has mask.
    #[inline]
    pub fn has_mask(&self) -> bool {
        self.inner.HasMask() != 0
    }

    /// `true` if the instruction uses zeroing.
    #[inline]
    pub fn has_zero(&self) -> bool {
        self.inner.HasZero() != 0
    }

    /// `true` if the instruction has embedded rounding.
    #[inline]
    pub fn has_er(&self) -> bool {
        self.inner.HasEr() != 0
    }

    /// `true` if the instruction has SAE.
    #[inline]
    pub fn has_sae(&self) -> bool {
        self.inner.HasSae() != 0
    }

    /// `true` if the instruction ignores embedded rounding.
    #[inline]
    pub fn has_ign_er(&self) -> bool {
        self.inner.HasIgnEr() != 0
    }

    /// Displacement sign. `false` if positive, `true` if negative.
    #[inline]
    pub fn sign_disp(&self) -> bool {
        self.inner.SignDisp() != 0
    }

    /// `true` if changing prefix.
    #[inline]
    pub fn has_mandatory_66(&self) -> bool {
        self.inner.HasMandatory66() != 0
    }

    /// 0x66 is mandatory prefix. Does not behave as REP prefix.
    #[inline]
    pub fn has_mandatory_f2(&self) -> bool {
        self.inner.HasMandatoryF2() != 0
    }

    /// 0x66 is mandatory prefix. Does not behave as REP prefix.
    #[inline]
    pub fn has_mandatory_f3(&self) -> bool {
        self.inner.HasMandatoryF3() != 0
    }

    /// The length of the instruction word. 2, 4 or 8.
    #[inline]
    pub fn word_length(&self) -> usize {
        self.inner.WordLength() as usize
    }

    /// The total number of bytes consumed by prefixes.
    ///
    /// This will also be the offset to the first opcode. The primary opcode will always be the last one.
    #[inline]
    pub fn pref_length(&self) -> usize {
        self.inner.PrefLength() as usize
    }

    /// Number of opcode bytes. Max 3.
    #[inline]
    pub fn op_length(&self) -> usize {
        self.inner.OpLength() as usize
    }

    /// Displacement length, in bytes. Maximum 4.
    #[inline]
    pub fn disp_length(&self) -> usize {
        self.inner.DispLength() as usize
    }

    /// Absolute address length, in bytes. Maximum 8 bytes.
    #[inline]
    pub fn addr_length(&self) -> usize {
        self.inner.AddrLength() as usize
    }

    /// Memory offset length, in bytes. Maximum 8 bytes.
    #[inline]
    pub fn moffset_length(&self) -> usize {
        self.inner.MoffsetLength() as usize
    }

    /// First immediate length, in bytes. Maximum 8 bytes.
    #[inline]
    pub fn imm1_length(&self) -> usize {
        self.inner.Imm1Length() as usize
    }

    /// Second immediate length, in bytes. Maximum 8 bytes.
    #[inline]
    pub fn imm2_length(&self) -> usize {
        self.inner.Imm2Length() as usize
    }

    /// Third immediate length, in bytes. Maximum 8 bytes.
    #[inline]
    pub fn imm3_length(&self) -> usize {
        self.inner.Imm3Length() as usize
    }

    /// Relative offset length, in bytes. Maximum 4 bytes.
    #[inline]
    pub fn rel_offs_length(&self) -> usize {
        self.inner.RelOffsLength() as usize
    }

    /// The offset of the first opcode, inside the instruction.
    #[inline]
    pub fn op_offset(&self) -> usize {
        self.inner.OpOffset() as usize
    }

    /// The offset of the nominal opcode, inside the instruction.
    #[inline]
    pub fn main_op_offset(&self) -> usize {
        self.inner.MainOpOffset() as usize
    }

    #[inline]
    /// The offset of the displacement, inside the instruction.
    pub fn disp_offset(&self) -> Option<usize> {
        let value = self.inner.DispOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    #[inline]
    /// The offset of the hard-coded address.
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
    pub fn imm2_offset(&self) -> Option<usize> {
        let value = self.inner.Imm2Offset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the third immediate, if any, inside the instruction.
    #[inline]
    pub fn imm3_offset(&self) -> Option<usize> {
        let value = self.inner.Imm3Offset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the relative offset used in instruction.
    #[inline]
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
    pub fn stack_words(&self) -> usize {
        self.inner.StackWords as usize
    }

    /// The last rep/repz/repnz prefix. if any.
    #[inline]
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
    pub fn seg(&self) -> Option<u8> {
        let value = self.inner.Seg;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The last segment override indicating a branch hint.
    #[inline]
    pub fn bhint(&self) -> u8 {
        self.inner.Bhint
    }

    /// Get the REX prefix.
    #[inline]
    pub fn rex(&self) -> Option<u8> {
        if self.has_rex() {
            Some(unsafe { self.inner.Rex.Rex })
        } else {
            None
        }
    }

    /// Get the `ModRM` byte.
    #[inline]
    pub fn mod_rm(&self) -> Option<u8> {
        if self.has_mod_rm() {
            Some(unsafe { self.inner.ModRm.ModRm })
        } else {
            None
        }
    }

    /// Get the `SIB` byte.
    #[inline]
    pub fn sib(&self) -> Option<u8> {
        if self.has_sib() {
            Some(unsafe { self.inner.Sib.Sib })
        } else {
            None
        }
    }

    /// Get the 2-bytes `VEX` prefix.
    #[inline]
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
    pub fn evex(&self) -> Option<(u8, u8, u8, u8)> {
        if self.has_evex() {
            let evex = self.inner.__bindgen_anon_1;
            let evex = unsafe { evex.Evex.Evex };

            Some((evex[0], evex[1], evex[2], evex[3]))
        } else {
            None
        }
    }

    /// Get the `segment:offset` address accessed by the instruction, if any.
    #[inline]
    pub fn address(&self) -> Option<OpAddr> {
        if self.has_addr() {
            let raw = self.inner.Address;
            let raw = unsafe { raw.__bindgen_anon_1 };

            Some(OpAddr::new(raw.Cs, u64::from(raw.Ip)))
        } else {
            None
        }
    }

    /// Get the absolute offset, if any.
    #[inline]
    pub fn moffset(&self) -> Option<u64> {
        if self.has_moffset() {
            Some(self.inner.Moffset)
        } else {
            None
        }
    }

    /// Get the displacement. Max 4 bytes. Used in `ModRM` instructions.
    #[inline]
    pub fn disp(&self) -> Option<u32> {
        if self.has_disp() {
            Some(self.inner.Displacement)
        } else {
            None
        }
    }

    /// Get the relative offset, used for branches. Max 4 bytes.
    #[inline]
    pub fn rel_offset(&self) -> Option<u32> {
        if self.has_rel_offs() {
            Some(self.inner.RelativeOffset)
        } else {
            None
        }
    }

    /// Get the first immediate.
    #[inline]
    pub fn immediate1(&self) -> Option<u64> {
        if self.has_imm1() {
            Some(self.inner.Immediate1)
        } else {
            None
        }
    }

    /// Get the second immediate. Used mainly for [`Mnemonic::Enter`](Mnemonic::Enter).
    #[inline]
    pub fn immediate2(&self) -> Option<u8> {
        if self.has_imm2() {
            Some(self.inner.Immediate2)
        } else {
            None
        }
    }

    /// Get the third additional immediate.
    #[inline]
    pub fn immediate3(&self) -> Option<u8> {
        if self.has_imm3() {
            Some(self.inner.Immediate3)
        } else {
            None
        }
    }

    /// Get the SSE immediate. It is used to select a register.
    #[inline]
    pub fn sse_immediate(&self) -> Option<u8> {
        if self.has_sse_imm() {
            Some(self.inner.SseImmediate)
        } else {
            None
        }
    }

    /// Get the `SSE` condition byte.
    #[inline]
    pub fn sse_cond(&self) -> Option<u8> {
        if (self.inner.Attributes & ffi::ND_FLAG_SSE_CONDB) != 0 {
            Some(self.inner.SseCondition)
        } else {
            None
        }
    }

    /// Get the condition byte.
    #[inline]
    pub fn cond(&self) -> Option<u8> {
        if (self.inner.Attributes & ffi::ND_FLAG_COND) != 0 {
            Some(self.inner.Condition)
        } else {
            None
        }
    }

    /// `true` if the instruction is 3DNow!.
    ///
    /// The opcode is the last byte.
    #[inline]
    pub fn is_3d_now(&self) -> bool {
        (self.inner.Attributes & ffi::ND_FLAG_3DNOW) != 0
    }

    /// Get the number of operands.
    #[inline]
    pub fn operands_count(&self) -> usize {
        self.inner.OperandsCount as usize
    }

    /// Number of explicit operands.
    ///
    /// Use this if you want to ignore implicit operands such as stack, flags, etc.
    #[inline]
    pub fn exp_operands_count(&self) -> usize {
        self.inner.ExpOperandsCount as usize
    }

    /// Get the `CS` access mode.
    #[inline]
    pub fn cs_access(&self) -> OpAccess {
        OpAccess::from_raw(ffi::ND_OPERAND_ACCESS {
            Access: self.inner.CsAccess,
        })
    }

    /// Get the `RIP` access mode.
    #[inline]
    pub fn rip_access(&self) -> OpAccess {
        OpAccess::from_raw(ffi::ND_OPERAND_ACCESS {
            Access: self.inner.RipAccess,
        })
    }

    /// Get the stack access mode.
    #[inline]
    pub fn stack_access(&self) -> OpAccess {
        OpAccess::from_raw(ffi::ND_OPERAND_ACCESS {
            Access: self.inner.StackAccess,
        })
    }

    /// Get the memory access mode.
    ///
    /// This includes the stack or shadow stack access.
    #[inline]
    pub fn memory_access(&self) -> OpAccess {
        OpAccess::from_raw(ffi::ND_OPERAND_ACCESS {
            Access: self.inner.MemoryAccess,
        })
    }

    /// `true` if the instruction is a branch.
    #[inline]
    pub fn is_branch(&self) -> bool {
        self.inner.BranchInfo.IsBranch() != 0
    }

    /// `true` if the instruction is a conditional branch.
    #[inline]
    pub fn is_conditional_branch(&self) -> bool {
        self.inner.BranchInfo.IsConditional() != 0
    }

    /// `true` if the instruction is a indirect branch.
    #[inline]
    pub fn is_indirect_branch(&self) -> bool {
        self.inner.BranchInfo.IsIndirect() != 0
    }

    /// `true` if the instruction is a far branch.
    #[inline]
    pub fn is_far_branch(&self) -> bool {
        self.inner.BranchInfo.IsFar() != 0
    }

    /// Get the rflags access.
    pub fn flags_access(&self) -> FlagsAccess {
        let facc = self.inner.FlagsAccess;

        let mode = OpAccess::from_raw(ffi::ND_OPERAND_ACCESS {
            Access: facc.RegAccess,
        });

        FlagsAccess {
            mode,
            tested: flags_raw(facc.Tested),
            modified: flags_raw(facc.Modified),
            set: flags_raw(facc.Set),
            cleared: flags_raw(facc.Cleared),
            undefined: flags_raw(facc.Undefined),
        }
    }

    /// `FPU` status word C0-C3 bits access.
    ///
    /// # Panics
    ///
    /// This function will panic if the access mode is unrecognized. This can not happen under normal circumstances.
    #[inline]
    pub fn fpu_flags_access(&self) -> FpuFlags {
        FpuFlags::from_raw(self.inner.FpuFlagsAccess).unwrap()
    }

    /// The exception class.
    ///
    /// # Panics
    ///
    /// This function will panic if the exception class is unrecognized. This can not happen under normal circumstances.
    #[inline]
    pub fn exception_class(&self) -> ExceptionClass {
        ExceptionClass::from_raw(self.inner.ExceptionClass).unwrap()
    }

    /// `EVEX` tuple type.
    ///
    /// # Panics
    ///
    /// This function will panic if the EVEX tuple type is unrecognized. This can not happen under normal circumstances.
    #[inline]
    pub fn evex_tuple(&self) -> Option<Tuple> {
        if self.has_evex() {
            Some(Tuple::from_raw(u32::from(self.inner.TupleType)).unwrap())
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
    pub fn evex_rounding(&self) -> Option<EvexRounding> {
        if self.has_er() {
            Some(EvexRounding::from_raw(self.inner.RoundingMode).unwrap())
        } else {
            None
        }
    }

    /// Get the instruction category.
    ///
    /// # Panics
    ///
    /// This function will panic if the cateogory not recognized. This can not happen under normal circumstances.
    #[inline]
    pub fn category(&self) -> Category {
        Category::try_from(self.inner.Category).unwrap()
    }

    /// Get the ISA set.
    ///
    /// # Panics
    ///
    /// This function will panic if the ISA set not recognized. This can not happen under normal circumstances.
    #[inline]
    pub fn isa_set(&self) -> IsaSet {
        IsaSet::try_from(self.inner.IsaSet).unwrap()
    }

    /// Get the CPU modes in which the instruction is valid.
    ///
    /// # Examples
    ///
    /// See [`cpu_modes`](crate::cpu_modes) for examples.
    #[inline]
    pub fn valid_cpu_modes(&self) -> CpuModes {
        CpuModes::from_raw(self.inner.ValidModes)
    }

    /// Get the valid prefixes for this instruction.
    #[inline]
    pub fn valid_prefixes(&self) -> ValidPrefixes {
        ValidPrefixes::from_raw(self.inner.ValidPrefixes)
    }

    /// Get the decorators accepted by the instruction.
    #[inline]
    pub fn valid_decorators(&self) -> ValidDecorators {
        ValidDecorators::from_raw(self.inner.ValidDecorators)
    }

    /// Get the main/nominal opcode.
    #[inline]
    pub fn primary_op_code(&self) -> u8 {
        self.inner.PrimaryOpCode
    }

    /// `true` if the instruction is a SIMD instruction that operates on vector regs.
    #[inline]
    pub fn has_vector(&self) -> bool {
        self.inner.Attributes & ffi::ND_FLAG_VECTOR != 0
    }
}

impl<'a> DecodedInstruction {
    /// Get the instruction bytes.
    #[inline]
    pub fn bytes(&'a self) -> &'a [u8] {
        &self.inner.InstructionBytes[..self.inner.Length as usize]
    }

    /// Get the opcode bytes (escape codes and main op code).
    #[inline]
    pub fn op_code_bytes(&'a self) -> &'a [u8] {
        &self.inner.OpCodeBytes[..self.op_length()]
    }

    /// Get an operand lookup table.
    ///
    /// This can be useful when needing to work with a specific operand without needing to iterate over the operands
    /// returned by [operands()](DecodedInstruction::operands), and without needing to rely on the order of the
    /// operands.
    ///
    /// # Examples
    ///
    /// See [`OperandsLookup`](OperandsLookup) for examples.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    pub fn operand_lookup(&'a self) -> OperandsLookup {
        OperandsLookup::from_raw(&self.inner)
    }
}

impl fmt::Display for DecodedInstruction {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let mut buffer: [u8; ffi::ND_MIN_BUF_SIZE as usize] = [0; ffi::ND_MIN_BUF_SIZE as usize];
        let status = unsafe {
            ffi::NdToText(
                &self.inner,
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

    #[test]
    fn decode() {
        let code = vec![0xb8, 0x00, 0x00, 0x00, 0x00];
        let ins = DecodedInstruction::decode(&code, DecodeMode::Bits32).expect("Unable to decode");
        assert_eq!(ins.instruction, Mnemonic::Mov);
        assert_eq!(ins.bytes(), code);
        assert_eq!(format!("{}", ins), "MOV       eax, 0x00000000");
    }

    #[test]
    fn decode_with_ip() {
        let code = b"\x48\x8b\x05\xf9\xff\xff\xff";
        let ins = DecodedInstruction::decode_with_ip(code, DecodeMode::Bits64, 0x100)
            .expect("Unable to decode");
        assert_eq!(ins.instruction, Mnemonic::Mov);
        assert_eq!(ins.bytes(), code);
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
        let mut exc_count: u8 = 0;
        let mut shadow_stack_count: u8 = 0;
        let mut tuple_count: u32 = 0;
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
            } else if line.starts_with("    ND_EXC_") {
                assert!(ExceptionClass::from_raw(exc_count).is_ok());
                exc_count += 1;
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
            } else if line.starts_with("    ND_TUPLE_") {
                // TODO: this test should be in `operand.rs`, but since we are already parsing `bddisasm.h` here it
                // felt wastefull to also parse it there.
                assert!(Tuple::from_raw(tuple_count).is_ok());
                tuple_count += 1;
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
        let status = include_str!("../../bddisasm-sys/csrc/inc/disasmstatus.h");
        for line in status.lines() {
            if line.starts_with("#define ND_STATUS_SUCCESS")
                || line.starts_with("#define ND_STATUS_HINT_OPERAND_NOT_USED")
            {
                assert!(status_to_error(get_tokens(line, 2)).is_ok());
            } else if line.starts_with("#define ND_STATUS_") {
                assert!(status_to_error(get_tokens(line, 2)).is_err());
            }
        }
    }

    #[test]
    fn check_all_exception_classes() {
        // This is a really contrieved way of making sure that we check all variants of `ffi::_ND_EX_CLASS`. If a new
        // one is added, this will fail to build. We do this because `ExceptionClass::from_raw` takes an `u8`.
        // NOTE: When a new variant is added, `ExceptionClass::from_raw` must be updated.
        match ffi::_ND_EX_CLASS::ND_EXC_None {
            ffi::_ND_EX_CLASS::ND_EXC_None => {}
            ffi::_ND_EX_CLASS::ND_EXC_SSE_AVX => {}
            ffi::_ND_EX_CLASS::ND_EXC_EVEX => {}
            ffi::_ND_EX_CLASS::ND_EXC_OPMASK => {}
            ffi::_ND_EX_CLASS::ND_EXC_AMX => {}
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
