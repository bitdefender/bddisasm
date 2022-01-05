/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Operand types and details.

use super::decode_error::{status_to_error, DecodeError};
use core::fmt;
use core::marker::PhantomData;
use core::mem::MaybeUninit;

/// Describes an address operand.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct OpAddr {
    /// The ID of the base segment selector.
    ///
    /// `ES` = 0, `CS` = 1, `SS` = 2, `DS` = 3, `FS` = 4, `GS` = 5.
    pub base_seg: u16,

    /// Offset inside the segment.
    pub offset: u64,
}

#[doc(hidden)]
impl OpAddr {
    pub(crate) fn from_raw(raw: ffi::ND_OPDESC_ADDRESS) -> Self {
        Self {
            base_seg: raw.BaseSeg,
            offset: raw.Offset,
        }
    }

    pub(crate) fn new(base_seg: u16, offset: u64) -> Self {
        Self { base_seg, offset }
    }
}

/// The type of a register.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum OpRegType {
    /// The register is a 8/16/32/64 bit general purpose register.
    Gpr,

    /// The register is a segment register.
    Seg,

    /// The register is a 80-bit FPU register.
    Fpu,

    /// The register is a 64-bit MMX register.
    Mmx,

    /// The register is a 128/256/512 bit SSE vector register.
    Sse,

    /// The register is a control register.
    Cr,

    /// The register is a debug register.
    Dr,

    /// The register is a test register.
    Tr,

    /// The register is a bound register.
    Bnd,

    /// The register is a mask register.
    Msk,

    /// The register is a tile register.
    Tile,

    /// The register is a model specific register.
    Msr,

    /// The register is a extended control register.
    Xcr,

    /// The register is a system register.
    Sys,

    /// The register is a x87 status/control register.
    X87,

    /// The register is the MXCSR register.
    Mxcsr,

    /// The register is the PKRU register.
    Pkru,

    /// The register is the SSP (Shadow Stack Pointer) register.
    Ssp,

    /// The register is the FLAGS register.
    Flg,

    /// The register is the instruction pointer register.
    Rip,

    /// The register is the User Interrupt Flag.
    Uif,
}

#[doc(hidden)]
impl OpRegType {
    pub(crate) fn from_raw(op_type: ffi::ND_REG_TYPE) -> Result<Self, DecodeError> {
        match op_type {
            ffi::_ND_REG_TYPE::ND_REG_NOT_PRESENT => {
                Err(DecodeError::InternalError(op_type as u64))
            }
            ffi::_ND_REG_TYPE::ND_REG_GPR => Ok(OpRegType::Gpr),
            ffi::_ND_REG_TYPE::ND_REG_SEG => Ok(OpRegType::Seg),
            ffi::_ND_REG_TYPE::ND_REG_FPU => Ok(OpRegType::Fpu),
            ffi::_ND_REG_TYPE::ND_REG_MMX => Ok(OpRegType::Mmx),
            ffi::_ND_REG_TYPE::ND_REG_SSE => Ok(OpRegType::Sse),
            ffi::_ND_REG_TYPE::ND_REG_CR => Ok(OpRegType::Cr),
            ffi::_ND_REG_TYPE::ND_REG_DR => Ok(OpRegType::Dr),
            ffi::_ND_REG_TYPE::ND_REG_TR => Ok(OpRegType::Tr),
            ffi::_ND_REG_TYPE::ND_REG_BND => Ok(OpRegType::Bnd),
            ffi::_ND_REG_TYPE::ND_REG_MSK => Ok(OpRegType::Msk),
            ffi::_ND_REG_TYPE::ND_REG_TILE => Ok(OpRegType::Tile),
            ffi::_ND_REG_TYPE::ND_REG_MSR => Ok(OpRegType::Msr),
            ffi::_ND_REG_TYPE::ND_REG_XCR => Ok(OpRegType::Xcr),
            ffi::_ND_REG_TYPE::ND_REG_SYS => Ok(OpRegType::Sys),
            ffi::_ND_REG_TYPE::ND_REG_X87 => Ok(OpRegType::X87),
            ffi::_ND_REG_TYPE::ND_REG_MXCSR => Ok(OpRegType::Mxcsr),
            ffi::_ND_REG_TYPE::ND_REG_PKRU => Ok(OpRegType::Pkru),
            ffi::_ND_REG_TYPE::ND_REG_SSP => Ok(OpRegType::Ssp),
            ffi::_ND_REG_TYPE::ND_REG_FLG => Ok(OpRegType::Flg),
            ffi::_ND_REG_TYPE::ND_REG_RIP => Ok(OpRegType::Rip),
            ffi::_ND_REG_TYPE::ND_REG_UIF => Ok(OpRegType::Uif),
        }
    }
}

