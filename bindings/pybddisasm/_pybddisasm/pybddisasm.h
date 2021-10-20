/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PYBDDISASM_H_
#define _PYBDDISASM_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "bddisasm.h"

int nd_decode_ex(INSTRUX *instrux, void *code, size_t size, int def_code, int def_data);

int nd_decode_ex2(INSTRUX *instrux, void *code, size_t size, int def_code, int def_data, int def_stack, int prefered_vendor);

int nd_to_text(INSTRUX *instrux, size_t rip, size_t bufsize, char *buf);

#endif // _PYBDDISASM_H_
