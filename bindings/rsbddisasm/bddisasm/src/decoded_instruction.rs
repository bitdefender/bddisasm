/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Decodes instructions.

extern crate bddisasm_sys as ffi;

pub use crate::cpu_modes::CpuModes;
pub use crate::cpuid::Cpuid;
pub use crate::decode_error::DecodeError;
pub use crate::fpu_flags::FpuFlags;
pub use crate::instruction_category::Category;
pub use crate::isa_set::IsaSet;
pub use crate::mnemonic::Mnemonic;
pub use crate::operand;
pub use crate::operand::{OpAccess, OpAddr};
pub use crate::rflags;
pub use crate::tuple::Tuple;

use crate::rflags::flags_raw;

use crate::decode_error;

use std::fmt;
use std::mem;

use std::convert::TryFrom;

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

impl From<u32> for EncodingMode {
    fn from(value: u32) -> EncodingMode {
        match value {
            ffi::ND_ENCM_LEGACY => EncodingMode::Legacy,
            ffi::ND_ENCM_XOP => EncodingMode::Xop,
            ffi::ND_ENCM_VEX => EncodingMode::Vex,
            ffi::ND_ENCM_EVEX => EncodingMode::Evex,
            _ => panic!("Unexpected encoding mode mode {}", value),
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

impl From<u32> for VexMode {
    fn from(value: u32) -> VexMode {
        match value {
            ffi::ND_VEXM_2B => VexMode::Vex2b,
            ffi::ND_VEXM_3B => VexMode::Vex3b,
            _ => panic!("Unexpected VEX mode mode {}", value),
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

impl From<u32> for AddressingMode {
    fn from(value: u32) -> AddressingMode {
        match value {
            ffi::ND_ADDR_16 => AddressingMode::Addr16,
            ffi::ND_ADDR_32 => AddressingMode::Addr32,
            ffi::ND_ADDR_64 => AddressingMode::Addr64,
            _ => panic!("Unexpected addressing mode {}", value),
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

impl From<u32> for OperandSize {
    fn from(value: u32) -> OperandSize {
        match value {
            ffi::ND_OPSZ_16 => OperandSize::OpSize16,
            ffi::ND_OPSZ_32 => OperandSize::OpSize32,
            ffi::ND_OPSZ_64 => OperandSize::OpSize64,
            _ => panic!("Unexpected operand size {}", value),
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

impl From<u32> for VectorSize {
    fn from(value: u32) -> VectorSize {
        match value {
            ffi::ND_VECM_128 => VectorSize::VecSize128,
            ffi::ND_VECM_256 => VectorSize::VecSize256,
            ffi::ND_VECM_512 => VectorSize::VecSize512,
            _ => panic!("Unexpected vector size {}", value),
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
impl From<ffi::ND_EX_CLASS> for ExceptionClass {
    fn from(value: ffi::ND_EX_CLASS) -> ExceptionClass {
        match value {
            ffi::_ND_EX_CLASS::ND_EXC_None => ExceptionClass::None,
            ffi::_ND_EX_CLASS::ND_EXC_SSE_AVX => ExceptionClass::SseAvx,
            ffi::_ND_EX_CLASS::ND_EXC_EVEX => ExceptionClass::Evex,
            ffi::_ND_EX_CLASS::ND_EXC_OPMASK => ExceptionClass::Opmask,
            ffi::_ND_EX_CLASS::ND_EXC_AMX => ExceptionClass::Amx,
            // NOTE: when updating this take care to also update the `From<u8>` implementation!
            // TODO: any way of keeping these in sync automagically?
        }
    }
}

impl From<u8> for ExceptionClass {
    fn from(value: u8) -> ExceptionClass {
        if value == ffi::_ND_EX_CLASS::ND_EXC_None as u8 {
            ExceptionClass::None
        } else if value == ffi::_ND_EX_CLASS::ND_EXC_SSE_AVX as u8 {
            ExceptionClass::SseAvx
        } else if value == ffi::_ND_EX_CLASS::ND_EXC_EVEX as u8 {
            ExceptionClass::Evex
        } else if value == ffi::_ND_EX_CLASS::ND_EXC_OPMASK as u8 {
            ExceptionClass::Opmask
        } else if value == ffi::_ND_EX_CLASS::ND_EXC_AMX as u8 {
            ExceptionClass::Amx
        } else {
            panic!("Unknown exception class {}", value)
        }
    }
}

/// Describes the way an instruction accesses the flags register.
///
/// Individual bits can be checked using the [rflags](rflags) module.
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
impl From<ffi::ND_ROUNDING> for EvexRounding {
    fn from(value: ffi::ND_ROUNDING) -> EvexRounding {
        match value {
            ffi::_ND_ROUNDING::ND_RND_RNE => EvexRounding::NearestEqual,
            ffi::_ND_ROUNDING::ND_RND_RD => EvexRounding::Down,
            ffi::_ND_ROUNDING::ND_RND_RU => EvexRounding::Up,
            ffi::_ND_ROUNDING::ND_RND_RZ => EvexRounding::Zero,
            // NOTE: when updating this take care to also update the `From<u8>` implementation!
            // TODO: any way of keeping these in sync automagically?
        }
    }
}

impl From<u8> for EvexRounding {
    fn from(value: u8) -> EvexRounding {
        if value == ffi::_ND_ROUNDING::ND_RND_RNE as u8 {
            EvexRounding::NearestEqual
        } else if value == ffi::_ND_ROUNDING::ND_RND_RD as u8 {
            EvexRounding::Down
        } else if value == ffi::_ND_ROUNDING::ND_RND_RU as u8 {
            EvexRounding::Up
        } else if value == ffi::_ND_ROUNDING::ND_RND_RZ as u8 {
            EvexRounding::Zero
        } else {
            panic!("Unknown EVEX rounding: {}", value)
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
impl From<ffi::ND_VALID_PREFIXES> for ValidPrefixes {
    fn from(value: ffi::ND_VALID_PREFIXES) -> ValidPrefixes {
        let raw = unsafe { value.__bindgen_anon_1 };
        ValidPrefixes {
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
impl From<ffi::ND_VALID_DECORATORS> for ValidDecorators {
    fn from(value: ffi::ND_VALID_DECORATORS) -> ValidDecorators {
        let raw = unsafe { value.__bindgen_anon_1 };
        ValidDecorators {
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
    /// This function reports back errors returned by the bddisasm library, as well as internal errors that usually
    /// signal a bug in this implementation.
    ///
    /// Note that [`DecodeError::UnknownStatus`] and [`DecodeError::UnknownInstruction`] should never be encountered,
    /// unless this crate has fallen out if sync with `bddisasm-sys`, thus they signal a bug in this crate.
    ///
    /// # Examples
    ///
    /// ```
    /// # use std::error::Error;
    /// #
    /// # fn main() -> Result<(), Box<dyn Error>> {
    /// use bddisasm::decoder::{DecodedInstruction, DecodeMode, Mnemonic};
    ///
    /// let code = vec![0x48, 0x8b, 0x05, 0xf9, 0xff, 0xff, 0xff];
    ///
    /// let instruction = DecodedInstruction::decode_with_ip(&code, DecodeMode::Bits64, 0)?;
    /// // Will print `MOV       rax, qword ptr [rel 0x0]`
    /// println!("{}", instruction);
    ///
    /// let instruction = DecodedInstruction::decode_with_ip(&code, DecodeMode::Bits64, 0x100)?;
    /// // Will print `MOV       rax, qword ptr [rel 0x100]`
    /// println!("{}", instruction);
    /// # Ok(())
    /// # }
    /// ```
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

        decode_error::status_to_error(status)?;

        let instrux = unsafe { *instrux };
        Ok(DecodedInstruction {
            inner: instrux,
            ip,
            instruction: Mnemonic::try_from(unsafe { instrux.__bindgen_anon_2.Instruction })?,
            length: instrux.Length as usize,
        })
    }

    /// Decodes an array of bytes.
    ///
    /// This function is a thin wrapper over [decode_with_ip](DecodedInstruction::decode_with_ip) and behaves exactly
    /// the same. It sets `ip` to 0.
    pub fn decode(code: &[u8], mode: DecodeMode) -> DecodeResult {
        Self::decode_with_ip(code, mode, 0)
    }

    /// Get the mnemonic of the instruction.
    #[inline]
    pub fn mnemonic(self) -> Mnemonic {
        self.instruction
    }

    /// Get the instruction length, in bytes.
    ///
    /// It is guaranteed that no instruction will exceed a length of 15 bytes.
    #[inline]
    pub fn length(self) -> usize {
        self.length
    }

    /// Get the instruction operands.
    ///
    /// The number of elements in the returned vector will always be equal to
    /// [`operands_count`](DecodedInstruction::operands_count).
    ///
    /// # Examples
    ///
    /// See [`operand`] for more in-depth examples on how to work with instruction operands.
    ///
    /// ```
    /// # use std::error::Error;
    /// #
    /// # fn main() -> Result<(), Box<dyn Error>> {
    /// use bddisasm::decoder::{DecodedInstruction, DecodeMode, Mnemonic};
    ///
    /// // `MOV       ebx, dword ptr [ecx+edi]`
    /// let instruction = DecodedInstruction::decode(b"\x8b\x1c\x39", DecodeMode::Bits32)?;    ///
    /// let operands = instruction.operands();
    /// // The first operand is the destination
    /// let dst = operands[0];
    /// // The second operand is the source
    /// let src = operands[0];
    ///
    /// // Print information about the operands
    /// println!("{:#?} {:#?}", dst.info, src.info);
    /// # Ok(())
    /// # }
    /// ```
    pub fn operands(self) -> Vec<operand::Operand> {
        let mut ops = Vec::new();

        for op_index in 0..self.inner.OperandsCount {
            ops.push(operand::Operand::from(
                self.inner.Operands[op_index as usize],
            ));
        }

        ops
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
    /// # use std::error::Error;
    /// #
    /// # fn main() -> Result<(), Box<dyn Error>> {
    /// use bddisasm::decoder::{DecodedInstruction, DecodeMode, Mnemonic};
    ///
    /// // `RDMSR`
    /// let instruction = DecodedInstruction::decode(b"\x0f\x32", DecodeMode::Bits64)?;
    /// let cpuid = instruction.cpuid();
    /// // Will print `leaf: 0x1 sub-leaf: - register: 2 bit to test: 0x5`
    /// if let Some(cpuid) = cpuid {
    ///     println!("{}", cpuid);
    /// }
    ///
    /// # Ok(())
    /// # }
    /// ```
    pub fn cpuid(self) -> Option<Cpuid> {
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
            let bit = cpuid.Bit() as u64;

            Some(Cpuid {
                leaf,
                sub_leaf,
                register,
                bit,
            })
        }
    }

    /// Get the encoding mode used.
    #[inline]
    pub fn encoding_mode(self) -> EncodingMode {
        EncodingMode::from(self.inner.EncMode() as u32)
    }

    /// Get the VEX mode, if any.
    #[inline]
    pub fn vex_mode(self) -> Option<VexMode> {
        if self.has_vex() {
            Some(VexMode::from(self.inner.VexMode() as u32))
        } else {
            None
        }
    }

    /// Get the addressing mode.
    #[inline]
    pub fn addr_mode(self) -> AddressingMode {
        AddressingMode::from(self.inner.AddrMode() as u32)
    }

    /// Get the operand mode/size.
    ///
    /// This is computed based on the passed-in [DecodeMode](DecodeMode) and instruction prefixes.
    ///
    /// # Remarks
    ///
    /// The effective mode can be different (see [effective_op_mode](DecodedInstruction::effective_op_mode)).
    ///
    /// # Examples
    ///
    /// Using [DecodeMode::Bits64](DecodeMode::Bits64), `0x50` encodes a `PUSH rax` instruction with an operand size of
    /// 32 because it has no prefix that promotes it, but the effective size is 64 because the instruction always
    /// operates on 64 bits.
    ///
    /// ```
    /// # use std::error::Error;
    /// #
    /// # fn main() -> Result<(), Box<dyn Error>> {
    /// use bddisasm::decoded_instruction::{DecodedInstruction, DecodeMode, Mnemonic, OperandSize};
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
    #[inline]
    pub fn op_mode(self) -> OperandSize {
        OperandSize::from(self.inner.OpMode() as u32)
    }

    /// Get the effective operand mode/size.
    ///
    /// Unlike [op_mode](DecodedInstruction::op_mode), this takes into account the actual instruction.
    #[inline]
    pub fn effective_op_mode(self) -> OperandSize {
        OperandSize::from(self.inner.EfOpMode() as u32)
    }

    /// Get the Vector mode/size, if any.
    ///
    /// This is computed based on the passed-in [DecodeMode](DecodeMode) and instruction prefixes.
    ///
    /// # Remarks
    ///
    /// The effective mode can be different (see [effective_vec_mode](DecodedInstruction::effective_vec_mode)).
    #[inline]
    pub fn vec_mode(self) -> Option<VectorSize> {
        if self.has_vector() {
            Some(VectorSize::from(self.inner.VecMode() as u32))
        } else {
            None
        }
    }

    /// Get the Vector mode/size, if any.
    ///
    /// Unlike [vec_mode](DecodedInstruction::vec_mode), this takes into account the actual instruction.
    #[inline]
    pub fn effective_vec_mode(self) -> Option<VectorSize> {
        if self.has_vector() {
            Some(VectorSize::from(self.inner.EfVecMode() as u32))
        } else {
            None
        }
    }

    /// `true` if REX is present.
    #[inline]
    pub fn has_rex(self) -> bool {
        self.inner.HasRex()
    }

    /// `true` if VEX is present.
    #[inline]
    pub fn has_vex(self) -> bool {
        self.inner.HasVex()
    }

    /// `true` if XOP is present.
    #[inline]
    pub fn has_xop(self) -> bool {
        self.inner.HasXop()
    }

    /// `true` if EVEX is present.
    #[inline]
    pub fn has_evex(self) -> bool {
        self.inner.HasEvex()
    }

    /// `true` if MVEX is present.
    #[inline]
    pub fn has_mvex(self) -> bool {
        self.inner.HasMvex()
    }

    /// `true` if 0x66 is present.
    #[inline]
    pub fn has_op_size(self) -> bool {
        self.inner.HasOpSize()
    }

    /// `true` if 0x67 is present.
    #[inline]
    pub fn has_addr_size(self) -> bool {
        self.inner.HasAddrSize()
    }

    /// `true` if 0xF0 is present.
    #[inline]
    pub fn has_lock(self) -> bool {
        self.inner.HasLock()
    }

    /// `true` if 0xF2 is present.
    #[inline]
    pub fn has_repnz_xacquire_bnd(self) -> bool {
        self.inner.HasRepnzXacquireBnd()
    }

    /// `true` if 0xF3 is present.
    #[inline]
    pub fn has_rep_repz_xrelease(self) -> bool {
        self.inner.HasRepRepzXrelease()
    }

    /// `true` if segment override is present.
    #[inline]
    pub fn has_seg(self) -> bool {
        self.inner.HasSeg()
    }

    /// `true` if the instruction is REPed up to RCX times.
    #[inline]
    pub fn is_repeated(self) -> bool {
        self.inner.IsRepeated()
    }

    /// `true` if the instruction is XACQUIRE enabled.
    #[inline]
    pub fn is_xacquire_enabled(self) -> bool {
        self.inner.IsXacquireEnabled()
    }

    /// `true` if the instruction is XRELEASE enabled.
    #[inline]
    pub fn is_xrelease_enabled(self) -> bool {
        self.inner.IsXreleaseEnabled()
    }

    /// `true` if the instruction uses RIP relative addressing.
    #[inline]
    pub fn is_rip_relative(self) -> bool {
        self.inner.IsRipRelative()
    }

    /// `true` if this is an indirect CALL/JMP that is CET tracked.
    #[inline]
    pub fn is_cet_tracked(self) -> bool {
        self.inner.IsCetTracked()
    }

    /// `true` if we have valid MODRM.
    #[inline]
    pub fn has_mod_rm(self) -> bool {
        self.inner.HasModRm()
    }

    /// `true` if we have valid SIB.
    #[inline]
    pub fn has_sib(self) -> bool {
        self.inner.HasSib()
    }

    /// `true` if we have valid DREX.
    #[inline]
    pub fn has_drex(self) -> bool {
        self.inner.HasDrex()
    }

    /// `true` if the instruction has displacement.
    #[inline]
    pub fn has_disp(self) -> bool {
        self.inner.HasDisp()
    }

    /// `true` if the instruction contains a direct address (ie, `CALL far 0x9A`).
    #[inline]
    pub fn has_addr(self) -> bool {
        self.inner.HasAddr()
    }

    /// `true` if the instruction contains a moffset (ie, `MOV al, [mem], 0xA0`).
    #[inline]
    pub fn has_moffset(self) -> bool {
        self.inner.HasMoffset()
    }

    /// `true` if immediate is present.
    #[inline]
    pub fn has_imm1(self) -> bool {
        self.inner.HasImm1()
    }

    /// `true` if second immediate is present.
    #[inline]
    pub fn has_imm2(self) -> bool {
        self.inner.HasImm2()
    }

    /// `true` if third immediate is present.
    #[inline]
    pub fn has_imm3(self) -> bool {
        self.inner.HasImm3()
    }

    /// `true` if the instruction contains a relative offset (ie, `Jcc 0x7x`).
    #[inline]
    pub fn has_rel_offs(self) -> bool {
        self.inner.HasRelOffs()
    }

    /// `true` if SSE immediate that encodes additional registers is present.
    #[inline]
    pub fn has_sse_imm(self) -> bool {
        self.inner.HasSseImm()
    }

    /// `true` if the instruction uses compressed displacement.
    #[inline]
    pub fn has_comp_disp(self) -> bool {
        self.inner.HasCompDisp()
    }

    /// `true` if the instruction uses broadcast addressing.
    #[inline]
    pub fn has_broadcast(self) -> bool {
        self.inner.HasBroadcast()
    }

    /// `true` if the instruction has mask.
    #[inline]
    pub fn has_mask(self) -> bool {
        self.inner.HasMask()
    }

    /// `true` if the instruction uses zeroing.
    #[inline]
    pub fn has_zero(self) -> bool {
        self.inner.HasZero()
    }

    /// `true` if the instruction has embedded rounding.
    #[inline]
    pub fn has_er(self) -> bool {
        self.inner.HasEr()
    }

    /// `true` if the instruction has SAE.
    #[inline]
    pub fn has_sae(self) -> bool {
        self.inner.HasSae()
    }

    /// `true` if the instruction ignores embedded rounding.
    #[inline]
    pub fn has_ign_er(self) -> bool {
        self.inner.HasIgnEr()
    }

    /// Displacement sign. `false` if positive, `true` if negative.
    #[inline]
    pub fn sign_disp(self) -> bool {
        self.inner.SignDisp()
    }

    /// `true` if changing prefix.
    #[inline]
    pub fn has_mandatory_66(self) -> bool {
        self.inner.HasMandatory66()
    }

    /// 0x66 is mandatory prefix. Does not behave as REP prefix.
    #[inline]
    pub fn has_mandatory_f2(self) -> bool {
        self.inner.HasMandatoryF2()
    }

    /// 0x66 is mandatory prefix. Does not behave as REP prefix.
    #[inline]
    pub fn has_mandatory_f3(self) -> bool {
        self.inner.HasMandatoryF3()
    }

    /// The length of the instruction word. 2, 4 or 8.
    #[inline]
    pub fn word_length(self) -> usize {
        self.inner.WordLength() as usize
    }

    /// The total number of bytes consumed by prefixes.
    ///
    /// This will also be the offset to the first opcode. The primary opcode will always be the last one.
    #[inline]
    pub fn pref_length(self) -> usize {
        self.inner.PrefLength() as usize
    }

    /// Number of opcode bytes. Max 3.
    #[inline]
    pub fn op_length(self) -> usize {
        self.inner.OpLength() as usize
    }

    /// Displacement length, in bytes. Maximum 4.
    #[inline]
    pub fn disp_length(self) -> usize {
        self.inner.DispLength() as usize
    }

    /// Absolute address length, in bytes. Maximum 8 bytes.
    #[inline]
    pub fn addr_length(self) -> usize {
        self.inner.AddrLength() as usize
    }

    /// Memory offset length, in bytes. Maximum 8 bytes.
    #[inline]
    pub fn moffset_length(self) -> usize {
        self.inner.MoffsetLength() as usize
    }

    /// First immediate length, in bytes. Maximum 8 bytes.
    #[inline]
    pub fn imm1_length(self) -> usize {
        self.inner.Imm1Length() as usize
    }

    /// Second immediate length, in bytes. Maximum 8 bytes.
    #[inline]
    pub fn imm2_length(self) -> usize {
        self.inner.Imm2Length() as usize
    }

    /// Third immediate length, in bytes. Maximum 8 bytes.
    #[inline]
    pub fn imm3_length(self) -> usize {
        self.inner.Imm3Length() as usize
    }

    /// Relative offset length, in bytes. Maximum 4 bytes.
    #[inline]
    pub fn rel_offs_length(self) -> usize {
        self.inner.RelOffsLength() as usize
    }

    /// The offset of the first opcode, inside the instruction.
    #[inline]
    pub fn op_offset(self) -> usize {
        self.inner.OpOffset() as usize
    }

    /// The offset of the nominal opcode, inside the instruction.
    #[inline]
    pub fn main_op_offset(self) -> usize {
        self.inner.MainOpOffset() as usize
    }

    #[inline]
    /// The offset of the displacement, inside the instruction.
    pub fn disp_offset(self) -> Option<usize> {
        let value = self.inner.DispOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    #[inline]
    /// The offset of the hard-coded address.
    pub fn addr_offset(self) -> Option<usize> {
        let value = self.inner.AddrOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the absolute address, inside the instruction.
    #[inline]
    pub fn moffset_offset(self) -> Option<usize> {
        let value = self.inner.MoffsetOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the immediate, inside the instruction.
    #[inline]
    pub fn imm1_offset(self) -> Option<usize> {
        let value = self.inner.Imm1Offset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the second immediate, if any, inside the instruction.
    #[inline]
    pub fn imm2_offset(self) -> Option<usize> {
        let value = self.inner.Imm2Offset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the third immediate, if any, inside the instruction.
    #[inline]
    pub fn imm3_offset(self) -> Option<usize> {
        let value = self.inner.Imm3Offset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the relative offset used in instruction.
    #[inline]
    pub fn rel_offs_offset(self) -> Option<usize> {
        let value = self.inner.RelOffsOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the SSE immediate, if any, inside the instruction.
    #[inline]
    pub fn sse_imm_offset(self) -> Option<usize> {
        let value = self.inner.SseImmOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The offset of the mod rm byte inside the instruction, if any.
    #[inline]
    pub fn mod_rm_offset(self) -> Option<usize> {
        let value = self.inner.ModRmOffset() as usize;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// Number of words accessed on/from the stack.
    #[inline]
    pub fn stack_words(self) -> usize {
        self.inner.StackWords as usize
    }

    /// The last rep/repz/repnz prefix. if any.
    #[inline]
    pub fn rep(self) -> Option<u8> {
        let value = self.inner.Rep;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The last segment override prefix. if none. `FS`/`GS` if 64 bit.
    #[inline]
    pub fn seg(self) -> Option<u8> {
        let value = self.inner.Seg;
        if value == 0 {
            None
        } else {
            Some(value)
        }
    }

    /// The last segment override indicating a branch hint.
    #[inline]
    pub fn bhint(self) -> u8 {
        self.inner.Bhint
    }

    /// Get the REX prefix.
    #[inline]
    pub fn rex(self) -> Option<u8> {
        if self.has_rex() {
            Some(unsafe { self.inner.Rex.Rex })
        } else {
            None
        }
    }

    /// Get the ModRM byte.
    #[inline]
    pub fn mod_rm(self) -> Option<u8> {
        if self.has_mod_rm() {
            Some(unsafe { self.inner.ModRm.ModRm })
        } else {
            None
        }
    }

    /// Get the SIB byte.
    #[inline]
    pub fn sib(self) -> Option<u8> {
        if self.has_sib() {
            Some(unsafe { self.inner.Sib.Sib })
        } else {
            None
        }
    }

    /// Get the 2-bytes VEX.
    #[inline]
    pub fn vex2(self) -> Option<(u8, u8)> {
        if matches!(self.vex_mode(), Some(VexMode::Vex2b)) {
            let vex2 = self.inner.__bindgen_anon_1;
            let vex2 = unsafe { vex2.Vex2.Vex };

            Some((vex2[0], vex2[1]))
        } else {
            None
        }
    }

    /// Get the 3-bytes VEX.
    #[inline]
    pub fn vex3(self) -> Option<(u8, u8, u8)> {
        if matches!(self.vex_mode(), Some(VexMode::Vex3b)) {
            let vex3 = self.inner.__bindgen_anon_1;
            let vex3 = unsafe { vex3.Vex3.Vex };

            Some((vex3[0], vex3[1], vex3[2]))
        } else {
            None
        }
    }

    /// Get the XOP bytes.
    #[inline]
    pub fn xop(self) -> Option<(u8, u8, u8)> {
        if self.has_xop() {
            let xop = self.inner.__bindgen_anon_1;
            let xop = unsafe { xop.Xop.Xop };

            Some((xop[0], xop[1], xop[2]))
        } else {
            None
        }
    }

    /// Get the EVEX bytes.
    #[inline]
    pub fn evex(self) -> Option<(u8, u8, u8, u8)> {
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
    pub fn address(self) -> Option<OpAddr> {
        if self.has_addr() {
            let raw = self.inner.Address;
            let raw = unsafe { raw.__bindgen_anon_1 };

            Some(OpAddr::new(raw.Cs, raw.Ip as u64))
        } else {
            None
        }
    }

    /// Get the absolute offset, if any.
    #[inline]
    pub fn moffset(self) -> Option<u64> {
        if self.has_moffset() {
            Some(self.inner.Moffset)
        } else {
            None
        }
    }

    /// Get the displacement. Max 4 bytes. Used in ModRM instructions.
    #[inline]
    pub fn disp(self) -> Option<u32> {
        if self.has_disp() {
            Some(self.inner.Displacement)
        } else {
            None
        }
    }

    /// Get the relative offset, used for branches. Max 4 bytes.
    #[inline]
    pub fn rel_offset(self) -> Option<u32> {
        if self.has_rel_offs() {
            Some(self.inner.RelativeOffset)
        } else {
            None
        }
    }

    /// Get the first immediate.
    #[inline]
    pub fn immediate1(self) -> Option<u64> {
        if self.has_imm1() {
            Some(self.inner.Immediate1)
        } else {
            None
        }
    }

    /// Get the second immediate. Used mainly for [Mnemonic::Enter](Mnemonic::Enter).
    #[inline]
    pub fn immediate2(self) -> Option<u8> {
        if self.has_imm2() {
            Some(self.inner.Immediate2)
        } else {
            None
        }
    }

    /// Get the third additional immediate.
    #[inline]
    pub fn immediate3(self) -> Option<u8> {
        if self.has_imm3() {
            Some(self.inner.Immediate3)
        } else {
            None
        }
    }

    /// Get the SSE immediate. It is used to select a register.
    #[inline]
    pub fn sse_immediate(self) -> Option<u8> {
        if self.has_sse_imm() {
            Some(self.inner.SseImmediate)
        } else {
            None
        }
    }

    /// Get the SSE condition byte.
    #[inline]
    pub fn sse_cond(self) -> Option<u8> {
        if (self.inner.Attributes & ffi::ND_FLAG_SSE_CONDB) != 0 {
            Some(self.inner.SseCondition)
        } else {
            None
        }
    }

    /// Get the condition byte.
    #[inline]
    pub fn cond(self) -> Option<u8> {
        if (self.inner.Attributes & ffi::ND_FLAG_COND) != 0 {
            Some(self.inner.Condition)
        } else {
            None
        }
    }

    /// Get the number of operands.
    #[inline]
    pub fn operands_count(self) -> usize {
        self.inner.OperandsCount as usize
    }

    /// Number of explicit operands.
    ///
    /// Use this if you want to ignore implicit operands such as stack, flags, etc.
    #[inline]
    pub fn exp_operands_count(self) -> usize {
        self.inner.ExpOperandsCount as usize
    }

    /// Get the RIP access mode.
    #[inline]
    pub fn rip_access(self) -> OpAccess {
        OpAccess::from(ffi::ND_OPERAND_ACCESS {
            Access: self.inner.RipAccess,
        })
    }

    /// Get the stack access mode.
    #[inline]
    pub fn stack_access(self) -> OpAccess {
        OpAccess::from(ffi::ND_OPERAND_ACCESS {
            Access: self.inner.StackAccess,
        })
    }

    /// Get the memory access mode.
    ///
    /// This includes the stack or shadow stack access.
    #[inline]
    pub fn memory_access(self) -> OpAccess {
        OpAccess::from(ffi::ND_OPERAND_ACCESS {
            Access: self.inner.MemoryAccess,
        })
    }

    /// Get the rflags access.
    pub fn flags_access(self) -> FlagsAccess {
        let facc = self.inner.FlagsAccess;

        let mode = OpAccess::from(ffi::ND_OPERAND_ACCESS {
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

    /// FPU status word C0-C3 bits access.
    #[inline]
    pub fn fpu_flags_access(self) -> FpuFlags {
        FpuFlags::from(self.inner.FpuFlagsAccess)
    }

    // The exception class.
    #[inline]
    pub fn exception_class(self) -> ExceptionClass {
        ExceptionClass::from(self.inner.ExceptionClass)
    }

    /// EVEX tuple type.
    #[inline]
    pub fn evex_tuple(self) -> Option<Tuple> {
        if self.has_evex() {
            Some(Tuple::from(self.inner.TupleType as u32))
        } else {
            None
        }
    }

    /// EVEX rounding mode.
    #[inline]
    pub fn evex_rounding(self) -> Option<EvexRounding> {
        if self.has_er() {
            Some(EvexRounding::from(self.inner.RoundingMode))
        } else {
            None
        }
    }

    /// Get the instruction category.
    ///
    /// # Errors
    ///
    /// This function can return an error if the category of the instruction is not known. This is most likely a bug
    /// in the current implementation.
    #[inline]
    pub fn category(self) -> Result<Category, DecodeError> {
        Category::try_from(self.inner.Category)
    }

    /// Get the ISA set.
    ///
    /// # Errors
    ///
    /// This function can return an error if the ISA set of the instruction is not known. This is most likely a bug
    /// in the current implementation.
    #[inline]
    pub fn isa_set(self) -> Result<IsaSet, DecodeError> {
        IsaSet::try_from(self.inner.IsaSet)
    }

    /// Get the CPU modes in which the instruction is valid.
    ///
    /// See [cpu_modes](crate::cpu_modes) for examples.
    #[inline]
    pub fn valid_cpu_modes(self) -> CpuModes {
        CpuModes::from(self.inner.ValidModes)
    }

    /// Get the valid prefixes for this instruction.
    #[inline]
    pub fn valid_prefixes(self) -> ValidPrefixes {
        ValidPrefixes::from(self.inner.ValidPrefixes)
    }

    /// Get the decorators accepted by the instruction.
    #[inline]
    pub fn valid_decorators(self) -> ValidDecorators {
        ValidDecorators::from(self.inner.ValidDecorators)
    }

    /// Get the main/nominal opcode.
    #[inline]
    pub fn primary_op_code(self) -> u8 {
        self.inner.PrimaryOpCode
    }

    /// `true` if the instruction is a SIMD instruction that operates on vector regs.
    #[inline]
    pub fn has_vector(self) -> bool {
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
}

impl fmt::Display for DecodedInstruction {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let mut buffer: [i8; ffi::ND_MIN_BUF_SIZE as usize] = [0; ffi::ND_MIN_BUF_SIZE as usize];
        let status = unsafe {
            ffi::NdToText(
                &self.inner,
                self.ip,
                buffer.len() as u32,
                buffer.as_mut_ptr(),
            )
        };
        match decode_error::status_to_error(status) {
            Ok(_) => {
                let text = String::from_utf8(buffer.iter().map(|&c| c as u8).collect()).unwrap();
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
                EncodingMode::from(get_tokens(line, 2));
            } else if line.starts_with("#define ND_VEXM_") {
                VexMode::from(get_tokens(line, 2));
            } else if line.starts_with("#define ND_ADDR_") {
                AddressingMode::from(get_tokens(line, 2));
            } else if line.starts_with("#define ND_OPSZ_") {
                OperandSize::from(get_tokens(line, 2));
            } else if line.starts_with("#define ND_VECM_") {
                VectorSize::from(get_tokens(line, 2));
            } else if line.starts_with("    ND_EXC_") {
                ExceptionClass::from(exc_count);
                exc_count += 1;
            } else if line.starts_with("#define ND_SIZE_")
                && !line.starts_with("#define ND_SIZE_TO_MASK(sz)")
            {
                // TODO: this test should be in `operand.rs`, but since we are already parsing `bddisasm.h` here it
                // felt wastefull to also parse it there.
                operand::OpSize::from(get_tokens(line, 2));
            } else if line.starts_with("    ND_SHSTK_") {
                // TODO: this test should be in `operand.rs`, but since we are already parsing `bddisasm.h` here it
                // felt wastefull to also parse it there.
                operand::ShadowStackAccess::from(shadow_stack_count);
                shadow_stack_count += 1;
            } else if line.starts_with("#define ND_FPU_FLAG_") {
                // TODO: this test should be in `fpu_flags.rs`, but since we are already parsing `bddisasm.h` here it
                // felt wastefull to also parse it there.
                crate::fpu_flags::FpuFlagsAccess::from(get_tokens(line, 2) as u8);
            } else if line.starts_with("    ND_TUPLE_") {
                // TODO: this test should be in `operand.rs`, but since we are already parsing `bddisasm.h` here it
                // felt wastefull to also parse it there.
                Tuple::from(tuple_count);
                tuple_count += 1;
            } else if line.starts_with("    ND_RND_") {
                // TODO: this test should be in `operand.rs`, but since we are already parsing `bddisasm.h` here it
                // felt wastefull to also parse it there.
                EvexRounding::from(evex_rounding);
                evex_rounding += 1;
            }
        }
    }
}
