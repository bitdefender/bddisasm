/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef DISASM_TYPES_H
#define DISASM_TYPES_H

#if defined(KERNEL_MODE) && defined(_MSC_VER)
# include <ntddk.h>
# include <Ntstrsafe.h>
# include <basetsd.h>

typedef UINT8 uint8_t;
typedef UINT16 uint16_t;
typedef UINT32 uint32_t;
typedef UINT64 uint64_t;

typedef INT8 int8_t;
typedef INT16 int16_t;
typedef INT32 int32_t;
typedef INT64 int64_t;

typedef _Bool bool;
#define false 0
#define true 1

#elif defined(__KERNEL__) && defined(__GNUC__)

# include <linux/types.h>

#else

# include <stddef.h>
# include <stdint.h>
# include <stdbool.h>

#endif

#endif
