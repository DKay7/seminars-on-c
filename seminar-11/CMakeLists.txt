cmake_minimum_required(VERSION 3.16)
project(seminar-11 C)

add_library(list STATIC list.c list_algorithms.c)
target_include_directories(list PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(list_demo main.c)
target_link_libraries(list_demo PRIVATE list)
