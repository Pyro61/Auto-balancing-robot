cmake_minimum_required(VERSION 3.17)

#Tootchain file
set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-gcc.cmake)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

#Languages
enable_language(C CXX ASM)

#Project name
project(robot)

#Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Source files
add_subdirectory(Drivers/FreeRTOS)
add_subdirectory(Src)
