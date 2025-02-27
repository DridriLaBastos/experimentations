project(acc)

add_executable(acc main.c
					src/parsing.c
					src/ast.c
					src/assembly_generator.c
					
					src/generators/assembly_generator_x86_64)
target_include_directories(acc PRIVATE ${PROJECT_SOURCE_DIR})
