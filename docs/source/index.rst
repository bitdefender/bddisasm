.. bddisasm documentation master file, created by
   sphinx-quickstart on Wed Jul  8 13:39:28 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to bddisasm's documentation!
====================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:
   
About
=====

The Bitdefender x86 disassembler (https://github.com/bitdefender/bddisasm) is a complete x86/x64 instruction decoder, 
capable of providing full information about each decoded instruction. The
library is written entirely in C, with some Python 3 code for the instruction
tables generation. It has no external dependencies, and it is thread safe
by design. It can be easily integrated in any environment - we use it in
user mode on both Windows and Linux, in kernel mode on Windows and inside 
VMX root of our own Napoca hypervisor.

This documentation provides a high-level overview of the project and 
how to use it. The information is bound to standard x86 instruction
semantics, hence there is no official Doxygen documentation Instead, the source
code and the header files contain rich comments, which provided detailed relevant
information.


Project goals
=============

When we first started the project more than 5 years ago, there were no good
disassemblers available which werecapable of providing extensive information about the
decoded instructions. We decided to create, from scratch, such a disassembler,
which would then be used by all the projects that require one. The first 
requirement was, therefore, that the library should be OS agnostic. We then
considered the main use-cases, where we decode and analyze instructions
in real time (for example, an instruction that just triggered a memory fault).
This meant that the decoder had to be very fast. Of course, having to be able
to integrate the library in any mode of operation and on any operating system
meant that it should be thread-safe by design, and no memory should be allocated. 
We then considered that some of our use-cases required instruction emulation, 
so the decoder must provide as much information about each instruction as 
possible. Finally, given the latest ISA extensions proposed by Intel, the 
disassembler must also be designed in a way that allows programmers to easily 
add support for new instructions. The requirements, in a nutshell, are, therefore:
    
.. hlist::
    :columns: 1

    * OS and operating mode agnostic (Windows/Linux, user/kernel/VMX root)
    * Speed (less than 500 clocks/decoded instruction)
    * No memory overhead (no memory is allocated by the disassembler)
    * Thread-safe (no global variables written, no threads created)
    * Full instruction information
    * Easy to extend
    
We managed to achieve all of these goals. 

The library is successfully being
used in various user/kernel/VMX root projects, on both Windows and Linux. 
On an Intel Core i7-8650U, it takes roughly 300 clocks to decode one instruction.
There is no memory overhead, the library is thread safe and it provides pretty
much all known information about each instruction. Finally, the instructions are
defined in an easy to understand, well documented textual format - adding support
for a new instruction requires describing the instruction in one
of these files, and re-building the tables.

Using the disassembler
======================

Using the disassembler is very easy. Include the **bddisasm.h** header file
and link with the disassembler library (which is **bddisasm.lib** on Windows or
**libbddisasm.a** on Linux). Once this has been done, make sure the **nd_vsnprintf_s** and
**nd_memset** functions are defined. For example, do the following:

.. code-block:: c
   :linenos:
   
    int nd_vsnprintf_s(
        char *buffer,
        size_t sizeOfBuffer,
        size_t count,
        const char *format,
        va_list argptr
        )
    {
        return _vsnprintf_s(buffer, sizeOfBuffer, count, format, argptr);
    }

    void* nd_memset(void *s, int c, size_t n)
    {
        return memset(s, c, n);
    }
    
These are required to allow the integrator to use whatever implementation of
vsnprintf and memset they wish. Once this has been done, decoding an instruction is as
simple as calling one of the decoding API:

.. code-block:: c
   :linenos:
   
   #include "bddisasm/disasmtypes.h"
   #include "bddisasm/bddisasm.h"
   
   int main()
   {
        INSTRUX ix;
        unsigned char ins[2] = { 0x33, 0xC0 };
        NDSTATUS status;
        
        status = NdDecodeEx(&ix, ins, sizeof(ins), ND_CODE_64, ND_DATA_64);
        if (!ND_SUCCESS(status))
        {
            printf("Decoding failed with error 0x%08x!\n", status);
            return -1;
        }

        printf("Decoded instruction with length %d!\n", ix.Length);
   }
   
When decoding an instruction, all the possible information is stored in the
output **INSTRUX** structure. There is no need to call any other helper functions.
However, the textual representation (the actual *disassembly*) of the instruction
has to be obtained by calling the **NdToText** API:

.. code-block:: c
   :linenos:
   
   #include "bddisasm/disasmtypes.h"
   #include "bddisasm/bddisasm.h"
   
   int main()
   {
        INSTRUX ix;
        unsigned char ins[2] = { 0x33, 0xC0 };
        NDSTATUS status;
        char txt[ND_MIN_BUF_SIZE];
        
        status = NdDecodeEx(&ix, ins, sizeof(ins), ND_CODE_64, ND_DATA_64);
        if (!ND_SUCCESS(status))
        {
            printf("Decoding failed with error 0x%08x!\n", status);
            return -1;
        }

        NdToText(&ix, 0, sizeof(txt), txt);

        printf("Decoded instruction %s!\n", txt);
   }
   
The disassembler provides the **NdToText** function, but this function only 
supports Intel style syntax. If one wishes to implement a different output
formatter, it can do so, by implementing a different **NdToText** function 
(the existing function can be used as a template).
   
Instruction information
=======================

The information provided inside the **INSTRUX** structure covers all the known information
about the instruction. There are several categories of info offered:

.. hlist::
    :columns: 1

    * Instruction encoding information - prefixes, opcodes, modrm, sib, immediates, displacement
    * Operand information, for both explicit and implicit operands - type, size, access, encoding
    * Instruction meta information - instruction set, instruction type, instruction class
    * Accepted prefixes - lock, rep, hle, etc.
    * Valid modes for the instruction - real, protected, long, ring 0/1/2/3, etc.
    * CPUID leaf information - allows to directly test if the instruction is available or not
    * Flags access - for each flag, access type (tested, modified, cleared, set to 1, undefined)
    * FPU flags access - for FPU instructions
    * For VEX/SSE instructions - exception class
    * For EVEX instructions - decorator info, tuple type, exception class
    
For memory operands, the following information is provided:

.. hlist::
    :columns: 1

    * Segment
    * Base register
    * Index register, with scale
    * Displacement
    * VSIB information - index size, element size, element count
    * Extended addressing information - address generation, mib, sibmem, string operation indicator, stack access indicator, bitbase, etc.
    
This is the most relevant information provided by it. For more info, 
look at the **bddisasm.h** file, which contains all the definitions, 
together with ample comments describing each one of them.
    
Examples
========

Assuming we have already decoded an instruction as follows:

.. code-block:: c
   :linenos:
   
   #include "bddisasm/disasmtypes.h"
   #include "bddisasm/bddisasm.h"
   
   int main()
   {
        INSTRUX ix;
        unsigned char ins[2] = { 0x33, 0xC0 };
        NDSTATUS status;
        char txt[ND_MIN_BUF_SIZE];
        
        status = NdDecodeEx(&ix, ins, sizeof(ins), ND_CODE_64, ND_DATA_64);
        if (!ND_SUCCESS(status))
        {
            printf("Decoding failed with error 0x%08x!\n", status);
            return -1;
        }
   }
   
we will provide some examples of how we can obtain specific information about the instruction.

Determining the length of an instruction
----------------------------------------

.. code-block:: c
   :linenos:
   
   ix.Length;   // Length in bytes.
   

Determining the number of operands of an instruction
----------------------------------------------------

.. code-block:: c
   :linenos:
   
   ix.OperandsCount;    // Total operands count.
   ix.ExpOperandsCount; // Explicit operands count.
   
Determining the operand and address size
----------------------------------------

.. code-block:: c
   :linenos:
   
   if (ix.OpMode == ND_OPSZ_16) // 16 bit operand size.
   if (ix.OpMode == ND_OPSZ_32) // 32 bit operand size.
   if (ix.OpMode == ND_OPSZ_64) // 64 bit operand size.
   if (ix.AddrMode == ND_ADDR_16) // 16 bit address size.
   if (ix.AddrMode == ND_ADDR_32) // 32 bit address size.
   if (ix.AddrMode == ND_ADDR_64) // 64 bit address size.
   
Determining if the instruction has modrm, SIB or displacement
-------------------------------------------------------------
  
.. code-block:: c
   :linenos:
   
   if (ix.HasModrm) // The instruction has Mod R/M.
   if (ix.HasSib) // The instruction has SIB.
   if (ix.HasDisp) // The instruction has displacement.
   
Determining if the first operand is register EAX
------------------------------------------------   

.. code-block:: c
   :linenos:
   
   if (ND_IS_OP_REG(&ix.Operands[0], ND_REG_GPR, 4, NDR_EAX))
   
Determining if the second operand is the stack
----------------------------------------------   

.. code-block:: c
   :linenos:
   
   if (ix.Operands[1].Type == ND_OP_MEM && 
       ix.Operands[1].Info.Memory.IsStack)
       
Determining if the instruction is a conditional branch
------------------------------------------------------

.. code-block:: c
   :linenos:
   
   if (ix.Category == ND_CAT_COND_BR)

Determining if the instruction is "ADD" or "SUB"
------------------------------------------------  

.. code-block:: c
   :linenos:
   
   if (ix.Instruction == ND_INS_ADD ||
       ix.Instruction == ND_INS_SUB)
       
Determining if the instruction accesses various flags
-----------------------------------------------------

.. code-block:: c
   :linenos:
   
   if (ix.FlagsAccess.Modified.CF) // Is CF modified?
   if (ix.FlagsAccess.Undefined.OF) // Is OF undefined?
   if (ix.FlagsAccess.Cleared.ZF) // Is ZF cleared?
   if (ix.FlagsAccess.Set.PF) // Is PF set?
   if (ix.FlagsAccess.Tested.CF) // Is CF tested?
   
   
Determining if the instruction is supported on the current CPU
--------------------------------------------------------------

.. code-block:: c
   :linenos:
   
    if (ix.CpuidFlag.Leaf == ND_CFF_NO_LEAF)
    {
        // The instruction is present on pretty much all CPUs, 
        // there is no CPUID bit flag test.
    }
    else
    {
        int regs[4];
        int sorted[4];

        if (ix.CpuidFlag.SubLeaf == ND_CFF_NO_SUBLEAF)
        {
            // No subleaf.
            __cpuid(regs, ix.CpuidFlag.Leaf);
        }
        else
        {
            __cpuidex(regs, ix.CpuidFlag.Leaf, ix.CpuidFlag.SubLeaf);
        }

        // regs is defined by MSDN as eax, ebx, ecx, edx, so sort them
        // in Intel order.
        sorted[0] = regs[0]; // eax
        sorted[1] = regs[2]; // ecx
        sorted[2] = regs[3]; // edx
        sorted[3] = regs[1]; // ebx

        if (sorted[ix.CpuidFlag.Reg] & (1ULL << ix.CpuidFlag.Bit))
        {
            // The instruction is supported!
        }
    }
   
Determining if the instruction accepts various prefixes
-------------------------------------------------------

.. code-block:: c
   :linenos:
      
    if (ix.ValidPrefixes.Lock) // The instruction accepts LOCK.
    if (ix.ValidPrefixes.Hle) // The instruction accepts hardware lock elision prefix.
    if (ix.ValidPrefixes.Rep) // The instruction accepts unconditional REP prefix.
    
Determining if the instruction is valid in various modes
--------------------------------------------------------

.. code-block:: c
   :linenos:
      
    if (ix.ValidModes.Real) // The instruction is valid in real mode.
    if (ix.ValidModes.Long) // The instruction is valid in long mode.
    if (ix.ValidModes.Tsx) // The instruction is valid in a transaction.
    if (!ix.ValidModes.Ring3) // The instruction is not valid in user-mode.
    
Determining if the instruction is a branch of any kind
------------------------------------------------------    

.. code-block:: c
   :linenos:
   
   if (ix.RipAccess & ND_ACCESS_ANY_WRITE) // Instruction writes RIP.
   
Computing the virtual address of a memory operand
------------------------------------------------

.. code-block:: c
   :linenos:
   
    // Note: we don't consider bitbase or VSIB addressing.
    if (memOp->Info.Memory.HasBase)
    {
        gla += ND_TRIM(memOp->Info.Memory.BaseSize, REG(memOp->Info.Memory.Base));
    }

    if (memOp->Info.Memory.HasIndex)
    {
        gla += ND_TRIM(memOp->Info.Memory.IndexSize, REG(memOp->Info.Memory.Index)) * memOp->Info.Memory.Scale;
    }

    if (memOp->Info.Memory.HasDisp)
    {
        gla += memOp->Info.Memory.Disp;
    }


