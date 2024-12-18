project(nn DESCRIPTION "Creating a neural network framework in C++ from scratch")

option(NN_HEADER_ONLY "Wether or not the framework should be compiled as a header only library" ON)

add_executable(nn main.cpp)

if (${NN_HEADER_ONLY})
    target_compile_definitions(nn PRIVATE NN_HEADER_ONLY)
else()
    target_sources(nn PRIVATE nn.cpp)
endif()
