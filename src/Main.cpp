#include "Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

int main() {
    const int width = 1920;
    const int height = 1080;
    GLFWwindow* window = createWindow(width, height);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        Framebuffer* fb = getWindowFramebuffer(window);
        fb->clear();
        displayWindowFramebuffer(window);
    }

    return 0;
}
