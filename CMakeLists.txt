cmake_minimum_required(VERSION 3.10)
project(experimentations)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra)

add_subdirectory(nn)
add_subdirectory(opencv)
add_subdirectory(hardwaresim)
add_subdirectory(raytracing)
add_subdirectory(sfml_game)