impl fmt::Display for OpRegType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Self::Gpr => write!(f, "GPR"),
            Self::Seg => write!(f, "Segment"),
            Self::Fpu => write!(f, "FPU"),
            Self::Mmx => write!(f, "MMX"),
            Self::Sse => write!(f, "SSE"),
            Self::Cr => write!(f, "CR"),
            Self::Dr => write!(f, "DR"),
            Self::Tr => write!(f, "TR"),
            Self::Bnd => write!(f, "BND"),
            Self::Msk => write!(f, "MSK"),
            Self::Tile => write!(f, "Tile"),
            Self::Msr => write!(f, "MSR"),
            Self::Xcr => write!(f, "XCR"),
            Self::Sys => write!(f, "Sys"),
            Self::X87 => write!(f, "X87"),
            Self::Mxcsr => write!(f, "MXCSR"),
            Self::Pkru => write!(f, "PKRU"),
            Self::Ssp => write!(f, "SSP"),
            Self::Flg => write!(f, "Flg"),
            Self::Rip => write!(f, "RIP"),
            Self::Uif => write!(f, "UIF"),
        }
    }
}

/// Describes a register operand.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct OpReg {
    /// The type of the register.
    pub kind: OpRegType,

    /// Indicates the register size, in bytes.
    ///
    /// This may not be equal to the [`Operand::size`] field, as a smaller amount of data may be processed from a
    /// register (especially if we have a SSE register or a mask register).
    pub size: u32,

    // TODO: create an enum for each register, instead of using an ID.
    /// The register index/ID.
    ///
    /// Registers are numbered as they are in the Intel documentation. Examples:
    ///
    /// - `AL` = `AX` = `EAX` = `RAX` = 0
    /// - `R15B` = `R15W` = `R15D` = `R15` = 15
    /// - `XMM0` = 0
    /// - `ZMM31` = 31
    /// - `ES` = 0
    /// - `CS` = 1
    ///
    /// # Remarks
    ///
    /// If [kind](OpReg::kind) is [OpRegType::Gpr](OpRegType::Gpr), the high and low part of 16-bit registers will have
    /// the same index (for example, `AH` and `AL`). To differentiate between them use [is_high8](OpReg::is_high8).
    pub index: usize,

    /// The number of registers accessed, starting with [id](OpReg::index).
    pub count: u32,

    /// `true` if this is the high part of a 16-bit GPR: `AH`, `CH`, `DH`, or `BH`.
    pub is_high8: bool,

    /// `true` if this is a block register addressing.
    pub is_block: bool,
}

#[doc(hidden)]
impl OpReg {
    pub(crate) fn from_raw(raw: ffi::ND_OPDESC_REGISTER) -> Result<Self, DecodeError> {
        let kind = OpRegType::from_raw(raw.Type)?;
        let is_high8 = raw.IsHigh8() != 0;
        let index = match kind {
            OpRegType::Gpr => {
                if is_high8 {
                    // See `ShemuGetGprValue` in `bdshemu.c`.
                    raw.Reg - 4
                } else {
                    raw.Reg
                }
            }
            _ => raw.Reg,
        } as usize;

        Ok(Self {
            kind,
            size: raw.Size,
            index,
            count: raw.Count,
            is_high8,
            is_block: raw.IsBlock() != 0,
        })
    }
}

/// Holds extra information for instruction that use VSIB addressing.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct Vsib {
    /// VSIB element size.
    pub vsib_element_size: u8,

    /// Number of elements scattered/gathered/prefetched.
    pub vsib_element_count: u8,
}

/// Shadow stack access types.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum ShadowStackAccess {
    None,

    /// Explicit memory operand accessed as shadow stack.
    Explicit,

    /// Shadow Stack Pointer (`SSP`) used as base for addressing using conventional load/store.
    SspLdSt,

    /// Shadow Stack Pointer (`SSP`) used as base for addressing using `push`/`pop`.
    SspPushPop,

    /// Privilege 0 `SSP` (`IA32_PL0_SSP`) used (`SETSSBSY`).
    Pl0Ssp,
}

#[doc(hidden)]
impl ShadowStackAccess {
    pub(crate) fn from_raw(value: u8) -> Result<Self, DecodeError> {
        let acc = u32::from(value);
        if acc == ffi::_ND_SHSTK_ACCESS::ND_SHSTK_NONE as u32 {
            Ok(ShadowStackAccess::None)
        } else if acc == ffi::_ND_SHSTK_ACCESS::ND_SHSTK_EXPLICIT as u32 {
            Ok(ShadowStackAccess::Explicit)
        } else if acc == ffi::_ND_SHSTK_ACCESS::ND_SHSTK_SSP_LD_ST as u32 {
            Ok(ShadowStackAccess::SspLdSt)
        } else if acc == ffi::_ND_SHSTK_ACCESS::ND_SHSTK_SSP_PUSH_POP as u32 {
            Ok(ShadowStackAccess::SspPushPop)
        } else if acc == ffi::_ND_SHSTK_ACCESS::ND_SHSTK_PL0_SSP as u32 {
            Ok(ShadowStackAccess::Pl0Ssp)
        } else {
            Err(DecodeError::InternalError(value.into()))
        }
    }
}

