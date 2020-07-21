# Bitdefender Shellocde Emulator Fuzzer

This assumes that you have [AFL](https://github.com/google/AFL) in your path.

It collects the `bddisasm` and `bdshemu` sources into a single executable, built with AFL instrumentation.

## Getting started

Build it with `make shfuzz`.

Start fuzzing with `make fuzz32` (for 32-bit mode samples) or `make fuzz64` (for 64-bit mode samples).

If you're in a hurry you can fuzz in the quick and dirty mode with `DIRTY=y`: `make fuzz32 DIRTY=y` or `make fuzz64 DIRTY=y`.

Activate support for [address sanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer) with `AFL_USE_ASAN=1 make shfuzz`. This can uncover more bugs, but it is a lot slower and requires more memory.

Input files are in the `in-32` and `in-64` directories, crashes will be in `out-32/crashes` or `out-64/crashes`, hangs will be in `out-32/hangs` or `out-64/hangs`.

## Details

If the `AFL_COMPILER` variable is not set, the build process looks for `afl-clang-fast` and if it is available, the tool is compiled with that. If not, it tries to choose between `afl-clang` and `afl-gcc`. 
If you want to use a certain compiler simply set `AFL_COMPILER` before invoking make: `AFL_COMPILER=afl-gcc make`.
Note that `afl-clang-fast` is not compiled by default when compiling AFL. See [llvm mode](https://github.com/google/AFL/tree/master/llvm_mode) in the AFL repository.

Run the fuzzer in 32-bit mode with `make fuzz32`, or in 64-bit mode with `make fuzz64`.

If you want to take advantage of multiple cores see [parallel fuzzing](https://github.com/google/AFL/blob/master/docs/parallel_fuzzing.txt).

If `afl-clang-fast` is used, we try to use AFL in persistent mode, which should provide better performance.

Note that both `fuzz32` and `fuzz64` assume that the tool was compiled with `afl-clang-fast`, `afl-clang`, or `afl-gcc`.

ASAN builds will have increased memory requirements. You may need to tweak the `-m` parameter (or set `AFL_MEMORY`) for `afl-fuzz` (see [notes for ASAN](https://github.com/google/AFL/blob/master/docs/notes_for_asan.txt)).

By default, all logging is disabled, as it is already not visible while AFL is running. Compile with `LOG=y` in order to enable some minimal logging.

## Using the tool

The `shfuzz` tool is a simplified version of `disasmtool`. It takes 3 positional arguments:

```bash
shfuzz file mode use_logging
```

- file - the path to a input file that contains instructions to be disassembled and emulated;
- mode - the mode in which to run: `32` for 32-bit instructions, `64` for 64-bit instruction;
- use_logging - if present, will actually log everything that `bdshemu` wants to log; even if absent, the `ShemuLog` implementation will still try to access every character in the string it should normally print, so if you want to find bugs related to logging you can still do, but it is a lot faster when the strings don't get printed. This is ignored if you compile `shfuzz` without `LOG=y`.

Any errors encountered while parsing the arguments, trying to open the input file, or allocate memory are handled by calling `abort()`. This makes it easier to detect these problems.
