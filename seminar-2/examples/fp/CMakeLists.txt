add_executable(fp-example fp-example.c)
add_executable(fp-example-more-digits fp-example.c)
target_compile_definitions(fp-example-more-digits PRIVATE MORE_DIGITS)
