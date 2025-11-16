/**
 * Main.cpp
 *
 * Ray Tracer Test-Programm
 *
 * Erzeugt Test-Bilder zur Visualisierung von:
 * 1. Pixel-Koordinaten (Launch ID) → archiv/
 * 2. Strahl-Richtungen → archiv/
 * 3. Strahl-Längen → archiv/
 * 4. Kugel-Strahlrichtungen (mit 0.01 Verkleinerung)
 * 5. Kugel-Schnittpunkt (Hit/Miss)
 * 6. Kugel-Schnittpunkt t-Wert
 * 7. Kugel-Normalenvektoren
 */
#include <iostream>
#include <cmath>

// Platform-spezifische Includes für mkdir
#ifdef _WIN32
#include <direct.h>  // _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "Point/Point.h"
#include "Vector/Vector.h"
#include "Color/Color.h"
#include "Canvas/Canvas.h"
#include "Ray/Ray.h"
#include "Shape/Sphere.h"
#include "Intersection/Intersections.h"

/**
 * Erzeugt ein Testbild: Pixel-Koordinaten als Farbe
 *
 * Kodierung:
 * - Rot-Kanal: x-Koordinate normalisiert (0 bis 1)
 * - Grün-Kanal: y-Koordinate normalisiert (0 bis 1)
 * - Blau-Kanal: 0
 */
void renderPixelCoordinates(int width, int height, const Point& viewerPos) {
    Canvas canvas(width, height, "archiv/test-pixel-coordinates");
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Normalisiere Pixel-Koordinaten auf [0,1]
            double r = static_cast<double>(x) / width;
            double g = static_cast<double>(y) / height;
            
            Color color(r, g, 0.0);
            canvas.setPixel(x, y, color);
        }
    }
    
    canvas.save();
    std::cout << " Pixel-Koordinaten Bild erstellt\n";
}

/**
 * Erzeugt ein Testbild: Strahl-Richtungen als Farbe
 *
 * Kodierung:
 * - Rot-Kanal: Absolut-Wert der x-Komponente der Richtung
 * - Grün-Kanal: Absolut-Wert der y-Komponente der Richtung
 * - Blau-Kanal: Absolut-Wert der z-Komponente der Richtung
 */
void renderRayDirections(int width, int height, const Point& viewerPos) {
    Canvas canvas(width, height, "archiv/test-ray-directions");
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Berechne Pixel-Position im Welt-Koordinatensystem
            // Bild-Mitte liegt im Ursprung (0, 0, 0)
            double px = (x + 0.5) - width / 2.0;
            double py = (y + 0.5) - height / 2.0;
            double pz = 0.0;
            
            Point pixelPos(px, py, pz);
            
            // Erzeuge Strahl vom Betrachter zum Pixel
            Ray ray = Ray::fromPoints(viewerPos, pixelPos);
            Vector dir = ray.getDirection();
            
            // Kodiere Richtung als Farbe (Absolut-Werte)
            Color color(std::abs(dir.x), std::abs(dir.y), std::abs(dir.z));
            canvas.setPixel(x, y, color);
        }
    }
    
    canvas.save();
    std::cout << " Strahl-Richtungen Bild erstellt\n";
}

/**
 * Erzeugt ein Testbild: Strahl-Längen als Graustufen
 *
 * Kodierung:
 * - Alle Kanäle: Länge des Strahls vom Betrachter zum Pixel
 *   (normalisiert durch Division durch maximale Länge)
 */
void renderRayLengths(int width, int height, const Point& viewerPos) {
    Canvas canvas(width, height, "archiv/test-ray-lengths");
    
    // Berechne maximale Strahl-Länge (Ecke des Bildes)
    double cornerX = width / 2.0;
    double cornerY = height / 2.0;
    Point corner(cornerX, cornerY, 0.0);
    Vector toCorner = corner - viewerPos;
    double maxLength = toCorner.magnitude();
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Berechne Pixel-Position im Welt-Koordinatensystem
            double px = (x + 0.5) - width / 2.0;
            double py = (y + 0.5) - height / 2.0;
            double pz = 0.0;
            
            Point pixelPos(px, py, pz);
            
            // Berechne Abstand vom Betrachter zum Pixel
            Vector toPixel = pixelPos - viewerPos;
            double length = toPixel.magnitude();
            
            // Normalisiere Länge auf [0,1]
            double normalized = length / maxLength;
            
            // Kodiere als Graustufen
            Color color(normalized, normalized, normalized);
            canvas.setPixel(x, y, color);
        }
    }
    
    canvas.save();
    std::cout << " Strahl-Längen Bild erstellt\n";
}

