project(Raytracing LANGUAGES CXX)

add_executable(rt main.cpp
        src/ppm.cpp
        src/ray.cpp)

target_include_directories(rt PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(rt PRIVATE module sfml-graphics)
