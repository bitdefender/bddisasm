/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Decodes instructions.

use crate::decoded_instruction::{DecodeMode, DecodeResult, DecodedInstruction};

/// Decodes instructions.
#[derive(Clone, Eq, PartialEq, Hash, Debug)]
pub struct Decoder<'a> {
    code: &'a [u8],
    ip: u64,
    mode: DecodeMode,
    offset: usize,
}

impl<'a> Decoder<'a> {
    /// Creates a new decoder.
    ///
    /// # Arguments
    ///
    /// * `code` - An [u8](u8) slice that holds the code to be decoded.
    /// * `mode` - The mode in which to decode the instruction.
    /// * `ip` - The instruction pointer value to use when formatting the decoded instruction. Does not affect the
    /// decoding process in any way.
    pub fn new(code: &'a [u8], mode: DecodeMode, ip: u64) -> Self {
        Self {
            code,
            mode,
            ip,
            offset: 0,
        }
    }

    /// Attempts to decode the next instruction from the given code chunk.
    ///
    /// # Returns
    ///
    /// * `Some(DecodeResult)` - if there are still undecoded bytes in the given code chunk. The decoding may have
    /// still failed. See `Remarks`.
    /// * `None` - if all the bytes in the given code chunk were decoded.
    ///
    /// # Remarks
    ///
    /// This function decodes one instruction from the given code chunk at a time. After each call, the offset inside
    /// the code chunk is advanced by:
    ///
    /// - the size of the decoded instruction, if decoding was succesfull
    /// - 1, if decoding was not succesfull
    ///
    /// The `ip` value specified when the decoder was created is automatically updated:
    ///
    /// - if the decoding was succesfull, it is incremented with the instruction size
    /// - if the decoding was not succesfull, it is incremented with 1
    ///
    /// # Examples
    ///
    /// ```
    /// # use bddisasm::DecodeError;
    /// #
    /// # fn main() -> Result<(), DecodeError> {
    /// use bddisasm::{Decoder, DecodeMode};
    ///
    /// let mut decoder = Decoder::new(&[0x51, 0x53], DecodeMode::Bits32, 0);
    ///
    /// // As long as we have something to decode
    /// while let Some(result) = decoder.decode_next() {
    ///     // Check if the decoding succeeded
    ///     match result {
    ///         Ok(instruction) => println!("{}", instruction),
    ///         Err(e) => println!("Unable to decode: {}", e),
    ///     }
    /// }
    ///
    /// # Ok(())
    /// # }
    /// ```
    pub fn decode_next(&mut self) -> Option<DecodeResult> {
        if self.offset >= self.code.len() {
            None
        } else {
            let result =
                DecodedInstruction::decode_with_ip(&self.code[self.offset..], self.mode, self.ip);
            match result {
                Ok(ins) => {
                    self.offset += ins.length() as usize;
                    self.ip += ins.length() as u64;
                }
                Err(_) => {
                    self.offset += 1;
                    self.ip += 1;
                }
            };

            Some(result)
        }
    }

    /// Attempts to decode the next instruction from the given code chunk.
    ///
    /// Behaves like [`decode_next`](Decoder::decode_next), but in addition to the [`DecodeResult`](DecodeResult) it
    /// will also return the offset from which decoding was attempted, as well as the corresponding instruction pointer.
    ///
    /// # Examples
    ///
    /// ```
    /// # use bddisasm::DecodeError;
    /// #
    /// # fn main() -> Result<(), DecodeError> {
    /// use bddisasm::{Decoder, DecodeMode};
    ///
    /// let mut decoder = Decoder::new(&[0x51, 0x53], DecodeMode::Bits32, 0x1000);
    ///
    /// // As long as we have something to decode
    /// while let Some((result, offset, ip)) = decoder.decode_next_with_info() {
    ///     // Check if the decoding succeeded
    ///     match result {
    ///         Ok(instruction) => println!("IP: {:#x}    {}", ip, instruction),
    ///         Err(e) => println!("Unable to decode at offset {:#x}: {}", offset, e),
    ///     }
    /// }
    ///
    /// # Ok(())
    /// # }
    /// ```
    #[inline]
    pub fn decode_next_with_info(&mut self) -> Option<(DecodeResult, usize, u64)> {
        let offset = self.offset;
        let ip = self.ip;

        self.decode_next().map(|res| (res, offset, ip))
    }

