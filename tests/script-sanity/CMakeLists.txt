# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# bash linting
add_test(NAME shell_script_lint
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bashlint.sh)