/// Describes a memory operand.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct OpMem {
    /// `true` if the memory operand is a broadcast operand.
    pub has_broadcast: bool,

    /// `true` if this is a rip-relative addressing. Base, Index, Scale are all ignored.
    pub is_rip_rel: bool,

    /// `true` if this is a stack op.
    ///
    /// # Note
    ///
    /// Explicit stack accesses are not included (eg: `mov eax, [rsp]`).
    pub is_stack: bool,

    /// `true` for `[RSI]` and `[RDI]` operands inside string operations.
    pub is_string: bool,

    /// `true` if direct addressing (`MOV [...], EAX, 0xA3`).
    pub is_direct: bool,
    /// `true` if this is a bit base. Used for `BT*` instructions.
    ///
    /// The bitbase stored in the second operand must be added to the linear address.
    pub is_bitbase: bool,

    /// `true` if the memory operand is address generation and no memory access is made.
    pub is_ag: bool,

    /// `true` MIB addressing is used (MPX instructions).
    pub is_mib: bool,

    /// `true` the addressing uses sibmem (AMX instructions).
    pub is_sib_mem: bool,

    /// Base segment used to address the memory.
    pub seg: Option<u8>,

    /// Base register.
    pub base: Option<u8>,

    /// Base register size, in bytes. Max 8 bytes.
    pub base_size: Option<u32>,

    /// Index register. Can be a vector reg (ZMM0-ZMM31).
    pub index: Option<u8>,

    /// Index register size, in bytes. Max 8 bytes.
    pub index_size: Option<u32>,

    /// The `index` register selects a vector register.
    pub vsib: Option<Vsib>,

    /// Scale: 1, 2, 4 or 8. Always present if `index` is present.
    pub scale: Option<u8>,

    /// Sign extended displacement.
    pub disp: Option<u64>,

    /// Displacement size. Max 4 bytes.
    pub disp_size: Option<u8>,

    /// Compressed displacement size - 1, 2, 4, 8, 16, 32, 64.
    pub comp_disp_size: Option<u8>,

    /// Shadow stack access type.
    ///
    /// Will be `None` if the shadow stack is not accessed.
    pub shadow_stack_access: Option<ShadowStackAccess>,
}

#[doc(hidden)]
impl OpMem {
    pub(crate) fn from_raw(raw: ffi::ND_OPDESC_MEMORY) -> Result<Self, DecodeError> {
        let seg = if raw.HasSeg() != 0 {
            Some(raw.Seg)
        } else {
            None
        };

        let (base, base_size) = if raw.HasBase() != 0 {
            (Some(raw.Base), Some(raw.BaseSize))
        } else {
            (None, None)
        };

        let (index, index_size, scale) = if raw.HasIndex() != 0 {
            (Some(raw.Index), Some(raw.IndexSize), Some(raw.Scale))
        } else {
            (None, None, None)
        };

        let (disp, disp_size) = if raw.HasDisp() != 0 {
            (Some(raw.Disp), Some(raw.DispSize))
        } else {
            (None, None)
        };

        let comp_disp_size = if raw.HasCompDisp() != 0 {
            Some(raw.CompDispSize)
        } else {
            None
        };

        let (vsib, index_size) = if raw.IsVsib() != 0 {
            (
                Some(Vsib {
                    vsib_element_size: raw.Vsib.ElemSize,
                    vsib_element_count: raw.Vsib.ElemCount,
                }),
                Some(raw.Vsib.IndexSize.into()),
            )
        } else {
            (None, index_size)
        };

        let shadow_stack_access = if raw.IsShadowStack() != 0 {
            Some(ShadowStackAccess::from_raw(raw.ShStkType)?)
        } else {
            None
        };

        Ok(Self {
            has_broadcast: raw.HasBroadcast() != 0,
            is_rip_rel: raw.IsRipRel() != 0,
            is_stack: raw.IsStack() != 0,
            is_string: raw.IsString() != 0,
            is_direct: raw.IsDirect() != 0,
            is_bitbase: raw.IsBitbase() != 0,
            is_ag: raw.IsAG() != 0,
            is_mib: raw.IsMib() != 0,
            is_sib_mem: raw.IsSibMem() != 0,
            seg,
            base,
            base_size,
            index,
            index_size,
            vsib,
            scale,
            disp,
            disp_size,
            comp_disp_size,
            shadow_stack_access,
        })
    }
}

/// Extended operand information, based on the actual type of the operand.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum OpInfo {
    None,

    /// The operand is a register.
    Reg(OpReg),

    /// The operand is located in memory.
    Mem(OpMem),

    /// The operand is an immediate.
    Imm(u64),

    /// The operand is a relative offset.
    Offs(u64),

    /// The operand is an absolute address, in the form `seg:offset`.
    Addr(OpAddr),

    /// The operand is an implicit constant.
    Const(u64),

    /// An entire bank/set of registers are being accessed. Used in `PUSHA`/`POPA`/`XSAVE`/`LOADALL`.
    Bank,
}

