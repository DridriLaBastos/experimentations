project(Evolve DESCRIPTION "Using SFML to toy with evolution algorithms")

include (FetchContent)

set (FASTNOISE2_STANDALONE_PROJECT OFF CACHE INTERNAL "" FORCE)
FetchContent_Declare(FastNoiseSIMD2
	GIT_REPOSITORY https://github.com/Auburn/FastNoise2
	GIT_TAG v0.10.0-alpha
	GIT_PROGRESS ON
	)

FetchContent_MakeAvailable(FastNoiseSIMD2)

add_executable(evolve main.cpp)

target_link_libraries(evolve PRIVATE box2d sfml-graphics FastNoise2)
target_compile_definitions(evolve PRIVATE RESOURCE_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/resources")
