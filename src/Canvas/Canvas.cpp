/**
 * Canvas.cpp
 * 
 * Implementierung der Canvas-Klasse für Bildverwaltung und PNG-Ausgabe.
 */
#include "Canvas.h"
#include <stdexcept>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <filesystem>

// Für PNG-Ausgabe verwenden wir eine einfache Lösung mit PPM-Format
// PPM ist ein einfaches Textformat, das von den meisten Bildbetrachtungsprogrammen unterstützt wird
#include <fstream>

/**
 * Konstruktor ohne Dateinamen
 */
Canvas::Canvas(int width, int height) 
    : width(width), height(height), fileName("") {
    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Canvas dimensions must be positive");
    }
    pixels.resize(width * height, Color::BLACK());
}

/**
 * Konstruktor mit Dateinamen
 */
Canvas::Canvas(int width, int height, const std::string& fileName)
    : width(width), height(height), fileName(fileName) {
    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Canvas dimensions must be positive");
    }
    pixels.resize(width * height, Color::BLACK());
}

/**
 * Setzt die Farbe eines Pixels
 */
void Canvas::setPixel(int x, int y, const Color& color) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    pixels[getIndex(x, y)] = color;
}

/**
 * Liest die Farbe eines Pixels
 */
Color Canvas::getPixel(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    return pixels[getIndex(x, y)];
}

/**
 * Generiert einen Dateinamen mit Timestamp
 * Format: baseName-YYYY-MM-DD-HH-MM-SS
 */
std::string Canvas::generateTimestampFilename(const std::string& baseName) const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << baseName << "-"
       << std::put_time(std::localtime(&time), "%Y-%m-%d-%H-%M-%S");
    
    return ss.str();
}

/**
 * Speichert das Bild mit dem gesetzten Dateinamen
 */
bool Canvas::save() const {
    if (fileName.empty()) {
        return save(generateTimestampFilename());
    } else {
        return save(fileName);
    }
}

/**
 * Speichert das Bild im PPM-Format (Portable Pixmap)
 * 
 * PPM ist ein einfaches, unkomprimiertes Bildformat.
 * Es wird von den meisten Bildbetrachtungsprogrammen unterstützt.
 * 
 * Format: P3 (ASCII)
 * Zeile 1: P3
 * Zeile 2: width height
 * Zeile 3: max_color_value (255)
 * Danach: RGB-Werte (0-255) für jeden Pixel
 */
bool Canvas::save(const std::string& filename) const {
    // Speichere standardmäßig in ./output relativ zum aktuellen Arbeitsverzeichnis.
    // So landen die Dateien konsistent neben den Binaries (build/output).
    std::string basePath = "output/";
    // Falls filename bereits einen Pfad enthält, nutze ihn unverändert.
    std::string fullFilename = filename.find('/') != std::string::npos
        ? filename + ".ppm"
        : basePath + filename + ".ppm";

    // Stelle sicher, dass der Ausgabeordner existiert (nur wenn basePath genutzt wird).
    if (filename.find('/') == std::string::npos) {
        std::error_code ec;
        std::filesystem::create_directories(basePath, ec);
    }

    std::ofstream file(fullFilename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << fullFilename << std::endl;
        return false;
    }

    // PPM Header schreiben
    file << "P3\n";
    file << width << " " << height << "\n";
    file << "255\n";

    // Pixel-Daten schreiben
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color c = pixels[getIndex(x, y)].clamped();
            
            int r = static_cast<int>(c.r * 255.0);
            int g = static_cast<int>(c.g * 255.0);
            int b = static_cast<int>(c.b * 255.0);
            
            file << r << " " << g << " " << b << "  ";
        }
        file << "\n";
    }

    file.close();
    std::cout << "Image saved to: " << fullFilename << std::endl;
    return true;
}
