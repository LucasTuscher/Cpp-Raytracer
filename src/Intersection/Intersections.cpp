/**
 * Intersections.cpp
 *
 * Implementierung der Intersections-Klasse zur Verwaltung mehrerer Schnittpunkte.
 */
#include "Intersections.h"
#include <algorithm>
#include <stdexcept>

/**
 * Standard-Konstruktor
 */
Intersections::Intersections() {
}

/**
 * Konstruktor mit initializer_list
 * Sortiert die Schnittpunkte automatisch
 */
Intersections::Intersections(std::initializer_list<Intersection> list)
    : intersections(list) {
    sort();
}

/**
 * Konstruktor mit Vektor
 * Sortiert die Schnittpunkte automatisch
 */
Intersections::Intersections(const std::vector<Intersection>& vec)
    : intersections(vec) {
    sort();
}

/**
 * Sortiert die Intersections nach t-Werten (aufsteigend)
 */
void Intersections::sort() {
    std::sort(intersections.begin(), intersections.end(),
        [](const Intersection& a, const Intersection& b) {
            return a.t < b.t;
        });
}

/**
 * Fügt einen Schnittpunkt hinzu und sortiert neu
 */
void Intersections::add(const Intersection& intersection) {
    intersections.push_back(intersection);
    sort();
}

/**
 * Gibt die Anzahl der Schnittpunkte zurück
 */
size_t Intersections::count() const {
    return intersections.size();
}

/**
 * Prüft, ob die Liste leer ist
 */
bool Intersections::isEmpty() const {
    return intersections.empty();
}

/**
 * Indizierter Zugriff (const)
 */
const Intersection& Intersections::operator[](size_t index) const {
    if (index >= intersections.size()) {
        throw std::out_of_range("Intersection index out of range");
    }
    return intersections[index];
}

/**
 * Indizierter Zugriff
 */
Intersection& Intersections::operator[](size_t index) {
    if (index >= intersections.size()) {
        throw std::out_of_range("Intersection index out of range");
    }
    return intersections[index];
}

/**
 * Bestimmt den Hit - den ersten sichtbaren Schnittpunkt
 *
 * Der Hit ist der Schnittpunkt mit dem kleinsten positiven t-Wert.
 */
const Intersection* Intersections::hit() const {
    // Die Intersections sind bereits sortiert
    for (const auto& intersection : intersections) {
        if (intersection.t >= 0.0) {
            return &intersection;
        }
    }
    return nullptr; // Kein positiver Schnittpunkt gefunden
}

/**
 * Gibt direkten Zugriff auf den internen Vektor
 */
const std::vector<Intersection>& Intersections::getIntersections() const {
    return intersections;
}
