#pragma once
#include <cmath>
#include <stdexcept>

/**
 * Vector-Klasse
 * 
 * Repräsentiert eine Richtung oder Verschiebung im 3D-Raum mit homogenen Koordinaten.
 * Die w-Komponente ist immer 0.0 für Vektoren (im Gegensatz zu Punkten mit w=1.0).
 * 
 * Vektoren haben keine Position, sondern nur eine Richtung und Länge.
 * Sie werden für Verschiebungen, Richtungen, Normalen und physikalische Größen verwendet.
 * 
 * Wichtige Operationen:
 * - Skalarprodukt (dot): Projektion, Winkelberechnung
 * - Kreuzprodukt (cross): Senkrechter Vektor, Flächenberechnung
 * - Normalisierung: Einheitsvektor in gleicher Richtung
 */
class Vector {
public:
    // Komponenten in homogenen Koordinaten
    double x, y, z, w;

    /**
     * Konstruktor
     * @param x X-Komponente (Standard: 0.0)
     * @param y Y-Komponente (Standard: 0.0)
     * @param z Z-Komponente (Standard: 0.0)
     * 
     * Die w-Komponente wird automatisch auf 0.0 gesetzt.
     */
    Vector(double x = 0.0, double y = 0.0, double z = 0.0)
        : x(x), y(y), z(z), w(0.0) {}

    /**
     * Addition: Vektor + Vektor
     * 
     * Addiert zwei Vektoren komponentenweise.
     * Geometrisch: Aneinanderreihen von Verschiebungen.
     * 
     * @param v Der zu addierende Vektor
     * @return Summenvektor
     */
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    
    /**
     * Subtraktion: Vektor - Vektor
     * 
     * Subtrahiert zwei Vektoren komponentenweise.
     * 
     * @param v Der zu subtrahierende Vektor
     * @return Differenzvektor
     */
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    
    /**
     * Negation: -Vektor
     * 
     * Kehrt die Richtung des Vektors um.
     * 
     * @return Vektor in entgegengesetzter Richtung
     */
    Vector operator-() const { return Vector(-x, -y, -z); }
    
    /**
     * Multiplikation: Vektor * Skalar
     * 
     * Skaliert die Länge des Vektors, behält die Richtung bei.
     * 
     * @param s Skalierungsfaktor
     * @return Skalierter Vektor
     */
    Vector operator*(double s) const { return Vector(x * s, y * s, z * s); }
    
    /**
     * Division: Vektor / Skalar
     * 
     * Teilt die Länge des Vektors durch einen Skalar.
     * 
     * @param s Divisor (darf nicht 0 sein)
     * @return Geteilter Vektor
     * @throws std::runtime_error bei Division durch Null
     */
    Vector operator/(double s) const {
        if (std::fabs(s) < 1e-12) throw std::runtime_error("Division durch Null");
        return Vector(x / s, y / s, z / s);
    }

    /**
     * Gleichheitsvergleich mit Epsilon-Toleranz
     * 
     * Vergleicht zwei Vektoren unter Berücksichtigung von Gleitkomma-Ungenauigkeiten.
     * 
     * @param v Der zu vergleichende Vektor
     * @return true wenn die Vektoren (nahezu) gleich sind
     */
    bool operator==(const Vector& v) const {
        const double eps = 1e-6;
        return std::fabs(x - v.x) <= eps && std::fabs(y - v.y) <= eps && std::fabs(z - v.z) <= eps;
    }
    
    /**
     * Ungleichheitsvergleich
     * 
     * @param v Der zu vergleichende Vektor
     * @return true wenn die Vektoren unterschiedlich sind
     */
    bool operator!=(const Vector& v) const { return !(*this == v); }

    /**
     * Berechnet die Länge (Magnitude) des Vektors
     * 
     * Verwendet die euklidische Norm: sqrt(x² + y² + z²)
     * 
     * @return Länge des Vektors
     */
    double magnitude() const { return std::sqrt(x*x + y*y + z*z); }
    
    /**
     * Berechnet die quadrierte Länge des Vektors
     * 
     * Effizienter als magnitude(), da keine Quadratwurzel berechnet wird.
     * Nützlich für Längenvergleiche: a.sqrMagnitude() < b.sqrMagnitude()
     * 
     * @return Quadrierte Länge (x² + y² + z²)
     */
    double sqrMagnitude() const { return x*x + y*y + z*z; }
    
    /**
     * Normalisiert den Vektor (Einheitsvektor)
     * 
     * Gibt einen Vektor mit Länge 1 in der gleichen Richtung zurück.
     * Bei Nullvektor wird (0,0,0) zurückgegeben.
     * 
     * @return Normalisierter Vektor (Länge = 1)
     */
    Vector normalized() const {
        double m = magnitude();
        if (m < 1e-12) return Vector(0,0,0);
        return *this / m;
    }
    
    /**
     * Skalarprodukt (Dot Product) zweier Vektoren
     * 
     * Berechnet: a.x*b.x + a.y*b.y + a.z*b.z
     * 
     * Geometrische Bedeutung:
     * - dot(a,b) = |a| * |b| * cos(θ)
     * - Positiv: Vektoren zeigen in ähnliche Richtung
     * - Null: Vektoren sind senkrecht zueinander
     * - Negativ: Vektoren zeigen in entgegengesetzte Richtungen
     * 
     * @param a Erster Vektor
     * @param b Zweiter Vektor
     * @return Skalarprodukt
     */
    static double dot(const Vector& a, const Vector& b) {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }
    
    /**
     * Kreuzprodukt (Cross Product) zweier Vektoren
     * 
     * Berechnet einen Vektor, der senkrecht auf beiden Eingangsvektoren steht.
     * Die Richtung folgt der Rechte-Hand-Regel im linkshändigen System.
     * 
     * Eigenschaften:
     * - cross(a,b) ist senkrecht zu a und b
     * - |cross(a,b)| = |a| * |b| * sin(θ)
     * - cross(a,b) = -cross(b,a) (antikommutativ)
     * 
     * Anwendungen:
     * - Berechnung von Normalen
     * - Flächenberechnung
     * - Koordinatensystem-Konstruktion
     * 
     * @param a Erster Vektor
     * @param b Zweiter Vektor
     * @return Kreuzprodukt (senkrecht zu a und b)
     */
    static Vector cross(const Vector& a, const Vector& b) {
        return Vector(
            a.y*b.z - a.z*b.y,
            a.z*b.x - a.x*b.z,
            a.x*b.y - a.y*b.x
        );
    }

    /**
     * Gibt den Nullvektor zurück
     * 
     * @return Vektor (0, 0, 0)
     */
    static Vector ZERO() { return Vector(0,0,0); }
};
