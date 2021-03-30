/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#include "include/nd_crt.h"

//
// nd_strcat_s
//
char *
nd_strcat_s(
    char *dst,
    size_t dst_size,
    const char *src
    )
{
    char *p;
    size_t available;

    p = dst;
    available = dst_size;
    while (available > 0 && *p != 0)
    {
        p++;
        available--;
    }

    if (available == 0)
    {
        nd_memzero(dst, dst_size);
        return NULL;
    }

    while ((*p++ = *src++) != 0 && --available > 0);

    if (available == 0)
    {
        nd_memzero(dst, dst_size);
        return NULL;
    }

    return dst;
}

#if !defined(BDDISASM_NO_FORMAT) && defined(BDDISASM_HAS_VSNPRINTF)
#include <stdio.h>

int nd_vsnprintf_s(char *buffer, size_t sizeOfBuffer, size_t count, const char *format, va_list argptr)
{
    UNREFERENCED_PARAMETER(count);
    return vsnprintf(buffer, sizeOfBuffer, format, argptr);
}
#endif // !defined(BDDISASM_NO_FORMAT) && defined(BDDISASM_HAS_VSNPRINTF)

#if !defined(BDDISASM_NO_FORMAT) && defined(BDDISASM_HAS_MEMSET)
#include <string.h>

void *nd_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}
#endif // !defined(BDDISASM_NO_FORMAT) && defined(BDDISASM_HAS_MEMSET)