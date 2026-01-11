project(Evolve DESCRIPTION "Using SFML to toy with evolution algorithms")

add_executable(evolve main.cpp
				scene/Scene.hpp
				scene/entt/entt.hpp
				scene/SceneEntity.hpp
				scene/SpriteComponent.hpp
				scene/PhysicComponent.hpp
				
				src/Scene.cpp
				src/SceneEntity.cpp
				src/AliveComponent.cpp
				src/SpriteComponent.cpp
				src/PhysicComponent.cpp
			)

target_link_libraries(evolve PRIVATE box2d sfml-graphics)
target_compile_definitions(evolve PRIVATE RESOURCE_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/resources")
target_include_directories(evolve PRIVATE ${PROJECT_SOURCE_DIR})