/**
 * Erzeugt ein Testbild: Strahlrichtungen für Kugel-Visualisierung
 *
 * Kodierung:
 * - Rot-Kanal: Absolutwert der x-Komponente der Strahlrichtung
 * - Grün-Kanal: Absolutwert der y-Komponente der Strahlrichtung
 * - Blau-Kanal: 0 (z-Komponente wird ignoriert)
 *
 * Verwendet die künstliche 0.01-Verkleinerung wie bei den anderen Kugel-Tests
 * Das Bild wird sehr dunkel sein, weil die Strahlen fast geradeaus gehen
 */
void renderSphereRayDirections(int width, int height, const Point& viewerPos) {
    Canvas canvas(width, height, "test-sphere-raydirections");

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Berechne Pixel-Position MIT künstlicher Verkleinerung (0.01)
            // Genau wie bei den anderen Kugel-Tests
            double px = 0.01 * ((x + 0.5) - width / 2.0);
            double py = 0.01 * ((y + 0.5) - height / 2.0);
            double pz = 0.0;

            Point pixelPos(px, py, pz);

            // Erzeuge Strahl vom Betrachter zum Pixel
            Ray ray = Ray::fromPoints(viewerPos, pixelPos);
            Vector dir = ray.getDirection();

            // Kodiere x und y Komponenten als Farbe (z wird ignoriert)
            // Absolutbeträge, um negative Werte zu vermeiden
            // Verstärke die Werte mit Faktor 5, damit sie besser sichtbar sind
            double r = std::abs(dir.x) * 5.0;
            double g = std::abs(dir.y) * 5.0;

            Color color(r, g, 0.0);
            canvas.setPixel(x, y, color);
        }
    }

    canvas.save();
    std::cout << " Kugel Strahlrichtungen Bild erstellt\n";
}

/**
 * Erzeugt ein Testbild: Kugel Hit/Miss Visualisierung
 *
 * Kodierung:
 * - Orange: Kugel wurde getroffen
 * - Blau: Kugel wurde nicht getroffen
 */
void renderSphereHitMiss(int width, int height, const Point& viewerPos) {
    Canvas canvas(width, height, "test-sphere-hitmiss");

    // Kugel im Ursprung mit Radius 1
    Sphere sphere;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Berechne Pixel-Position (künstlich verkleinert, wie im Skript)
            double px = 0.01 * ((x + 0.5) - width / 2.0);
            double py = 0.01 * ((y + 0.5) - height / 2.0);
            double pz = 0.0;

            Point pixelPos(px, py, pz);

            // Erzeuge Strahl vom Betrachter zum Pixel
            Ray ray = Ray::fromPoints(viewerPos, pixelPos);

            // Teste auf Schnittpunkt mit Kugel
            Intersections xs = sphere.intersect(ray);

            // Färbe basierend auf Hit/Miss
            Color color;
            if (xs.count() > 0) {
                color = Color(1.0, 0.5, 0.0); // Orange
            } else {
                color = Color(0.0, 0.0, 1.0); // Blau
            }

            canvas.setPixel(x, y, color);
        }
    }

    canvas.save();
    std::cout << " Kugel Hit/Miss Bild erstellt\n";
}

/**
 * Erzeugt ein Testbild: Kugel t-Wert Visualisierung
 *
 * Kodierung:
 * - Graustufen: t-Wert - 9 (da alle Schnittpunkte zwischen 9 und 10 liegen)
 * - Schwarz: Kein Schnittpunkt
 */
void renderSphereTValue(int width, int height, const Point& viewerPos) {
    Canvas canvas(width, height, "test-sphere-tvalue");

    // Kugel im Ursprung mit Radius 1
    Sphere sphere;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Berechne Pixel-Position (künstlich verkleinert)
            double px = 0.01 * ((x + 0.5) - width / 2.0);
            double py = 0.01 * ((y + 0.5) - height / 2.0);
            double pz = 0.0;

            Point pixelPos(px, py, pz);

            // Erzeuge Strahl vom Betrachter zum Pixel
            Ray ray = Ray::fromPoints(viewerPos, pixelPos);

            // Teste auf Schnittpunkt mit Kugel
            Intersections xs = sphere.intersect(ray);

            // Färbe basierend auf t-Wert
            Color color(0, 0, 0); // Standard: Schwarz (kein Hit)

            if (xs.count() > 0) {
                // Nimm den kleinsten t-Wert (erster Schnittpunkt)
                double t = xs[0].t;

                // Subtrahiere 9 und nutze als Graustufen
                // (da t zwischen ca. 9 und 10 liegt)
                double gray = t - 9.0;
                gray = std::max(0.0, std::min(1.0, gray)); // Clamping

                color = Color(gray, gray, gray);
            }

            canvas.setPixel(x, y, color);
        }
    }

    canvas.save();
    std::cout << " Kugel t-Wert Bild erstellt\n";
}

