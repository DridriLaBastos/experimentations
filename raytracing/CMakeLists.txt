project(Raytracing LANGUAGES CXX)

add_executable(rt main.cpp)

add_library(raytracing_draw_module MODULE 
											raytracing.cpp
											src/ray.cpp
											src/vec3f.cpp)

target_include_directories(rt PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(raytracing_draw_module PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(rt PRIVATE module)

add_dependencies(rt raytracing_draw_module)

set(RAYTRACING_DRAW_MODULE_COPY_FULL_PATH "$<TARGET_FILE_DIR:raytracing_draw_module>/$<TARGET_FILE_PREFIX:raytracing_draw_module>$<TARGET_FILE_BASE_NAME:raytracing_draw_module>_c$<TARGET_FILE_SUFFIX:raytracing_draw_module>")
target_compile_definitions(rt PRIVATE
	RAYTRACING_DRAW_MODULE_FULL_PATH="$<TARGET_FILE:raytracing_draw_module>"
	RAYTRACING_DRAW_MODULE_COPY_FULL_PATH="${RAYTRACING_DRAW_MODULE_COPY_FULL_PATH}")

target_compile_definitions(raytracing_draw_module PRIVATE ASSET_PATH="${PROJECT_SOURCE_DIR}/assets")
target_link_libraries(raytracing_draw_module PRIVATE sfml-graphics)

if (${WIN32})
add_custom_command(TARGET rt POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sfml-graphics> $<TARGET_FILE:sfml-system> $<TARGET_FILE:sfml-window> $<TARGET_FILE_DIR:rt>)
endif()
