cmake_minimum_required(VERSION 3.16)

option(BDD_INCLUDE_TOOL "Include the disasmtool executable" ON)
option(BDD_INCLUDE_ISAGENERATOR "Include the isagenerator target (if a python interpreter is found)" ON)
option(BDD_INCLUDE_FUZZERS "Include the bdshemu fuzzer" OFF)
option(BDD_USE_EXTERNAL_VSNPRINTF "Expect nd_vsnprintf_s implementation from the integrator" OFF)
option(BDD_USE_EXTERNAL_MEMSET "Expect nd_memset implementation from the integrator" OFF)

set(BDD_VER_FILE ${CMAKE_CURRENT_LIST_DIR}/inc/version.h)

file(STRINGS ${BDD_VER_FILE} disasm_ver_major REGEX "DISASM_VERSION_MAJOR")
file(STRINGS ${BDD_VER_FILE} disasm_ver_minor REGEX "DISASM_VERSION_MINOR")
file(STRINGS ${BDD_VER_FILE} disasm_ver_patch REGEX "DISASM_VERSION_REVISION")

string(REGEX REPLACE "#define DISASM_VERSION_MAJOR[ \t\r\n]*" "" disasm_ver_major ${disasm_ver_major})
string(REGEX REPLACE "#define DISASM_VERSION_MINOR[ \t\r\n]*" "" disasm_ver_minor ${disasm_ver_minor})
string(REGEX REPLACE "#define DISASM_VERSION_REVISION[ \t\r\n]*" "" disasm_ver_patch ${disasm_ver_patch})

message(STATUS "Extracted version from ${BDD_VER_FILE}: ${disasm_ver_major}.${disasm_ver_minor}.${disasm_ver_patch}")

project(
    bddisasm
    VERSION ${disasm_ver_major}.${disasm_ver_minor}.${disasm_ver_patch}
    DESCRIPTION "Bitdefender x86 instruction decoder and emulator"
    LANGUAGES C
    HOMEPAGE_URL https://github.com/bitdefender/bddisasm)

# Use Release as the build type if no build type was specified and we're not using a multi-config generator    .
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type given. Will use 'Release'")
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui.
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif ()

# These are shared by bddisasm and bdshemu.
if (MSVC OR "${CMAKE_C_COMPILER_FRONTEND_VARIANT}" STREQUAL "MSVC")
    set(BDDISASM_COMMON_COMPILE_OPTIONS /W4 /WX)
else ()
    set(BDDISASM_COMMON_COMPILE_OPTIONS
        "$<$<CONFIG:Release>:-U_FORTIFY_SOURCE>"
        "$<$<CONFIG:Release>:-D_FORTIFY_SOURCE=2>"
        -Wall
        -Wno-unknown-pragmas
        -Wextra
        -Wshadow
        -Wformat-security
        -Wstrict-overflow=2
        -Wstrict-prototypes
        -Wwrite-strings
        -Wshadow
        -Winit-self
        -Wno-unused-function
        -Wno-multichar
        -Wno-incompatible-pointer-types
        -Wnull-dereference
        -Werror=format-security
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
        -grecord-gcc-switches
        -march=westmere)

    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        list(APPEND BDDISASM_COMMON_COMPILE_OPTIONS
            -Wduplicated-cond
            -Wno-discarded-qualifiers)
    endif ()
endif ()

set(BDDISASM_PUBLIC_HEADERS
    "inc/bddisasm.h"
    "inc/constants.h"
    "inc/cpuidflags.h"
    "inc/disasmstatus.h"
    "inc/disasmtypes.h"
    "inc/registers.h"
    "inc/version.h")

include(GNUInstallDirs)

set(BDDISASM_INSTALL_INCLUDE_DIR
    "${CMAKE_INSTALL_INCLUDEDIR}/bddisasm"
    CACHE STRING "Path to bddisasm public include files.")

# -- bddisasm --

include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckCCompilerFlag)

add_library(
    bddisasm STATIC
    bddisasm/crt.c
    bddisasm/bddisasm.c
    bddisasm/bdformat.c
    bddisasm/bdhelpers.c
    # Add the headers so they will show up in IDEs.
    bddisasm/include/instructions.h
    bddisasm/include/mnemonics.h
    bddisasm/include/nd_crt.h
    bddisasm/include/prefixes.h
    bddisasm/include/table_evex.h
    bddisasm/include/table_root.h
    bddisasm/include/table_vex.h
    bddisasm/include/table_xop.h
    bddisasm/include/tabledefs.h
    "${BDDISASM_PUBLIC_HEADERS}")

if (NOT BDD_USE_EXTERNAL_VSNPRINTF)
    check_function_exists(vsnprintf HAS_VSNPRINTF)
    if (HAS_VSNPRINTF)
        target_compile_definitions(bddisasm PUBLIC -DBDDISASM_HAS_VSNPRINTF)
    else ()
        # See https://cmake.org/Bug/view.php?id=15659
        check_symbol_exists(vsnprintf stdio.h HAS_VSNPRINTF_SYMBOL)
        if (HAS_VSNPRINTF_SYMBOL)
            target_compile_definitions(bddisasm PUBLIC -DBDDISASM_HAS_VSNPRINTF)
        endif ()
    endif ()
endif ()

