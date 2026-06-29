cmake_minimum_required(VERSION 3.10)
project(dumbcoin)

set(CMAKE_CXX_STANDARD 20) # std::format available since C++20 and its a cool enough feature that it justify the use of C++20
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

add_executable(dumbc main.cpp)


target_compile_options(dumbc PRIVATE -Wall -Wextra)
