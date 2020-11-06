/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef DISASM_TYPES_H
#define DISASM_TYPES_H

#if defined(KERNEL_MODE) && defined(_MSC_VER)
# include <ntddk.h>
# include <Ntstrsafe.h>

#elif defined(__KERNEL__) && defined(__GNUC__)

# include <linux/types.h>

#else

# include <stddef.h>
# include <stdint.h>
# include <stdbool.h>

#endif

#endif
