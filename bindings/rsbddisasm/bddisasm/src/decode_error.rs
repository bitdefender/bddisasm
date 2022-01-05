/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Errors that can be encountered when decoding an instruction or when trying to get details about a decoded
//! instruction.
//!
//! # Notes
//!
//! All error codes that can be returned by `bddisasm-sys` are encapsulated in the [`DecodeError`](DecodeError) enum.
//! However, some of these are unlikely to be encountered when using this crate (for example,
//! [`BufferOverflow`](DecodeError::BufferOverflow)) which indicates that a buffer passed to the `bddisasm` C library is
//! not large enough.

use core::fmt;

/// Holds all the possible errors that can be encountered by the decoder.
///
/// # Notes
///
/// If the `std` feature is disabled, [`DecodeError`] does not implement the `Error` trait.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub enum DecodeError {
    /// The provided input buffer is too small and does not contain a valid instruction.
    BufferTooSmall,

    /// Invalid encoding/instruction.
    InvalidEncoding,

    /// Instruction exceeds the maximum 15 bytes.
    InstructionTooLong,

    /// Invalid prefix sequence is present.
    InvalidPrefixSequence,

    /// The instruction uses an invalid register.
    InvalidRegisterInInstruction,

    /// XOP is present, but also a legacy prefix.
    XopWithPrefix,

    /// VEX is present, but also a legacy prefix.
    VexWithPrefix,

    /// EVEX is present, but also a legacy prefix.
    EvexWithPrefix,

    /// Invalid encoding/instruction.
    InvalidEncodingInMode,

    /// Invalid usage of LOCK.
    BadLockPrefix,

    /// An attempt to load the CS register.
    CsLoad,

    /// 0x66 prefix is not accepted.
    Prefix66NotAccepted,

    /// 16 bit addressing mode not supported.
    AddressingNotSupported16Bit,

    /// RIP-relative addressing not supported.
    RipRelAddressingNotSupported,

    /// Instruction uses VSIB, but SIB is not present.
    VsibWithoutSib,

    /// VSIB addressing, same vector reg used more than once.
    InvalidVsibRegs,

    /// VEX.VVVV field must be zero.
    VexVvvvMustBeZero,

    /// Masking is not supported.
    MaskNotSupported,

    /// Masking is mandatory.
    MaskRequired,

    /// Embedded rounding/SAE not supported.
    ErSaeNotSupported,

    /// Zeroing not supported.
    ZeroingNotSupported,

    /// Zeroing on memory.
    ZeroingOnMemory,

    /// Zeroing without masking.
    ZeroingNoMask,

    /// Broadcast not supported.
    BroadcastNotSupported,

    /// EVEX.V' field must be one (negated 0).
    BadEvexVPrime,

    /// EVEX.L'L field is invalid for the instruction.
    BadEvexLl,

    /// Instruction uses SIBMEM, but SIB is not present.
    SibmemWithoutSib,

    /// Tile registers are not unique.
    InvalidTileRegs,

    /// Destination register is not unique (used as src).
    InvalidDestRegs,

    /// An invalid parameter was provided.
    InvalidParameter,

    /// The INSTRUX contains unexpected values.
    InvalidInstrux,

    /// Not enough space is available.
    BufferOverflow,

    /// Internal library error.
    InternalError(u64),
}

impl fmt::Display for DecodeError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            DecodeError::BufferTooSmall => write!(f, "the provided input buffer is too small"),
            DecodeError::InvalidEncoding => write!(f, "invalid encoding/instruction"),
            DecodeError::InstructionTooLong => {
                write!(f, "instruction exceeds the maximum 15 bytes")
            }
            DecodeError::InvalidPrefixSequence => write!(f, "invalid prefix sequence is present"),
            DecodeError::InvalidRegisterInInstruction => {
                write!(f, "the instruction uses an invalid register")
            }
            DecodeError::XopWithPrefix => write!(f, "XOP is present, but also a legacy prefix"),
            DecodeError::VexWithPrefix => write!(f, "VEX is present, but also a legacy prefix"),
            DecodeError::EvexWithPrefix => write!(f, "EVEX is present, but also a legacy prefix"),
            DecodeError::InvalidEncodingInMode => {
                write!(f, "invalid encoding/instruction in the given mode")
            }
            DecodeError::BadLockPrefix => write!(f, "invalid usage of LOCK"),
            DecodeError::CsLoad => write!(f, "an attempt to load the CS register"),
            DecodeError::Prefix66NotAccepted => write!(f, "0x66 prefix is not accepted"),
            DecodeError::AddressingNotSupported16Bit => {
                write!(f, "16 bit addressing mode not supported")
            }
            DecodeError::RipRelAddressingNotSupported => {
                write!(f, "RIP-relative addressing not supported")
            }
            DecodeError::VsibWithoutSib => {
                write!(f, "instruction uses VSIB, but SIB is not present")
            }
            DecodeError::InvalidVsibRegs => {
                write!(
                    f,
                    "VSIB addressing with the same vector register used more than once"
                )
            }
            DecodeError::VexVvvvMustBeZero => write!(f, "VEX.VVVV field must be zero"),
            DecodeError::MaskNotSupported => write!(f, "masking is not supported"),
            DecodeError::MaskRequired => write!(f, "masking is mandatory"),
            DecodeError::ErSaeNotSupported => write!(f, "embedded rounding/SAE not supported"),
            DecodeError::ZeroingNotSupported => write!(f, "zeroing not supported"),
            DecodeError::ZeroingOnMemory => write!(f, "zeroing on memory"),
            DecodeError::ZeroingNoMask => write!(f, "zeroing without masking"),
            DecodeError::BroadcastNotSupported => write!(f, "broadcast not supported"),
            DecodeError::BadEvexVPrime => write!(f, "EVEX.V' field must be one (negated 0)"),
            DecodeError::BadEvexLl => write!(f, "EVEX.L'L field is invalid for the instruction"),
            DecodeError::SibmemWithoutSib => {
                write!(f, "instruction uses SIBMEM, but SIB is not present")
            }
            DecodeError::InvalidTileRegs => write!(f, "tile registers are not unique"),
            DecodeError::InvalidDestRegs => {
                write!(f, "destination register is not unique (used as src)")
            }
            DecodeError::InvalidParameter => write!(f, "an invalid parameter was provided"),
            DecodeError::InvalidInstrux => {
                write!(f, "the INSTRUX structure contains unexpected values")
            }
            DecodeError::BufferOverflow => {
                write!(f, "not enough space is available to format instruction")
            }
            DecodeError::InternalError(e) => write!(f, "internal error: {}", e),
        }
    }
}

