project(BinarySearch C)

include_directories(include)

add_compile_options("-O0")
add_executable(binsearch_iter
    src/main.c
    src/binsearch-iter.c
)

add_executable(binsearch_recursive
    src/main.c
    src/binsearch-recursive.c
)
