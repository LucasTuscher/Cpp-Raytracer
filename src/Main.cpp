#include <iostream>

#include "Color/Color.h"
#include "Point/Point.h"
#include "Vector/Vector.h"

int main() {
    std::cout << "C++ Raytracer Project";

    // Farben erzeugen
    Color c1(0.3, 0.5, 0.7);
    Color c2 = Color::RED();  // vordefinierte Farbe

    // Addition
    Color sum = c1 + c2;

    // Skalarmultiplikation
    Color brighter = c1 * 1.5;

    // Hadamard-Produkt (komponentenweise Multiplikation)
    Color mixed = c1 * c2;

    // Clamping
    Color clamped = brighter.clamped();

    // Vergleich
    if (c1.equals(c2)) {
        std::cout << "Farben sind gleich\n";
    } else {
        std::cout << "Farben sind unterschiedlich\n";
    }

    // Ausgabe der Werte
    std::cout << "c1: (" << c1.r << ", " << c1.g << ", " << c1.b << ")\n";
    std::cout << "sum: (" << sum.r << ", " << sum.g << ", " << sum.b << ")\n";
    std::cout << "clamped: (" << clamped.r << ", " << clamped.g << ", " << clamped.b << ")\n";
    
    return 0;
}
