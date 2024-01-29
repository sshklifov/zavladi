#pragma once

#include "Bitmap.hpp"
#include "Config.hpp"

#include <algorithm>
#include <cmath>

enum class BoxType { box_coin, box_wall, box_pacman, box_ghost };
struct Point {
  Point() {}
  Point(int x, int y) : x(x), y(y) {}

  Point &operator+=(const Point &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  int x;
  int y;
};

struct Box {
  Box(const Point &p, int rad) {
    pmin.x = p.x - rad;
    pmin.y = p.y - rad;
    pmax.x = p.x + rad;
    pmax.y = p.y + rad;
  }

  Box(const Point &p1, const Point &p2) {
    pmin.x = std::min(p1.x, p2.x);
    pmin.y = std::min(p1.y, p2.y);
    pmax.x = std::max(p1.x, p2.x);
    pmax.y = std::max(p1.y, p2.y);
  }

  bool inside(const Point &p) const {
    return p.x >= pmin.x && p.x <= pmax.x && p.y >= pmin.y && p.y <= pmax.y;
  }

  bool intersect(const Box &rhs) const {
    return !(pmin.x > rhs.pmax.x || pmax.x < rhs.pmin.x ||
             pmin.y > rhs.pmax.y || pmax.y < rhs.pmin.y);
  }

  int xmin() const { return pmin.x; }
  int xmax() const { return pmax.x; }
  int ymin() const { return pmin.y; }
  int ymax() const { return pmax.y; }

  int width() const { return pmax.x - pmin.x; }
  int height() const { return pmax.y - pmin.y; }

  Point pmin;
  Point pmax;

  virtual BoxType type() const = 0;
};

struct DrawableBox : public Box {
  DrawableBox(const Box &b) : Box(b) {}
  DrawableBox(const Point &p, int rad) : Box(p, rad) {}
  DrawableBox(const Point &p1, const Point &p2) : Box(p1, p2) {}

  virtual bool shouldDraw(const Point &p) const = 0;
  virtual Color3f getColor(const Point &p) const = 0;

  virtual BoxType type() const override = 0;
};

struct Coin : public DrawableBox {
  Coin(const Point &pos) : DrawableBox(pos, COIN_RAD), pos(pos) {}

  bool shouldDraw(const Point &p) const override {
    int dx = p.x - pos.x;
    int dy = p.y - pos.y;
    int dist2 = dx * dx + dy * dy;
    return dist2 <= COIN_RAD * COIN_RAD;
  }

  Color3f getColor(const Point &) const override {
    return Color3f(0.965f, 0.733f, 0.686f);
  }

  BoxType type() const override { return BoxType::box_coin; }

private:
  Point pos;
};

struct Wall : public DrawableBox {
  Wall(const Point &p1, const Point &p2) : DrawableBox(p1, p2) {}

  bool shouldDraw(const Point &) const override { return true; }

  Color3f getColor(const Point &) const override {
    return Color3f(0.122f, 0.153f, 0.824f);
  }

  BoxType type() const override { return BoxType::box_wall; }
};

enum Direction { dir_left, dir_right, dir_up, dir_down };

struct Pacman : public DrawableBox {
  Pacman(const Point &pos) : DrawableBox(pos, PACMAN_RAD) {
    center = pos;
    dir = dir_right;
  }

  bool shouldDraw(const Point &p) const override {
    int dx = p.x - center.x;
    int dy = p.y - center.y;
    int dist2 = dx * dx + dy * dy;
    if (dist2 > PACMAN_RAD * PACMAN_RAD) {
      return false;
    }

    float dist = sqrt(dist2);
    float cos = 0.f;
    switch (dir) {
    case dir_right:
      cos = dx / dist;
      break;
    case dir_up:
      cos = dy / dist;
      break;
    case dir_left:
      cos = -dx / dist;
      break;
    case dir_down:
      cos = -dy / dist;
      break;
    }
    return cos < PACMAN_ANGLE;
  }

  Color3f getColor(const Point &) const override {
    return Color3f(1.f, 0.937f, 0.f);
  }
  void translate(const Point &p) {
    center += p;
    this->pmin += p;
    this->pmax += p;
  }
  void moveLeft() {
    center.x -= PACMAN_SPEED;
    pmin.x -= PACMAN_SPEED;
    pmax.x -= PACMAN_SPEED;
    dir = dir_left;
  }
  void moveRight() {
    center.x += PACMAN_SPEED;
    pmin.x += PACMAN_SPEED;
    pmax.x += PACMAN_SPEED;
    dir = dir_right;
  }
  void moveDown() {
    center.y -= PACMAN_SPEED;
    pmin.y -= PACMAN_SPEED;
    pmax.y -= PACMAN_SPEED;
    dir = dir_down;
  }
  void moveUp() {
    center.y += PACMAN_SPEED;
    pmin.y += PACMAN_SPEED;
    pmax.y += PACMAN_SPEED;
    dir = dir_up;
  }

  BoxType type() const override { return BoxType::box_pacman; }

private:
  Point center;
  Direction dir;
};

struct Ghost : public DrawableBox {
  struct Singleton {
    static const Bitmap &get() {
      static Bitmap ghost(GHOST_BITMAP_PATH, GHOST_BITMAP_WIDTH,
                          GHOST_BITMAP_HEIGHT);
      return ghost;
    }
  };

  Ghost(const Point &pos, Color3f color)
      : DrawableBox(pos, GHOST_RAD), color(color) {}

  Color3f getColor(const Point &p) const override {
    if (inside(p)) {
      int xoff = p.x - pmin.x;
      int yoff = p.y - pmin.y;
      float u = (float)xoff / width();
      float v = (float)yoff / height();
      float alpha = Singleton::get().sample(u, v).intensity();
      return Color3f(color.r * alpha, color.g * alpha, color.b * alpha);
    }
    return Color3f(0.f, 0.f, 0.f);
  }

  bool shouldDraw(const Point &p) const override {
    float alpha = getColor(p).intensity();
    return alpha > 0.05f;
  }

BoxType type() const override { return BoxType::box_ghost; }

private:
  Color3f color;
};
