# bddisasm changelog

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
