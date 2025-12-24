#pragma once
#include "Shape.h"
#include "../Intersection/Intersections.h"

/**
 * TestShape-Klasse (nur für Unit-Tests)
 *
 * Diese Klasse dient dazu, die korrekte Transformation von Strahlen und Normalen
 * in der Basisklasse Shape zu testen.
 *
 * - localIntersect() speichert den übergebenen lokalen Strahl in savedRay_
 * - localNormalAt() gibt den Ortsvektor des übergebenen Punktes zurück
 */
class TestShape : public Shape {
private:
    mutable Ray savedRay_;

public:
    TestShape() : Shape(), savedRay_(Point(0, 0, 0), Vector(0, 0, 0)) {}
    explicit TestShape(const std::string& name) : Shape(name), savedRay_(Point(0, 0, 0), Vector(0, 0, 0)) {}
    ~TestShape() override = default;

    const Ray& getSavedRay() const { return savedRay_; }

    Intersections localIntersect(const Ray& localRay) const override {
        savedRay_ = localRay;
        return Intersections();
    }

    Vector localNormalAt(const Point& localPoint) const override {
        return Vector(localPoint.x, localPoint.y, localPoint.z);
    }

    UV localUVAt(const Point& /*localPoint*/) const override {
        return UV(0.0, 0.0);
    }
};

