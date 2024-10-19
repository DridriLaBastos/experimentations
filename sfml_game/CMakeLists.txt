project(SfmlGame)

include(FetchContent)
FetchContent_Declare(sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
	GIT_PROGRESS ON
	CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF)
FetchContent_MakeAvailable(sfml)

add_executable(game main.cpp
					src/game.cpp
					src/texture_holder.cpp)

target_link_libraries(game PRIVATE sfml-graphics)
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR} ${sfml_SOURCE_DIR}/include)
target_compile_definitions(game PRIVATE RESOURCE_FOLDER="${PROJECT_SOURCE_DIR}/resources")
