cmake_minimum_required(VERSION 3.21)
project(zavladi)
set(CMAKE_CXX_STANDARD 14)

add_compile_definitions(GHOST_BITMAP_WIDTH=107)
add_compile_definitions(GHOST_BITMAP_HEIGHT=128)
find_file(GHOST_RGB NAMES ghost.rgb PATHS ${CMAKE_CURRENT_LIST_DIR}/res REQUIRED NO_CACHE NO_DEFAULT_PATH)
add_compile_definitions(GHOST_BITMAP_PATH="${GHOST_RGB}")

add_compile_options(-Wall -Wextra)
include_directories(include/)

add_library(glad src/glad.c)

find_package(glfw3 REQUIRED)
link_libraries(glfw glad)

add_executable(window src/Main.cpp src/Framebuffer.cpp src/Window.cpp src/Bitmap.cpp)
