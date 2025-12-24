#pragma once
#include "../Color/Color.h"
#include <cstdint>
#include <istream>
#include <string>
#include <vector>

/**
 * Image
 *
 * Ein simples Bild-Containerformat (RGB als Color, row-major).
 * Wird u.a. von ImageTexture genutzt.
 */
class Image {
private:
    int width_;
    int height_;
    std::vector<Color> pixels_;

public:
    Image();
    Image(int width, int height, std::vector<Color> pixels);

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

    Color getPixel(int x, int y) const;

    static Image loadPPM(std::istream& in);
    static Image loadPPM(const std::string& path);

    static Image loadBMP(std::istream& in);
    static Image loadBMP(const std::string& path);

    // Auto-detect by file signature (PPM P3/P6, BMP).
    static Image loadFromFile(const std::string& path);
};
