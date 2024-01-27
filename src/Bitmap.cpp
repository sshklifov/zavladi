#include "Bitmap.hpp"

#include <cstdio>
#include <stdexcept>

Bitmap::Bitmap(int width, int height) : width(width), height(height) {
    int pixels = width * height;
    data = new Color3f[pixels];
}

Bitmap::Bitmap(const char* path, int width, int height) {
    FILE* fp = fopen(path, "r");
    if (!fp) {
        throw std::runtime_error("Failed to open bitmap file");
    }

    int pixels = width * height;
    Color3f* buf = new Color3f[pixels];
    int components = pixels * 3;
    int n = fread(buf, sizeof(float), components, fp);
    fclose(fp);

    if (n != components) {
        delete[] buf;
        throw std::runtime_error("Failed to read bitmap data");
    }

    this->width = width;
    this->height = height;
    this->data = buf;
}

Bitmap::~Bitmap() {
    delete[] data;
}

void Bitmap::draw(int x, int y, Color3f col) {
    if (x >= 0 && x < width) {
        if (y >= 0 && y < height) {
            data[y * width + x] = col;
        }
    }
}

static int clamp(int x, int xmin, int xmax) {
    if (x < xmin) {
        return xmin;
    }
    if (x > xmax) {
        return xmax;
    }
    return x;
}

Color3f Bitmap::sample(int x, int y) const {
    x = clamp(x, 0, width - 1);
    y = clamp(y, 0, height - 1);
    return data[y * width + x];
}

Color3f Bitmap::sample(float u, float v) const {
    int x = u * (width - 1);
    int y = v * (height - 1);
    return sample(x, y);
}
