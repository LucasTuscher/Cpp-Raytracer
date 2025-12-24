#pragma once
#include "Shape.h"
#include "../Intersection/Intersections.h"
#include <limits>

/**
 * Cone-Klasse
 *
 * Doppelseitiger Kreiskegel in Standardlage:
 * - Achse entlang der y-Achse
 * - Unendlich in y-Richtung (minimum=-inf, maximum=inf)
 * - Radius in Höhe y ist |y|
 *
 * Optional:
 * - Höhenbegrenzung über minimum/maximum (exklusiv)
 * - Deckflächen über closed=true
 */
class Cone : public Shape {
public:
    double minimum;
    double maximum;
    bool closed;

    Cone();
    explicit Cone(const std::string& name);
    ~Cone() override = default;

    Intersections localIntersect(const Ray& localRay) const override;
    Vector localNormalAt(const Point& localPoint) const override;
    UV localUVAt(const Point& /*localPoint*/) const override { return UV(0.0, 0.0); }

private:
    static bool checkCap(const Ray& localRay, double t, double radius);
    Intersections intersectCaps(const Ray& localRay) const;
};

