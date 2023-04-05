cmake_minimum_required(VERSION 3.16)

option(BDD_FUZZ_WITH_LOGS "Enable logging for the fuzzer" OFF)

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

add_custom_target(shfuzz DEPENDS shfuzzx86 shfuzzx64)
