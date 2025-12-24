#pragma once
#include "Shape.h"
#include "../Intersection/Intersections.h"
#include <limits>

/**
 * Cylinder-Klasse
 *
 * Zylinder in Standardlage:
 * - Achse entlang der y-Achse
 * - Radius = 1
 * - Standardmäßig unendlich in y-Richtung (minimum=-inf, maximum=inf)
 *
 * Optional:
 * - Höhenbegrenzung über minimum/maximum (exklusiv)
 * - Deckflächen über closed=true
 */
class Cylinder : public Shape {
public:
    double minimum;
    double maximum;
    bool closed;

    Cylinder();
    explicit Cylinder(const std::string& name);
    ~Cylinder() override = default;

    Intersections localIntersect(const Ray& localRay) const override;
    Vector localNormalAt(const Point& localPoint) const override;
    UV localUVAt(const Point& /*localPoint*/) const override { return UV(0.0, 0.0); }

private:
    static bool checkCap(const Ray& localRay, double t);
    Intersections intersectCaps(const Ray& localRay) const;
};

