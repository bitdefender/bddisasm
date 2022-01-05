/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//! Rust bindings for [bddisasm](https://github.com/bitdefender/bddisasm).
//!
//! See [bddisasm](https://crates.io/crates/bddisasm) if you're looking for a Rust wrapper for these bindings.
//!
//! # Requirements
//!
//! [bindgen](https://crates.io/crates/bindgen) is used to generate the bindings at build time. Because of this, users
//! need to have `clang` installed.
//! Check the [bindgen documentation](https://rust-lang.github.io/rust-bindgen/requirements.html) for more information.
#![cfg_attr(not(test), no_std)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
#![allow(deref_nullptr)] // See https://github.com/rust-lang/rust-bindgen/issues/1651
#![allow(clippy::all)]
include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

#[cfg(test)]
mod tests {
    use super::*;
    use ::std::os::raw::c_char;
    use std::mem;

    #[test]
    fn get_version() {
        let mut major: u32 = 0;
        let mut minor: u32 = 0;
        let mut revision: u32 = 0;
        let mut build_date: *mut c_char = std::ptr::null_mut();
        let mut build_time: *mut c_char = std::ptr::null_mut();

        unsafe {
            NdGetVersion(
                &mut major,
                &mut minor,
                &mut revision,
                &mut build_date,
                &mut build_time,
            );
        }

        println!("major: {} minor: {} rev: {}", major, minor, revision);

        let build_date = unsafe { std::ffi::CStr::from_ptr(build_date) };
        let build_date = build_date.to_str().unwrap().to_string();
        let build_time = unsafe { std::ffi::CStr::from_ptr(build_time) };
        let build_time = build_time.to_str().unwrap().to_string();
        println!("Build date: {} build time: {}", build_date, build_time);

        // There are no other asserts in this test. Enforcing a known minor version is not worth it, we mainly want to
        // see that `NdGetVersion` works.
        assert_eq!(major, 1);
    }

    fn do_decode(code: &[u8]) -> (INSTRUX, NDSTATUS) {
        let mut instrux: mem::MaybeUninit<INSTRUX> = mem::MaybeUninit::uninit();
        let instrux = instrux.as_mut_ptr();

        let status = unsafe {
            NdDecodeEx(
                instrux,
                code.as_ptr(),
                code.len() as u64,
                ND_CODE_32 as u8,
                ND_DATA_32 as u8,
            )
        };

        (unsafe { *instrux }, status)
    }

    #[test]
    fn decode() {
        let code = vec![0x90];
        let (instrux, status) = do_decode(&code);

        assert_eq!(status, 0, "Failed to decode instruction {:#x?}", code);
        assert_eq!(
            unsafe { instrux.__bindgen_anon_2.Instruction },
            _ND_INS_CLASS::ND_INS_NOP
        );
    }

    #[test]
    fn format() {
        let code = vec![0x89, 0x29];
        let (instrux, status) = do_decode(&code);

        assert_eq!(status, 0, "Failed to decode instruction {:#x?}", code);

        let mut buffer: [i8; ND_MIN_BUF_SIZE as usize] = [0; ND_MIN_BUF_SIZE as usize];
        let status = unsafe { NdToText(&instrux, 0, ND_MIN_BUF_SIZE, buffer.as_mut_ptr()) };
        assert_eq!(status, 0, "Failed to decode format {:#x?}", code);

        let text = String::from_utf8(buffer.iter().map(|&c| c as u8).collect()).unwrap();
        let text = text.trim_matches(char::from(0));
        assert_eq!(text, "MOV       dword ptr [ecx], ebp");
    }
}
