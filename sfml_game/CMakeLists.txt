project(SfmlGame)

add_executable(game main.cpp
					src/game.cpp
					src/world.cpp
					src/entity.cpp
					src/aircraft.cpp
					src/scene_node.cpp
					src/sprite_node.cpp)

target_link_libraries(game PRIVATE sfml-graphics)
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR} ${sfml_SOURCE_DIR}/include)
target_compile_definitions(game PRIVATE RESOURCE_FOLDER="${PROJECT_SOURCE_DIR}/assets")

if (${WIN32})
add_custom_command(TARGET game POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sfml-graphics> $<TARGET_FILE:sfml-system> $<TARGET_FILE:sfml-window> $<TARGET_FILE_DIR:game>)
endif()
