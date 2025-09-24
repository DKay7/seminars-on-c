project(arrays C)


add_library(array-io STATIC array-io.c)
target_include_directories(array-io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(sort)
add_subdirectory(maximum)
