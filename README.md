# The Bitdefender disassembler

The Bitdefender disassembler is a lightweight, x86/x64 only instruction decoder. It is easy to integrate, easy to work with, it has no external dependencies, it is thread-safe, it allocates no memory at all, it works in virtually any environment (we use it inside user, kernel, hypervisor, on both Windows and Linux environments), and it provides lots of info regarding the decoded instructions, such as: operands (both explicit and implicit), access mode for each operand, CPUID feature flag, flags access, etc.

## Projects

1. disasm - this is the main disassembler project. In order to use the Bitdefender disassembler, all you have to do is build this project, and link with the output library. The only headers you need are located inside the inc folder.

2. shemu - this project makes use of the main disasm lib in order to build a simple, lightweight, fast, instructions emulator, designated to target shellcodes. This project is also integrated inside the disasmtool, so you can
emulate raw binary files, and see their output. Note that this simple emulator supports basic x86/x64 instructions, and does not support emulating any kind of API call.

3. isagenerator - this project contains the instruction definitions and the scripts required to generate the disassembly tables. If you wish to add support for a new instruction, this is the place. This project will automatically generate several header files (instructions.h, mnemonics.h, constants.h, table_\*.h), so please make sure you don't manually edit any of these files.

4. disasmtool - this project is a command line disassembler tool, used mainly as an example of how to integrate the disasm lib.

5. disasmtool_lix - like disasmtool, but for Linux.

6. pydis - this is the Python binding for the disasm project.

## Objectives

The main objectives of this disassembler are:

1. Lighetweight - it's written in C, with no external dependencies, no memory allocated, and thread safe by design.
2. Fast - less than 300 CPU clocks on a Intel Core i7-8650U per decoded instruction.
3. Resilient - tested against internal fuzzers and the famous [mishegos](https://github.com/trailofbits/mishegos) tool
4. Easy to work with - just include the main header file, disasm.h, link with the disasmlib library, and call the NdDecode API!
5. Complete - support every x86 instruction to date, and provide as much information as possible.

## Build

### Windows

In order to build the projects on Windows you need:

* [Visual Studio 2019](https://visualstudio.microsoft.com/vs/) with the Desktop development with C++ workload.
* [Windows SDK 10.0.18362.0](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/).

When you first open the solution Visual Studio should prompt you to install any missing components.
This should be enough to build disasm, disasmtool and shemu for the Debug and Release configurations.

For the DebugKernel and ReleaseKernel configurations, [WDK 1903](https://go.microsoft.com/fwlink/?linkid=2085767) is needed, alongside the Windows Driver Kit Visual Studio extension (the WDK installer should take care of this).

For isagenerator, python is needed.

Building any of the projects is done directly from Visual Studio.

The results will be in the bin directory in the root of the repository.

### Linux

In order to build the projects on Linux you need:

* gcc
* make

For disasmtool_lix you also need:

* g++
* cmake 3.12 or newer
* [RapidJSON](https://github.com/Tencent/rapidjson/)

In order to build disasm and shemu run `make` in the root of the repository. The results will be placed in the bin directory.

In order to build disasmtool_lix go to the disasmtool_lix directory and run `make`. The results will be in the bin directory in the disasmtool_lix/build directory.

## Example

Working with disasm is very easy. Decoding and printing the disassembly of an instruction is quick & simple:

```c
#include "disasm.h"

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
    if (ND_IS_OP_REG(&ix.Operands[0], ND_REG_GPR, 8, REG_RCX))
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

## Credits

The entire Bitdefender HVI team.