/**
 * Erzeugt ein Testbild: Kugel Normalenvektoren Visualisierung
 *
 * Kodierung:
 * - RGB: Absolutbeträge der Normalenvektor-Komponenten
 * - Schwarz: Kein Schnittpunkt
 */
void renderSphereNormals(int width, int height, const Point& viewerPos) {
    Canvas canvas(width, height, "test-sphere-normals");

    // Kugel im Ursprung mit Radius 1
    Sphere sphere;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Berechne Pixel-Position (künstlich verkleinert)
            double px = 0.01 * ((x + 0.5) - width / 2.0);
            double py = 0.01 * ((y + 0.5) - height / 2.0);
            double pz = 0.0;

            Point pixelPos(px, py, pz);

            // Erzeuge Strahl vom Betrachter zum Pixel
            Ray ray = Ray::fromPoints(viewerPos, pixelPos);

            // Teste auf Schnittpunkt mit Kugel
            Intersections xs = sphere.intersect(ray);

            // Färbe basierend auf Normalenvektor
            Color color(0, 0, 0); // Standard: Schwarz (kein Hit)

            if (xs.count() > 0) {
                // Nimm den ersten Schnittpunkt
                double t = xs[0].t;

                // Berechne Schnittpunkt-Position
                Point hitPoint = ray.pointAt(t);

                // Berechne Normalenvektor an diesem Punkt
                Vector normal = sphere.normalAt(hitPoint);

                // Kodiere Normalenvektor als Farbe (Absolutbeträge)
                color = Color(
                    std::abs(normal.x),
                    std::abs(normal.y),
                    std::abs(normal.z)
                );
            }

            canvas.setPixel(x, y, color);
        }
    }

    canvas.save();
    std::cout << " Kugel Normalen Bild erstellt\n";
}

/**
 * Erstellt einen Ordner (falls nicht vorhanden)
 */
void createDirectory(const std::string& path) {
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0755);
#endif
}

/**
 * Hauptprogramm
 */
int main() {
    std::cout << "=== Ray Tracer - Kapitel 4: Kugeln und Schnitte ===\n\n";

    // Erstelle archiv-Ordner für alte Test-Bilder
    createDirectory("archiv");

    // Bild-Einstellungen
    const int width = 400;
    const int height = 300;

    // Betrachter-Position für Kugel-Tests (z = -10)
    Point viewerPos(0, 0, -10);

    // Betrachter-Position für Kapitel 2 Tests (z = -100)
    Point viewerPos2(0, 0, -100);

    std::cout << "Bildgröße: " << width << " x " << height << "\n";
    std::cout << "Betrachter-Position Kugel-Tests: ("
              << viewerPos.x << ", "
              << viewerPos.y << ", "
              << viewerPos.z << ")\n";
    std::cout << "Betrachter-Position Grundlagen-Tests: ("
              << viewerPos2.x << ", "
              << viewerPos2.y << ", "
              << viewerPos2.z << ")\n\n";

    std::cout << "Erzeuge Visualisierungen...\n\n";

    // ========================================================================
    // Kapitel 2: Grundlegende Tests → archiv/
    // ========================================================================
    std::cout << "Grundlagen-Visualisierungen (→ archiv/):\n";

    // Test 1: Pixel-Koordinaten
    renderPixelCoordinates(width, height, viewerPos2);

    // Test 2: Strahl-Richtungen
    renderRayDirections(width, height, viewerPos2);

    // Test 3: Strahl-Längen
    renderRayLengths(width, height, viewerPos2);

    std::cout << "\n";

    // ========================================================================
    // Kapitel 4: Kugel-Schnittpunkt Tests → Hauptordner
    // ========================================================================
    std::cout << "Kugel-Visualisierungen (→ Hauptordner):\n";

    // Test 4: Strahlrichtungen für Kugel-Tests
    renderSphereRayDirections(width, height, viewerPos);

    // Test 5: Kugel Hit/Miss
    renderSphereHitMiss(width, height, viewerPos);

    // Test 6: Kugel t-Wert
    renderSphereTValue(width, height, viewerPos);

    // Test 7: Kugel Normalenvektoren
    renderSphereNormals(width, height, viewerPos);

    std::cout << "\n=== Alle 7 Bilder erfolgreich erstellt! ===\n";
    std::cout << "Archiv-Bilder (3): archiv/*.ppm\n";
    std::cout << "Kugel-Bilder (4): *.ppm (Hauptordner)\n";
    std::cout << "\nDie Bilder wurden im PPM-Format gespeichert.\n";
    std::cout << "Sie können mit den meisten Bildbetrachtungsprogrammen geöffnet werden.\n";

    return 0;
}
