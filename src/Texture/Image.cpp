#include "Image.h"
#include <fstream>
#include <stdexcept>
#include <vector>

Image::Image() : width_(0), height_(0), pixels_() {}

Image::Image(int width, int height, std::vector<Color> pixels)
    : width_(width), height_(height), pixels_(std::move(pixels)) {
    if (width_ <= 0 || height_ <= 0) {
        throw std::runtime_error("Image dimensions must be positive");
    }
    if (static_cast<int>(pixels_.size()) != width_ * height_) {
        throw std::runtime_error("Image pixel buffer has wrong size");
    }
}

Color Image::getPixel(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("Image::getPixel coordinates out of bounds");
    }
    return pixels_[y * width_ + x];
}

static bool readPPMToken(std::istream& in, std::string& token) {
    while (in >> token) {
        if (!token.empty() && token[0] == '#') {
            std::string restOfLine;
            std::getline(in, restOfLine);
            continue;
        }
        return true;
    }
    return false;
}

Image Image::loadPPM(std::istream& in) {
    std::string token;

    if (!readPPMToken(in, token)) {
        throw std::runtime_error("PPM: missing magic header");
    }
    const std::string magic = token;
    if (magic != "P3" && magic != "P6") {
        throw std::runtime_error("PPM: only P3 and P6 are supported");
    }

    if (!readPPMToken(in, token)) {
        throw std::runtime_error("PPM: missing width");
    }
    int width = std::stoi(token);

    if (!readPPMToken(in, token)) {
        throw std::runtime_error("PPM: missing height");
    }
    int height = std::stoi(token);

    if (!readPPMToken(in, token)) {
        throw std::runtime_error("PPM: missing max color value");
    }
    int maxVal = std::stoi(token);
    if (width <= 0 || height <= 0 || maxVal <= 0) {
        throw std::runtime_error("PPM: invalid header values");
    }

    std::vector<Color> pixels;
    pixels.reserve(static_cast<size_t>(width) * static_cast<size_t>(height));
    const double invMax = 1.0 / static_cast<double>(maxVal);

    if (magic == "P3") {
        for (int i = 0; i < width * height; ++i) {
            std::string rTok, gTok, bTok;
            if (!readPPMToken(in, rTok) || !readPPMToken(in, gTok) || !readPPMToken(in, bTok)) {
                throw std::runtime_error("PPM: not enough pixel data");
            }

            int r = std::stoi(rTok);
            int g = std::stoi(gTok);
            int b = std::stoi(bTok);

            pixels.emplace_back(r * invMax, g * invMax, b * invMax);
        }
    } else { // P6 binary
        // Consume exactly one header whitespace character (usually '\n', sometimes "\r\n").
        char c = 0;
        if (!in.get(c)) {
            throw std::runtime_error("PPM: missing binary pixel data");
        }
        if (c == '\r' && in.peek() == '\n') {
            in.get();
        }

        const size_t byteCount = static_cast<size_t>(width) * static_cast<size_t>(height) * 3u;
        std::vector<unsigned char> data(byteCount);
        in.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(byteCount));
        if (static_cast<size_t>(in.gcount()) != byteCount) {
            throw std::runtime_error("PPM: not enough binary pixel data");
        }

        for (size_t i = 0; i < byteCount; i += 3) {
            int r = static_cast<int>(data[i + 0]);
            int g = static_cast<int>(data[i + 1]);
            int b = static_cast<int>(data[i + 2]);
            pixels.emplace_back(r * invMax, g * invMax, b * invMax);
        }
    }

    return Image(width, height, std::move(pixels));
}

Image Image::loadPPM(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("PPM: could not open file: " + path);
    }
    return loadPPM(file);
}

static uint16_t readLE16(std::istream& in) {
    unsigned char b[2];
    in.read(reinterpret_cast<char*>(b), 2);
    if (in.gcount() != 2) {
        throw std::runtime_error("BMP: unexpected end of file");
    }
    return static_cast<uint16_t>(b[0]) | (static_cast<uint16_t>(b[1]) << 8);
}

static uint32_t readLE32(std::istream& in) {
    unsigned char b[4];
    in.read(reinterpret_cast<char*>(b), 4);
    if (in.gcount() != 4) {
        throw std::runtime_error("BMP: unexpected end of file");
    }
    return static_cast<uint32_t>(b[0]) |
           (static_cast<uint32_t>(b[1]) << 8) |
           (static_cast<uint32_t>(b[2]) << 16) |
           (static_cast<uint32_t>(b[3]) << 24);
}

