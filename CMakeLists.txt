cmake_minimum_required(VERSION 3.12)

include("${CMAKE_CURRENT_LIST_DIR}/project-meta-info.in")

set(disasm_version_file ${CMAKE_CURRENT_LIST_DIR}/inc/version.h)

file(STRINGS ${disasm_version_file} disasm_ver_major REGEX "DISASM_VERSION_MAJOR")
file(STRINGS ${disasm_version_file} disasm_ver_minor REGEX "DISASM_VERSION_MINOR")
file(STRINGS ${disasm_version_file} disasm_ver_patch REGEX "DISASM_VERSION_REVISION")

string(REGEX REPLACE "#define DISASM_VERSION_MAJOR[ \t\r\n]*" "" disasm_ver_major ${disasm_ver_major})
string(REGEX REPLACE "#define DISASM_VERSION_MINOR[ \t\r\n]*" "" disasm_ver_minor ${disasm_ver_minor})
string(REGEX REPLACE "#define DISASM_VERSION_REVISION[ \t\r\n]*" "" disasm_ver_patch ${disasm_ver_patch})

message(STATUS "Extracted version from ${disasm_version_file}: ${disasm_ver_major}.${disasm_ver_minor}.${disasm_ver_patch}")

project(bddisasm
    VERSION ${disasm_ver_major}.${disasm_ver_minor}.${disasm_ver_patch}
    DESCRIPTION ${project_description}
    LANGUAGES C
    )

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type given. Will use 'Release'")
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/x64/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/x64/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/x64/${CMAKE_BUILD_TYPE})

message(STATUS "Output directory set to: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

set(bddisasm_src
    bddisasm/crt.c
    bddisasm/bddisasm.c
    )
add_library(bddisasm STATIC ${bddisasm_src})
set_target_properties(bddisasm PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    C_STANDARD 11
    VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    )

set(bdshemu_src
    bdshemu/bdshemu.c
    )
add_library(bdshemu STATIC ${bdshemu_src})
set_target_properties(bdshemu PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    C_STANDARD 11
    VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    )
add_dependencies(bdshemu bddisasm)

include_directories(
    inc
    bddisasm/include
    )

if (CMAKE_BUILD_TYPE EQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -DNDEBUG")
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -D_DEBUG -DDEBUG")
endif ()

set(BDDISASM_COMPILE_OPTIONS
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
    -Wno-discarded-qualifiers
    -Wnull-dereference
    -Wduplicated-cond
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
    -march=nehalem
    )

target_compile_options(bddisasm PRIVATE ${BDDISASM_COMPILE_OPTIONS})
target_compile_options(bdshemu PRIVATE ${BDDISASM_COMPILE_OPTIONS})

include(GNUInstallDirs)

set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

get_filename_component(public_header_path "${PROJECT_SOURCE_DIR}/inc" REALPATH)
file(GLOB_RECURSE public_headers "${public_header_path}/*.h")
set_target_properties(bddisasm PROPERTIES PUBLIC_HEADER "${public_headers}")

set(DEST_DIR "${CMAKE_INSTALL_PREFIX}")
set(LIB_DIR "${CMAKE_INSTALL_LIBDIR}")
set(INC_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
set(PRIVATE_LIBS "-lbdshemu -lbddisasm")
set(DATA_DIR "${CMAKE_INSTALL_DATADIR}")

CONFIGURE_FILE("${CMAKE_STATIC_LIBRARY_PREFIX}bddisasm.pc.in"
    "${PROJECT_SOURCE_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}bddisasm.pc"
    @ONLY
    )

INSTALL(TARGETS bddisasm bdshemu
    EXPORT bddisasmTargets
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/bddisasm"
    )

set(CMAKE_INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/bddisasm)
install(EXPORT bddisasmTargets
    DESTINATION ${CMAKE_INSTALL_CONFIGDIR}
    )

include(CMakePackageConfigHelpers)

configure_package_config_file(${PROJECT_SOURCE_DIR}/bddisasmConfig.cmake.in ${PROJECT_BINARY_DIR}/bddisasmConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_CONFIGDIR}
    PATH_VARS CMAKE_INSTALL_FULL_INCLUDEDIR CMAKE_INSTALL_FULL_LIBDIR
    )

write_basic_package_version_file(${PROJECT_BINARY_DIR}/bddisasmConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
    )

install(FILES ${PROJECT_BINARY_DIR}/bddisasmConfig.cmake ${PROJECT_BINARY_DIR}/bddisasmConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_CONFIGDIR}
    )

INSTALL(FILES "${PROJECT_SOURCE_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}bddisasm.pc"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/pkgconfig"
    )

if (NOT CPACK_GENERATOR)
    set(CPACK_GENERATOR "DEB")
endif()

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Bitdefender HVI Team <hvmi-oss@bitdefender.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")

if (INCLUDE_TOOL)
    add_subdirectory("disasmtool_lix")
    add_dependencies(disasmtool bdshemu)
endif()

include(CPack)
