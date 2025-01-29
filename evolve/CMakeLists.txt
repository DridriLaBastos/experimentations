project(Evolve DESCRIPTION "Using SFML to toy with evolution algorithms")

add_executable(evolve main.cpp)

target_link_libraries(evolve PRIVATE box2d sfml-graphics)
target_compile_definitions(evolve PRIVATE RESOURCE_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/resources")