#[doc(hidden)]
impl Default for OpInfo {
    fn default() -> Self {
        Self::None
    }
}

impl OpInfo {
    /// Returns the associated [OpReg](OpReg) for register operands. Returns [`None`] otherwise.
    pub fn as_reg(&self) -> Option<&OpReg> {
        if let OpInfo::Reg(o) = self {
            Some(o)
        } else {
            None
        }
    }

    /// Returns the associated [OpMem](OpMem) for memory operands. Returns [`None`] otherwise.
    pub fn as_mem(&self) -> Option<&OpMem> {
        if let OpInfo::Mem(o) = self {
            Some(o)
        } else {
            None
        }
    }

    /// Returns the associated immediate value for immediate operands. Returns [`None`] otherwise.
    pub fn as_imm(&self) -> Option<u64> {
        if let OpInfo::Imm(o) = self {
            Some(*o)
        } else {
            None
        }
    }

    /// Returns the associated [OpAddr](OpAddr) for absolute address operands. Returns [`None`] otherwise.
    pub fn as_addr(&self) -> Option<&OpAddr> {
        if let OpInfo::Addr(o) = self {
            Some(o)
        } else {
            None
        }
    }

    /// Returns the associated constant value for constant operands. Returns [`None`] otherwise.
    pub fn as_const(&self) -> Option<u64> {
        if let OpInfo::Const(o) = self {
            Some(*o)
        } else {
            None
        }
    }

    /// Returns `Some` for bank operands. Returns [`None`] otherwise.
    pub fn as_bank(&self) -> Option<()> {
        if let OpInfo::Bank = self {
            Some(())
        } else {
            None
        }
    }

    /// Returns `true` for register operands. Returns `false` otherwise.
    pub fn is_reg(&self) -> bool {
        self.as_reg().is_some()
    }

    /// Returns `true` for memory operands. Returns `false` otherwise.
    pub fn is_mem(&self) -> bool {
        self.as_mem().is_some()
    }

    /// Returns `true` for immediate operands. Returns `false` otherwise.
    pub fn is_imm(&self) -> bool {
        self.as_imm().is_some()
    }

    /// Returns `true` for absolute address operands. Returns `false` otherwise.
    pub fn is_addr(&self) -> bool {
        self.as_addr().is_some()
    }

    /// Returns `true` for constant operands. Returns `false` otherwise.
    pub fn is_const(&self) -> bool {
        self.as_const().is_some()
    }

    /// Returns `true` for bank operands. Returns `false` otherwise.
    pub fn is_bank(&self) -> bool {
        self.as_bank().is_some()
    }
}

#[doc(hidden)]
impl OpInfo {
    pub(crate) fn from_raw(raw: ffi::ND_OPERAND) -> Result<Self, DecodeError> {
        match raw.Type {
            ffi::_ND_OPERAND_TYPE::ND_OP_NOT_PRESENT => Ok(OpInfo::None),
            ffi::_ND_OPERAND_TYPE::ND_OP_REG => {
                Ok(OpInfo::Reg(OpReg::from_raw(unsafe { raw.Info.Register })?))
            }
            ffi::_ND_OPERAND_TYPE::ND_OP_MEM => {
                Ok(OpInfo::Mem(OpMem::from_raw(unsafe { raw.Info.Memory })?))
            }
            ffi::_ND_OPERAND_TYPE::ND_OP_IMM => Ok(OpInfo::Imm(unsafe { raw.Info.Immediate }.Imm)),
            ffi::_ND_OPERAND_TYPE::ND_OP_OFFS => {
                Ok(OpInfo::Offs(unsafe { raw.Info.RelativeOffset }.Rel))
            }
            ffi::_ND_OPERAND_TYPE::ND_OP_ADDR => {
                Ok(OpInfo::Addr(OpAddr::from_raw(unsafe { raw.Info.Address })))
            }
            ffi::_ND_OPERAND_TYPE::ND_OP_CONST => {
                Ok(OpInfo::Const(unsafe { raw.Info.Constant }.Const))
            }
            ffi::_ND_OPERAND_TYPE::ND_OP_BANK => Ok(OpInfo::Bank),
        }
    }
}

impl fmt::Display for OpInfo {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Self::None => write!(f, "None"),
            Self::Reg(_) => write!(f, "register"),
            Self::Mem(_) => write!(f, "memory"),
            Self::Imm(_) => write!(f, "immediate"),
            Self::Offs(_) => write!(f, "relative offset"),
            Self::Addr(_) => write!(f, "absolute address"),
            Self::Const(_) => write!(f, "implicit constant"),
            Self::Bank => write!(f, "register bank"),
        }
    }
}

/// Operand size.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum OpSize {
    /// The inner value contains the operand size in bytes.
    Bytes(usize),

    /// The size of a cache line.
    CacheLine,

    /// Unknown size.
    Unknown,
}

