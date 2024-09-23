# Disassembler & Emulator Tests

These tests are used to validate bdshemu and bddisam functionality. Each test consists of up to three files:
* The binary test file. The name format for this type of file is `name_32|64[_r0].test`. 32 indicates 32 bit test file, 64 indicates 64 bit test file, and r0 indicates kernel payload
* The output result file. Must be named the same as the binary test file, but with the extension .result
* Optional assembly file, used to generate the binary test file
The information about the files mentioned above is stored in a configuration file.


To run the tests, `main.py` script must be called.
```
$ py -3 main.py --help
usage: main.py [-h] [--conf CONF] --disasm DISASM [--nasm NASM] [--report] [--regenerate] [--verbose]

BDSHEMU Test-Suite

options:
  -h, --help       show this help message and exit
  --conf CONF      Path of the conf file
  --disasm DISASM  Path of the disasm tool
  --nasm NASM      Path of the nasm tool
  --report         Generate report
  --regenerate     Regenerate samples
  --verbose        Verbose
```

The `--conf` option is used to specify the configuration file (multiple configuration files allowed).
The `--disasm` option is used to specify the path of the `disasmtool` (required).
The `--nasm` option is used to specify the path of the `nasm` assembler to generate the `.test` files.
The `--report` options is used to generate a report that contains the failed tests.
The `--regenerate` option, regenerates the `.result` files and the `.test` files if missing (requires valid `--nasm`).
The `--verbose` option is used to print more information.

Note that bddisasm will be run with the `-exi` option, in order to dump all possible information about the instructions.

# Configuration File
The configuration file contains a list of entries that each describe a test.
Each entry in the configuration file has the following structure:
```
[
...
{
    "f_source": "path/to/file.asm",     # the path to .asm file
    "f_result": "path/to/file.result",  # the path to .result file
    "f_test": "path/to/file.test",      # the path to .test file
    "options": "disasmtool option",     # options for disasmtool
    "command": "disasmtool command"     # command for disasmtool (decode/shemu)
}
...
]
```

NOTE: The paths in the configuration file are relative to it.