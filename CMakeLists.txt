cmake_minimum_required(VERSION 3.13)

include("${CMAKE_CURRENT_LIST_DIR}/project-meta-info.in")

project(bddisasm
    VERSION ${project_version}
    DESCRIPTION ${project_description}
    LANGUAGES C
    )

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type given. Will use 'Debug'")
    set(CMAKE_BUILD_TYPE Debug)
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
    inc/bdshemu    
    bddisasm/include
    )

if (CMAKE_BUILD_TYPE EQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -DNDEBUG")
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -D_DEBUG -DDEBUG")
endif ()

add_compile_options(
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
    -Wswitch-default
    -Wmissing-variable-declarations
    -Wused-but-marked-unused
    -Wswitch-enum
    -Wno-unused-function
    -Wno-multichar
    -Wno-incompatible-pointer-types
    -Wcast-qual
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
set(PRIVATE_LIBS "-lbddisasm -lbdshemu")
set(DATA_DIR "${CMAKE_INSTALL_DATADIR}")

CONFIGURE_FILE("${CMAKE_STATIC_LIBRARY_PREFIX}bddisasm.pc.in"
    "${PROJECT_SOURCE_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}bddisasm.pc"
    @ONLY
    )

INSTALL(TARGETS bddisasm bdshemu
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/bddisasm"
    )

INSTALL(FILES "${PROJECT_SOURCE_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}bddisasm.pc"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/pkgconfig"
    )
