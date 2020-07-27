/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ND_CRT_H
#define ND_CRT_H

#include "../../inc/disasmtypes.h"

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P)       ((void)(P))
#endif

#if defined(_MSC_VER)

typedef char * va_list;

# ifndef _ADDRESSOF
#  ifdef  __cplusplus
#   define _ADDRESSOF(v)           ( &reinterpret_cast<const char &>(v) )
#  else
#   define _ADDRESSOF(v)           ( &(v) )
#  endif  // __cplusplus
# endif // !_ADDRESSOF

# ifndef KERNEL_MODE

#  if defined(AMD64) || defined(WIN64)

extern void __cdecl __va_start(__out va_list *, ...);       // is this exported by VC compiler?

#   define _crt_va_start(ap, x)    ( __va_start(&ap, x) )
#   define _crt_va_arg(ap, t)      ( ( sizeof(t) > sizeof(QWORD) || ( sizeof(t) & (sizeof(t) - 1) ) != 0 ) \
                                    ? **(t **)( ( ap += sizeof(QWORD) ) - sizeof(QWORD) ) \
                                    :  *(t  *)( ( ap += sizeof(QWORD) ) - sizeof(QWORD) ) )
#   define _crt_va_end(ap)         ( ap = (va_list)0 )

#  else

// a guess at the proper definitions for other platforms

#   ifndef _INTSIZEOF
#    define _INTSIZEOF(n)           ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#   endif // !_INTSIZEOF


#   define _crt_va_start(ap,v)     ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
#   define _crt_va_arg(ap,t)       ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#   define _crt_va_end(ap)         ( ap = (va_list)0 )

#  endif // AMD64 || WIN64

# define va_start _crt_va_start
# define va_arg _crt_va_arg
# define va_end _crt_va_end

#endif // KERNEL_MODE

#else

# include <stdarg.h>

# define __forceinline       inline __attribute__((always_inline))

#endif // _MSC_VER

#ifndef KERNEL_MODE
// Declared here only. Expecting it to be defined in the integrator.
extern int nd_vsnprintf_s(
    char *buffer,
    size_t sizeOfBuffer,
    size_t count,
    const char *format,
    va_list argptr
    );
#endif // KERNEL_MODE

// Declared here only. Expecting it to be defined in the integrator.
extern void *nd_memset(void *s, int c, size_t n);

#define nd_memzero(Dest, Size)         nd_memset((Dest), 0, (Size))

char *
nd_strcat_s(
    char *dst,
    size_t dst_size,
    const char *src
    );

#endif // ND_CRT_H
