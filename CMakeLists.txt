cmake_minimum_required(VERSION 3.15)
project(brick_breaker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(SDL2_INCLUDE_DIR "/usr/include/SDL2")
set(SDL2_LIBRARY "/usr/lib/x86_64-linux-gnu/libSDL2.so")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(brick_breaker main.cpp Ball.cpp Ball.h Paddle.cpp Paddle.h pixel_art.h menu.h)
target_link_libraries(brick_breaker ${SDL2_LIBRARY})