if (NOT BDD_USE_EXTERNAL_MEMSET)
    check_function_exists(memset HAS_MEMSET)
    if (HAS_MEMSET)
        target_compile_definitions(bddisasm PUBLIC -DBDDISASM_HAS_MEMSET)
    endif ()
endif ()

set_target_properties(
    bddisasm
    PROPERTIES POSITION_INDEPENDENT_CODE ON
               C_STANDARD 11
               VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

target_include_directories(bddisasm PRIVATE bddisasm/include)
target_include_directories(bddisasm PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
                                           $<INSTALL_INTERFACE:${BDDISASM_INSTALL_INCLUDE_DIR}>)

target_compile_options(bddisasm PRIVATE ${BDDISASM_COMMON_COMPILE_OPTIONS})

set_target_properties(
    bddisasm
    PROPERTIES PUBLIC_HEADER "${BDDISASM_PUBLIC_HEADERS}"
               VERSION ${CMAKE_PROJECT_VERSION}
               SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR})

add_library(bddisasm::bddisasm ALIAS bddisasm)

# -- bdshemu --

add_library(
    bdshemu STATIC
    bdshemu/bdshemu.c
    # Add the headers so they will show up in IDEs.
    inc/bdshemu.h)

set_target_properties(
    bdshemu
    PROPERTIES POSITION_INDEPENDENT_CODE ON
               C_STANDARD 11
               VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

target_include_directories(bdshemu PRIVATE bddisasm/include)
target_include_directories(bddisasm PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
                                           $<INSTALL_INTERFACE:${BDDISASM_INSTALL_INCLUDE_DIR}>)

target_link_libraries(bdshemu PUBLIC bddisasm)

target_compile_options(bdshemu PRIVATE ${BDDISASM_COMMON_COMPILE_OPTIONS})

check_c_compiler_flag(-maes HAS_MAES)
if (HAS_MAES)
    target_compile_options(bdshemu PRIVATE -maes)
endif ()

set_target_properties(
    bdshemu
    PROPERTIES PUBLIC_HEADER "inc/bdshemu.h"
               VERSION ${CMAKE_PROJECT_VERSION}
               SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR})

add_library(bddisasm::bdshemu ALIAS bdshemu)

# If this is the master project (and if the user requested it) add disasmtool.
if ((${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME}) AND BDD_INCLUDE_TOOL)
    if (WIN32)
        add_subdirectory(disasmtool)
    else ()
        add_subdirectory(disasmtool_lix)
    endif ()
endif ()

# If this is the master project (and if the user requested it) add isagenerator.
if ((${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME}) AND BDD_INCLUDE_ISAGENERATOR)
    add_subdirectory(isagenerator)
endif ()

# If this is the master project (and if the user requested it) add the fuzzer.
if ((${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME}) AND BDD_INCLUDE_FUZZERS)
    add_subdirectory(bdshemu_fuzz)
endif ()

# If this is the master project add install and package targets.
if (${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    set(BDDISASM_INSTALL_CMAKEDIR
        "${CMAKE_INSTALL_LIBDIR}/cmake/bddisasm"
        CACHE STRING "Path to bddisasm cmake files.")

    set(BDDISASM_INSTALL_PCDIR
        "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
        CACHE STRING "Path to bddisasm pkgconfig files.")

    set(CMAKE_SKIP_BUILD_RPATH TRUE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

    install(
        TARGETS bddisasm bdshemu
        EXPORT bddisasmTargets
        INCLUDES
        DESTINATION ${BDDISASM_INSTALL_INCLUDE_DIR}
        PUBLIC_HEADER DESTINATION ${BDDISASM_INSTALL_INCLUDE_DIR} COMPONENT bddisasm_Development
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT bddisasm_Runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT bddisasm_Runtime
                NAMELINK_COMPONENT bddisasm_Development
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT bddisasm_Development)
    
    if (BDD_INCLUDE_TOOL)
        install(
            TARGETS disasmtool
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT bddisasm_Runtime)
    endif ()

    install(
        EXPORT bddisasmTargets
        DESTINATION ${BDDISASM_INSTALL_CMAKEDIR}
        NAMESPACE bddisasm::
        FILE bddisasmTargets.cmake
        COMPONENT bddisasm_Development)

    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(
        "bddisasmConfigVersion.cmake"
        VERSION ${CMAKE_PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

    install(
        FILES bddisasmConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/bddisasmConfigVersion.cmake
        DESTINATION ${BDDISASM_INSTALL_CMAKEDIR}
        COMPONENT bddisasm_Development)

    configure_file("bddisasm.pc.in" "${CMAKE_STATIC_LIBRARY_PREFIX}bddisasm.pc" @ONLY)

    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}bddisasm.pc"
        DESTINATION "${BDDISASM_INSTALL_PCDIR}"
        COMPONENT bddisasm_Development)

    set(CPACK_PACKAGE_VENDOR "Bitdefender")

    if (NOT CPACK_GENERATOR)
        if (NOT WIN32)
            set(CPACK_GENERATOR "DEB")
        else ()
            set(CPACK_GENERATOR "ZIP")
        endif ()
    endif ()

    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Bitdefender HVI Team <hvmi-oss@bitdefender.com>")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_DEBIAN_PACKAGE_SECTION "devel")

    include(CPack)
endif ()