#[doc(hidden)]
impl Default for OpSize {
    fn default() -> Self {
        Self::Unknown
    }
}

impl fmt::Display for OpSize {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Self::Bytes(size) => write!(f, "{} bytes", size),
            Self::CacheLine => write!(f, "cache line"),
            Self::Unknown => write!(f, "unknown"),
        }
    }
}

#[doc(hidden)]
impl OpSize {
    pub(crate) fn from_raw(value: ffi::ND_OPERAND_SIZE) -> Result<Self, DecodeError> {
        match value {
            ffi::ND_SIZE_8BIT => Ok(OpSize::Bytes(1)),
            ffi::ND_SIZE_16BIT => Ok(OpSize::Bytes(2)),
            ffi::ND_SIZE_32BIT => Ok(OpSize::Bytes(4)),
            ffi::ND_SIZE_48BIT => Ok(OpSize::Bytes(6)),
            ffi::ND_SIZE_64BIT => Ok(OpSize::Bytes(8)),
            ffi::ND_SIZE_80BIT => Ok(OpSize::Bytes(10)),
            ffi::ND_SIZE_112BIT => Ok(OpSize::Bytes(14)),
            ffi::ND_SIZE_128BIT => Ok(OpSize::Bytes(16)),
            ffi::ND_SIZE_224BIT => Ok(OpSize::Bytes(28)),
            ffi::ND_SIZE_256BIT => Ok(OpSize::Bytes(32)),
            ffi::ND_SIZE_384BIT => Ok(OpSize::Bytes(48)),
            ffi::ND_SIZE_512BIT => Ok(OpSize::Bytes(64)),
            ffi::ND_SIZE_752BIT => Ok(OpSize::Bytes(94)),
            ffi::ND_SIZE_864BIT => Ok(OpSize::Bytes(108)),
            ffi::ND_SIZE_4096BIT => Ok(OpSize::Bytes(512)),
            ffi::ND_SIZE_1KB => Ok(OpSize::Bytes(1024)),
            ffi::ND_SIZE_CACHE_LINE => Ok(OpSize::CacheLine),
            ffi::ND_SIZE_UNKNOWN => Ok(OpSize::Unknown),
            _ => Err(DecodeError::InternalError(value.into())),
        }
    }
}

/// Operand access mode.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug, Default)]
pub struct OpAccess {
    /// The operand is read.
    pub read: bool,

    /// The operand is written.
    pub write: bool,

    /// The operand is read only under some conditions.
    pub cond_read: bool,

    /// The operand is written only under some conditions.
    pub cond_write: bool,

    /// The operand is prefetched.
    pub prefetch: bool,
}

#[doc(hidden)]
impl OpAccess {
    pub(crate) fn from_raw(raw: ffi::ND_OPERAND_ACCESS) -> Self {
        let acc = unsafe { raw.__bindgen_anon_1 };

        Self {
            read: acc.Read() != 0,
            write: acc.Write() != 0,
            cond_read: acc.CondRead() != 0,
            cond_write: acc.CondWrite() != 0,
            prefetch: acc.Prefetch() != 0,
        }
    }
}

/// Broadcast specifier.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct Broadcast {
    /// Number of times to broadcast the element.
    pub count: u8,

    /// Size of one element.
    pub size: u8,
}

/// Decorator information.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug, Default)]
pub struct Decorator {
    /// If mask is present, holds the ID of the mask register (`K0` - `K7`) used.
    pub mask_register: Option<u8>,

    /// `true` if zeroing will be made, `false` if merging will be made.
    pub has_zero: bool,

    /// If broadcasting is being made, holds the broadcast specifier.
    pub broadcast: Option<Broadcast>,
}

#[doc(hidden)]
impl Decorator {
    pub(crate) fn from_raw(raw: ffi::ND_OPERAND_DECORATOR) -> Decorator {
        let mask_register = if raw.HasMask() != 0 {
            Some(raw.Mask.Msk)
        } else {
            None
        };

        let broadcast = if raw.HasBroadcast() != 0 {
            Some(Broadcast {
                count: raw.Broadcast.Count,
                size: raw.Broadcast.Size,
            })
        } else {
            None
        };

        Self {
            mask_register,
            has_zero: raw.HasZero() != 0,
            broadcast,
        }
    }
}

/// Describes an instruction operand.
///
/// Each operand type encodes different information. See [`OpInfo`] for details.
///
/// # Examples
///
/// ```
/// # use bddisasm::DecodeError;
/// #
/// # fn main() -> Result<(), DecodeError> {
/// use bddisasm::{DecodedInstruction, DecodeMode, OpRegType, OpSize};
///
/// // `MOV       ah, byte ptr [rcx+rdx*2+0x8]`
/// let code = vec![0x8a, 0x64, 0x51, 0x08];
/// let ins = DecodedInstruction::decode(&code, DecodeMode::Bits64)?;
///
/// let operands = ins.operands();
/// let dst = operands[0];
/// let src = operands[1];
///
/// // Check the size of each operand
/// assert_eq!(dst.size, OpSize::Bytes(1));
/// assert_eq!(src.size, OpSize::Bytes(1));
///
/// // Check the type of the destination operand
/// assert!(dst.info.is_reg());
/// let dst_reg = dst.info.as_reg().unwrap();
/// assert_eq!(dst_reg.kind, OpRegType::Gpr);
///
/// # Ok(())
/// # }
/// ```
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug, Default)]
pub struct Operand {
    /// Extended operand information.
    pub info: OpInfo,

