/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#include "pybddisasm.h"
#include <stdlib.h>


int nd_decode_ex(INSTRUX *instrux, void *code, size_t size, int def_code, int def_data)
{
    return NdDecodeEx(instrux, code, size, def_code, def_data);
}


int nd_decode_ex2(INSTRUX *instrux, void *code, size_t size, int def_code, int def_data, int def_stack, int prefered_vendor)
{
    return NdDecodeEx2(instrux, code, size, def_code, def_data, def_stack, prefered_vendor);
}


int nd_to_text(INSTRUX *instrux, size_t rip, size_t bufsize, char *buf)
{
    return NdToText(instrux, rip, bufsize, buf);
}

#include <stdio.h>
#include <string.h>

int
nd_vsnprintf_s(char *str, size_t sizeOfBuffer, size_t count, const char *format, va_list args)
{
    (void)(sizeOfBuffer);
    return vsnprintf(str, count, format, args);
}

void *nd_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}
