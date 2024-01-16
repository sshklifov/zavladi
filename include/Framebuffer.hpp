#pragma once

struct Framebuffer {
    Framebuffer(int width, int height);
    ~Framebuffer();

    int getWidth() const;
    int getHeight() const;
    const float* getData() const;

    void fillRed();

private:
    int width;
    int height;
    float* fb;
};
