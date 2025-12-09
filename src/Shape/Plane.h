#pragma once
#include "Shape.h"
#include "../Intersection/Intersections.h"

/**
 * Plane-Klasse
 *
 * Unendliche Ebene in der xz-Ebene (y = 0) im lokalen Koordinatensystem.
 * Transformationen werden wie bei anderen Shapes über die Transformationsmatrix abgebildet.
 */
class Plane : public Shape {
public:
    Plane() : Shape() {}
    explicit Plane(const std::string& name) : Shape(name) {}
    ~Plane() override = default;

    /**
     * Schnitt mit unendlicher Ebene:
     * - Wenn die y-Komponente der Strahlrichtung ~0 ist, kein Schnitt (parallel).
     * - Sonst t = -origin.y / direction.y.
     */
    Intersections localIntersect(const Ray& localRay) const override;

    /**
     * Normale ist konstant (0, 1, 0) überall auf der Ebene.
     */
    Vector localNormalAt(const Point& localPoint) const override;
};
