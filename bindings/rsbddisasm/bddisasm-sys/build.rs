/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
use std::env;
use std::path::PathBuf;

fn main() {
    println!("cargo:rerun-if-changed=csrc");

    cc::Build::new()
        .file("csrc/bddisasm/bddisasm.c")
        .file("csrc/bddisasm/bdformat.c")
        .file("csrc/bddisasm/bdhelpers.c")
        .file("csrc/bddisasm/crt.c")
        .include("csrc/bddisasm/include")
        .include("csrc/inc")
        .define("BDDISASM_HAS_VSNPRINTF", Some("1"))
        .define("BDDISASM_HAS_MEMSET", Some("1"))
        .compile("bddisasm");

    let bindings = bindgen::Builder::default()
        .header("csrc/bddisasm_wrapper.h")
        .allowlist_function("Nd.*")
        .allowlist_type("ND.*")
        .allowlist_var("ND.*")
        .rustified_enum(".*")
        .ctypes_prefix("cty")
        .use_core()
        .impl_debug(true)
        .generate_comments(false)
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");

    println!("cargo:rustc-link-lib=static=bddisasm");
}
