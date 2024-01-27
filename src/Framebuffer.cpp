#include "Framebuffer.hpp"

Framebuffer::Framebuffer(int width, int height) : fb(width, height) {}

int Framebuffer::getWidth() const {
    return fb.width;
}

int Framebuffer::getHeight() const {
    return fb.height;
}

const float* Framebuffer::getData() const {
    return reinterpret_cast<float*>(fb.data);
}

void Framebuffer::clear() {
    for (int x = 0; x < fb.width; ++x) {
        for (int y = 0; y < fb.height; ++y) {
            fb.draw(x, y, Color3f(0.f, 0.f, 0.f));
        }
    }
}

void Framebuffer::draw(const DrawableBox& element) {
    for (int x = element.xmin(); x <= element.xmax(); ++x) {
        for (int y = element.ymin(); y <= element.ymax(); ++y) {
            Point p(x, y);
            if (element.shouldDraw(p)) {
                fb.draw(x, y, element.getColor(p));
            }
        }
    }
}
