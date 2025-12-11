cmake_minimum_required(VERSION 3.16)
project(avl C)

add_executable(avl main.c avl.c)
target_include_directories(avl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(avl PRIVATE -Wall -Wextra)
