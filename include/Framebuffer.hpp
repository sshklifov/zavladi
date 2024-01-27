#pragma once

#include "Drawable.hpp"

struct Framebuffer {
    Framebuffer(int width, int height);

    int getWidth() const;
    int getHeight() const;
    const float* getData() const;

    void clear();
    void draw(const DrawableBox& element);

private:
    Bitmap fb;
};
