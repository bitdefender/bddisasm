# Changelog

All notable (user-facing) changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).


## [3.0.0] - 2025-07-14

### Added
- Support for minimal decoding - the new minimal decode APIs `NdDecodeMini`, `NdDecodeWithContextMini` decode an instruction and provide only the core information in a new `INSTRUX_MINI` structure, which is only 64-bytes in size.
- Operands are not provided by default in an `INSTRUX_MINI` - in order to decode an operand, the integrator must use `NdGetOperandMini`, which will output a `ND_OPERAND`, which is identical to the legacy format.
- Metadata is not provided by default in an `INSTRUX_MINI` - in order to access metadata such as instruction set, valid prefixes, CPUID info, etc., new API is provided in `bdx86_api_mini.h`.
- All the fields from the legacy `INSTRUX` are either already present inside `INSTRUX_MINI`, or accessible via the API.
- The legacy API continues to function without any change needed.

### Fixed
- Allow `REX2` prefix with `MPX` instruction, and ignore `R4` and `B4` fields.
- Fixed `EVEX.U` handling.

### Changed
- The following instructions are no longer supported: `JMPE Ev`, `JMPE Jz`.
- Improved decoding performance by moving validty checks during the decode tree walk - this way, checks are only performed for those instructions that need them.


## [2.3.0] - 2024-11-07

### Added
- Support for SIMD Exceptions reporting (for instructions that generate SIMD exceptions; this is equivalent to also setting or testing the associated bits in the `MXCSR`).
- Support for the following new x86 ISAs: MOVRS, MSR_IMM, AMX-FP8, AMX-TRANSPOSE, AMX-TF32, AMX-AVX512, AMX-MOVRS and EVEX-encoded SM4 instructions.


## [2.2.0] - 2024-09-16

### Added
- Support in bddisasm for AMD `RMPREAD` instruction.
- Support in bddisasm for Intel AVX 10.2 instructions.

### Fixed
- `LEAVE` instruction: size attribute for the `RSP` operand is stack address size, not operand size.
- `ENTER` instruction: size attribute for the `RSP` operand is operand size, not stack address size. 
- `ENTER` instruction: added missing operands - `RBP` (stack address size), `[RBP]` (operand size).
- `EVEX` instructions in maps 5 & 6 may not be decoded if `ND_FEAT_APX` feature is not enabled.

### Changed
- Proper handling of the `EVEX.U` bit, depending on the instruction type.
- Removed APX encodings for SHA & KEYLOCKER instruction.
- Implemented new test scripts.


## [2.1.5] - 2024-05-28

### Fixed
- Potential unaligned load, as reported by UBSAN.


## [2.1.4] - 2024-03-27

### Changed
- Removed no longer needed static assert.


## [2.1.3] - 2024-03-04

### Changed
- Aligned BDDISASM APX instructions syntax with some of the Intel recomandations (using the suffix notation for `NF` and `ZU` indications, using finite set notation for `DFV` operands).

### Disassembly differences
- The `ZU` indication is appended as a mnemonic sufix, as per recomandations. However, in case of `SETcc` instructions, BDDISASM will append the `ZU` indication AFTER the condition code (similar to `CMPccXADD` and with initial `SETcc.ZU` specification).
- The `DFV` (default flags value) operand obeys the finite set notation, but it is placed as the last operand of the instruction.


## [2.1.2] - 2024-02-27

### Added
- `Read` access for the `rIP` operand for the `SYSCALL` instruction.
- `SCS`, `rCX`, `rDX` operands for the `SYSEXIT` instruction.


## [2.1.1] - 2024-02-26

### Added
- `Read` access for the `rIP` operand for some `CALL` instructions.


## [2.1.0] - 2024-02-20

### Added
- Support in bddisasm for Intel REX2 prefix.
- Support in bddisasm for Intel APX extensions.
- Support in bddisasm for Intel USERMSR extensions.
- Support in bddisasm for prefix activation fields inside `INSTRUX` - these fields can be consulted to determine whether a particular prefix is present, accepted & active.
- New feature in bdshemu: `SHEMU_OPT_DIRECT_MAPPED_SHELL` - allows emulation with a smaller `IntBuff` at the cost of not having `WRITE_SELF` detections. The shellcode can be provided directly from its original location, without the need to allocate it in a dedicated memory region.
- New feature in bdshemu: `SHEMU_OPT_TRACK_LOOPS` - loops can now be tracked by bdshemu. `SHEMU_OPT_TRACE_LOOPS` can be used to log loop information.
- Support in bdshemu for APX instructions (both REX2 and EVEX encoded instructions) - the new `SHEMU_OPT_SUPPORT_APX` must be set in order to enable APX emulation.

### Changed
- Reduced the size of the `INSTRUX` structure from 856 bytes to 488 bytes (almost -43%!).
- Increased decoding performance from average 300 clocks/instruction to average 235 clocks/instruction (almost +20%!).
- New decode options - do not decode implicit operands - this further increases performance from average 235 clocks/instruction to 200 clocks/instruction (almost +15%!).
- Re-worked the Python scripts - both `disasmlib.py` and `generate_tables.py` have been significantly reworked, improving readability, and making them more efficient.
- `disasmtool` builds on Linux.

### Removed
- Support for Cyrix & VIA instructions - only current Intel & AMD instructions remain supported.
- disasmtool_lix has been removed. `disasmtool` is available on Linux as well.

### Breaking changes

#### Inside INSTRUX
- Removed `Iclass` field - it was aliased over `Instruction` field, which must be used from now on.
- Removed `OperandsEncodingMap` field - one can consult the `Encoding` field in each operand to determine the encoding.
- Removed `ExceptionClass` field - only `ExceptionType` remains, which contains an enum with all the exception types.
- Removed `Predicate` field - only `Condition` remains, which was aliased over `Predicate`.
- Removed `HasImm3`, `Immediate3`, `Imm3Length` and `Imm3Offset` fields, as they were not used/needed.
- Removed `Bhint`, `SseCondition`, `SignDisp` fields, as they were not used.
- Moved `FlagsAccess.RegAccess` outside and renamed it to `RflAccess`, to save more space.
- Switched from `char Mnemonic[32]` to `const char *Mnemonic` - this decreases INSTRUX size by almost 32 bytes, and increases perf.

#### Inside ND_OPERAND
- Removed `RawSize` - in most cases, `Size` and `RawSize` are identical; the only case where they might differ is for `Immediate` and `RelativeOffset` operands - in that case, one can consult the `RawSize` field in `Immediate` or `RelativeOffset`.

#### Inside ND_OPERAND_DECORATOR
- Removed `Broadcast` field, moved it inside `ND_OPDESC_MEMORY`.
- Removed `HasSae`, `HasEr` - they are per instruction, not per operand, and can be consulted directly inside `INSTRUX`.
- Moved `Msk` one level up, inside the `ND_OPERAND_DECORATOR` structure.

#### Defines & constants
- Removed `ND_PRED_*` defines - search & replace them with `ND_COND_*`.
- Removed `ND_HAS_PREDICATE` - use `ND_HAS_CONDITION` instead.
- Removed `ND_VEND_GEODE` and `ND_VEND_CYRIX`.
