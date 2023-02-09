# Introduction
This project is used to generate all the C header files and structures used by the main disassembler. The project contains two main components:

1. Instruction specifications (inside the instructions folder)
2. Generation scripts (disasmlib.py & generate_tables.py)

The instruction specification also contains additional information such as CPUID feature flag, valid operating modes or FLAGS access.
There are two scripts in the project: the disasmlib.py, which is a library used to parse to instruction specifications and provide all the information in classes, and the generate_tables.py, which uses the disasmlib.py library in order to generate the relevant C files & structures. Adding support for a brand new instruction is usually done by simply adding it inside the relevant table file. The modes, flags or cpuid files may also be modified, if the instruction operates on the flags, has some operating mode restrictions and requires a certain CPUID flag to be set. Normally, the disasmlib.py and generate_tables.py scripts don't need to be modified; the only exception to this is if a new encoding is created, which would also require the main disassembler to be updated.

## Instruction specification syntax
Instruction specifiers have the following format:
mnemonic; explicit operands; implicit operands; encoding; flags

NOTE: Everything is case sensitive, except for the mnemonic.

### Mnemonic
This is plain and simple the instruction mnemonic, as defined by the Intel docs.

### Explicit operands
This sections lists all the explicit operands of the instruction. An operand is explicit if it needs to be manually specified when assembling an instruction. For example, the instruction "ADD eax, ecx" has two explicit operands: "eax" and "ecx". The format for the operands can be seen in the "Operands specification syntax" section.

### Implicit operands
Many instructions operate on additional implicit operands. A very good example is the flags register, which is tested or modified by many instructions, but it doesn't have to be specified in the instruction. In our previous example, "ADD eax, ecx" has two explicit operands, but it also has an implicit operand, the "FLAGS" register which is modified according to the result. The format for the operands can be seen in the "Operands specification syntax" section.

## Operands specification syntax

### Type + size specifiers

#### Operand type
Please refer to valid_optype inside disasmlib.py for the list & description of valid operand type.

#### Operand size
Please refer to valid_opsize inside disasmlib.py for the list & description of valid operand sizes.

### Hard-coded specifiers
Please refer to valid_impops inside disasmlib.py for the list & description of valid implicit operands.

## Instruction encoding
Instruction encoding must follow the explicit & implicit operands sections, and must be enclosed in square brackets.

### Legacy encoding
The following specifiers can be used to describe a legacy instruction encoding:
- 0x66 - the 0x66 prefix must be present
- 0xF3 - the 0xF3 prefix must be present
- 0xF2 - the 0xF2 prefix must be present
- NP - the instruction must be encoded without 0x66, 0xF3 and 0xF2 prefixes; presence of any of them will cause a #UD
- a0xF3 - the 0xF3 prefix must be present, and its absence encodes another instruction. This is used for instructions
          which are still valid with 0x66 prefix (for example, PAUSE vs NOP)
- o64 - the instruction encoding is valid only in 64 bit mode
- rexw - REX prefix must be present and the REX.W bit must be set
- rex - REX prefix must be present
- rep - any REP prefix must be present
- ds16 - encoding valid when data size is 16 bit
- ds32 - encoding valid when data size is 32 bit
- ds64 - encoding valid when data size is 64 bit
- dds64 - encoding valid when default data size is 64 bit
- fds64 - encoding valid when data size is forced to 64 bit
- as16 - encoding valid when 16 bit addressing is used
- as32 - encoding valid when 32 bit addressing is used
- as64 - encoding valid when 64 bit addressing is used
- /r - a modrm byte follows the instruction
- /x - where x is a natural number in the interval [0, 7], indicates that the encoding requires the modrm.reg field
       to have the indicated value x (for example: /3 indicates that the encoding requires modrm.reg == 3)
- /0xXX - where 0xXX is a hex number, indicates that the modrm value must be fixed to 0xXX
- /x:mem - like /x, but modrm.mod must encode a memory access (modrm.mod != 3)
- /x:reg - like /x, but modrm.mod must encode a reg access (modrm.mod == 3)
- modrmpmp - prefer the modrm.mod for encoding redirection instead of modrm.reg
- ib - an immediate byte follows the instruction
- iw - an immediate word follows the instruction
- iz - an immediate word or dword follows the instruction
- iv - an immediate word, dword or qword follows the instruction
- cb - a byte encoded relative offset follows the instruction
- cz - a word or dword encoded relative offset follows the instruction
- cp - a far pointer follows the instruction
- bitbase - instruction uses bitbase addressing
- mib - instruction uses MIB addressing
- 0xXX - instruction opcode (at least one must be present)
- is4 - a registers encoded in a following immediate byte

