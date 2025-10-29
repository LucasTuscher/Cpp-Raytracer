/**
 * Main.cpp
 * 
 * Demo-Programm für den C++ Raytracer.
 * Zeigt die Verwendung der Point-, Vector- und Color-Klassen.
 */
#include <iostream>
#include "Point/Point.h"
#include "Vector/Vector.h"
#include "Color/Color.h"

int main() {
    // Beispiel-Punkte im 3D-Raum erstellen
    Point p1(1, 2, 3);
    Point p2(3, 2, 1);
    
    // Beispiel-Vektor erstellen
    Vector v1(1, -2, 3);
    
    // Beispiel-Farben erstellen
    Color c1(0.3, 0.5, 0.7);  // Benutzerdefinierte Farbe
    Color c2 = Color::RED();   // Vordefinierte Farbe

    // Vektor-Operationen demonstrieren
    Vector diff = p1 - p2;             // Punkt - Punkt = Vektor(-2, 0, 2)
    Point moved = p1 + v1;             // Punkt + Vektor = Punkt(2, 0, 6)
    Vector cross = Vector::cross(v1, diff);  // Kreuzprodukt berechnen
    
    // Farb-Operationen demonstrieren
    Color mixed = c1 * c2;             // Hadamard-Produkt (komponentenweise Multiplikation)
    Color bright = (c1 * 2.5).clamped();  // Skalierung und Clamping auf [0,1]

    // Ergebnisse ausgeben
    std::cout << "Differenzvektor (p1 - p2): (" 
              << diff.x << ", " << diff.y << ", " << diff.z << ")\n";
    
    std::cout << "Verschobener Punkt (p1 + v1): (" 
              << moved.x << ", " << moved.y << ", " << moved.z << ")\n";
    
    std::cout << "Kreuzprodukt (v1 x diff): (" 
              << cross.x << ", " << cross.y << ", " << cross.z << ")\n";
    
    std::cout << "Aufgehellte und geclampte Farbe: (" 
              << bright.r << ", " << bright.g << ", " << bright.b << ")\n";

    return 0;
}
