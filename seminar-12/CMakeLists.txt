cmake_minimum_required(VERSION 3.16)
project(seminar-12 C)

add_executable(bintree main.c bintree.c)
target_compile_options(bintree PRIVATE -Wall -Wextra)
target_include_directories(bintree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(bintree PRIVATE -Wall -Wextra)
