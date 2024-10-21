cmake_minimum_required(VERSION 3.10)
project(experimentations)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
	GIT_PROGRESS ON
	CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF)
FetchContent_MakeAvailable(sfml)

add_compile_options(-Wall -Wextra)
include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(util)

add_subdirectory(nn)
add_subdirectory(opencv)
add_subdirectory(hardwaresim)
add_subdirectory(raytracing)
add_subdirectory(sfml_game)
