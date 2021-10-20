/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Operand types and details.

extern crate bddisasm_sys as ffi;

use std::fmt;

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
impl From<ffi::ND_OPDESC_ADDRESS> for OpAddr {
    fn from(op: ffi::ND_OPDESC_ADDRESS) -> OpAddr {
        OpAddr {
            base_seg: op.BaseSeg,
            offset: op.Offset,
        }
    }
}

#[doc(hidden)]
impl OpAddr {
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
impl From<ffi::ND_OPDESC_REGISTER> for OpRegType {
    fn from(op: ffi::ND_OPDESC_REGISTER) -> OpRegType {
        match op.Type {
            ffi::_ND_REG_TYPE::ND_REG_NOT_PRESENT => panic!("Unexpected ND_REG_NOT_PRESENT"),
            ffi::_ND_REG_TYPE::ND_REG_GPR => OpRegType::Gpr,
            ffi::_ND_REG_TYPE::ND_REG_SEG => OpRegType::Seg,
            ffi::_ND_REG_TYPE::ND_REG_FPU => OpRegType::Fpu,
            ffi::_ND_REG_TYPE::ND_REG_MMX => OpRegType::Mmx,
            ffi::_ND_REG_TYPE::ND_REG_SSE => OpRegType::Sse,
            ffi::_ND_REG_TYPE::ND_REG_CR => OpRegType::Cr,
            ffi::_ND_REG_TYPE::ND_REG_DR => OpRegType::Dr,
            ffi::_ND_REG_TYPE::ND_REG_TR => OpRegType::Tr,
            ffi::_ND_REG_TYPE::ND_REG_BND => OpRegType::Bnd,
            ffi::_ND_REG_TYPE::ND_REG_MSK => OpRegType::Msk,
            ffi::_ND_REG_TYPE::ND_REG_TILE => OpRegType::Tile,
            ffi::_ND_REG_TYPE::ND_REG_MSR => OpRegType::Msr,
            ffi::_ND_REG_TYPE::ND_REG_XCR => OpRegType::Xcr,
            ffi::_ND_REG_TYPE::ND_REG_SYS => OpRegType::Sys,
            ffi::_ND_REG_TYPE::ND_REG_X87 => OpRegType::X87,
            ffi::_ND_REG_TYPE::ND_REG_MXCSR => OpRegType::Mxcsr,
            ffi::_ND_REG_TYPE::ND_REG_PKRU => OpRegType::Pkru,
            ffi::_ND_REG_TYPE::ND_REG_SSP => OpRegType::Ssp,
            ffi::_ND_REG_TYPE::ND_REG_FLG => OpRegType::Flg,
            ffi::_ND_REG_TYPE::ND_REG_RIP => OpRegType::Rip,
            ffi::_ND_REG_TYPE::ND_REG_UIF => OpRegType::Uif,
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
impl From<ffi::ND_OPDESC_REGISTER> for OpReg {
    fn from(op: ffi::ND_OPDESC_REGISTER) -> OpReg {
        let kind = OpRegType::from(op);
        let is_high8 = op.IsHigh8();
        let index = match kind {
            OpRegType::Gpr => {
                if is_high8 {
                    // See `ShemuGetGprValue` in `bdshemu.c`.
                    op.Reg - 4
                } else {
                    op.Reg
                }
            }
            _ => op.Reg,
        } as usize;

        OpReg {
            kind,
            size: op.Size,
            index,
            count: op.Count,
            is_high8,
            is_block: op.IsBlock(),
        }
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
impl From<ffi::ND_SHSTK_ACCESS> for ShadowStackAccess {
    fn from(acc: ffi::ND_SHSTK_ACCESS) -> ShadowStackAccess {
        match acc {
            ffi::_ND_SHSTK_ACCESS::ND_SHSTK_NONE => ShadowStackAccess::None,
            ffi::_ND_SHSTK_ACCESS::ND_SHSTK_EXPLICIT => ShadowStackAccess::Explicit,
            ffi::_ND_SHSTK_ACCESS::ND_SHSTK_SSP_LD_ST => ShadowStackAccess::SspLdSt,
            ffi::_ND_SHSTK_ACCESS::ND_SHSTK_SSP_PUSH_POP => ShadowStackAccess::SspPushPop,
            ffi::_ND_SHSTK_ACCESS::ND_SHSTK_PL0_SSP => ShadowStackAccess::Pl0Ssp,
            // NOTE: when updating this take care to also update the `From<u8>` implementation!
            // TODO: any way of keeping these in sync automagically?
        }
    }
}

impl From<u8> for ShadowStackAccess {
    fn from(acc: u8) -> ShadowStackAccess {
        if acc == ffi::_ND_SHSTK_ACCESS::ND_SHSTK_NONE as u8 {
            ShadowStackAccess::None
        } else if acc == ffi::_ND_SHSTK_ACCESS::ND_SHSTK_EXPLICIT as u8 {
            ShadowStackAccess::Explicit
        } else if acc == ffi::_ND_SHSTK_ACCESS::ND_SHSTK_SSP_LD_ST as u8 {
            ShadowStackAccess::SspLdSt
        } else if acc == ffi::_ND_SHSTK_ACCESS::ND_SHSTK_SSP_PUSH_POP as u8 {
            ShadowStackAccess::SspPushPop
        } else if acc == ffi::_ND_SHSTK_ACCESS::ND_SHSTK_PL0_SSP as u8 {
            ShadowStackAccess::Pl0Ssp
        } else {
            panic!("Unexpected shadow stack access type: {}", acc)
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
impl From<ffi::ND_OPDESC_MEMORY> for OpMem {
    fn from(op: ffi::ND_OPDESC_MEMORY) -> OpMem {
        let seg = if op.HasSeg() { Some(op.Seg) } else { None };

        let (base, base_size) = if op.HasBase() {
            (Some(op.Base), Some(op.BaseSize))
        } else {
            (None, None)
        };

        let (index, index_size, scale) = if op.HasIndex() {
            (Some(op.Index), Some(op.IndexSize), Some(op.Scale))
        } else {
            (None, None, None)
        };

        let (disp, disp_size) = if op.HasDisp() {
            (Some(op.Disp), Some(op.DispSize))
        } else {
            (None, None)
        };

        let comp_disp_size = if op.HasCompDisp() {
            Some(op.CompDispSize)
        } else {
            None
        };

        let (vsib, index_size) = if op.IsVsib() {
            (
                Some(Vsib {
                    vsib_element_size: op.Vsib.ElemSize,
                    vsib_element_count: op.Vsib.ElemCount,
                }),
                Some(op.Vsib.IndexSize as u32),
            )
        } else {
            (None, index_size)
        };

        let shadow_stack_access = if op.IsShadowStack() {
            Some(ShadowStackAccess::from(op.ShStkType))
        } else {
            None
        };

        OpMem {
            has_broadcast: op.HasBroadcast(),
            is_rip_rel: op.IsRipRel(),
            is_stack: op.IsStack(),
            is_string: op.IsString(),
            is_direct: op.IsDirect(),
            is_bitbase: op.IsBitbase(),
            is_ag: op.IsAG(),
            is_mib: op.IsMib(),
            is_sib_mem: op.IsSibMem(),
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
        }
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
impl From<ffi::ND_OPERAND> for OpInfo {
    fn from(op: ffi::ND_OPERAND) -> OpInfo {
        match op.Type {
            ffi::_ND_OPERAND_TYPE::ND_OP_NOT_PRESENT => OpInfo::None,
            ffi::_ND_OPERAND_TYPE::ND_OP_REG => {
                OpInfo::Reg(OpReg::from(unsafe { op.Info.Register }))
            }
            ffi::_ND_OPERAND_TYPE::ND_OP_MEM => OpInfo::Mem(OpMem::from(unsafe { op.Info.Memory })),
            ffi::_ND_OPERAND_TYPE::ND_OP_IMM => OpInfo::Imm(unsafe { op.Info.Immediate }.Imm),
            ffi::_ND_OPERAND_TYPE::ND_OP_OFFS => {
                OpInfo::Offs(unsafe { op.Info.RelativeOffset }.Rel)
            }
            ffi::_ND_OPERAND_TYPE::ND_OP_ADDR => {
                OpInfo::Addr(OpAddr::from(unsafe { op.Info.Address }))
            }
            ffi::_ND_OPERAND_TYPE::ND_OP_CONST => OpInfo::Const(unsafe { op.Info.Constant }.Const),
            ffi::_ND_OPERAND_TYPE::ND_OP_BANK => OpInfo::Bank,
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
impl From<ffi::ND_OPERAND_SIZE> for OpSize {
    fn from(sz: ffi::ND_OPERAND_SIZE) -> OpSize {
        match sz {
            ffi::ND_SIZE_8BIT => OpSize::Bytes(1),
            ffi::ND_SIZE_16BIT => OpSize::Bytes(2),
            ffi::ND_SIZE_32BIT => OpSize::Bytes(4),
            ffi::ND_SIZE_48BIT => OpSize::Bytes(6),
            ffi::ND_SIZE_64BIT => OpSize::Bytes(8),
            ffi::ND_SIZE_80BIT => OpSize::Bytes(10),
            ffi::ND_SIZE_112BIT => OpSize::Bytes(14),
            ffi::ND_SIZE_128BIT => OpSize::Bytes(16),
            ffi::ND_SIZE_224BIT => OpSize::Bytes(28),
            ffi::ND_SIZE_256BIT => OpSize::Bytes(32),
            ffi::ND_SIZE_384BIT => OpSize::Bytes(48),
            ffi::ND_SIZE_512BIT => OpSize::Bytes(64),
            ffi::ND_SIZE_752BIT => OpSize::Bytes(94),
            ffi::ND_SIZE_864BIT => OpSize::Bytes(108),
            ffi::ND_SIZE_4096BIT => OpSize::Bytes(512),
            ffi::ND_SIZE_1KB => OpSize::Bytes(1024),
            ffi::ND_SIZE_CACHE_LINE => OpSize::CacheLine,
            ffi::ND_SIZE_UNKNOWN => OpSize::Unknown,
            _ => panic!("Unespected operand size: {}", sz),
        }
    }
}

/// Operand access mode.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
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
impl From<ffi::ND_OPERAND_ACCESS> for OpAccess {
    fn from(acc: ffi::ND_OPERAND_ACCESS) -> OpAccess {
        let acc = unsafe { acc.__bindgen_anon_1 };

        OpAccess {
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
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct Decorator {
    /// If mask is present, holds the ID of the mask register (`K0` - `K7`) used.
    pub mask_register: Option<u8>,

    /// `true` if zeroing will be made, `false` if merging will be made.
    pub has_zero: bool,

    /// If broadcasting is being made, holds the broadcast specifier.
    pub broadcast: Option<Broadcast>,
}

#[doc(hidden)]
impl From<ffi::ND_OPERAND_DECORATOR> for Decorator {
    fn from(decorator: ffi::ND_OPERAND_DECORATOR) -> Decorator {
        let mask_register = if decorator.HasMask() {
            Some(decorator.Mask.Msk)
        } else {
            None
        };

        let broadcast = if decorator.HasBroadcast() {
            Some(Broadcast {
                count: decorator.Broadcast.Count,
                size: decorator.Broadcast.Size,
            })
        } else {
            None
        };

        Decorator {
            mask_register,
            has_zero: decorator.HasZero(),
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
/// # use std::error::Error;
/// #
/// # fn main() -> Result<(), Box<dyn Error>> {
/// use bddisasm::decoder::{DecodedInstruction, DecodeMode};
/// use bddisasm::operand::*;
///
/// // `MOV       ah, byte ptr [rcx+rdx*2+0x8]`
/// let code = vec![0x8a, 0x64, 0x51, 0x08];
/// let ins = DecodedInstruction::decode(&code, DecodeMode::Bits64)?;
///
/// let operands = ins.operands();
/// let dst = operands[0];
/// let src = operands[1];
///
/// // Get the size of each operand
/// println!("Destination size: {}", dst.size);
/// println!("Source size: {}", src.size);
///
/// // Get the type of the destination operand
/// match dst.info {
///     OpInfo::Reg(reg) => {
///         println!("Register kind: {} Size: {} Index: {}", reg.kind, reg.size, reg.index)
///     },
///     // In this case we know that the destination operand is a register
///     _ => println!("Unexpected operand info type"),
/// }
///
/// # Ok(())
/// # }
/// ```
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
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
impl From<ffi::ND_OPERAND> for Operand {
    fn from(op: ffi::ND_OPERAND) -> Operand {
        Operand {
            info: OpInfo::from(op),
            size: OpSize::from(op.Size),
            raw_size: OpSize::from(op.RawSize),
            access: OpAccess::from(op.Access),
            is_default: unsafe { op.Flags.__bindgen_anon_1 }.IsDefault() != 0,
            decorator: Decorator::from(op.Decorator),
        }
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
}
