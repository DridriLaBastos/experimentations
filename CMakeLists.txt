cmake_minimum_required(VERSION 3.10)
project(experimentations)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

include(FetchContent)
FetchContent_Declare(sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
	GIT_PROGRESS ON
	)

set(GLM_BUILD_LIBRARY OFF CACHE INTERNAL "")
set(GLM_BUILD_INSTALL OFF CACHE INTERNAL "")
set(GLM_ENABLE_CXX_17 ON CACHE INTERNAL "")
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
	GIT_PROGRESS ON
	)

FetchContent_Declare(box2d
	GIT_REPOSITORY https://github.com/erincatto/box2d
	GIT_TAG v3.0.0
	GIT_PROGRESS ON
	)

FetchContent_MakeAvailable(sfml glm box2d)

if (${MSVC})
	add_compile_options(W4)
else()
	add_compile_options(-Wall -Wextra)
endif()
include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(util)

add_subdirectory(nn)
add_subdirectory(pong_from_scratch)
add_subdirectory(hardwaresim)
add_subdirectory(raytracing)
add_subdirectory(sfml_game)
add_subdirectory(evolve)
