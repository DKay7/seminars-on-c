add_executable(Watch main.c)
target_compile_features(Watch PRIVATE c_std_99)
target_compile_options(Watch PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(Watch PRIVATE LOCAL_TESTING)
