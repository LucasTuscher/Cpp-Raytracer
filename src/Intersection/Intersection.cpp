/**
 * Intersection.cpp
 *
 * Implementierung der Intersection-Klasse zum Speichern von Schnittpunktdaten.
 */
#include "Intersection.h"

/**
 * Konstruktor
 */
Intersection::Intersection(double t, const Shape* shape)
    : t(t), shape(shape) {
}

/**
 * Gleichheitsvergleich
 * Zwei Intersections sind gleich, wenn sie denselben t-Wert und dasselbe Objekt haben
 */
bool Intersection::operator==(const Intersection& other) const {
    return t == other.t && shape == other.shape;
}

/**
 * Ungleichheitsvergleich
 */
bool Intersection::operator!=(const Intersection& other) const {
    return !(*this == other);
}

/**
 * Kleiner-Vergleich
 * Vergleicht basierend auf dem t-Wert (für Sortierung)
 */
bool Intersection::operator<(const Intersection& other) const {
    return t < other.t;
}

/**
 * Größer-Vergleich
 */
bool Intersection::operator>(const Intersection& other) const {
    return t > other.t;
}

/**
 * Kleiner-gleich-Vergleich
 */
bool Intersection::operator<=(const Intersection& other) const {
    return t <= other.t;
}

/**
 * Größer-gleich-Vergleich
 */
bool Intersection::operator>=(const Intersection& other) const {
    return t >= other.t;
}
