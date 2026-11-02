add_executable(Pirates main.c)
target_compile_features(Pirates PRIVATE c_std_99)
target_compile_options(Pirates PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(Pirates PRIVATE LOCAL_TESTING)
