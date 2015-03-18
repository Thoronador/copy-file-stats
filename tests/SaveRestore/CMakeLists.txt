# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# mode test
project(mode_test)

set(mode_t_sources
    ../../program/AuxiliaryFunctions.cpp
    ../../program/FileUtilities.cpp
    ../../program/SaveRestore.cpp
    mode_test.cpp)

add_definitions (-Wall -O2 -fexceptions -std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(mode_test ${mode_t_sources})

# add test for saving and restoring file modes
add_test(class_SaveRestore_mode_codec ${CMAKE_CURRENT_BINARY_DIR}/mode_test)


# test for SaveRestore::stringToMode() function
project(string_to_mode)

set(string_to_mode_sources
    ../../program/AuxiliaryFunctions.cpp
    ../../program/FileUtilities.cpp
    ../../program/SaveRestore.cpp
    string_to_mode.cpp)

add_definitions (-Wall -O2 -fexceptions -std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(string_to_mode ${string_to_mode_sources})

# add test for getting file modes from strings
add_test(class_SaveRestore_stringToMode ${CMAKE_CURRENT_BINARY_DIR}/string_to_mode)



# stat file test
project(stat_file_test)

set(stat_file_test_sources
    ../../program/AuxiliaryFunctions.cpp
    ../../program/FileUtilities.cpp
    ../../program/SaveRestore.cpp
    statFile/stat_file_test.cpp)

add_definitions (-Wall -O2 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(stat_file_test ${stat_file_test_sources})

# add scripted stat file test
add_test(NAME class_SaveRestore_save
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/statFile/stat_file_test.sh $<TARGET_FILE:stat_file_test>)
