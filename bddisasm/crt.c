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
