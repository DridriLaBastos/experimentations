project(Evolve DESCRIPTION "Using SFML to toy with evolution algorithms")

include (FetchContent)

set(FASTNOISE2_GIT_BRANCH v0.10.0-alpha)

if (MINGW)
	set(FASTNOISE2_GIT_BRANCH NewFastSIMD)
	message(STATUS "Using branche ${FASTNOISE2_GIT_BRANCH} because the default one has a bug when using mingw with SIMD instructions")
endif()

set (FASTNOISE2_STANDALONE_PROJECT OFF CACHE INTERNAL "" FORCE)
FetchContent_Declare(FastNoiseSIMD2
	GIT_REPOSITORY https://github.com/Auburn/FastNoise2
	GIT_TAG ${FASTNOISE2_GIT_BRANCH}
	GIT_PROGRESS ON
	)

FetchContent_MakeAvailable(FastNoiseSIMD2)

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

target_link_libraries(evolve PRIVATE box2d sfml-graphics FastNoise2)
target_compile_definitions(evolve PRIVATE RESOURCE_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/resources")
target_include_directories(evolve PRIVATE ${PROJECT_SOURCE_DIR})
