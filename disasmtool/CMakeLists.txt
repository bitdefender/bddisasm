cmake_minimum_required(VERSION 3.16)

project(disasmtool LANGUAGES C)

include(FindPython3)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Use Release as the build type if no build type was specified and we're not using a multi-config generator    .
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type given. Will use 'Release'")
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui.
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif ()

if (NOT TARGET bddisasm)
    find_package(bddisasm REQUIRED)
endif ()

# Add main executable target.
add_executable(disasmtool disasmtool.c handle_disasm.c handle_shemu.c)
target_link_libraries(disasmtool PRIVATE bddisasm::bddisasm bddisasm::bdshemu)

# Add tests target.
if (BDD_INCLUDE_TESTS)
    add_custom_target(test ALL
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tests/main.py --conf ${CMAKE_SOURCE_DIR}/bddisasm_test/conf-x86.json --disasm $<TARGET_FILE:disasmtool>
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tests/main.py --conf ${CMAKE_SOURCE_DIR}/bdshemu_test/conf-x86.json --disasm $<TARGET_FILE:disasmtool>
        DEPENDS disasmtool
        COMMENT "Runs decoder and emulator tests"
    )
endif ()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT USE_IPO)
    if (USE_IPO)
        set_target_properties(disasmtool PROPERTIES INTERPROCEDURAL_OPTIMIZATION True)
    endif ()
endif ()

if (MSVC OR "${CMAKE_C_COMPILER_FRONTEND_VARIANT}" STREQUAL "MSVC")
    target_compile_options(
        disasmtool 
        PRIVATE /W4 /WX)

    if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(disasmtool PRIVATE -Wno-format)
    endif ()
else ()
    target_compile_options(
        disasmtool
        PRIVATE -Wall
                -Wno-unknown-pragmas
                -Wextra
                -Wshadow
                -Wno-format
                -Wno-cast-function-type
                -Wstrict-overflow=2
                -Wstrict-prototypes
                -Wwrite-strings
                -Wshadow
                -Winit-self
                -Wno-unused-function
                -Wno-multichar
                -Wno-incompatible-pointer-types
                -Wnull-dereference
                -Werror=implicit-function-declaration
                -pipe
                -fwrapv
                -fno-strict-aliasing
                -fstack-protector-strong
                -fno-omit-frame-pointer
                -ffunction-sections
                -fdata-sections
                -g3
                -gdwarf-4
                -grecord-gcc-switches)
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(
            disasmtool
            PRIVATE -Wno-discarded-qualifiers
                    -Wduplicated-cond)
    endif ()
endif ()

if (BDD_ASAN)
    target_compile_options(disasmtool PUBLIC "-fsanitize=address")
    target_link_options(disasmtool PUBLIC "-fsanitize=address")
endif ()

if (BDD_UBSAN)
    target_compile_options(disasmtool PUBLIC
        "-fsanitize=undefined"
        "-fno-sanitize=alignment")
    target_link_options(disasmtool PUBLIC
        "-fsanitize=undefined"
        "-fno-sanitize=alignment")
endif ()

if (BDD_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT USE_IPO)
    if (USE_IPO)
        set_target_properties(disasmtool PROPERTIES INTERPROCEDURAL_OPTIMIZATION True)
    endif ()
endif ()
