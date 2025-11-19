add_library(BinaryFileUtils STATIC file_utils.c)
target_compile_features(BinaryFileUtils PRIVATE c_std_99)
target_compile_options(BinaryFileUtils PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(BinaryFileUtils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
