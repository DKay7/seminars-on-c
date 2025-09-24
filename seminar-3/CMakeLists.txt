cmake_minimum_required(VERSION 3.6)
project(sem-3 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

add_subdirectory(out-params)
add_subdirectory(gcd)
add_subdirectory(arrays)
