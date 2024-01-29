#include "Drawable.hpp"
#include "Window.hpp"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <list>
#include <memory>
#include <vector>

class GameField {
public:
  GameField(int width, int height)
      : width(width), height(height), pacman(Point(400, 300)) {
    // TODO: starting points in config
    window = createWindow(width, height);

    objects.push_back(std::make_unique<Ghost>(
        Point(1400, 400), Color3f(1.f, 0.341f, 0.016f))); // red
    objects.push_back(std::make_unique<Ghost>(
        Point(900, 600), Color3f(0.149f, 0.729f, 0.157f))); // green
    objects.push_back(std::make_unique<Ghost>(
        Point(1000, 800), Color3f(0.341f, 0.675f, 1.f))); // blue

    for (int i = 1; i < 30; ++i) {
      int x = rand() % width;
      int y = rand() % height;
      objects.push_back(std::make_unique<Coin>(Point(x, y)));
    }

    wallLocations = {
        Point(50, 100),  Point(1800, 100), Point(1800, 900), Point(200, 900),
        Point(200, 600), Point(50, 600),   Point(50, 100),
    };
    for (size_t i = 0; i < wallLocations.size() - 1; ++i) {
      Point wallStart = wallLocations[i];
      Point wallEnd = wallLocations[i + 1];
      walls.emplace_back(wallStart, wallEnd);
      // objects.push_back(std::make_unique<Wall>(wallStart, wallEnd));
    }
  }

  void play() {
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
      } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (!checkWallCollisions()) {
          pacman.moveLeft();
        }
      } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (!checkWallCollisions()) {
          pacman.moveRight();
        }
      } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (!checkWallCollisions()) {
          pacman.moveUp();
        }
      } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (!checkWallCollisions()) {
          pacman.moveDown();
        }
      }

      Framebuffer *fb = getWindowFramebuffer(window);
      fb->clear();
      checkPacmanCollisions();
      for (const auto &obj : objects) {
        fb->draw(*obj);
      }
      for (const auto &wall : walls) {
        fb->draw(wall);
      }
      fb->draw(pacman);
      displayWindowFramebuffer(window);
    }
  }

private:
  void checkPacmanCollisions() {
    for (auto it = objects.begin(); it != objects.end();) {
      if (pacman.intersect(**it)) {
        if ((*it)->getType() == BoxType::box_coin) {
          it = objects.erase(it);
          std::cout << "Coin collected!\n";
        } else if ((*it)->getType() == BoxType::box_ghost) {
          std::cout << "Pacman died!\n";
          // or exit(0)
          it = objects.erase(it);
          glfwSetWindowShouldClose(window, true);
        } else {
          ++it;
        }
      } else {
        ++it;
      }
    }
  }

  bool checkWallCollisions() {
    for (const auto &wall : walls) {
      if (pacman.intersect(wall)) {
        std::cout << "Pacman hit a wall!\n";
        return true;
      }
    }
    return false;
  }

  int width;
  int height;
  GLFWwindow *window;

  Pacman pacman;

  std::vector<Point> wallLocations;
  std::vector<Wall> walls;
  std::list<std::unique_ptr<DrawableBox>> objects;
};