    // Operand size in bytes.
    //
    // This should be used when operating with the operand. It includes sign-extension or zero-extensions.
    //
    // # Note
    ///
    /// It indicates the actual amount of data used for processing. If the operand is a [register](OpInfo::Reg), it
    /// MAY NOT indicate the register size. Use the [info](Operand::info) field to get the actual register size in these
    /// cases.
    pub size: OpSize,

    /// Raw size inside the instruction.
    ///
    /// This will usually be identical to [size](Operand::size), however, some instructions force the actual size of
    /// their operands to 64 bit (`PUSH`/`POP` or branches are good examples).
    ///
    /// Although the raw size of the relative offset or the immediate will be [raw_size](Operand::raw_size), internally,
    /// the CPU will use [size](Operand::size) (usually sign-extended).
    pub raw_size: OpSize,

    ///  Access mode.
    pub access: OpAccess,

    /// `true` if the operand is default. This also applies to implicit operands.
    pub is_default: bool,

    /// Decorator information.
    pub decorator: Decorator,
}

#[doc(hidden)]
impl Operand {
    pub(crate) fn from_raw(raw: ffi::ND_OPERAND) -> Result<Self, DecodeError> {
        Ok(Self {
            info: OpInfo::from_raw(raw)?,
            size: OpSize::from_raw(raw.Size)?,
            raw_size: OpSize::from_raw(raw.RawSize)?,
            access: OpAccess::from_raw(raw.Access),
            is_default: unsafe { raw.Flags.__bindgen_anon_1 }.IsDefault() != 0,
            decorator: Decorator::from_raw(raw.Decorator),
        })
    }
}

/// Operands lookup table.
///
/// This can be useful when needing to work with a specific operand without needing to iterate over the operands
/// returned by [operands()](crate::decoded_instruction::DecodedInstruction::operands), and without needing to rely on
/// the order of the operands.
///
/// # Examples
///
/// ```rust
/// # use bddisasm::DecodeError;
/// #
/// # fn main() -> Result<(), DecodeError> {
/// use bddisasm::{DecodedInstruction, DecodeMode, OperandsLookup, OpRegType};
///
/// // `PUSH      rbx`
/// let ins = DecodedInstruction::decode(b"\x53", DecodeMode::Bits64).unwrap();
/// let operands = ins.operand_lookup();
///
/// // The first destination is the stack.
/// let first_destination = operands.dest(0);
/// assert!(first_destination.is_some());
/// let first_destination = first_destination.unwrap();
///
/// // And it is the same as the first memory operand.
/// let first_mem = operands.mem(0);
/// assert!(first_mem.is_some());
/// let first_mem = first_mem.unwrap();
/// assert_eq!(first_destination, first_mem);
///
/// // And the same as the stack operand.
/// let stack = operands.stack();
/// assert!(stack.is_some());
/// let stack = stack.unwrap();
/// assert_eq!(first_destination, stack);
///
/// assert!(first_destination.is_default);
/// assert!(first_destination.info.is_mem());
/// assert!(first_destination.info.as_mem().unwrap().is_stack);
///
/// // Although the source operand is the RBX register, it is not one of the default operands.
/// let rbx = operands.rbx();
/// assert!(rbx.is_none());
///
/// // There is only one destination operand.
/// let second_destination = operands.dest(1);
/// assert!(second_destination.is_none());
///
/// // The first source is RBX.
/// let first_source = operands.src(0);
/// assert!(first_source.is_some());
/// let first_source = first_source.unwrap();
///
/// assert_eq!(first_source.is_default, false);
/// assert!(first_source.info.is_reg());
///
/// let first_source = first_source.info.as_reg().unwrap();
/// assert_eq!(first_source.kind, OpRegType::Gpr);
/// assert_eq!(first_source.index, 3);
///
/// // There is only one source operand.
/// let second_source = operands.src(1);
/// assert!(second_source.is_none());
///
/// // There is no other memory operand.
/// let second_mem = operands.mem(1);
/// assert!(second_mem.is_none());
///
/// // The FLAGS register is not accessed.
/// let flags = operands.flags();
/// assert!(flags.is_none());
/// # Ok(())
/// # }
/// ```
#[derive(Clone, Debug)]
pub struct OperandsLookup<'a> {
    // The C library fills a `ND_OPERAND_RLUT` structure with pointers to `ND_OPERAND` structures taken from the
    // `INSTRUX` structure. As such, the `INSTRUX` structure must live at least as long as the `ND_OPERAND_RLUT`
    // structure. This expresses that life-time dependency.
    _instruction: PhantomData<&'a ffi::INSTRUX>,
    op_rlut: ffi::ND_OPERAND_RLUT,
}

