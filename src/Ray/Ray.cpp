/**
 * Ray.cpp
 * 
 * Implementierung der Ray-Klasse für Strahlverfolgung.
 */
#include "Ray.h"

/**
 * Konstruktor: Strahl aus Ursprung und Richtung
 * 
 * Der Richtungsvektor wird unverändert übernommen.
 */
Ray::Ray(const Point& origin, const Vector& direction)
    : origin(origin), direction(direction) {
}

/**
 * Erstellt einen Strahl aus zwei Punkten
 * 
 * Der Richtungsvektor wird aus der Differenz berechnet
 * und automatisch normalisiert.
 */
Ray Ray::fromPoints(const Point& origin, const Point& target) {
    Vector dir = target - origin;
    return Ray(origin, dir.normalized());
}

/**
 * Berechnet einen Punkt auf dem Strahl
 * 
 * Verwendet die Parameterform: P(t) = O + t * d
 */
Point Ray::pointAt(double t) const {
    return origin + direction * t;
}
