cmake_minimum_required(VERSION 3.0.0)
project(chip8-emulator VERSION 0.1.0)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/)
set(SOURCES ${SRC_DIR}/chip8.cpp ${SRC_DIR}/chip8/system.cpp ${SRC_DIR}/chip8/romHandler.cpp ${SRC_DIR}/chip8/graphicsHandler.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})