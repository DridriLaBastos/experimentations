cmake_minimum_required(VERSION 3.10)
project(experimentations)

set(CMAKE_CXX_STANDARD 20) # std::format available since C++20 and its a cool enough feature that it justify the use of C++20
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

if (${MSVC})
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(test test.cpp)
add_executable (example_xor_mat examples/mat_xor.cpp)
