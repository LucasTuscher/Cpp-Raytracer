/**
 * Main.cpp
 * 
 * Ray Tracer Test-Programm
 * 
 * Erzeugt Test-Bilder zur Visualisierung von:
 * 1. Pixel-Koordinaten (Launch ID)
 * 2. Strahl-Richtungen
 * 3. Strahl-Längen
 */
#include <iostream>
#include <cmath>
#include "Point/Point.h"
#include "Vector/Vector.h"
#include "Color/Color.h"
#include "Canvas/Canvas.h"
#include "Ray/Ray.h"

/**
 * Erzeugt ein Testbild: Pixel-Koordinaten als Farbe
 * 
 * Kodierung:
 * - Rot-Kanal: x-Koordinate normalisiert (0 bis 1)
 * - Grün-Kanal: y-Koordinate normalisiert (0 bis 1)
 * - Blau-Kanal: 0
 */
void renderPixelCoordinates(int width, int height, const Point& viewerPos) {
    Canvas canvas(width, height, "test-pixel-coordinates");
    
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
    Canvas canvas(width, height, "test-ray-directions");
    
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
    Canvas canvas(width, height, "test-ray-lengths");
    
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
 * Hauptprogramm
 */
int main() {
    std::cout << "=== Ray Tracer Test-Programm ===\n\n";
    
    // Bild-Einstellungen
    const int width = 400;
    const int height = 300;
    
    // Betrachter-Position (z = -100, wie in der Aufgabenstellung)
    Point viewerPos(0, 0, -100);
    
    std::cout << "Bildgröße: " << width << " x " << height << "\n";
    std::cout << "Betrachter-Position: (" 
              << viewerPos.x << ", " 
              << viewerPos.y << ", " 
              << viewerPos.z << ")\n\n";
    
    std::cout << "Erzeuge Test-Bilder...\n";
    
    // Test 1: Pixel-Koordinaten
    renderPixelCoordinates(width, height, viewerPos);
    
    // Test 2: Strahl-Richtungen
    renderRayDirections(width, height, viewerPos);
    
    // Test 3: Strahl-Längen
    renderRayLengths(width, height, viewerPos);
    
    std::cout << "\n=== Alle Test-Bilder erfolgreich erstellt! ===\n";
    std::cout << "Die Bilder wurden im PPM-Format gespeichert.\n";
    std::cout << "Sie können mit den meisten Bildbetrachtungsprogrammen geöffnet werden.\n";
    
    return 0;
}