impl<'a> OperandsLookup<'a> {
    #[doc(hidden)]
    pub(crate) fn from_raw(instruction: &'a ffi::INSTRUX) -> Self {
        let mut op_rlut: MaybeUninit<ffi::ND_OPERAND_RLUT> = MaybeUninit::uninit();
        let op_rlut = op_rlut.as_mut_ptr();

        let status = unsafe {
            ffi::NdGetOperandRlut(
                instruction as *const ffi::INSTRUX as *mut ffi::INSTRUX,
                op_rlut,
            )
        };

        // It is ok to unwrap here because `NdGetOperandRlut` fails only if the pointers passed to it are `NULL`.
        status_to_error(status).unwrap();
        let op_rlut = unsafe { *op_rlut };

        Self {
            _instruction: PhantomData,
            op_rlut,
        }
    }

    /// Get one of the destination operands.
    ///
    /// Most instructions have just one destination operand, but some will have two.
    ///
    /// # Arguments
    ///
    /// * `index` - The index of the destination operand. First destination operand has index 0, the second one has
    /// index 1, etc.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if the requested destination operand exists.
    /// * [`None`] if the requested destination operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn dest(&self, index: usize) -> Option<Operand> {
        let op = match index {
            0 => unsafe { self.op_rlut.Dst1.as_ref() },
            1 => unsafe { self.op_rlut.Dst2.as_ref() },
            _ => None,
        };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get one of the source operands.
    ///
    /// Most instructions have just one souce operand, but some can have up to 4.
    ///
    /// # Arguments
    ///
    /// * `index` - The index of the source operand. First source operand has index 0, the second one has
    /// index 1, etc.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if the requested source operand exists.
    /// * [`None`] if the requested source operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn src(&self, index: usize) -> Option<Operand> {
        let op = match index {
            0 => unsafe { self.op_rlut.Src1.as_ref() },
            1 => unsafe { self.op_rlut.Src2.as_ref() },
            2 => unsafe { self.op_rlut.Src3.as_ref() },
            3 => unsafe { self.op_rlut.Src4.as_ref() },
            _ => None,
        };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get one of the memory operands.
    ///
    /// Most instructions have just one memory operand, but some will have two.
    ///
    /// # Arguments
    ///
    /// * `index` - The index of the memory operand. First memory operand has index 0, the second one has index 1, etc.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if the requested memory operand exists.
    /// * [`None`] if the requested memory operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn mem(&self, index: usize) -> Option<Operand> {
        let op = match index {
            0 => unsafe { self.op_rlut.Mem1.as_ref() },
            1 => unsafe { self.op_rlut.Mem2.as_ref() },
            _ => None,
        };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the stack operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a stack operand exists.
    /// * [`None`] if a stack operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn stack(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Stack.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the default flags register operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a flags register operand exists.
    /// * [`None`] if a flags register operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn flags(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Flags.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the default RIP operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a RIP operand exists.
    /// * [`None`] if a RIP operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn rip(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Rip.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the implicit CS operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a implicit CS operand exists.
    /// * [`None`] if a implicit CS operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn cs(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Cs.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the implicit SS operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a implicit SS operand exists.
    /// * [`None`] if a implicit SS operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn ss(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Ss.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the implicit RAX operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a implicit RAX operand exists.
    /// * [`None`] if a implicit RAX operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn rax(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Rax.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the implicit RCX operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a implicit RCX operand exists.
    /// * [`None`] if a implicit RCX operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn rcx(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Rcx.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the implicit RDX operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a implicit RDX operand exists.
    /// * [`None`] if a implicit RDX operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn rdx(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Rdx.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the implicit RBX operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a implicit RBX operand exists.
    /// * [`None`] if a implicit RBX operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn rbx(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Rbx.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the implicit RSP operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a implicit RSP operand exists.
    /// * [`None`] if a implicit RSP operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn rsp(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Rsp.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the implicit RBP operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a implicit RBP operand exists.
    /// * [`None`] if a implicit RBP operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn rbp(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Rbp.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the implicit RSI operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a implicit RSI operand exists.
    /// * [`None`] if a implicit RSI operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn rsi(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Rsi.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }

    /// Get the implicit RDI operand.
    ///
    /// # Returns
    ///
    /// * [Some(Operand)](Operand) if a implicit RDI operand exists.
    /// * [`None`] if a implicit RDI operand does not exist.
    ///
    /// # Panics
    ///
    /// This function will panic if the result of the C library is unrecognized. This can not happen under normal
    /// circumstances.
    #[inline]
    pub fn rdi(&self) -> Option<Operand> {
        let op = unsafe { self.op_rlut.Rdi.as_ref() };

        op.map(|op| Operand::from_raw(*op).unwrap())
    }
}

