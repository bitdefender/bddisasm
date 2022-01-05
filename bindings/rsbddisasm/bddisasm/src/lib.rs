/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! bddisasm x86/x64 instruction decoder
//!
//! `no_std` Rust bindings for the [bddisasm](https://github.com/bitdefender/bddisasm) x86/x64 decoder library, built
//! on top of [bddisasm-sys](https://crates.io/crates/bddisasm-sys).
//!
//! It supports all existing 16-bit, 32-bit and 64-bit instruction, offering a wide range of information about each one,
//! including:
//!
//! - implicit operands
//! - explicit operands
//! - access mode for each operand
//! - CPUID feature flags
//! - CPU modes in which an instruction is valid
//!
//! # Usage
//!
//! Add `bddisasm` to your `Cargo.toml`:
//!
//! ```toml
//! [dependencies]
//! bddisasm = "0.1.0"
//! ```
//!
//! # Examples
//!
//! ## Decoding one instruction
//!
//! Use [`DecodedInstruction::decode`](crate::decoded_instruction::DecodedInstruction::decode) to decode an instruction
//! from a chunk of code.
//!
//! ```
//! use bddisasm::{DecodedInstruction, DecodeMode, Mnemonic};
//!
//! let code = vec![0x31, 0xc0];
//! match DecodedInstruction::decode(&code, DecodeMode::Bits32) {
//!     Ok(ins) => {
//!         assert_eq!(ins.mnemonic(), Mnemonic::Xor);
//!         println!("{}", ins);
//!     },
//!     Err(err) => println!("Unable to decode: {}", err),
//! }
//! ```
//!
//! ## Decoding multiple instructions
//!
//! Use [`Decoder`](crate::decoder::Decoder) to decode multiple instructions from a chunk of code.
//!
//! ```
//! use bddisasm::{Decoder, DecodeMode};
//!
//! let code = [
//!     // ENCLS
//!     0x0f, 0x01, 0xcf,
//!     // MOV       rax, qword ptr [rbx+rcx*4+0x1234]
//!     0x48, 0x8b, 0x84, 0x8b, 0x34, 0x12, 0x00, 0x00,
//!     // Not a valid instruction
//!     0x0f,
//!     // WRMSR
//!     0x0f, 0x30,
//! ];
//! let decoder = Decoder::new(&code, DecodeMode::Bits64, 0x1234);
//!
//! for ins in decoder {
//!     match ins {
//!         Ok(ins) => println!("{}", ins),
//!         Err(e) => println!("{}", e),
//!     }
//! }
//! ```
//!
//! This will print:
//!
//! ```text
//! ENCLS
//! MOV       rax, qword ptr [rbx+rcx*4+0x1234]
//! the provided input buffer is too small
//! WRMSR
//! ```
//!
//! Use [`Decoder::decode_next_with_info`](crate::decoder::Decoder::decode_next_with_info) to get information about the
//! offset inside the code chunk at which an instruction was decoded from.
//!
//! ```
//! use bddisasm::{Decoder, DecodeMode};
//!
//! let code = [
//!     // ENCLS
//!     0x0f, 0x01, 0xcf,
//!     // MOV       rax, qword ptr [rbx+rcx*4+0x1234]
//!     0x48, 0x8b, 0x84, 0x8b, 0x34, 0x12, 0x00, 0x00,
//!     // Not a valid instruction
//!     0x0f,
//!     // WRMSR
//!     0x0f, 0x30,
//! ];
//! let mut decoder = Decoder::new(&code, DecodeMode::Bits64, 0x1234);
//!
//!
//! // Keep decoding until there's nothing left to decode
//! while let Some((result, offset, _)) = decoder.decode_next_with_info() {
//!     match result {
//!         Ok(ins) => println!("{:#x} {}", offset, ins),
//!         Err(e) => println!("Error: `{}` at offset {:#x}", e, offset),
//!     }
//! }
//! ```
//!
//! This will print:
//!
//! ```text
//! 0x0 ENCLS
//! 0x3 MOV       rax, qword ptr [rbx+rcx*4+0x1234]
//! Error `the provided input buffer is too small` at offset 0xb
//! 0xc WRMSR
//! ```
//!
//! ## Working with instruction operands
//!
//! Instruction operands can be analyzed using the [operand](crate::operand) module. Rich informaion is offered for
//! each type of operand. Bellow is a minimal example that looks at a memory operand.
//!
//! ```
//! # use bddisasm::DecodeError;
//! # fn test() -> Result<(), DecodeError> {
//! use bddisasm::{DecodedInstruction, DecodeMode, OpInfo};
//!
//! // ` MOV       rax, qword ptr [rcx+r15*2]`
//! let code = b"\x4a\x8b\x04\x79";
//! let ins = DecodedInstruction::decode(code, DecodeMode::Bits64).unwrap();
//! // Get the operands
//! let operands = ins.operands();
//! // Get the second operand which is the source (`[rcx+r15*2]`)
//! let src = operands[1];
//!
//! println!("Source operand type: {}", src.info);
//! match src.info {
//!     OpInfo::Mem(mem) => {
//!         if let Some(base) = mem.base {
//!             println!("Base register: {}", base);
//!         } else {
//!             println!("No base register");
//!         }
//!
//!         if let Some(index) = mem.index {
//!             println!("Index register: {}", index);
//!         } else {
//!             println!("No index register");
//!         }
//!
//!         if let Some(scale) = mem.scale {
//!             println!("Scale: {}", scale);
//!         } else {
//!             println!("No scale");
//!         }
//!
//!         if let Some(displacement) = mem.disp {
//!             println!("Displacement: {}", displacement);
//!         } else {
//!             println!("No displacement");
//!         }
//!     },
//!     _ => unreachable!(),
//! }
//! # Ok(())
//! # }
//! ```
//!
//! Will print:
//!
//! ```text
//! Source operand type: memory
//! Base register: 1
//! Index register: 15
//! Scale: 2
//! No displacement
//! ```
//!
//! ## Accessing the raw bindings
//!
//! The raw `bddisasm_sys` bindings are available via the `ffi` re-export.
//!
//! # Feature Flags
//!
//! - `std` - adds a `std` dependency - the only visible difference when doing this is that [`DecodeError`] implements
//! the `Error` trait
//!

#![cfg_attr(all(not(test), not(feature = "std")), no_std)]

pub extern crate bddisasm_sys as ffi;

pub mod cpu_modes;
pub mod cpuid;
pub mod decode_error;
pub mod decoded_instruction;
pub mod decoder;
pub mod fpu_flags;
pub mod instruction_category;
pub mod isa_set;
pub mod mnemonic;
pub mod operand;
pub mod rflags;
pub mod tuple;

pub use crate::decode_error::DecodeError;
pub use crate::decoded_instruction::{DecodeMode, DecodeResult, DecodedInstruction, OperandSize};
pub use crate::decoder::Decoder;
pub use crate::instruction_category::Category;
pub use crate::isa_set::IsaSet;
pub use crate::mnemonic::Mnemonic;
pub use crate::operand::{
    OpAddr, OpInfo, OpMem, OpReg, OpRegType, OpSize, Operand, OperandsLookup,
};
