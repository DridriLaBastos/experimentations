project(Raytracing LANGUAGES CXX)

add_executable(rt main.cpp
        ppm.cpp
        ppm.hpp
        colors.hpp)
