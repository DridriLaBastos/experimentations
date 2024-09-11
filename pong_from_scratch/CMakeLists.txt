cmake_minimum_required(VERSION 3.10)

project(PongFromScratch LANGUAGES CXX)

set(CMAKE_C_COMPILER i686-elf-gcc CACHE FILEPATH "C compiler")
set(CMAKE_CXX_COMPILER i686-elf-g++ CACHE STRING "C++ compiler")

add_compile_options(-ffreestanding)
add_link_options(-nostdlib)

add_executable(tmp main.cpp)
