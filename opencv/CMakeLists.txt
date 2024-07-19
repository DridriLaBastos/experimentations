project(opencv)

find_package(OpenCV REQUIRED HINTS $ENV{DEV})

add_executable(opencv main.cpp)

target_include_directories(opencv PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencv ${OpenCV_LIBS})