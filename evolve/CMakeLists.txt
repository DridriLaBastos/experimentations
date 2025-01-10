project(Evolve DESCRIPTION "Using SFML to toy with evolution algorithms")

add_executable(evolve main.cpp)

target_link_libraries(evolve PRIVATE box2d sfml-graphics)
