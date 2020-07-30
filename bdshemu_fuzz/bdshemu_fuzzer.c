/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "bddisasm.h"
#include "bdshemu.h"

#ifdef ENABLE_LOGGING
#define LOG(fmt, ...)   printf(fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif // ENABLE_LOGGING

#ifndef __AFL_LOOP
#warning "Persistent mode will not work (you are probably not using afl-clang-fast)"
#endif

int nd_vsnprintf_s(
    char *buffer,
    size_t sizeOfBuffer,
    size_t count,
    const char *format,
    va_list argptr
)
{
    return vsnprintf(buffer, sizeOfBuffer, format, argptr);
}

void *nd_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}

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

static bool gUseLog;

void ShemuLog(char *data)
{
    if (gUseLog)
    {
        LOG("%s", data);
    }
}

void run_shemu(uint8_t *Data, size_t Size, uint8_t Def)
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

    ctx.Mode = Def;
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

void run_test(uint8_t *Data, size_t Size, uint8_t Def)
{
    run_shemu(Data, Size, Def);    
}

int8_t arch_to_def_code(const char *arch)
{
    uint32_t a = strtoul(arch, NULL, 0);

    switch (a)
    {
    case 16:
        return ND_CODE_16;

    case 32:
        return ND_CODE_32;

    case 64:
        return ND_CODE_64;

    default:
        LOG("[-] Unknown arch: \"%s\"\n", arch);
        return -1;
    }
}

int main(int argc, char **argv)
{
    size_t size = 0;
    uint8_t *buffer;
    int8_t defCode;

    if (argc < 3)
    {
        LOG("[-] Missing arguments: <file path> <16|32|64> [activate shemu print]\n");
        abort();
    }

    defCode = arch_to_def_code(argv[2]);
    if (defCode == -1)
    {
        abort();
    }

    if (argc >= 4)
    {
        LOG("[+] Will activate ShemuLog\n");
        gUseLog = true;
    }
    else
    {
        gUseLog = false;
    }

    LOG("[+] Using def code/data = %d\n", defCode);

// __AFL_LOOP is defined only when compiling with afl-clang-fast and allows us to use
// AFL's persistent mode. We still want to be able to compile and run even if we are
// using afl-gcc or afl-clang, but there is no reason to actually have a loop here
// if we are not using persistent mode.
#ifdef __AFL_LOOP
    while (__AFL_LOOP(1000))
#endif // __AFL_LOOP
    {
        LOG("[+] Reading data from \"%s\"\n", argv[1]);
        buffer = read_file(argv[1], &size);
        if (buffer == NULL)
        {
            abort();
        }

        LOG("[+] Read %zu bytes at %p (range: [%p, %p))\n", size, buffer, buffer, buffer + size);

        run_test(buffer, size, (uint8_t)defCode);

        free(buffer);
    }

    LOG("[+] Done!\n");

    return 0;
}
