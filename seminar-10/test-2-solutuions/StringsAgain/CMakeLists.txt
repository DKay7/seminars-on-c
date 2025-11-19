add_executable(StringsAgain main.c)
target_compile_features(StringsAgain PRIVATE c_std_99)
target_compile_options(StringsAgain PRIVATE -Wall -Wextra -Wpedantic)
