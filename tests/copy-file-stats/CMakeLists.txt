# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# add test for help parameter
add_test(NAME executable_parameter_help
         COMMAND $<TARGET_FILE:copy-file-stats> --help)

# add test for version parameter
add_test(NAME executable_parameter_version
         COMMAND $<TARGET_FILE:copy-file-stats> --version)

# add test for license parameter
add_test(NAME executable_parameter_license
         COMMAND $<TARGET_FILE:copy-file-stats> --license)
