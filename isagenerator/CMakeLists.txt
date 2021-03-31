cmake_minimum_required(VERSION 3.12)

project(isagenerator)

include(FindPython3)

find_package(Python3 COMPONENTS Interpreter)
if (Python3_FOUND)
    add_custom_target(
        isagenerator
        COMMAND Python3::Interpreter generate_tables.py instructions
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
        COMMENT "Generating instruction tables")
endif (Python3_FOUND)
