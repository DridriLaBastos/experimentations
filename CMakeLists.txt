cmake_minimum_required(VERSION 3.10)
project(experimentations)

set(CMAKE_C_STANDARD 11) # std::format available since C++20 and its a cool enough feature that it justify the use of C++20
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

if (${MSVC})
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra)
endif()

include_directories(${CMAKE_SOURCE_DIR})

add_executable(acc
	main.c
	src/ast.c
	src/parsing.c
	src/assembly_generator.c
	src/generators/assembly_generator_x86_64.c)

target_include_directories(acc PRIVATE src/generators)