### XOP encoding
An instruction uses XOP encoding if the keyword xop is used as the first specifier. Other specifiers:
- m - indicates the XOP encoding map
- w - indicates the value of the XOP.W flag; can be 0 or 1

### VEX encoding
An instruction uses VEX encoding if the keyword vex is used as the first specifier. Other specifiers:
- m - indicates the VEX encoding map (1 - 0x0F, 2 - 0x0F 0x38, 3 - 0x0F 0x3A)
- p - indicates the VEX legacy compressed prefix value (0 - NP, 1 - 0x66, 2 - 0xF3, 3 - 0xF2)
- w - indicates the VEX.W value; (0 - 32 bit, 1 - 64 bit, x - both values are valid, i - ignored)
- l - indicates the VEX vector length (0 - 128 bit, 1 - 256 bit, x - both values are valid, i - ignored)
- vsib - VSIB addressing is used

### EVEX encoding
An instruction uses EVEX encoding if the keyword evex is used as the first specifier. Other specifiers:
- m - indicates the EVEX encoding map (1 - 0x0F, 2 - 0x0F 0x38, 3 - 0x0F 0x3A)
- p - indicates the VEX legacy compressed prefix value (0 - NP, 1 - 0x66, 2 - 0xF3, 3 - 0xF2)
- w - indicates the VEX.W value; (0 - 32 bit, 1 - 64 bit, x - both values are valid, i - ignored)
- l - indicates the VEX vector length (0 - 128 bit, 1 - 256 bit, 2 - 512 bit, x - all values are valid, i - ignored)
- vsib - VSIB addressing is used

## Flags
Zero or more flags can follow the instruction encoding, specifying additional information about the instruction. Each flag is preceded by a keyword formed of only one letter.

### Instruction set - s
Defining an instruction set will automatically define a new constant inside the constants.h file. Anything can be specified for the instruction set. Specifying, for example, the set "TEST" would automatically define "ND_ISA_TEST" which can then be used to identify instructions belonging to that set. If the set is not specified, the default set "UNKNOWN" will be used.

### Instruction type - t
Defining an instruction type will automatically define a new constant inside the constants.h file. Anything can be specified for the instruction type. Specifying, for example, the category "TEST" would automatically define "ND_CAT_TEST" which can then be used to identify instructions. Absence of the type specifier will default to the "UNKNOWN" instruction category.

### Instruction class - c
By default, the mnemonic defines the instruction class. Specifying something else will override this (for example, in case of LODSB/LODSW/LODSD/LODSQ, the generic LODS instruction class is specified). If the mnemonic is "TEST", for example, the class "ND_INS_TEST" will be defined automatically which can then be used to identify the instruction. Explicitly specifying the class will override the default value.

### Attributes - a
The instruction attributes are the instruction properties (for example, that the modrm is present, or the instruction uses mandatory masking). Please refer to valid_attributes inside disasmlib.py for the list of valid attributes.

### Operand access map - w
For each instruction operand (both explicit and implicit), the access type must be specified using the w keyword. In order to separate access for each operand, use |. Please refer to valid_access inside disasmlib.py for valid options. Note that in case of EVEX instructions that support masking, the mask register is considered a separate operand. For example, given the instruction "ADD Ev,Gv Fv" would need the access to be specified for all three operands like follows: "w:RW|R|W", which means the first operand (Ev) is read and written, the second one (Gv) is read and the third (Fv) is written.

### Accepted prefixes map - p
Indicates the prefixes accepted by the instruction. Please refer to valid_prefixes inside disasmlib.py for valid prefixes. In some cases (for example, the "LOCK" prefix), if the instruction is encoded using the "LOCK" prefix but this is not specified in the instruction description, decoding will fail. In all the other cases, the prefix is simply ignored - it will be digested by the decoder, but it will not be shown in the disassembly, nor it will cause the instruction description to be different, aside from the fact that the relevant field indicating the presence of the prefix being set.

