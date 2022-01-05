/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! CPUID information used to check if an instruction is supported on a CPU or not.
//!
//! # Examples
//!
//! ```
//! # use bddisasm::DecodeError;
//! #
//! # fn test() -> Option<()> {
//! use bddisasm::{DecodedInstruction, DecodeMode};
//!
//! // `ENCLS`
//! let ins = DecodedInstruction::decode(&[0x0f, 0x01, 0xcf], DecodeMode::Bits64).ok()?;
//! let cpuid = ins.cpuid()?;
//! println!("Leaf: {}", cpuid.leaf);
//! match cpuid.sub_leaf {
//!     Some(sub_leaf) => println!("Sub-leaf: {}", sub_leaf),
//!     None => println!("Sub-leaf: None"),
//! }
//! println!("Register to check: {} Bit: {}", cpuid.register, cpuid.bit);
//!
//! # Some(())
//! # }

use core::fmt;

/// Describes the CPUID leaf, sub-leaf, register & bit that indicate whether an instruction is supported or not.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
pub struct Cpuid {
    /// CPUID leaf.
    pub leaf: u32,

    /// CPUID sub-leaf.
    pub sub_leaf: Option<u32>,

    /// The register that contains information regarding the instruction.
    pub register: u8,

    /// Bit inside the register that indicates whether the instruction is present.
    pub bit: u64,
}

impl fmt::Display for Cpuid {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self.sub_leaf {
            Some(sub_leaf) => write!(
                f,
                "leaf: {:#x} sub-leaf: {:#x} register: {} bit to test: {:#x}",
                self.leaf, sub_leaf, self.register, self.bit
            ),
            None => write!(
                f,
                "leaf: {:#x} sub-leaf: - register: {} bit to test: {:#x}",
                self.leaf, self.register, self.bit
            ),
        }
    }
}
