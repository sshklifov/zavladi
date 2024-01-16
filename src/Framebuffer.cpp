#include "Framebuffer.hpp"

Framebuffer::Framebuffer(int width, int height) : width(width), height(height) {
    int pixels = width * height;
    fb = new Color3f[pixels];
}

Framebuffer::~Framebuffer() {
    delete[] fb;
}

int Framebuffer::getWidth() const {
    return width;
}

int Framebuffer::getHeight() const {
    return height;
}

const float* Framebuffer::getData() const {
    return reinterpret_cast<float*>(fb);
}

void Framebuffer::set(int x, int y, Color3f col) {
    fb[x * height + y] = col;
}

void Framebuffer::clear() {
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            set(i, j, Color3f(0.f, 0.f, 0.f));
        }
    }
}

void Framebuffer::drawCoin(int x, int y) {
    const int rad = 20;
}
