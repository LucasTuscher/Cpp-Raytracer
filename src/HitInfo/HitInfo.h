#pragma once
#include "../Point/Point.h"
#include "../Vector/Vector.h"
#include "../Shape/Shape.h"

/**
 * HitInfo-Struktur
 *
 * Speichert alle relevanten Informationen über einen Schnittpunkt eines Strahls
 * mit einem Objekt. Diese Informationen werden für die Beleuchtungsberechnung benötigt.
 *
 * Enthält:
 * - t: Der t-Wert des Schnittpunkts (Distanz entlang des Strahls)
 * - shape: Zeiger auf das geschnittene Objekt
 * - point: Der eigentliche Schnittpunkt in Weltkoordinaten
 * - pointPlus: Punkt leicht über der Oberfläche (gegen Schatten-Akne)
 * - eyeVector: Vektor vom Punkt zur Kamera (normalisiert)
 * - normal: Normalenvektor am Schnittpunkt (normalisiert)
 */
class HitInfo {
public:
    double t;              // t-Wert des Schnittpunkts
    const Shape* shape;    // Zeiger auf das geschnittene Objekt
    Point point;           // Schnittpunkt in Weltkoordinaten
    Point pointPlus;       // Punkt leicht über der Oberfläche (für Schattenfühler)
    Vector eyeVector;      // Vektor vom Punkt zur Kamera (normalisiert)
    Vector normal;         // Normalenvektor am Schnittpunkt (normalisiert)
    Vector reflectVector;  // Reflektierter Vektor (für Reflexionsberechnung)

    /**
     * Standard-Konstruktor
     */
    HitInfo()
        : t(0.0),
          shape(nullptr),
          point(Point(0, 0, 0)),
          pointPlus(Point(0, 0, 0)),
          eyeVector(Vector(0, 0, 0)),
          normal(Vector(0, 0, 0)),
          reflectVector(Vector(0, 0, 0)) {}

    /**
     * Konstruktor mit allen Parametern
     */
    HitInfo(double t, const Shape* shape, const Point& point,
            const Vector& eyeVector, const Vector& normal)
        : t(t),
          shape(shape),
          point(point),
          pointPlus(point + normal * 1e-4),  // Epsilon-Verschiebung gegen Schatten-Akne
          eyeVector(eyeVector),
          normal(normal),
          reflectVector(Vector::reflect(-eyeVector, normal)) {}
};
