cmake_minimum_required (VERSION 3.12)

project(disasmtool LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type given. Will use 'Release'!")
  set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_CXX_FLAGS)
  message(STATUS "Passed CXXFLAGS: ${CMAKE_CXX_FLAGS}")
endif()

if (("${CMAKE_BUILD_TYPE}" STREQUAL "Debug") AND (CMAKE_CXX_FLAGS_DEBUG))
  message(STATUS "Passed CXXFLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

if (("${CMAKE_BUILD_TYPE}" STREQUAL "Release") AND (CMAKE_CXX_FLAGS_RELEASE))
  message(STATUS "Passed CXXFLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

get_filename_component(DISASM_DIRECTORY "${PROJECT_SOURCE_DIR}/.." REALPATH)
set(DISASM_BUILD_PREFIX "bin/x64")

set(disasmtool_src_files
  disasmtool.cpp
  dumpers.cpp
  )

find_library(DISASM_LIB bddisasm PATHS "${DISASM_DIRECTORY}/${DISASM_BUILD_PREFIX}/${CMAKE_BUILD_TYPE}" NO_DEFAULT_PATH)
message(STATUS "Disasm lib used: ${DISASM_LIB}")

find_package(RapidJSON)

if (RapidJSON_FOUND)
  message(STATUS "Dependency: rapidjson found")
  include_directories(${RapidJSON_INCLUDE_DIRS})
  add_compile_definitions(HAS_RAPIDJSON)
  list(APPEND disasmtool_src_files rapidjson.cpp)
else()
  message(FATAL_ERROR "Dependency: rapidjson not found")
endif()

add_compile_options(
  "$<$<CONFIG:Release>:-U_FORTIFY_SOURCE>"
  "$<$<CONFIG:Release>:-D_FORTIFY_SOURCE=2>"
  -Wall
  -Wextra
  -Wshadow
  -Wformat-security
  -Wstrict-overflow=2
  -Wno-unused-function
  -Wno-multichar
  -Werror=format-security
  -pipe
  -fpie
  -fwrapv
  -fno-strict-aliasing
  -fstack-protector-strong
  -ffunction-sections
  -fdata-sections
  -g3
  -gdwarf-4
  -grecord-gcc-switches
  -march=native
  -fno-omit-frame-pointer
  # -fsanitize=leak
  # -fsanitize=address
  )

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
-Wl,--fatal-warnings \
-Wl,--warn-common \
-Wl,--no-undefined \
-Wl,-z,noexecstack \
-Wl,-z,relro \
-Wl,-z,now \
-Wl,--build-id")

set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -Wl,-O1")

if (CMAKE_COMPILER_IS_GNUCC)
  if (NOT (CMAKE_C_COMPILER_VERSION LESS 6.0))
    add_compile_options(
      -Wshift-overflow=2
      -Wnull-dereference
      -Wduplicated-cond
      -Wlogical-op
      -Wvla
      )
  endif()

  if (NOT (CMAKE_C_COMPILER_VERSION LESS 7.0))
    add_compile_options(
      -Wdangling-else
      -Wshadow=global
      -Walloc-zero
      )
  endif()

  if (NOT (CMAKE_C_COMPILER_VERSION LESS 8.0))
    add_compile_options(
      -Wmultistatement-macros
      -Warray-bounds=2
      -Wformat-overflow=2
      -Wformat-truncation=1
      -Wstringop-truncation
      -Wpointer-arith
      -Wdouble-promotion
      -Wmissing-include-dirs
      -Wuninitialized
      -Wmissing-noreturn
      -Wsuggest-attribute=noreturn
      -Walloca
      -Wtrampolines
      -Wcast-qual
      -Wcast-align
      -Wparentheses
      -Wjump-misses-init
      -Wfloat-conversion
      -Wmissing-prototypes
      -Wredundant-decls
      -Wdisabled-optimization
      -Woverlength-strings
      -fstack-clash-protection
      -static
      )
  endif()
else()
  add_compile_options(
    -Wno-missing-braces
    )

  if (NOT (CMAKE_C_COMPILER_VERSION LESS 6.0))
    add_compile_options(
      -Wshift-overflow
      -Wnull-dereference
      -Wvla
      -Wdangling-else
      -Wshadow
      -Wpragmas
      -Wtautological-compare
      -Wzero-as-null-pointer-constant
      )
  endif()
endif()

include_directories(
  ${DISASM_DIRECTORY}/inc
  ${DISASM_DIRECTORY}/inc/bdshemu
  ${DISASM_DIRECTORY}/bddisasm/include
  )

add_executable(${CMAKE_PROJECT_NAME} ${disasmtool_src_files})

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  include(CheckIPOSupported)
  check_ipo_supported(RESULT USE_IPO)
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION True)
endif()

set_target_properties(${CMAKE_PROJECT_NAME}
  PROPERTIES
  POSITION_INDEPENDENT_CODE ON
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS ON
  )

target_link_libraries(${CMAKE_PROJECT_NAME} ${DISASM_LIB})

unset(DISASM_LIB CACHE)