    /// Attempts to decode the next instruction from the given code chunk.
    ///
    /// Behaves like [`decode_next`](Decoder::decode_next), but in addition to the [`DecodeResult`](DecodeResult) it
    /// will also return the offset from which decoding was attempted.
    ///
    /// # Examples
    ///
    /// ```
    /// # use bddisasm::DecodeError;
    /// #
    /// # fn main() -> Result<(), DecodeError> {
    /// use bddisasm::{Decoder, DecodeMode};
    ///
    /// let mut decoder = Decoder::new(&[0x51, 0x53], DecodeMode::Bits32, 0x1000);
    ///
    /// // As long as we have something to decode
    /// while let Some((result, offset)) = decoder.decode_next_with_offset() {
    ///     // Check if the decoding succeeded
    ///     match result {
    ///         Ok(instruction) => println!("{} at offset {:#x}", instruction, offset),
    ///         Err(e) => println!("Unable to decode at offset {:#x}: {}", offset, e),
    ///     }
    /// }
    ///
    /// # Ok(())
    /// # }
    /// ```
    #[inline]
    pub fn decode_next_with_offset(&mut self) -> Option<(DecodeResult, usize)> {
        let offset = self.offset;

        self.decode_next().map(|res| (res, offset))
    }

    /// Attempts to decode the next instruction from the given code chunk.
    ///
    /// Behaves like [`decode_next`](Decoder::decode_next), but in addition to the [`DecodeResult`](DecodeResult) it
    /// will also return the corresponding instruction pointer.
    ///
    /// # Examples
    ///
    /// ```
    /// # use bddisasm::DecodeError;
    /// #
    /// # fn main() -> Result<(), DecodeError> {
    /// use bddisasm::{Decoder, DecodeMode};
    ///
    /// let mut decoder = Decoder::new(&[0x51, 0x53], DecodeMode::Bits32, 0x1000);
    ///
    /// // As long as we have something to decode
    /// while let Some((result, ip)) = decoder.decode_next_with_ip() {
    ///     // Check if the decoding succeeded
    ///     match result {
    ///         Ok(instruction) => println!("{:#x}    {}", ip, instruction),
    ///         Err(e) => println!("Unable to decode: {}", e),
    ///     }
    /// }
    ///
    /// # Ok(())
    /// # }
    /// ```
    #[inline]
    pub fn decode_next_with_ip(&mut self) -> Option<(DecodeResult, u64)> {
        let ip = self.ip;

        self.decode_next().map(|res| (res, ip))
    }
}

impl Iterator for Decoder<'_> {
    type Item = DecodeResult;

    #[inline]
    fn next(&mut self) -> Option<Self::Item> {
        self.decode_next()
    }
}

impl core::iter::FusedIterator for Decoder<'_> {}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::*;

    #[test]
    fn decode_next() {
        let code = vec![0xb8, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8b, 0xf9, 0xff, 0xff];
        let mut decoder = Decoder::new(&code, DecodeMode::Bits64, 0x1000);
        let expected: Vec<Result<(Mnemonic, &str, &[u8]), DecodeError>> = vec![
            Ok((
                Mnemonic::Mov,
                "MOV       eax, 0x00000000",
                &[0xb8, 0x00, 0x00, 0x00, 0x00],
            )),
            Ok((Mnemonic::Mov, "MOV       rdi, rcx", &[0x48, 0x8b, 0xf9])),
            Err(DecodeError::InvalidEncoding),
            Err(DecodeError::BufferTooSmall),
        ];
        let mut exected_index = 0usize;
        while let Some(ins) = decoder.decode_next() {
            match expected[exected_index] {
                Ok((i, s, b)) => {
                    let ins = ins.expect("Unable to decode");
                    assert_eq!(i, ins.mnemonic());
                    assert_eq!(b, ins.bytes());
                    assert_eq!(s, format!("{}", ins));
                }
                Err(e) => {
                    assert_eq!(e, ins.expect_err("Expected error"));
                }
            };

            exected_index += 1;
        }
    }

    #[test]
    fn decoder_iter() {
        let code = vec![0xb8, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8b, 0xf9, 0xff, 0xff];
        let decoder = Decoder::new(&code, DecodeMode::Bits64, 0x1000);
        let expected: Vec<Result<(Mnemonic, &str, &[u8]), DecodeError>> = vec![
            Ok((
                Mnemonic::Mov,
                "MOV       eax, 0x00000000",
                &[0xb8, 0x00, 0x00, 0x00, 0x00],
            )),
            Ok((Mnemonic::Mov, "MOV       rdi, rcx", &[0x48, 0x8b, 0xf9])),
            Err(DecodeError::InvalidEncoding),
            Err(DecodeError::BufferTooSmall),
        ];

        for (index, ins) in decoder.enumerate() {
            match expected[index] {
                Ok((i, s, b)) => {
                    let ins = ins.expect("Unable to decode");
                    assert_eq!(i, ins.mnemonic());
                    assert_eq!(b, ins.bytes());
                    assert_eq!(s, format!("{}", ins));
                }
                Err(e) => {
                    assert_eq!(e, ins.expect_err("Expected error"));
                }
            };
        }
    }
}
