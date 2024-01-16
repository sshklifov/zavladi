#pragma once

struct Color3f {
    Color3f() = default;
    Color3f(float r, float g, float b) : r(r), g(g), b(b) {}

    float r;
    float g;
    float b;
};

struct Framebuffer {
    Framebuffer(int width, int height);
    ~Framebuffer();

    int getWidth() const;
    int getHeight() const;
    const float* getData() const;

    void set(int x, int y, Color3f col);
    void clear();
    void drawCoin(int x, int y);


private:
    int width;
    int height;
    Color3f* fb;
};
