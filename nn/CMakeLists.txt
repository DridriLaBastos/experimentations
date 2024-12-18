project(nn DESCRIPTION "Creating a neural network framework in C++ from scratch")

add_executable(nn main.cpp)

target_compile_definitions(nn PRIVATE NN_HEADER_ONLY)
