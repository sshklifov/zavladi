#include "Framebuffer.hpp"

Framebuffer::Framebuffer(int width, int height) : width(width), height(height) {
    int bufsiz = width * height * 3;
    fb = new float[bufsiz];
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
    return fb;
}

void Framebuffer::fillRed() {
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int pos = (i * height + j) * 3;
            fb[pos] = 1.f;
            fb[pos + 1] = 0.f;
            fb[pos + 2] = 0.f;
        }
    }
}
