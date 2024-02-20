import os
import subprocess
import sys
from pathlib import Path

from git import Repo

repo = Repo(search_parent_directories=True)
root = Path(repo.working_tree_dir)

print(f"Repo root: {root}")

hdr_dirs = root / "inc"
x86_constants = hdr_dirs / "bdx86_constants.h"

print(f"Looking for enums in {x86_constants}")

with open(x86_constants, "r") as data:
    nd_ins = []
    nd_ins_set = []
    nd_ins_type = []

    for line in data:
        if "INVALID" in line:
            continue

        if line.startswith("    ND_CAT_"):
            token = line.replace(',', '').strip()
            nd_ins_type.append(token)
            continue

        if line.startswith("    ND_SET_"):
            token = line.replace(',', '').strip()
            nd_ins_set.append(token)
            continue

        if line.startswith("    ND_INS_"):
            token = line.replace(',', '').strip()
            nd_ins.append(token)
            continue

print(f"ND_INS_: {len(nd_ins)} ND_SET_: {len(nd_ins_set)} ND_CAT_: {len(nd_ins_type)}")

bindings = root / "bindings/rsbddisasm/bddisasm/src"
mnemonic = bindings / "mnemonic.rs"
ins_cat = bindings / "instruction_category.rs"
isa_set = bindings / "isa_set.rs"

license_header = """
/*
 * Copyright (c) 2021 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
"""

print(f"Generating {mnemonic}")

with open(mnemonic, "w") as fmnemonic:
    fmnemonic.write(license_header)
    fmnemonic.write("//! Mnemonics.\n")
    fmnemonic.write("\n")
    fmnemonic.write("use super::decode_error::DecodeError;\n")
    fmnemonic.write("use core::convert::TryFrom;\n")
    
    mnemonic_enum_def = """
/// Uniquely identifies an instruction.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
#[allow(non_camel_case_types)]
pub enum Mnemonic {
    """
    fmnemonic.write(mnemonic_enum_def)
    for ins in nd_ins:
        line = ins.replace("ND_INS_", "")
        fmnemonic.write(f"{line},\n")

    fmnemonic.write("}\n")

    try_from = """
#[doc(hidden)]
impl TryFrom<ffi::ND_INS_CLASS> for Mnemonic {
    type Error = DecodeError;

    #[allow(clippy::too_many_lines)]
    fn try_from(value: ffi::ND_INS_CLASS) -> Result<Self, Self::Error> {
        match value {
            ffi::_ND_INS_CLASS::ND_INS_INVALID => Err(DecodeError::InternalError(value as u64)),
    """
    fmnemonic.write(try_from)
    for ins in nd_ins:
        line = f"ffi::_ND_INS_CLASS::{ins} => Ok(Mnemonic::{ins.replace('ND_INS_', '')}),\n"
        fmnemonic.write(line)

    end = """
        }
    }
}
    """
    fmnemonic.write(end)

print(f"Generating {ins_cat}")

with open(ins_cat, "w") as finscat:
    finscat.write(license_header)
    finscat.write("//! Instruction categories.\n")
    finscat.write("\n")
    finscat.write("use super::decode_error::DecodeError;\n")
    finscat.write("use core::convert::TryFrom;\n")
    
    ins_cat_enum_def = """
/// Instruction category.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
#[allow(non_camel_case_types)]
pub enum Category {
    """
    finscat.write(ins_cat_enum_def)
    for ins in nd_ins_type:
        line = ins.replace("ND_CAT_", "")
        if line[0].isdigit():
            line = f"I{line}"
        finscat.write(f"{line},\n")

    finscat.write("}\n")

    try_from = """
#[doc(hidden)]
impl TryFrom<ffi::ND_INS_CATEGORY> for Category {
    type Error = DecodeError;

    #[allow(clippy::too_many_lines)]
    fn try_from(value: ffi::ND_INS_CATEGORY) -> Result<Self, Self::Error> {
        match value {
            ffi::_ND_INS_TYPE::ND_CAT_INVALID => Err(DecodeError::InternalError(value as u64)),
    """
    finscat.write(try_from)
    for ins in nd_ins_type:
        ok = ins.replace('ND_CAT_', '')
        if ok[0].isdigit():
            ok = f"I{ok}"
        line = f"ffi::_ND_INS_TYPE::{ins} => Ok(Category::{ok}),\n"
        finscat.write(line)

    end = """
        }
    }
}
    """
    finscat.write(end)

print(f"Generating {isa_set}")

with open(isa_set, "w") as fset:
    fset.write(license_header)
    fset.write("//! Instruction sets.\n")
    fset.write("\n")
    fset.write("use super::decode_error::DecodeError;\n")
    fset.write("use core::convert::TryFrom;\n")
    
    ins_set_enum_def = """
/// ISA set.
#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
#[allow(non_camel_case_types)]
pub enum IsaSet {
    """
    fset.write(ins_set_enum_def)
    for ins in nd_ins_set:
        line = ins.replace("ND_SET_", "")
        if line[0].isdigit():
            line = f"I{line}"
        fset.write(f"{line},\n")

    fset.write("}\n")

    try_from = """
#[doc(hidden)]
impl TryFrom<ffi::ND_INS_SET> for IsaSet {
    type Error = DecodeError;

    #[allow(clippy::too_many_lines)]
    fn try_from(value: ffi::ND_INS_SET) -> Result<Self, Self::Error> {
        match value {
            ffi::_ND_INS_SET::ND_SET_INVALID => Err(DecodeError::InternalError(value as u64)),
    """
    fset.write(try_from)
    for ins in nd_ins_set:
        ok = ins.replace('ND_SET_', '')
        if ok[0].isdigit():
            ok = f"I{ok}"
        line = f"ffi::_ND_INS_SET::{ins} => Ok(IsaSet::{ok}),\n"
        fset.write(line)

    end = """
        }
    }
}
    """
    fset.write(end)

subprocess.check_output(["cargo", "fmt"])
