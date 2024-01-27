#pragma once

struct Color3f {
    Color3f() = default;
    Color3f(float r, float g, float b) : r(r), g(g), b(b) {}

    float intensity() const { return (r + g + b) / 3.f; }

    float r;
    float g;
    float b;
};

struct Bitmap {
    Bitmap(int width, int height);
    Bitmap(const char* path, int width, int height);
    ~Bitmap();

    void draw(int x, int y, Color3f col);
    Color3f sample(int x, int y) const;
    Color3f sample(float u, float v) const;

    int width;
    int height;
    Color3f* data;
};
