/**
 * Debug-Programm um zu prüfen, ob Strahlen die Kugel treffen
 */
#include <iostream>
#include "src/Point/Point.h"
#include "src/Vector/Vector.h"
#include "src/Ray/Ray.h"
#include "src/Shape/Sphere.h"

int main() {
    const int width = 400;
    const int height = 300;
    Point viewerPos(0, 0, -10);
    Sphere sphere;

    int hitCount = 0;
    int missCount = 0;

    // Teste ein paar Pixel
    std::cout << "Teste einige Pixel:\n\n";

    // Zentrum des Bildes
    int x = width / 2;
    int y = height / 2;
    double px = 0.01 * ((x + 0.5) - width / 2.0);
    double py = 0.01 * ((y + 0.5) - height / 2.0);
    double pz = 0.0;
    Point pixelPos(px, py, pz);
    Ray ray = Ray::fromPoints(viewerPos, pixelPos);
    Intersections xs = sphere.intersect(ray);

    std::cout << "Zentrum (" << x << ", " << y << "):\n";
    std::cout << "  Pixel-Position: (" << px << ", " << py << ", " << pz << ")\n";
    std::cout << "  Schnittpunkte: " << xs.count() << "\n\n";

    // Ecke des Bildes
    x = 0;
    y = 0;
    px = 0.01 * ((x + 0.5) - width / 2.0);
    py = 0.01 * ((y + 0.5) - height / 2.0);
    pz = 0.0;
    pixelPos = Point(px, py, pz);
    ray = Ray::fromPoints(viewerPos, pixelPos);
    xs = sphere.intersect(ray);

    std::cout << "Ecke oben-links (" << x << ", " << y << "):\n";
    std::cout << "  Pixel-Position: (" << px << ", " << py << ", " << pz << ")\n";
    std::cout << "  Schnittpunkte: " << xs.count() << "\n\n";

    // Zähle alle Hits und Misses
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double px = 0.01 * ((x + 0.5) - width / 2.0);
            double py = 0.01 * ((y + 0.5) - height / 2.0);
            double pz = 0.0;
            Point pixelPos(px, py, pz);
            Ray ray = Ray::fromPoints(viewerPos, pixelPos);
            Intersections xs = sphere.intersect(ray);

            if (xs.count() > 0) {
                hitCount++;
            } else {
                missCount++;
            }
        }
    }

    std::cout << "Gesamtstatistik:\n";
    std::cout << "  Treffer: " << hitCount << " (" << (100.0 * hitCount / (width * height)) << "%)\n";
    std::cout << "  Fehltreffer: " << missCount << " (" << (100.0 * missCount / (width * height)) << "%)\n";
    std::cout << "  Gesamt: " << (hitCount + missCount) << "\n";

    return 0;
}
