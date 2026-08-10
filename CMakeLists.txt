cmake_minimum_required(VERSION 3.10)
project(experimentations)

set(CMAKE_CXX_STANDARD 20) # std::format available since C++20 and its a cool enough feature that it justify the use of C++20
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

include(FetchContent)
FetchContent_Declare(sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
	GIT_PROGRESS ON
	)

FetchContent_Declare(box2d
	GIT_REPOSITORY https://github.com/erincatto/box2d
	GIT_TAG v3.0.0
	GIT_PROGRESS ON
	)

FetchContent_MakeAvailable(sfml box2d)

if (${MSVC})
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra)
endif()

add_executable(evolve main.cpp)
target_link_libraries(evolve sfml-graphics sfml-window sfml-system sfml-audio box2d)
