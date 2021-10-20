# bddisasm Rust bindings

## Build

Run `cargo build` or `cargo build --release`.

## Directory structure

### bddisasm-sys

This crate uses the [*-sys crate convention](https://doc.rust-lang.org/cargo/reference/build-scripts.html#-sys-packages) and links with `libbddisasm`, exposing a raw, low-level, interface.

### bddisasm

This crate aims to offer a higher-level interface over [bddisasm-sys](#bddisasm-sys).

Parts of it are auto-generated, with slight manual changes (for example, the `Mnemonic` enum and related functions).

#### TODO

- [ ] encode registers, not just the register index (for example, instead of `Gpr` we should have `Gpr::Rax`, `Gpr::Eax`, etc)
- [ ] more documentation for the `operand` module, with examples on how to work wit h each operand type
- [ ] more examples for `cpuid`
- [ ] more examples for `cpu_modes`
- [ ] an API to check if an instruction is supported with certain CPU modes on or  off (currently a user has to manually check the `CpuModes` structure)
- [ ] implement `Display` for more types (especially those in `operand`)
- [ ] remove the `bindgen` dev dependency? See [this issue](https://github.com/rust-lang/rust-bindgen/issues/918) as to why we may want to do that
- [ ] better directory structure
