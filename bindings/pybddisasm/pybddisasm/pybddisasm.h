/*
 * Copyright (c) 2023 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PYBDDISASM_H_
#define _PYBDDISASM_H_

#include "bddisasm.h"

void nd_get_version(ND_UINT32 *major, ND_UINT32 *minor, ND_UINT32 *revision, const char **build_date, const char **build_time);
NDSTATUS nd_decode(INSTRUX *__output_instr, const ND_UINT8 *code, ND_UINT8 def_code, ND_UINT8 def_data);
NDSTATUS nd_decode_ex(INSTRUX *__output_instr, const ND_UINT8 *code, ND_SIZET size, ND_UINT8 def_code, ND_UINT8 def_data);
NDSTATUS nd_decode_ex2(INSTRUX *__output_instr, const ND_UINT8 *code, ND_SIZET size, ND_UINT8 def_code, ND_UINT8 def_data,
                       ND_UINT8 def_stack, ND_UINT8 prefered_vendor);
NDSTATUS nd_decode_with_context(INSTRUX *__output_instr, const ND_UINT8 *code, ND_SIZET size, ND_CONTEXT *context);
NDSTATUS nd_to_text(const INSTRUX *__input_instr, ND_UINT64 rip, ND_UINT32 buffer_size, char *__output_buffer);
ND_BOOL nd_is_instrux_rip_relative(const INSTRUX *__input_instr);
NDSTATUS nd_get_full_access_map(const INSTRUX *__input_instr, ND_ACCESS_MAP *__output_access_map);
NDSTATUS nd_get_operand_rlut(const INSTRUX *__input_instr, ND_OPERAND_RLUT *__output_rlut);
void nd_init_context(ND_CONTEXT *__input_context);
ND_UINT8 *nd_get_instrux_bytes(const INSTRUX *__input_instr, size_t *__output_convert_length);
ND_UINT8 *nd_get_intrux_op_code_bytes(const INSTRUX *__input_instr, size_t *__output_convert_length);
ND_OPERAND *nd_get_intrux_operands(const INSTRUX *__input_instr, size_t *__output_convert_length);

#endif // !_PYBDDISASM_H_
