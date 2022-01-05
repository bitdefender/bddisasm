extern crate bddisasm;

use std::fmt;

use bddisasm::{DecodeMode, DecodedInstruction, Decoder, Mnemonic, OpRegType, Operand};

use anyhow::Result;
use clap::{App, Arg};

static ABOUT: &str = "
Simple emulator example.
Supports only a few 64-bit instructions (MOV, INC, DEC) and operand types (register, and immediate).
All registers are initially set to 0.
";

/// Holds the reigster state.
#[derive(Debug, Default)]
struct Context {
    /// Registers are in the Intel documented order (RAX, RCX, RDX, etc).
    regs: [u64; 16],
}

impl Context {
    /// Emulates a 64-bit instruction.
    pub fn emulate(&mut self, ins: &DecodedInstruction) -> Result<()> {
        let operands = ins.operands();

        match ins.mnemonic() {
            Mnemonic::Mov => {
                self.set_operand_value(&operands[0], self.get_operand_value(&operands[1])?)?
            }
            Mnemonic::Inc => self.set_operand_value(
                &operands[0],
                self.get_operand_value(&operands[0])?.wrapping_add(1),
            )?,
            Mnemonic::Dec => self.set_operand_value(
                &operands[0],
                self.get_operand_value(&operands[0])?.wrapping_sub(1),
            )?,
            _ => anyhow::bail!("Unsupported instruction: {}", ins),
        }

        Ok(())
    }

    fn get_operand_value(&self, op: &Operand) -> Result<u64> {
        if let Some(reg) = op.info.as_reg() {
            if reg.kind != OpRegType::Gpr {
                anyhow::bail!("Unsupported register type: {}", reg.kind)
            } else {
                match reg.size {
                    1 => {
                        if reg.is_high8 {
                            // For `AH`, `BH`, etc.
                            Ok((self.regs[reg.index] & 0xFF00) >> 8)
                        } else {
                            Ok(self.regs[reg.index] & 0xFF)
                        }
                    }
                    2 => Ok(self.regs[reg.index] & 0xFFFF),
                    4 => Ok(self.regs[reg.index] & 0xFFFFFFFF),
                    8 => Ok(self.regs[reg.index]),
                    _ => unreachable!("Unexpected GPR size: {}", reg.size),
                }
            }
        } else if let Some(imm) = op.info.as_imm() {
            Ok(imm)
        } else {
            anyhow::bail!("Unsupported operand type: {}", op.info)
        }
    }

    fn set_operand_value(&mut self, op: &Operand, value: u64) -> Result<()> {
        if let Some(reg) = op.info.as_reg() {
            if reg.kind != OpRegType::Gpr {
                anyhow::bail!("Unsupported register type: {}", reg.kind)
            } else {
                let orig_value = self.regs[reg.index];

                let value = match reg.size {
                    1 => {
                        if reg.is_high8 {
                            let value = value << 8;
                            value | (orig_value & 0xFFFFFFFFFFFF00FF)
                        } else {
                            value | (orig_value & 0xFFFFFFFFFFFFFF00)
                        }
                    }
                    2 => value | (orig_value & 0xFFFFFFFFFFFF0000),
                    4 => {
                        // The upper 32-bits are always set to 0.
                        value & 0xFFFFFFFF
                    }
                    8 => value,
                    _ => unreachable!("Unexpected GPR size: {}", reg.size),
                };

                self.regs[reg.index] = value;
                Ok(())
            }
        } else {
            anyhow::bail!("Unsupported operand type: {}", op.info)
        }
    }
}

impl fmt::Display for Context {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let names = [
            "RAX", "RCX", "RDX", "RBX", "RSP", "RBP", "RSI", "RDI", " R8", " R9", "R10", "R11",
            "R12", "R13", "R14", "R15",
        ];

        for (i, name) in names.iter().enumerate() {
            if let Err(e) = write!(f, "{}: {:#018x} ", name, self.regs[i]) {
                return Err(e);
            }

            if (i + 1) % 4 == 0 {
                if let Err(e) = writeln!(f) {
                    return Err(e);
                }
            }
        }

        Ok(())
    }
}

fn main() -> Result<()> {
    let matches = App::new("emulator")
        .about(ABOUT)
        .arg(
            Arg::with_name("INPUT")
                .short("f")
                .long("file")
                .value_name("PATH")
                .help("Path to the binary file from which to load code")
                .takes_value(true),
        )
        .get_matches();

    let code = std::fs::read(matches.value_of("INPUT").unwrap())?;

    let decoder = Decoder::new(&code, DecodeMode::Bits64, 0);
    let mut ctx = Context::default();

    for ins in decoder {
        match ins {
            Ok(ins) => {
                ctx.emulate(&ins)?;
                println!("{}\n{}", ins, ctx);
            }
            Err(e) => anyhow::bail!("Failed to decode: {}", e),
        }
    }

    Ok(())
}
