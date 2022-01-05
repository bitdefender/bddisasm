# bddisasm-sys

`no_std` Rust bindings for [bddisasm](https://github.com/bitdefender/bddisasm).

See [bddisasm](https://crates.io/crates/bddisasm) if you're looking for a Rust wrapper for these bindings.

## Requirements

[bindgen](https://crates.io/crates/bindgen) is used to generate the bindings at build time. Because of this, users need to have `clang` installed. Check the [bindgen documentation](https://rust-lang.github.io/rust-bindgen/requirements.html) for more information.

## Notes

While this crate is `no_std`, the `bddisasm` library it links against depends on a C library because it needs `vsnprintf` and `memset`. It is possible to [work around this limitation](https://github.com/bitdefender/bddisasm#nd_vsnprintf_s-and-nd_memset), but this is not currently done for these bindings.
