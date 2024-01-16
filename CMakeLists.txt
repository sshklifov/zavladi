cmake_minimum_required(VERSION 3.16)
project(zavladi)
set(CMAKE_CXX_STANDARD 11)

add_compile_options(-Wall -Wextra)
include_directories(include/)

add_library(glad src/glad.c)

find_package(glfw3 REQUIRED)
link_libraries(glfw glad)

add_executable(window src/Main.cpp src/Framebuffer.cpp src/Window.cpp)
