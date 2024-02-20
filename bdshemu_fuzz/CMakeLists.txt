cmake_minimum_required(VERSION 3.16)

option(BDD_FUZZ_WITH_LOGS "Enable logging for the fuzzer" OFF)
option(BDD_FUZZ_DIRECT_MAP "Enable direct SHEMU_OPT_DIRECT_MAPPED_SHELL" OFF)

project(bdshemu_fuzzer LANGUAGES C)

add_executable(shfuzzx86 bdshemu_fuzzer.c)
target_link_libraries(shfuzzx86 PRIVATE bddisasm::bdshemu)
target_compile_definitions(shfuzzx86 PRIVATE FUZZ_X86)

add_executable(shfuzzx64 bdshemu_fuzzer.c)
target_link_libraries(shfuzzx64 PRIVATE bddisasm::bdshemu)
target_compile_definitions(shfuzzx64 PRIVATE FUZZ_X64)

if (BDD_FUZZ_WITH_LOGS)
    target_compile_definitions(shfuzzx86 PRIVATE ENABLE_LOGGING)
    target_compile_definitions(shfuzzx64 PRIVATE ENABLE_LOGGING)
endif (BDD_FUZZ_WITH_LOGS)

if (BDD_FUZZ_DIRECT_MAP)
    target_compile_definitions(shfuzzx86 PRIVATE DIRECT_MAP)
    target_compile_definitions(shfuzzx64 PRIVATE DIRECT_MAP)
endif (BDD_FUZZ_DIRECT_MAP)

# Using CMAKE_C_COMPILER_ID to check for this will not work because afl-gcc is reported as gcc, while afl-clang and
# afl-clang-fast are reported as clang.
# We also don't want to use libfuzzer with AFL because it seems to have some build issues.
# TODO: but it should work, see https://chromium.googlesource.com/chromium/src/+/master/testing/libfuzzer/AFL_integration.md#how
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" AND NOT "${CMAKE_C_COMPILER}" MATCHES "afl-.*")
    message(STATUS "Will use libfuzzer")

    target_compile_options(shfuzzx86 PRIVATE -fsanitize=fuzzer)
    target_link_libraries(shfuzzx86 PRIVATE -fsanitize=fuzzer)

    target_compile_options(shfuzzx64 PRIVATE -fsanitize=fuzzer)
    target_link_libraries(shfuzzx64 PRIVATE -fsanitize=fuzzer)
endif ()

add_custom_target(shfuzz DEPENDS shfuzzx86 shfuzzx64)
