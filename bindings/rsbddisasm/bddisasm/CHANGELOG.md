# bddisasm changelog

## 0.3.1

### Added

- support for new Intel ISA, per Intel Architecture Instruction Set Extensions and Future Features document #319433-049 (June 2023): AVX-NNI-INT16, SHA512, SM3, SM4, TSE.

### Changed

- `Mnemonic`, `IsaSet`, and `Category` use all caps for the enum variants

## 0.3.0

### Fixed

- #84: handle 0 sizes in `OpSize::from_raw`

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
