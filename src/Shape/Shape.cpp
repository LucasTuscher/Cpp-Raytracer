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

/**
 * Berechnet den Normalenvektor an einem Punkt auf der Oberfläche (Weltkoordinaten)
 *
 * Algorithmus:
 * 1. Transformiere den Punkt von Welt- in lokale Koordinaten
 * 2. Berechne die lokale Normale (formspezifisch)
 * 3. Transformiere die Normale zurück (mit Transponierter der Inversen!)
 * 4. Normalisiere das Ergebnis
 */
Vector Shape::normalAt(const Point& worldPoint) const {
    // Lazy Evaluation für inverse Matrix
    if (!inverseCached_) {
        if (cachedInverse_ != nullptr) {
            delete cachedInverse_;
        }
        cachedInverse_ = new Matrix(transformation_.inverse());
        inverseCached_ = true;
    }

    // 1. Transformiere Punkt ins lokale Koordinatensystem
    Point localPoint = (*cachedInverse_) * worldPoint;

    // 2. Berechne lokale Normale (formspezifisch)
    Vector localNormal = localNormalAt(localPoint);

    // 3. Transformiere Normale ins Weltkoordinatensystem
    // WICHTIG: Normalen werden mit der Transponierten der inversen Matrix transformiert!
    Matrix inverseTranspose = cachedInverse_->transpose();
    Vector worldNormal = inverseTranspose * localNormal;

    // 4. w-Komponente auf 0 setzen und normalisieren
    worldNormal.w = 0.0;
    return worldNormal.normalized();
}

UV Shape::uvAt(const Point& worldPoint) const {
    // Lazy Evaluation für inverse Matrix
    if (!inverseCached_) {
        if (cachedInverse_ != nullptr) {
            delete cachedInverse_;
        }
        cachedInverse_ = new Matrix(transformation_.inverse());
        inverseCached_ = true;
    }

    // Punkt ins lokale Koordinatensystem transformieren
    Point localPoint = (*cachedInverse_) * worldPoint;
    return localUVAt(localPoint);
}
