/**
 * TransformationDemo.cpp
 *
 * Demonstriert transformierte Kugeln aus Kapitel 5
 *
 * Erzeugt 4 Test-Bilder zur Visualisierung von:
 * 1. Keine Transformation (Referenz)
 * 2. Skalierung um 0.5 in x-Richtung
 * 3. Skalierung um 0.5 in y-Richtung
 * 4. Rotation um π/4 um z-Achse + Skalierung um 0.5 in x-Richtung
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
#include "Matrix/Matrix.h"
#include "Intersection/Intersections.h"

/**
 * Rendert eine Silhouette der Kugel mit gegebener Transformation
 *
 * Setup (aus der Aufgabenbeschreibung):
 * - Strahl-Ursprung: (0, 0, -5)
 * - Wand-Position: z = 10
 * - Wand-Größe: 7 Einheiten
 * - Canvas-Größe: 400x400 Pixel
 *
 * @param sphere Die zu rendernde Kugel (mit Transformation)
 * @param filename Dateiname ohne Erweiterung
 * @param description Beschreibung für die Konsole
 */
void renderSphereTransformation(const Sphere& sphere, const std::string& filename, const std::string& description) {
    const int WIDTH = 400;
    const int HEIGHT = 400;
    const double WALL_Z = 10.0;
    const double WALL_SIZE = 7.0;
    const double HALF_SIZE = WALL_SIZE / 2.0;
    const double PIXEL_SIZE = WALL_SIZE / WIDTH;

    Canvas canvas(WIDTH, HEIGHT, filename);

    // Strahl-Ursprung
    Point rayOrigin(0, 0, -5);

    // Farben
    Color orange(1.0, 0.5, 0.0);  // Hit
    Color blue(0.0, 0.3, 0.8);    // Miss

    for (int y = 0; y < HEIGHT; y++) {
        // Berechne Welt-y-Koordinate
        double worldY = HALF_SIZE - PIXEL_SIZE * y;

        for (int x = 0; x < WIDTH; x++) {
            // Berechne Welt-x-Koordinate
            double worldX = -HALF_SIZE + PIXEL_SIZE * x;

            // Zielposition auf der Wand
            Point position(worldX, worldY, WALL_Z);

            // Erzeuge Strahl vom Ursprung zur Position
            Ray ray = Ray::fromPoints(rayOrigin, position);

            // Teste Schnittpunkt mit der Kugel
            Intersections xs = sphere.intersect(ray);

            // Färbe Pixel basierend auf Hit/Miss
            Color color;
            if (xs.count() > 0) {
                color = orange;
            } else {
                color = blue;
            }

            canvas.setPixel(x, y, color);
        }
    }

    canvas.save();
    std::cout << " " << description << " erstellt (" << filename << ".ppm)\n";
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
    std::cout << "=== Ray Tracer - Kapitel 5: Transformationen ===\n\n";

    // Erstelle transformations-Ordner
    createDirectory("transformations");

    std::cout << "Konfiguration:\n";
    std::cout << "  Canvas: 400 x 400 Pixel\n";
    std::cout << "  Strahl-Ursprung: (0, 0, -5)\n";
    std::cout << "  Wand: z = 10, Größe 7 x 7\n\n";

    std::cout << "Erzeuge transformierte Kugel-Visualisierungen...\n\n";

    // ========================================================================
    // Test 1: Keine Transformation (Referenz)
    // ========================================================================
    {
        Sphere sphere;
        // Keine Transformation → Einheitsmatrix (Standard)
        renderSphereTransformation(
            sphere,
            "transformations/01-no-transformation",
            "Test 1: Keine Transformation"
        );
    }

    // ========================================================================
    // Test 2: Skalierung um 0.5 in x-Richtung
    // ========================================================================
    {
        Sphere sphere;
        sphere.setTransform(Matrix::scale(0.5, 1.0, 1.0));
        renderSphereTransformation(
            sphere,
            "transformations/02-scale-x-0.5",
            "Test 2: Skalierung um 0.5 in x-Richtung"
        );
    }

    // ========================================================================
    // Test 3: Skalierung um 0.5 in y-Richtung
    // ========================================================================
    {
        Sphere sphere;
        sphere.setTransform(Matrix::scale(1.0, 0.5, 1.0));
        renderSphereTransformation(
            sphere,
            "transformations/03-scale-y-0.5",
            "Test 3: Skalierung um 0.5 in y-Richtung"
        );
    }

    // ========================================================================
    // Test 4: Rotation um π/4 um z-Achse + Skalierung um 0.5 in x-Richtung
    // ========================================================================
    {
        Sphere sphere;
        const double PI = 3.14159265358979323846;
        Matrix rotation = Matrix::rotateZ(PI / 4.0);
        Matrix scaling = Matrix::scale(0.5, 1.0, 1.0);
        // Wichtig: Transformationen werden von rechts nach links angewendet
        // Zuerst skalieren, dann rotieren
        sphere.setTransform(rotation * scaling);
        renderSphereTransformation(
            sphere,
            "transformations/04-rotate-z-scale-x",
            "Test 4: Rotation π/4 um z + Skalierung 0.5 in x"
        );
    }

    std::cout << "\n=== Alle 4 Transformations-Bilder erfolgreich erstellt! ===\n";
    std::cout << "Speicherort: transformations/*.ppm\n\n";

    std::cout << "Erklärung der Transformationen:\n";
    std::cout << "  1. Keine Transformation: Perfekter Kreis (Einheitskugel)\n";
    std::cout << "  2. Scale X 0.5: Ellipse, horizontal gestaucht\n";
    std::cout << "  3. Scale Y 0.5: Ellipse, vertikal gestaucht\n";
    std::cout << "  4. Rotate + Scale: Ellipse, diagonal gedreht\n\n";

    std::cout << "Die Bilder wurden im PPM-Format gespeichert.\n";
    std::cout << "Sie können mit den meisten Bildbetrachtungsprogrammen geöffnet werden.\n";

    return 0;
}
