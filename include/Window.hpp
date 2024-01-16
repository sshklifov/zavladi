#pragma once

#include "Framebuffer.hpp"

struct GLFWwindow;

GLFWwindow* createWindow(int width, int height);
void destroyWindow(GLFWwindow* window);

Framebuffer* getWindowFramebuffer(GLFWwindow* window);
void displayWindowFramebuffer(GLFWwindow* window);
