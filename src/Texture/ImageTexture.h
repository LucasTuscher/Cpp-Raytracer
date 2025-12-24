#pragma once
#include "Image.h"
#include "Texture.h"

/**
 * ImageTexture
 *
 * Textur-Sampling aus einem geladenen Bild (PPM P3).
 */
class ImageTexture : public Texture {
private:
    Image image_;

public:
    explicit ImageTexture(const std::string& ppmPath);
    explicit ImageTexture(Image image);

    Color sample(const UV& uv, const Point& point) const override;
};

