/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <ctime>
#include <cstdint>

#include <rapidjson/stringbuffer.h>
using StringBuffer = rapidjson::StringBuffer;


extern "C"
{

    // On windows, simply include Windows.h
#ifndef _WIN32
typedef void VOID, *PVOID;
typedef unsigned char BOOLEAN, *PBOOLEAN;
typedef char CHAR, *PCHAR;
typedef unsigned char BYTE, *PBYTE;
typedef unsigned short WORD, *PWORD;
typedef unsigned int DWORD, *PDWORD;
typedef unsigned long long QWORD, *PQWORD;
typedef size_t SIZE_T;
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef MAX_PATH
#define MAX_PATH  255
#endif

// Main disasm header file.
#include "bdshemu.h"
#include "bddisasm.h"

#include <unistd.h>
#include <time.h>
}


#include <string>


std::string enc_mode_to_str(const uint8_t enc_mode);
std::string op_type_to_str(const ND_OPERAND_TYPE type);
std::string op_enc_to_str(const ND_OPERAND_ENCODING Encoding);
std::string ins_class_to_str(const ND_INS_CLASS cls);
std::string ins_cat_to_str(ND_INS_CATEGORY category);
std::string ins_set_to_str(ND_INS_SET ins_set);
std::string reg_to_str(const int reg, const ND_REG_TYPE type);
std::string reg_type_to_str(const ND_REG_TYPE type);

StringBuffer instrux_to_json(INSTRUX *instrux, size_t rip, bool text_only = false);
StringBuffer byte_to_json(uint8_t byte, size_t rip);
StringBuffer disassemble_one(uint8_t *bytes, size_t size, size_t rip, uint8_t bits, uint8_t vendor = ND_VEND_INTEL);

bool regs_from_json(const std::string &str, SHEMU_GPR_REGS &regs, bool &update_rsp);