#[cfg(feature = "std")]
impl std::error::Error for DecodeError {}

pub(crate) fn status_to_error(status: ffi::NDSTATUS) -> Result<(), DecodeError> {
    if status == ffi::ND_STATUS_SUCCESS || status == ffi::ND_STATUS_HINT_OPERAND_NOT_USED {
        Ok(())
    } else {
        match status {
            ffi::ND_STATUS_BUFFER_TOO_SMALL => Err(DecodeError::BufferTooSmall),
            ffi::ND_STATUS_INVALID_ENCODING => Err(DecodeError::InvalidEncoding),
            ffi::ND_STATUS_INSTRUCTION_TOO_LONG => Err(DecodeError::InstructionTooLong),
            ffi::ND_STATUS_INVALID_PREFIX_SEQUENCE => Err(DecodeError::InvalidPrefixSequence),
            ffi::ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION => {
                Err(DecodeError::InvalidRegisterInInstruction)
            }
            ffi::ND_STATUS_XOP_WITH_PREFIX => Err(DecodeError::XopWithPrefix),
            ffi::ND_STATUS_VEX_WITH_PREFIX => Err(DecodeError::VexWithPrefix),
            ffi::ND_STATUS_EVEX_WITH_PREFIX => Err(DecodeError::EvexWithPrefix),
            ffi::ND_STATUS_INVALID_ENCODING_IN_MODE => Err(DecodeError::InvalidEncodingInMode),
            ffi::ND_STATUS_BAD_LOCK_PREFIX => Err(DecodeError::BadLockPrefix),
            ffi::ND_STATUS_CS_LOAD => Err(DecodeError::CsLoad),
            ffi::ND_STATUS_66_NOT_ACCEPTED => Err(DecodeError::Prefix66NotAccepted),
            ffi::ND_STATUS_16_BIT_ADDRESSING_NOT_SUPPORTED => {
                Err(DecodeError::AddressingNotSupported16Bit)
            }
            ffi::ND_STATUS_RIP_REL_ADDRESSING_NOT_SUPPORTED => {
                Err(DecodeError::RipRelAddressingNotSupported)
            }
            ffi::ND_STATUS_VSIB_WITHOUT_SIB => Err(DecodeError::VsibWithoutSib),
            ffi::ND_STATUS_INVALID_VSIB_REGS => Err(DecodeError::InvalidVsibRegs),
            ffi::ND_STATUS_VEX_VVVV_MUST_BE_ZERO => Err(DecodeError::VexVvvvMustBeZero),
            ffi::ND_STATUS_MASK_NOT_SUPPORTED => Err(DecodeError::MaskNotSupported),
            ffi::ND_STATUS_MASK_REQUIRED => Err(DecodeError::MaskRequired),
            ffi::ND_STATUS_ER_SAE_NOT_SUPPORTED => Err(DecodeError::ErSaeNotSupported),
            ffi::ND_STATUS_ZEROING_NOT_SUPPORTED => Err(DecodeError::ZeroingNotSupported),
            ffi::ND_STATUS_ZEROING_ON_MEMORY => Err(DecodeError::ZeroingOnMemory),
            ffi::ND_STATUS_ZEROING_NO_MASK => Err(DecodeError::ZeroingNoMask),
            ffi::ND_STATUS_BROADCAST_NOT_SUPPORTED => Err(DecodeError::BroadcastNotSupported),
            ffi::ND_STATUS_BAD_EVEX_V_PRIME => Err(DecodeError::BadEvexVPrime),
            ffi::ND_STATUS_BAD_EVEX_LL => Err(DecodeError::BadEvexLl),
            ffi::ND_STATUS_SIBMEM_WITHOUT_SIB => Err(DecodeError::SibmemWithoutSib),
            ffi::ND_STATUS_INVALID_TILE_REGS => Err(DecodeError::InvalidTileRegs),
            ffi::ND_STATUS_INVALID_DEST_REGS => Err(DecodeError::InvalidDestRegs),
            ffi::ND_STATUS_INVALID_PARAMETER => Err(DecodeError::InvalidParameter),
            ffi::ND_STATUS_INVALID_INSTRUX => Err(DecodeError::InvalidInstrux),
            ffi::ND_STATUS_BUFFER_OVERFLOW => Err(DecodeError::BufferOverflow),
            ffi::ND_STATUS_INTERNAL_ERROR => Err(DecodeError::InternalError(0)),
            _ => panic!("Unexpected status: {:#x}", status),
        }
    }
}
