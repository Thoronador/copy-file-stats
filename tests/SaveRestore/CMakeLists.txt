# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(mode_test)

set(mode_t_sources
    ../../program/AuxiliaryFunctions.cpp
    ../../program/SaveRestore.cpp
    mode_test.cpp)

add_definitions (-Wall -O2 -fexceptions -std=c++11)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(mode_test ${mode_t_sources})

# add test for saving and restoring file modes
add_test(class_SaveRestore_mode ${CMAKE_CURRENT_BINARY_DIR}/mode_test)