/// A collection of [Operand](Operand)s.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug, Default)]
pub struct Operands {
    pub(crate) operands: [Operand; 10],
    pub(crate) actual_count: usize,
}

impl core::ops::Deref for Operands {
    type Target = [Operand];

    fn deref(&self) -> &[Operand] {
        &self.operands[..self.actual_count]
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::decoded_instruction::{DecodeMode, DecodedInstruction};

    #[test]
    fn ops() {
        let code = vec![0x8a, 0x64, 0x51, 0x08];
        let ins = DecodedInstruction::decode(&code, DecodeMode::Bits64).unwrap();

        let operands = ins.operands();
        assert_eq!(operands.len(), 2);

        let dest = operands[0];
        assert_eq!(dest.size, OpSize::Bytes(1));
        assert_eq!(dest.raw_size, OpSize::Bytes(1));
        assert_eq!(dest.is_default, false);
        assert!(dest.access.write);

        if let OpInfo::Reg(dst_reg) = dest.info {
            assert_eq!(dst_reg.kind, OpRegType::Gpr);
            assert_eq!(dst_reg.size, 1);
            assert_eq!(dst_reg.index, 0);
            assert_eq!(dst_reg.is_high8, true);
        } else {
            unreachable!();
        }

        let src = operands[1];
        assert_eq!(src.size, OpSize::Bytes(1));
        assert_eq!(src.raw_size, OpSize::Bytes(1));
        assert_eq!(src.is_default, false);
        assert!(src.access.read);

        if let OpInfo::Mem(src_mem) = src.info {
            assert_eq!(src_mem.seg, Some(3));
            assert_eq!(src_mem.base, Some(1));
            assert_eq!(src_mem.base_size, Some(8));
            assert_eq!(src_mem.index, Some(2));
            assert_eq!(src_mem.index_size, Some(8));
            assert_eq!(src_mem.scale, Some(2));
            assert_eq!(src_mem.disp, Some(8));
            assert_eq!(src_mem.disp_size, Some(1));
        } else {
            unreachable!();
        }
    }

    #[test]
    fn op_lut() {
        // `PUSH      rbx`
        let ins = DecodedInstruction::decode(b"\x53", DecodeMode::Bits64).unwrap();
        let operands = ins.operand_lookup();

        // The first destination is the stack.
        let first_destination = operands.dest(0);
        assert!(first_destination.is_some());
        let first_destination = first_destination.unwrap();

        // And it is the same as the first memory operand.
        let first_mem = operands.mem(0);
        assert!(first_mem.is_some());
        let first_mem = first_mem.unwrap();
        assert_eq!(first_destination, first_mem);

        // And the same as the stack operand.
        let stack = operands.stack();
        assert!(stack.is_some());
        let stack = stack.unwrap();
        assert_eq!(first_destination, stack);

        assert!(first_destination.is_default);
        assert!(first_destination.info.is_mem());
        assert!(first_destination.info.as_mem().unwrap().is_stack);

        // Although the source operand is the RBX register, it is not one of the default operands.
        let rbx = operands.rbx();
        assert!(rbx.is_none());

        // There is only one destination operand.
        let second_destination = operands.dest(1);
        assert!(second_destination.is_none());

        // The first source is RBX.
        let first_source = operands.src(0);
        assert!(first_source.is_some());
        let first_source = first_source.unwrap();

        assert_eq!(first_source.is_default, false);
        assert!(first_source.info.is_reg());

        let first_source = first_source.info.as_reg().unwrap();
        assert_eq!(first_source.kind, OpRegType::Gpr);
        assert_eq!(first_source.index, 3);

        // There is only one source operand.
        let second_source = operands.src(1);
        assert!(second_source.is_none());

        // There is no other memory operand.
        let second_mem = operands.mem(1);
        assert!(second_mem.is_none());

        // The FLAGS register is not accessed.
        let flags = operands.flags();
        assert!(flags.is_none());
    }

    #[test]
    fn check_all_shstk_access_values() {
        // This is a really contrieved way of making sure that we check all variants of `ffi::_ND_SHSTK_ACCESS`. If a
        // new one is added, this will fail to build. We do this because `ShadowStackAccess::from_raw` takes an `u8`.
        // NOTE: When a new variant is added, `ShadowStackAccess::from_raw` must be updated.
        match ffi::_ND_SHSTK_ACCESS::ND_SHSTK_NONE {
            ffi::_ND_SHSTK_ACCESS::ND_SHSTK_NONE => {}
            ffi::_ND_SHSTK_ACCESS::ND_SHSTK_EXPLICIT => {}
            ffi::_ND_SHSTK_ACCESS::ND_SHSTK_SSP_LD_ST => {}
            ffi::_ND_SHSTK_ACCESS::ND_SHSTK_SSP_PUSH_POP => {}
            ffi::_ND_SHSTK_ACCESS::ND_SHSTK_PL0_SSP => {}
        }
    }
}
