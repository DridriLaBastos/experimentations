cmake_minimum_required(VERSION 3.10)

project(PongFromScratch LANGUAGES CXX ASM_NASM)

add_custom_command(OUTPUT init.bin
					COMMAND nasm -o init.bin ${PROJECT_SOURCE_DIR}/init.asm
					DEPENDS ${PROJECT_SOURCE_DIR}/init.asm
					)

add_custom_command(OUTPUT main.bin
	COMMAND nasm -o main.bin ${PROJECT_SOURCE_DIR}/main.asm
	DEPENDS ${PROJECT_SOURCE_DIR}/main.asm
	)

add_custom_target(floppy ALL
					COMMAND cat init.bin main.bin /dev/zero | dd of=${PROJECT_SOURCE_DIR}/pong.floppy bs=512 count=2880
					DEPENDS init.bin main.bin )