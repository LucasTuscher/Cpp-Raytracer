#include "ImageTexture.h"
#include <algorithm>
#include <cmath>

ImageTexture::ImageTexture(const std::string& path) : image_(Image::loadFromFile(path)) {}

ImageTexture::ImageTexture(Image image) : image_(std::move(image)) {}

Color ImageTexture::sample(const UV& uv, const Point& /*point*/) const {
    if (image_.getWidth() <= 0 || image_.getHeight() <= 0) {
        return Color::BLACK();
    }

    // u wrappt, v wird geclampet (typisch für Sphere-Mapping).
    double u = uv.u - std::floor(uv.u);
    if (u < 0.0) {
        u += 1.0;
    }
    double v = std::clamp(uv.v, 0.0, 1.0);

    int x = static_cast<int>(u * image_.getWidth());
    if (x >= image_.getWidth()) {
        x = image_.getWidth() - 1;
    }

    int y = static_cast<int>(v * image_.getHeight());
    if (y >= image_.getHeight()) {
        y = image_.getHeight() - 1;
    }

    return image_.getPixel(x, y);
}
