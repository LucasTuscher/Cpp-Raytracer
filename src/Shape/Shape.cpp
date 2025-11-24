/**
 * Shape.cpp
 *
 * Implementierung der Shape-Basisklasse.
 */
#include "Shape.h"
#include "../Intersection/Intersections.h"

// Initialisierung des statischen Zählers
int Shape::nextId_ = 0;

/**
 * Berechnet die Schnittpunkte zwischen diesem Objekt und einem Strahl (Weltkoordinaten)
 */
Intersections Shape::intersect(const Ray& worldRay) const {
    // Lazy Evaluation für inverse Matrix
    if (!inverseCached_) {
        if (cachedInverse_ != nullptr) {
            delete cachedInverse_;
        }
        cachedInverse_ = new Matrix(transformation_.inverse());
        inverseCached_ = true;
    }

    // Strahl ins lokale Koordinatensystem transformieren
    Ray localRay = worldRay.transform(*cachedInverse_);

    // Lokale Schnittberechnung durchführen
    return localIntersect(localRay);
}
