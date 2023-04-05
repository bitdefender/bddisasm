/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "bddisasm.h"
#include "bdshemu.h"

#pragma clang optimize off
#pragma GCC            optimize("O0")

#ifdef ENABLE_LOGGING
#define LOG(fmt, ...)   printf(fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif // ENABLE_LOGGING

void ShemuLog(char *data)
{
    LOG("%s", data);
}

#ifdef FUZZ_X86
#define DEF_CODE    ND_CODE_32
#define FUZZER_TYPE "x86"
#else
#define DEF_CODE    ND_CODE_64
#define FUZZER_TYPE "x64"
#endif // FUZZ_X86

void run_shemu(uint8_t *Data, size_t Size)
{
    // More or less copy pasted from disasmtool

    SHEMU_CONTEXT ctx = { 0 };
    SHEMU_STATUS shs;

    ctx.Shellcode = Data;

    ctx.Stack = calloc(1, 0x2000);
    if (ctx.Stack == NULL)
    {
        LOG("[-] Failed to allocate Stack!\n");
        abort();
    }

    ctx.Intbuf = calloc(1, Size + 0x2000);
    if (ctx.Stack == NULL)
    {
        LOG("[-] Failed to allocate Intbuf!\n");
        abort();
    }

    ctx.ShellcodeBase = 0x200000;
    ctx.ShellcodeSize = (uint32_t)Size;
    ctx.StackBase = 0x100000;
    ctx.StackSize = 0x2000;
    ctx.Registers.RegRsp = 0x101000;
    ctx.IntbufSize = (uint32_t)Size + 0x2000;

    ctx.Registers.RegFlags = NDR_RFLAG_IF | 2;
    ctx.Registers.RegRip = ctx.ShellcodeBase;

    ctx.Segments.Cs.Selector = 0x10;
    ctx.Segments.Ds.Selector = 0x28;
    ctx.Segments.Es.Selector = 0x28;
    ctx.Segments.Ss.Selector = 0x28;
    ctx.Segments.Fs.Selector = 0x30;
    ctx.Segments.Fs.Base = 0x7FFF0000;
    ctx.Segments.Gs.Selector = 0x30;
    ctx.Segments.Gs.Base = 0x7FFF0000;

    ctx.Mode = DEF_CODE;
    ctx.Ring = 3;
    ctx.TibBase = ctx.Mode == ND_CODE_32 ? ctx.Segments.Fs.Base : ctx.Segments.Gs.Base;
    ctx.MaxInstructionsCount = 4096;
    ctx.Log = &ShemuLog;
    ctx.Flags = 0;
    ctx.Options = SHEMU_OPT_TRACE_EMULATION;

    shs = ShemuEmulate(&ctx);
    LOG("[+] Shemu returned: 0x%08x\n", shs);

    free(ctx.Intbuf);
    free(ctx.Stack);
}

#if defined(__AFL_FUZZ_TESTCASE_LEN)
#include <unistd.h>

// See https://github.com/AFLplusplus/AFLplusplus/blob/stable/instrumentation/README.persistent_mode.md
__AFL_FUZZ_INIT();

int main()
{
    __AFL_INIT();

    uint8_t *buffer = __AFL_FUZZ_TESTCASE_BUF;
    while (__AFL_LOOP(UINT_MAX))
    {
        size_t size = __AFL_FUZZ_TESTCASE_LEN;
        run_shemu(buffer, size);
    }

    return 0;
}
#elif defined(__clang__)
int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    // Shemu will write into the shellcode buffer, so copy it to a new buffer first.

    uint8_t *buffer = malloc(Size);
    if (!buffer) return 1;

    memcpy(buffer, Data, Size);
    run_shemu(buffer, Size);

    free(buffer);
    return 0;
}
#else
uint8_t *read_file(const char *fpath, size_t *size)
{
    uint8_t *buffer;
    FILE *fd = fopen(fpath, "rb");
    if (fd == NULL)
    {
        LOG("[-] Failed to open \"%s\"\n", fpath);
        return NULL;
    }

    fseek(fd, 0ull, SEEK_END);
    *size = ftell(fd);
    rewind(fd);

    buffer = malloc(*size);
    if (buffer == NULL)
    {
        LOG("[-] Failed to allocate %zu bytes\n", *size);
    }
    else
    {
        fread(buffer, *size, 1, fd);
    }

    fclose(fd);
    return buffer;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        LOG("Missing file argument\n");
        abort();
    }

    size_t size;
    uint8_t *buffer = read_file(argv[1], &size);
    if (!buffer)
    {
        LOG("[-] read_file failed for \"%s\"\n", argv[1]);
        abort();
    }

    run_shemu(buffer, size);
    free(buffer);

    return 0;
}
#endif // defined(__AFL_FUZZ_TESTCASE_LEN)
