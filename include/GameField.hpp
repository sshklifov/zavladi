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


std::vector<std::vector<Point>> allPoints;
int height;
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  if (allPoints.empty()) {
    allPoints.push_back(std::vector<Point>{});
  }
  std::vector<Point>& currPoints = allPoints.back();

  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  int x = static_cast<int>(xpos);
  int y = height - static_cast<int>(ypos);
  printf("Coordinates: %d %d\n", x, y);

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    if (!currPoints.empty()) {
      int dx = abs(currPoints.back().x - x);
      int dy = abs(currPoints.back().y - y);
      if (dx < dy) {
        currPoints.emplace_back(currPoints.back().x, y);
      } else {
        currPoints.emplace_back(x, currPoints.back().y);
      }
    } else {
      currPoints.emplace_back(x, y);
    }
  }
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    if (currPoints.size() >= 4) {
      int dx = abs(currPoints[currPoints.size()-1].x - currPoints[currPoints.size()-2].x);
      int dy = abs(currPoints[currPoints.size()-1].y - currPoints[currPoints.size()-2].y);
      if (dx == 0) {
        currPoints[currPoints.size()-1].y = currPoints[0].y;
        currPoints.push_back(currPoints.front());
        allPoints.push_back({});
      } else if (dy == 0) {
        currPoints[currPoints.size()-1].x = currPoints[0].x;
        currPoints.push_back(currPoints.front());
        allPoints.push_back(std::vector<Point>{});
      }
    }
  }
}

void exportCode() {
  printf("std::vector<Wall> walls\n");
  for (int i = 0; i < (int)allPoints.size(); ++i) {
    for (int j = 0; j < (int)allPoints[i].size(); ++j) {
      Point prev = allPoints[i][j-1];
      Point curr = allPoints[i][j];
      printf("walls.emplace_back(Point(%d, %d), Point(%d, %d))\n",
        prev.x, prev.y, curr.x, curr.y);
    }
  }
}

class GameField {
public:
  GameField(int width, int height)
      : width(width), height(height), pacman(Point(400, 300)) {
    // TODO: starting points in config
    ::height = height;
    window = createWindow(width, height);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

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
  }

  void play() {
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        exportCode();
        glfwSetWindowShouldClose(window, true);
      }

      Framebuffer *fb = getWindowFramebuffer(window);
      fb->clear();
      for (int i = 0; i < (int)allPoints.size(); ++i) {
        for (int j = 0; j < (int)allPoints[i].size(); ++j) {
          fb->draw(Coin(allPoints[i][j]));
        }
      }
      for (int i = 0; i < (int)allPoints.size(); ++i) {
        for (int j = 1; j < (int)allPoints[i].size(); ++j) {
          fb->draw(Wall(allPoints[i][j-1], allPoints[i][j]));
        }
      }
      fb->draw(pacman);
      displayWindowFramebuffer(window);
    }
  }

private:
  int width;
  int height;
  GLFWwindow *window;

  Pacman pacman;
  std::list<std::unique_ptr<DrawableBox>> objects;
};
