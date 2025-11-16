/**
 * Debug-Programm für Strahlrichtungen-Visualisierung
 */
#include <iostream>
#include <cmath>
#include "src/Point/Point.h"
#include "src/Vector/Vector.h"
#include "src/Ray/Ray.h"

int main() {
    const int width = 400;
    const int height = 300;
    Point viewerPos(0, 0, -10);

    std::cout << "=== Debug: Strahlrichtungen ===\n\n";

    // Teste Zentrum
    int x = width / 2;
    int y = height / 2;
    double px = 0.01 * ((x + 0.5) - width / 2.0);
    double py = 0.01 * ((y + 0.5) - height / 2.0);
    double pz = 0.0;
    Point pixelPos(px, py, pz);
    Ray ray = Ray::fromPoints(viewerPos, pixelPos);
    Vector dir = ray.getDirection();

    std::cout << "Zentrum (x=" << x << ", y=" << y << "):\n";
    std::cout << "  Pixel-Position: (" << px << ", " << py << ", " << pz << ")\n";
    std::cout << "  Strahl-Richtung (normalisiert): (" << dir.x << ", " << dir.y << ", " << dir.z << ")\n";
    std::cout << "  Farbe (abs): R=" << std::abs(dir.x) << ", G=" << std::abs(dir.y) << ", B=0\n\n";

    // Teste Ecke oben-links
    x = 0;
    y = 0;
    px = 0.01 * ((x + 0.5) - width / 2.0);
    py = 0.01 * ((y + 0.5) - height / 2.0);
    pz = 0.0;
    pixelPos = Point(px, py, pz);
    ray = Ray::fromPoints(viewerPos, pixelPos);
    dir = ray.getDirection();

    std::cout << "Ecke oben-links (x=" << x << ", y=" << y << "):\n";
    std::cout << "  Pixel-Position: (" << px << ", " << py << ", " << pz << ")\n";
    std::cout << "  Strahl-Richtung (normalisiert): (" << dir.x << ", " << dir.y << ", " << dir.z << ")\n";
    std::cout << "  Farbe (abs): R=" << std::abs(dir.x) << ", G=" << std::abs(dir.y) << ", B=0\n\n";

    // Teste Ecke unten-rechts
    x = width - 1;
    y = height - 1;
    px = 0.01 * ((x + 0.5) - width / 2.0);
    py = 0.01 * ((y + 0.5) - height / 2.0);
    pz = 0.0;
    pixelPos = Point(px, py, pz);
    ray = Ray::fromPoints(viewerPos, pixelPos);
    dir = ray.getDirection();

    std::cout << "Ecke unten-rechts (x=" << x << ", y=" << y << "):\n";
    std::cout << "  Pixel-Position: (" << px << ", " << py << ", " << pz << ")\n";
    std::cout << "  Strahl-Richtung (normalisiert): (" << dir.x << ", " << dir.y << ", " << dir.z << ")\n";
    std::cout << "  Farbe (abs): R=" << std::abs(dir.x) << ", G=" << std::abs(dir.y) << ", B=0\n\n";

    // Teste Punkt rechts vom Zentrum
    x = width - 1;
    y = height / 2;
    px = 0.01 * ((x + 0.5) - width / 2.0);
    py = 0.01 * ((y + 0.5) - height / 2.0);
    pz = 0.0;
    pixelPos = Point(px, py, pz);
    ray = Ray::fromPoints(viewerPos, pixelPos);
    dir = ray.getDirection();

    std::cout << "Rechts vom Zentrum (x=" << x << ", y=" << y << "):\n";
    std::cout << "  Pixel-Position: (" << px << ", " << py << ", " << pz << ")\n";
    std::cout << "  Strahl-Richtung (normalisiert): (" << dir.x << ", " << dir.y << ", " << dir.z << ")\n";
    std::cout << "  Farbe (abs): R=" << std::abs(dir.x) << ", G=" << std::abs(dir.y) << ", B=0\n";
    std::cout << "  RGB (0-255): R=" << (int)(std::abs(dir.x) * 255)
              << ", G=" << (int)(std::abs(dir.y) * 255) << ", B=0\n\n";

    return 0;
}
