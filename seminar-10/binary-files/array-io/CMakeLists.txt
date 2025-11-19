add_executable(BinaryArrayIO main.c)
target_compile_features(BinaryArrayIO PRIVATE c_std_99)
target_compile_options(BinaryArrayIO PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(BinaryArrayIO PRIVATE BinaryFileUtils)
