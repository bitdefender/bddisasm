# Disassembler Tests

These tests are used to validate bddisasm. Each test consists of up to three files:
* The binary test file. The name format for this type of file is `name_16|32|64`. No extension must be provided; 16/32/64 indicates disassembly mode
* The output result file. Must be named the same as the binary test file, but with the extension .result
* Optional assembly file, used to generate the binary test file

The `test_all.py` script will iterate all the test folders, and it will run bddisasm on each identified test file.
The result file will be compared with the output of the test run. If they are not the same, the test will fail.
Note that bddisasm will be run with the `-exi` option, in order to dump all possible information about the instructions.

NOTE: This test will assume `disasm` is in the path. Works on Windows only.