static int32_t readLE32s(std::istream& in) {
    return static_cast<int32_t>(readLE32(in));
}

Image Image::loadBMP(std::istream& in) {
    // BMP file header (14 bytes)
    unsigned char sig[2];
    in.read(reinterpret_cast<char*>(sig), 2);
    if (in.gcount() != 2) {
        throw std::runtime_error("BMP: missing signature");
    }
    if (sig[0] != 'B' || sig[1] != 'M') {
        throw std::runtime_error("BMP: invalid signature");
    }

    (void)readLE32(in); // file size (unused)
    (void)readLE16(in); // reserved1
    (void)readLE16(in); // reserved2
    uint32_t pixelDataOffset = readLE32(in);

    // DIB header
    uint32_t dibHeaderSize = readLE32(in);
    if (dibHeaderSize < 40) {
        throw std::runtime_error("BMP: unsupported DIB header");
    }

    int32_t width = readLE32s(in);
    int32_t height = readLE32s(in);
    uint16_t planes = readLE16(in);
    uint16_t bitsPerPixel = readLE16(in);
    uint32_t compression = readLE32(in);
    (void)readLE32(in); // imageSize
    (void)readLE32s(in); // xPixelsPerMeter
    (void)readLE32s(in); // yPixelsPerMeter
    (void)readLE32(in); // colorsUsed
    (void)readLE32(in); // importantColors

    // Skip rest of DIB header if present
    if (dibHeaderSize > 40) {
        in.ignore(static_cast<std::streamsize>(dibHeaderSize - 40));
    }

    if (planes != 1) {
        throw std::runtime_error("BMP: invalid planes value");
    }
    if (compression != 0) {
        throw std::runtime_error("BMP: only uncompressed BI_RGB is supported");
    }
    if (bitsPerPixel != 24 && bitsPerPixel != 32) {
        throw std::runtime_error("BMP: only 24-bit and 32-bit BMP are supported");
    }
    if (width <= 0 || height == 0) {
        throw std::runtime_error("BMP: invalid dimensions");
    }

    const bool isTopDown = height < 0;
    const int absHeight = isTopDown ? -height : height;

    const int bytesPerPixel = bitsPerPixel / 8;
    const size_t unpaddedRowSize = static_cast<size_t>(width) * static_cast<size_t>(bytesPerPixel);
    const size_t paddedRowSize = (unpaddedRowSize + 3u) & ~3u;

    // Jump to pixel data
    in.seekg(static_cast<std::streamoff>(pixelDataOffset), std::ios::beg);
    if (!in.good()) {
        throw std::runtime_error("BMP: invalid pixel data offset");
    }

    std::vector<Color> pixels;
    pixels.resize(static_cast<size_t>(width) * static_cast<size_t>(absHeight));

    std::vector<unsigned char> row(paddedRowSize);
    for (int fileRow = 0; fileRow < absHeight; ++fileRow) {
        in.read(reinterpret_cast<char*>(row.data()), static_cast<std::streamsize>(paddedRowSize));
        if (static_cast<size_t>(in.gcount()) != paddedRowSize) {
            throw std::runtime_error("BMP: not enough pixel data");
        }

        const int y = isTopDown ? fileRow : (absHeight - 1 - fileRow);
        for (int x = 0; x < width; ++x) {
            const size_t idx = static_cast<size_t>(x) * static_cast<size_t>(bytesPerPixel);
            int b = static_cast<int>(row[idx + 0]);
            int g = static_cast<int>(row[idx + 1]);
            int r = static_cast<int>(row[idx + 2]);
            pixels[static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x)] =
                Color(r / 255.0, g / 255.0, b / 255.0);
        }
    }

    return Image(width, absHeight, std::move(pixels));
}

Image Image::loadBMP(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("BMP: could not open file: " + path);
    }
    return loadBMP(file);
}

Image Image::loadFromFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Image: could not open file: " + path);
    }

    unsigned char sig[2] = {0, 0};
    file.read(reinterpret_cast<char*>(sig), 2);
    if (file.gcount() != 2) {
        throw std::runtime_error("Image: file too short: " + path);
    }
    file.seekg(0, std::ios::beg);

    // PPM
    if (sig[0] == 'P' && (sig[1] == '3' || sig[1] == '6')) {
        return loadPPM(file);
    }

    // BMP
    if (sig[0] == 'B' && sig[1] == 'M') {
        return loadBMP(file);
    }

    throw std::runtime_error("Image: unsupported file format: " + path);
}
