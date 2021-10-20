# bddisasm-sys

Rust bindings for [bddisasm](https://github.com/bitdefender/bddisasm).

See [bddisasm](https://crates.io/crates/bddisasm) if you're looking for a Rust wrapper for these bindings.

## Requirements

[bindgen](https://crates.io/crates/bindgen) is used to generate the bindings at build time. Because of this, users need to have `clang` installed. Check the [bindgen documentation](https://rust-lang.github.io/rust-bindgen/requirements.html) for more information.
