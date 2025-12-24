#pragma once
#include "Shape.h"
#include "../Intersection/Intersections.h"

/**
 * Cube-Klasse
 *
 * Axis-aligned Einheitswürfel im lokalen Koordinatensystem:
 * - min corner: (-1, -1, -1)
 * - max corner: ( 1,  1,  1)
 */
class Cube : public Shape {
public:
    Cube() : Shape() {}
    explicit Cube(const std::string& name) : Shape(name) {}
    ~Cube() override = default;

    /**
     * Berechnet die Schnittpunkte eines Strahls mit dem Einheitswürfel.
     */
    Intersections localIntersect(const Ray& localRay) const override;

    /**
     * Berechnet die Normale am Würfel (lokal).
     */
    Vector localNormalAt(const Point& localPoint) const override;

    /**
     * UV-Mapping: aktuell nicht spezifiziert -> (0,0).
     */
    UV localUVAt(const Point& /*localPoint*/) const override { return UV(0.0, 0.0); }
};

