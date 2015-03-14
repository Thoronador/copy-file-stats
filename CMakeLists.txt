# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

add_subdirectory (program)

# enable testing and add test suite directory
enable_testing()
add_subdirectory (tests)
