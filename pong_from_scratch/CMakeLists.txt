cmake_minimum_required(VERSION 3.10)

project(PongFromScratch LANGUAGES CXX ASM_NASM)

add_custom_command(OUTPUT init.bin
					COMMAND pwd
					COMMAND ${CMAKE_ASM_NASM_COMPILER} -o init.bin ${PROJECT_SOURCE_DIR}/init.asm
					DEPENDS ${PROJECT_SOURCE_DIR}/init.asm
					)

add_custom_target(floppy ALL
					COMMAND pwd
					COMMAND cat init.bin /dev/zero | dd of=${PROJECT_SOURCE_DIR}/pong.floppy bs=512 count=2880
					DEPENDS init.bin)