/*
 * Copyright (c) 2023 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "pybddisasm.h"


#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

#if _MSC_VER
int nd_vsnprintf_s(char *buffer, size_t size, size_t count, const char *format, va_list argptr)
{
    return vsnprintf(buffer, size, format, argptr);
}

void * nd_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}
#endif

void nd_get_version(ND_UINT32 *major, ND_UINT32 *minor, ND_UINT32 *revision, char **build_date, char **build_time)
{
    NdGetVersion(major, minor, revision, build_date, build_time);
}

NDSTATUS nd_decode(INSTRUX *__output_instr, const ND_UINT8 *code, ND_UINT8 def_code, ND_UINT8 def_data)
{
    return NdDecode(__output_instr, code, def_code, def_data);
}

NDSTATUS nd_decode_ex(INSTRUX *__output_instr, const ND_UINT8 *code, ND_SIZET size, ND_UINT8 def_code, ND_UINT8 def_data)
{
    return NdDecodeEx(__output_instr, code, size, def_code, def_data);
}

NDSTATUS nd_decode_ex2(INSTRUX *__output_instr, const ND_UINT8 *code, ND_SIZET size, ND_UINT8 def_code, ND_UINT8 def_data,
                       ND_UINT8 def_stack, ND_UINT8 prefered_vendor)
{
    return NdDecodeEx2(__output_instr, code, size, def_code, def_data, def_stack, prefered_vendor);
}

NDSTATUS nd_decode_with_context(INSTRUX *__output_instr, const ND_UINT8 *code, ND_SIZET size, ND_CONTEXT *context)
{
    return NdDecodeWithContext(__output_instr, code, size, context);
}

NDSTATUS nd_to_text(const INSTRUX *__input_instr, ND_UINT64 rip, ND_UINT32 buffer_size, char *__output_buffer)
{
    return NdToText(__input_instr, rip, buffer_size, __output_buffer);
}

ND_BOOL nd_is_instrux_rip_relative(const INSTRUX *__input_instr)
{
    return NdIsInstruxRipRelative(__input_instr);
}

NDSTATUS nd_get_full_access_map(const INSTRUX *__input_instr, ND_ACCESS_MAP *__output_access_map)
{
    return NdGetFullAccessMap(__input_instr, __output_access_map);
}

NDSTATUS nd_get_operand_rlut(const INSTRUX *__input_instr, ND_OPERAND_RLUT *__output_rlut)
{
    return NdGetOperandRlut(__input_instr, __output_rlut);
}

void nd_init_context(ND_CONTEXT *__input_context)
{
    NdInitContext(__input_context);
}

ND_UINT8 *nd_get_instrux_bytes(const INSTRUX *__input_instr, size_t *__output_convert_length)
{
    *__output_convert_length = sizeof(__input_instr->InstructionBytes);
    return (ND_UINT8 *)__input_instr->InstructionBytes;
}

ND_UINT8 *nd_get_intrux_op_code_bytes(const INSTRUX *__input_instr, size_t *__output_convert_length)
{
    *__output_convert_length = sizeof(__input_instr->OpCodeBytes);
    return (ND_UINT8 *)__input_instr->OpCodeBytes;
}


ND_OPERAND *nd_get_intrux_operands(const INSTRUX *__input_instr, size_t *__output_convert_length)
{
    *__output_convert_length = ARRAY_SIZE(__input_instr->Operands);
    return (ND_OPERAND *)__input_instr->Operands;
}
