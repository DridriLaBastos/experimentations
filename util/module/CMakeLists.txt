project(Module
		DESCRIPTION "A library that creates tools to dynamically reload a shared library")

add_library(module STATIC src/module.cpp)

target_include_directories(module PRIVATE ${PROJECT_SOURCE_DIR})

if (${UNIX})
	target_sources(module PRIVATE src/platform_unix.cpp)
elseif (${WIN32})
	target_sources(module PRIVATE src/platform_win32.cpp)
else()
endif()
