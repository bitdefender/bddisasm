# Shellcode Emulator Tests

These tests are used to validate basic bdshemu functionality. Each test consists of up to three files:
* The binary test file. The name format for this type of file is `name_32|64[_r0]`. No extension must be provided; 32 indicates 32 bit test file, 64 indicates 64 bit test file, and r0 indicates kernel payload
* The output result file. Must be named the same as the binary test file, but with the extension .result
* Optional assembly file, used to generate the binary test file

The `test_all.py` script will iterate all the test folders, and it will run bdshemu on each identified test file.
The result file will be compared with the output of the test run. If they are not the same, the test will fail.

NOTE: This test will assume `disasm` is in the path. Works on Windows only.