#pragma once
#include "../Vector/Vector.h"
#include <stdexcept>
#include <cmath>
#include <algorithm>

/**
 * Point-Klasse
 * 
 * Repräsentiert einen Punkt im 3D-Raum mit homogenen Koordinaten.
 * Die w-Komponente ist immer 1.0 für Punkte (im Gegensatz zu Vektoren mit w=0.0).
 * 
 * Homogene Koordinaten ermöglichen einheitliche Transformationen durch Matrizen,
 * insbesondere für Translationen, die in 3D keine linearen Transformationen sind.
 * 
 * Koordinatensystem: Linkshändig (Unity-Konvention)
 * - x-Achse: nach rechts
 * - y-Achse: nach oben
 * - z-Achse: vom Betrachter weg
 */
class Point {
public:
    // Komponenten in homogenen Koordinaten
    double x, y, z, w;

    /**
     * Konstruktor
     * @param x X-Koordinate (Standard: 0.0)
     * @param y Y-Koordinate (Standard: 0.0)
     * @param z Z-Koordinate (Standard: 0.0)
     * 
     * Die w-Komponente wird automatisch auf 1.0 gesetzt.
     */
    Point(double x = 0.0, double y = 0.0, double z = 0.0)
        : x(x), y(y), z(z), w(1.0) {}

    /**
     * Subtraktion: Punkt - Punkt = Vektor
     * 
     * Berechnet den Richtungsvektor vom Punkt p zu diesem Punkt.
     * Dies ist eine der fundamentalen Operationen in der Geometrie.
     * 
     * @param p Der andere Punkt
     * @return Vektor von p zu diesem Punkt
     */
    Vector operator-(const Point& p) const { return Vector(x - p.x, y - p.y, z - p.z); }

    /**
     * Addition: Punkt + Vektor = Punkt
     * 
     * Verschiebt den Punkt um den gegebenen Vektor.
     * 
     * @param v Verschiebungsvektor
     * @return Neuer verschobener Punkt
     */
    Point operator+(const Vector& v) const { return Point(x + v.x, y + v.y, z + v.z); }
    
    /**
     * Subtraktion: Punkt - Vektor = Punkt
     * 
     * Verschiebt den Punkt entgegen dem gegebenen Vektor.
     * 
     * @param v Verschiebungsvektor
     * @return Neuer verschobener Punkt
     */
    Point operator-(const Vector& v) const { return Point(x - v.x, y - v.y, z - v.z); }

    /**
     * Multiplikation: Punkt * Skalar
     * 
     * Skaliert die Koordinaten des Punktes.
     * Nützlich für Interpolationen und Transformationen.
     * 
     * @param s Skalierungsfaktor
     * @return Neuer skalierter Punkt
     */
    Point operator*(double s) const { return Point(x * s, y * s, z * s); }
    
    /**
     * Division: Punkt / Skalar
     * 
     * Teilt die Koordinaten des Punktes durch einen Skalar.
     * 
     * @param s Divisor (darf nicht 0 sein)
     * @return Neuer geteilter Punkt
     * @throws std::runtime_error bei Division durch Null
     */
    Point operator/(double s) const {
        if (std::fabs(s) < 1e-12) throw std::runtime_error("Division durch Null");
        return Point(x / s, y / s, z / s);
    }

    /**
     * Gleichheitsvergleich mit Epsilon-Toleranz
     * 
     * Vergleicht zwei Punkte unter Berücksichtigung von Gleitkomma-Ungenauigkeiten.
     * Verwendet eine Toleranz von 1e-6.
     * 
     * @param p Der zu vergleichende Punkt
     * @return true wenn die Punkte (nahezu) gleich sind
     */
    bool operator==(const Point& p) const {
        const double eps = 1e-6;
        return std::fabs(x - p.x) <= eps && std::fabs(y - p.y) <= eps && std::fabs(z - p.z) <= eps;
    }
    
    /**
     * Ungleichheitsvergleich
     * 
     * @param p Der zu vergleichende Punkt
     * @return true wenn die Punkte unterschiedlich sind
     */
    bool operator!=(const Point& p) const { return !(*this == p); }

    /**
     * Komponentenweises Minimum zweier Punkte
     * 
     * Nützlich für die Berechnung von Bounding Boxes.
     * 
     * @param a Erster Punkt
     * @param b Zweiter Punkt
     * @return Punkt mit minimalen Komponenten
     */
    static Point min(const Point& a, const Point& b) {
        return Point(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    }
    
    /**
     * Komponentenweises Maximum zweier Punkte
     * 
     * Nützlich für die Berechnung von Bounding Boxes.
     * 
     * @param a Erster Punkt
     * @param b Zweiter Punkt
     * @return Punkt mit maximalen Komponenten
     */
    static Point max(const Point& a, const Point& b) {
        return Point(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }
};

/**
 * Kommutative Multiplikation: Skalar * Punkt
 * 
 * Ermöglicht die Schreibweise "2.0 * punkt" zusätzlich zu "punkt * 2.0".
 * 
 * @param s Skalierungsfaktor
 * @param p Punkt
 * @return Skalierter Punkt
 */
inline Point operator*(double s, const Point& p) {
    return p * s;
}
