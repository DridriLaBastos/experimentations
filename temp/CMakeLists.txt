project(temp)

add_executable(temp main.cpp)

add_library(static STATIC static.cpp static.hpp)
add_library(shared MODULE dynamic.cpp)

target_link_libraries(temp static)

target_compile_definitions(temp PRIVATE DYLIB_FULL_PATH="$<TARGET_FILE:shared>")

add_dependencies(temp shared)
