# The Bitdefender disassembler

The Bitdefender disassembler (bddisasm) is a lightweight, x86/x64 only instruction decoder. It is easy to integrate, easy to work with, it has no external dependencies, it is thread-safe, it allocates no memory at all, it works in virtually any environment (we use it inside user, kernel, hypervisor, on both Windows and Linux environments), and it provides lots of info regarding the decoded instructions, such as: operands (both explicit and implicit), access mode for each operand, CPUID feature flag, flags access, etc. More examples and info about the project can be found on the official documentation: [Bitdefender disassembler](http://bddisasm.readthedocs.io)

## Projects

1. [bddisasm](https://github.com/bitdefender/bddisasm/tree/master/bddisasm) - this is the main disassembler project. In order to use the Bitdefender disassembler, all you have to do is build this project, and link with the output library. The only headers you need are located inside the `inc` folder.
2. [bdshemu](https://github.com/bitdefender/bddisasm/tree/master/bdshemu) - this project makes use of the main bddisasm lib in order to build a simple, lightweight, fast, instructions emulator, designated to target shellcodes. This project is also integrated inside the disasmtool, so you can
emulate raw binary files, and see their output. Note that this simple emulator supports basic x86/x64 instructions, and does not support emulating any kind of API call. In addition, the only supported memory accesses are inside the shellcode itself, and on the emulated stack.
3. [isagenerator](https://github.com/bitdefender/bddisasm/tree/master/isagenerator) - this project contains the instruction definitions and the scripts required to generate the disassembly tables. If you wish to add support for a new instruction, this is the place. This project will automatically generate several header files (instructions.h, mnemonics.h, constants.h, table_\*.h), so please make sure you don't manually edit any of these files. You will need Python 3 to run the generation scripts.
4. [disasmtool](https://github.com/bitdefender/bddisasm/tree/master/disasmtool) - this project is a command line disassembler tool, used mainly as an example of how to integrate the bddisasm and bdshemu libraries.
5. [disasmtool_lix](https://github.com/bitdefender/bddisasm/tree/master/disasmtool_lix) - like disasmtool, but for Linux.
6. [bindings](https://github.com/bitdefender/bddisasm/tree/master/bindings) - bindings for [python](https://github.com/bitdefender/bddisasm/tree/master/bindings/pybddisasm), and [Rust](https://github.com/bitdefender/bddisasm/tree/master/bindings/rsbddisasm).

## Objectives

The main objectives of this disassembler are:

1. Lightweight - it's written in C, with no external dependencies, no memory allocated, and thread safe by design.
2. Fast - less than 300 CPU clocks on an Intel Core i7-8650U per decoded instruction (more than 7M instructions per second).
3. Resilient - tested against internal fuzzers and the famous [mishegos](https://github.com/trailofbits/mishegos) tool.
4. Easy to work with - just include the main header file, bddisasm.h, link with the bddisasm library, and call the NdDecode API!
5. Complete - support every x86 instruction to date, and provide as much information as possible.

## Build and install

### Using [vcpkg](https://github.com/microsoft/vcpkg)

The easiest way of installing bddisasm is with [vcpkg](https://github.com/microsoft/vcpkg):

```console
vcpkg install bddisasm
```

This will install `bddisasm` and `bdshemu` static libraries.

Note that the version available on vcpkg may not always be the latest.

### Using CMake

This is the recommended way of using the library.

Prerequesites:

- CMake 3.16 or newer (older version may work, but they have not been tested)
- Ninja (optional, but recommended, especially on Windows)

The build process was tested with GCC and Clang on Linux and MSVC on Windows. Note that the Windows kernel build target is available only when using [MSBuild](#Using-MSBuild-on-Windows).

To build the project run:

```console
mkdir build
cmake -B build .
cmake --build build
```

This will build `bddisasm`, `bdshemu`, and `disasmtool`. For skipping the `disasmtool` build configure CMake with `BDD_INCLUDE_TOOL=OFF`:

```console
mkdir build
cmake -B build . -DBDD_INCLUDE_TOOL=OFF
```

To install the project use:

```console
cmake --build build --target install
```

This will install the `bddisasm` and `bdshemu` static libraries and their public headers. If `disasmtool` was built it will also be installed. Depending on the install location you may need to run the command as root.

Optionally, if a python 3 interpreter is found the instruction tables can be regenerated with:

```console
cmake --build build --target isagenerator
```

To disable the `isagenerator` target configure CMake with `BDD_INCLUDE_ISAGENERATOR=OFF`.

Once installed, CMake projects can use `find_package` to find the library:

```cmake
find_package(bddisasm REQUIRED)
```

Two targets are provided:

- `bddisasm::bddisasm` - this should be used for targets that need only the decoder, without the shell code emulator
- `bddisasm::bdshemu` - this should be used for targets that need the shell code emulator (note that it will pull in `bddisasm::bddisasm` automatically)

There is no need to manually set include or link directories, simply use `target_link_libraries` with the needed target, for example:

```cmake
find_package(bddisasm REQUIRED)

# decoder-tool needs only the decoder library
target_link_libraries(decoder-tool PRIVATE bddisasm::bddisasm)

# emulator-tool needs bdshemu
target_link_libraries(emulator-tool PRIVATE bddisasm::bdshemu)
```

### nd_vsnprintf_s and nd_memset

By default, if `vsnprintf` and `memset` functions are available, the `nd_vsnprintf_s` and `nd_memset` functions are implemented directly by `bddisasm`. To signal this, `BDDISASM_HAS_VSNPRINTF` and `BDDISASM_HAS_MEMSET` will be added to the public compile definitions of `bddisasm`. This can be disabled by configuring CMake with `BDD_USE_EXTERNAL_VSNPRINTF=ON` and `BDD_USE_EXTERNAL_MEMSET=ON`.

#### Using as a sub-project

The project can be consumed as a sub-project, either by adding it as a git submodule, or by using [CMake's FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html):

With `FetchContent`:

```cmake
FetchContent_Declare(
    bddisasm
    GIT_REPOSITORY https://github.com/bitdefender/bddisasm
    GIT_TAG origin/master
)

FetchContent_MakeAvailable(bddisasm)
```

As a git submodule:

```cmake
# Assuming the submodule is checked out at external/bddisasm
add_subdirectory(external/bddisasm)
```

In both cases the `bddisasm::bddisasm` and `bddisasm::bdshemu` targets will be provided.

When used as a sub-project the `disasmtool`, `isagenerator`, and `install` targets are not available.

### Using Make on Linux

To build the project run `make` in the root of the repository. This will build only the `bddisasm` and `bdshemu` static libraries, without `disasmtool`.

To install the project run `make install`. Depending on the install location you may need to run the command as root.

[nd_vsnprintf_s and nd_memset](#nd_vsnprintf_s-and-nd_memset) will not be defined by `bddisasm`, integrators must provide these functions.

### Using MSBuild on Windows

In order to build the projects on Windows you need:

- [Visual Studio 2019](https://visualstudio.microsoft.com/vs/) with the Desktop development with C++ workload.
- [Windows SDK 10.0.18362.0](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/).
- [Python 3.7 or newer](https://www.python.org/downloads/release/python-373/) (optional)

When you first open `bddisasm.sln` in Visual Studio should prompt you to install any missing components.
This should be enough to build `bddisasm`, `bdshemu`, and `disasmtool`.

For the DebugKernel and ReleaseKernel configurations, [WDK 1903](https://go.microsoft.com/fwlink/?linkid=2085767) is needed, alongside the Windows Driver Kit Visual Studio extension (the WDK installer should take care of this).

For `isagenerator`, Python 3 is needed.

Building any of the projects is done directly from Visual Studio.

The results will be in the bin directory in the root of the repository.

[nd_vsnprintf_s and nd_memset](#nd_vsnprintf_s-and-nd_memset) will not be defined by `bddisasm`, integrators must provide these functions.

## Decoding instructions

### Decoding API

There are 4 decoding functions, but internally, they all do the same, albeit some of them with implicit arguments:

- `NDSTATUS NdDecode(INSTRUX *Instrux, const uint8_t *Code, uint8_t DefCode, uint8_t DefData)` - this API should be used only if you don't care about the length of the input buffer; 
- `NDSTATUS NdDecodeEx(INSTRUX *Instrux, const uint8_t *Code, size_t Size, uint8_t DefCode, uint8_t DefData);` - decode instruction from a buffer with maximum length `Size`;
- `NDSTATUS NdDecodeEx2(INSTRUX *Instrux, const uint8_t *Code, size_t Size, uint8_t DefCode, uint8_t DefData, uint8_t DefStack, uint8_t PreferedVendor);` - decode instructions with a preferred vendor;
- `NDSTATUS NdDecodeWithContext(INSTRUX *Instrux, const uint8_t *Code, size_t Size, ND_CONTEXT *Context);` - base decode API; the input parameters - `DefCode`, `DefData`, `DefStack`, `VendMode` and `FeatMode` must all be filled in the `Context` structure before calling this function. The Context structure should also be initialized using `NdInitContext` before the first decode call.

Note that by default, the default vendor `ND_VEND_ANY` is used for decoding (which means that bddisasm will try to decode as much as possible). Also, the default features mask is `ND_FEAT_ALL`, meaning that bddisasm will optimistically try to decode instructions which are mapped onto the wide NOP space as well (for example, MPX or CET). If these parameters must be changed, it is advised to use the `NdDecodeWithContext` API.

Converting decoded instructions to textual disassembly must be done using the `NdToText` API. bddisasm only supports Intel, masm-style syntax.

### Example

Working with bddisasm is very easy. Decoding and printing the disassembly of an instruction is quick & simple:

```c
#include <stdio.h>
#include "bddisasm/disasmtypes.h"
#include "bddisasm/bddisasm.h"

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

void* nd_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}


int main()
{
    INSTRUX ix;
    uint8_t code[] = { 0x48, 0x8B, 0x48, 0x28 };

    NDSTATUS status = NdDecodeEx(&ix, code, sizeof(code), ND_CODE_64, ND_DATA_64);

    // Check if decoding failed.
    if (!ND_SUCCESS(status))
    {
        printf("Decode failed with error %x!\n", status);
        return -1;
    }

    // Checking if the instruction is a MOV.
    if (ix.Instruction == ND_INS_MOV)
    {
        printf("The instruction is mov!\n");
    }

    // Checking if memory is read by the instruction.
    if (!!(ix.MemoryAccess & ND_ACCESS_ANY_READ))
    {
        printf("The instruction reads memory!\n");
    }

    // Checking if the destination is the RCX register.
    if (ND_IS_OP_REG(&ix.Operands[0], ND_REG_GPR, 8, NDR_RCX))
    {
        printf("The first op is RCX!\n");
    }

    // Checking if the source is memory.
    if (ix.Operands[1].Type == ND_OP_MEM)
    {
        printf("Source is memory!\n");

        if (ix.Operands[1].Info.Memory.HasSeg)
        {
            printf("    Segment reg: %d\n", ix.Operands[1].Info.Memory.Seg);
        }

        if (ix.Operands[1].Info.Memory.HasBase)
        {
            printf("    Base reg: %d\n", ix.Operands[1].Info.Memory.Base);
        }

        if (ix.Operands[1].Info.Memory.HasIndex)
        {
            printf("    Index reg: %d, scale: %d\n", ix.Operands[1].Info.Memory.Index, ix.Operands[1].Info.Memory.Scale);
        }

        if (ix.Operands[1].Info.Memory.HasDisp)
        {
            printf("    Displacement: 0x%llx\n", ix.Operands[1].Info.Memory.Disp);
        }
    }

    // Create the text disassembly for this instruction.
    char text[ND_MIN_BUF_SIZE];

    NdToText(&ix, 0, sizeof(text), text);

    printf("Instruction: %s\n", text);

    return 0;
}
```

Working with the extended API is also trivial:

```c
    INSTRUX ix;
    ND_CONTEXT ctx;
    uint8_t code[] = { 0x48, 0x8B, 0x48, 0x28 };
    
    // This has to be done only once.
    NdInitContext(&ctx);
    
    ctx.DefCode = ND_CODE_64;
    ctx.DefData = ND_DATA_64;
    ctx.DefStack = ND_STACK_64;
    ctx.VendMode = ND_VEND_ANY;
    ctx.FeatMode = ND_FEAT_ALL; // Use ND_FEAT_NONE, if you wish to see NOPs instead of MPX/CET/CLDEMOTE instructions.

    // From here one, the ctx can be reused for any number of NdDecodeWithContext calls.
    NDSTATUS status = NdDecodeWithContext(&ix, code, sizeof(code), &ctx);
    ...
```

## Credits

The entire Bitdefender HVI team.
