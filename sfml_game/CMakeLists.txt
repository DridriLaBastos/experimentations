project(SfmlGame)

add_executable(game main.cpp
					src/game.cpp)

target_link_libraries(game PRIVATE sfml-graphics)
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR} ${sfml_SOURCE_DIR}/include)
target_compile_definitions(game PRIVATE RESOURCE_FOLDER="${PROJECT_SOURCE_DIR}/resources")
