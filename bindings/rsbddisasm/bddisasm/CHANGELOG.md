# bddisasm changelog

## 0.3.1

### Removed

- the `ExceptionClass` enum
- `sse_cond`, `has_mvex`, `has_drex`, `has_imm3`, `immediate3`, `sign_disp`, `imm3_length`, `imm3_offset`, `bhint`  from `DecodedInstruction`
- multiple variants from `Category`, `IsaSet`, `Mnemonic` in accordance with the changes in `bddisasm`
- `raw_size`, `decorator` from `Operand`
- the `Operand` struct

## 0.2.1

### Added

- support for new Intel instructions, per Intel ISA extensions document #319433-046 (September 2022): `PREFETCHITI`, `RAO-INT`, `CMPCCXADD`, `WRMSRNS`, `MSRLIST`, `AMX-FP16`, `AVX-IFMA`, `AVX-NE-CONVERT`, `AVX-VNNI-INT8`

## 0.2.0

### Added

- implement `Decoder::decode_next_with_offset`
- implement ``Decoder::decode_next_with_ip`
- `OperandLookup` struct which makes working with operands easier
- re-export `bddisasm_sys` as `ffi`
- re-export commonly used items
- Implement `as_*` and `is_*` accessors for the `OpInfo` enum

### Changed

- the crate is now a `no_std` crate
- public types no longer implement `From` and no longer `panic!` when an unexpected value is encountered
