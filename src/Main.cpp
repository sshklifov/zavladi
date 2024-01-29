#include "Drawable.hpp"
#include "Window.hpp"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <cstdio>
#include <cstdlib>

#include <vector>

int main() {
  const int width = 1920;
  const int height = 1080;
  GLFWwindow *window = createWindow(width, height);

  Pacman pacman(Point(400, 300));
  Ghost redGhost(Point(1400, 400), Color3f(1.f, 0.341f, 0.016f));
  Ghost greenGhost(Point(900, 600), Color3f(0.149f, 0.729f, 0.157f));
  Ghost blueGhost(Point(1000, 800), Color3f(0.341f, 0.675f, 1.f));
  std::vector<Coin> coins;
  for (int i = 0; i < 30; ++i) {
    int x = rand() % width;
    int y = rand() % height;
    coins.emplace_back(Point(x, y));
  }

  Point ps[] = {
      Point(50, 100),  Point(1800, 100), Point(1800, 900), Point(200, 900),
      Point(200, 600), Point(50, 600),   Point(50, 100),
  };
  Wall walls[] = {
      Wall(ps[0], ps[1]), Wall(ps[1], ps[2]), Wall(ps[2], ps[3]),
      Wall(ps[3], ps[4]), Wall(ps[4], ps[5]), Wall(ps[5], ps[6]),
  };

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
    Framebuffer *fb = getWindowFramebuffer(window);
    fb->clear();
    for (const Coin &c : coins) {
      fb->draw(c);
    }
    for (const Wall &w : walls) {
      fb->draw(w);
    }
    fb->draw(pacman);
    fb->draw(redGhost);
    fb->draw(greenGhost);
    fb->draw(blueGhost);
    displayWindowFramebuffer(window);
  }

  return 0;
}
