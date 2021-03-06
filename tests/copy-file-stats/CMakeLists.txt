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

# add test for --save parameter
add_test(NAME executable_save
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/save/save.sh $<TARGET_FILE:copy-file-stats>)

# add 2nd test for --save parameter
add_test(NAME executable_save_slash
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/save/save_with_slash.sh $<TARGET_FILE:copy-file-stats>)

# add test for --restore parameter (chmod only so far)
add_test(NAME executable_restore_chmod
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/restore/restore_chmod.sh $<TARGET_FILE:copy-file-stats>)

# add 2nd test for --restore parameter (chmod only so far)
add_test(NAME executable_restore_chmod_slash
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/restore/restore_chmod_slash.sh $<TARGET_FILE:copy-file-stats>)


# add test for directory-to-directory copy of stats
add_test(NAME executable_dir_to_dir_chmod
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/directory_to_directory/source_to_destination.sh $<TARGET_FILE:copy-file-stats>)

# add test for directory-to-directory copy of stats with slash on source
add_test(NAME executable_dir_to_dir_chmod_slash10
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/directory_to_directory/source_to_destination_slash10.sh $<TARGET_FILE:copy-file-stats>)

# add test for directory-to-directory copy of stats with slash on destination
add_test(NAME executable_dir_to_dir_chmod_slash01
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/directory_to_directory/source_to_destination_slash01.sh $<TARGET_FILE:copy-file-stats>)

# add test for directory-to-directory copy of stats with slash on both dirs
add_test(NAME executable_dir_to_dir_chmod_slash11
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/directory_to_directory/source_to_destination_slash11.sh $<TARGET_FILE:copy-file-stats>)
