# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(copy-file-stats)

set(cfs_sources
    AuxiliaryFunctions.cpp
    FileUtilities.cpp
    SaveRestore.cpp
    main.cpp)

message ( "Info: CMAKE_CXX_COMPILER is set to ${CMAKE_CXX_COMPILER}." )

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(copy-file-stats ${cfs_sources})

# enable testing and add test suite directory
enable_testing()
add_subdirectory (tests)