### CPUID feature flag - i
Indicates the CPUID feature flag required to identify the support for the instruction. CPUID feature flags are defined inside the cpuid.dat flag. The CPUID feature flags defined in the cpuid.dat flag can then be specified to the i directive.

### Valid operating modes - m
This indicates what modes the instruction is valid in. The modes are defined inside the modes.dat file, and can then be used with the m directive.
NOTE: The mode can be specified inline inside the m directive as well.

### Flags access - f
This indicates the per flag access of the instruction. The flags access are defined inside the flags.dat file, and can then be used with the f directive.
NOTE: The flags access can be specified inline inside the f directive as well.

### EVEX tuple type - l
Specifies the EVEX tuple type. Please refer to valid_tuples inside disasmlib.py for more info.

### EVEX exception class - e
Specifies the EVEX exception class, as defined by Intel.

### FPU flags access - u
Indicates the access type for the FPU status word flags C0, C1, C2, C3. Applicable to x87 instructions only. Each flag can be:
- 0 - cleared to 0
- 1 - set to 1
- m - modified according to a result
- u - undefined or unaffected
Specification syntax is similar to the regular flags: "u:C0=m|C1=u|C2=0|C3=1". Missing flags will be undefined by default.

## Important files

The instructions folder contains all the relevant files:

- table_* - contain the actual instruction encodings.
- cpuid.dat - contains the CPUID feature flags used by the instructions.
- modes.dat - contains the valid operating modes used by the instructions.
- flags.dat - contains the flags access for the instructions.

Generic example:

```
Mnemonic        Op1,Op2      Op3        [encoding]       s:SET, t:TYPE, c:CLASS, p:PREFIXES, w:R|R|R|R|R, f:CF=m, i:CPUID

Anatomy of an instruction specifier:
Mnemonic       Explicit ops  Implicit ops   Encoding                                Attributes
CMPXCHG      ; Eb,Gb       ; AL,Fv          ;            0x0F 0xB0 /r             ; s:I486REAL, t:SEMAPHORE, w:RCW|R|RCW|W, f:ARITH, p:LOCK|HLE
   A           A  A          A  A                        A    A    A                A           A            A              A        A
   |           |  |          |  |                        |    |    |                |           |            |              |        |
   |           |  |          |  |                        |    |    |                |           |            |              |        |
   |           |  |          |  |                        |    |    |                |           |            |              |        +---------- Accepted prefixes: "LOCK" and "HLE"
   |           |  |          |  |                        |    |    |                |           |            |              +------------------- Instruction flags operation: arithmetic class
   |           |  |          |  |                        |    |    |                |           |            +---------------------------------- Operand access map: operand 1 (Eb) is read, but conditionally written
   |           |  |          |  |                        |    |    |                |           |                                                                    operand 2 (Gb) is read
   |           |  |          |  |                        |    |    |                |           |                                                                    operand 3 (AL) is read and conditionally written
   |           |  |          |  |                        |    |    |                |           |                                                                    operand 4 (Fv) is written
   |           |  |          |  |                        |    |    |                |           +----------------------------------------------- Instruction type: "SEMAPHORE"
   |           |  |          |  |                        |    |    |                +----------------------------------------------------------- Instruction set: "I486REAL"
   |           |  |          |  |                        |    |    +---------------------------------------------------------------------------- The instruction has a modrm byte following
   |           |  |          |  |                        |    +--------------------------------------------------------------------------------- Second opcode is "0xB0"
   |           |  |          |  |                        +-------------------------------------------------------------------------------------- First opcode is "0x0F"
   |           |  |          |  +--------------------------------------------------------------------------------------------------------------- Implicit operand 2: the RFLAGS register
   |           |  |          +------------------------------------------------------------------------------------------------------------------ Implicit operand 1: the 8 bit register "AL"
   |           |  +----------------------------------------------------------------------------------------------------------------------------- Explicit operand 2: "G" = general purpose register encoded in modrm.reg
   |           |                                                                                                                                                     "b" = 1 byte in size
   |           +-------------------------------------------------------------------------------------------------------------------------------- Explicit operand 1: "E" = general purpose register or memory, encoded in modrm.rm, 
   |                                                                                                                                                                 "b" = 1 byte in size
   +-------------------------------------------------------------------------------------------------------------------------------------------- The instruction mnemonic: "CMPXCHG"
```