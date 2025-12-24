#include "Cylinder.h"
#include "../Intersection/Intersection.h"
#include <algorithm>
#include <cmath>

Cylinder::Cylinder()
    : Shape(),
      minimum(-std::numeric_limits<double>::infinity()),
      maximum(std::numeric_limits<double>::infinity()),
      closed(false) {
}

Cylinder::Cylinder(const std::string& name)
    : Shape(name),
      minimum(-std::numeric_limits<double>::infinity()),
      maximum(std::numeric_limits<double>::infinity()),
      closed(false) {
}

bool Cylinder::checkCap(const Ray& ray, double t) {
    const double x = ray.getOrigin().x + t * ray.getDirection().x;
    const double z = ray.getOrigin().z + t * ray.getDirection().z;
    return (x * x + z * z) <= 1.0;
}

Intersections Cylinder::intersectCaps(const Ray& ray) const {
    constexpr double kEps = 1e-6;

    if (!closed) {
        return Intersections();
    }

    if (std::fabs(ray.getDirection().y) < kEps) {
        return Intersections();
    }

    Intersections xs;

    // Untere Deckfläche: y = minimum
    double t = (minimum - ray.getOrigin().y) / ray.getDirection().y;
    if (checkCap(ray, t)) {
        xs.add(Intersection(t, this));
    }

    // Obere Deckfläche: y = maximum
    t = (maximum - ray.getOrigin().y) / ray.getDirection().y;
    if (checkCap(ray, t)) {
        xs.add(Intersection(t, this));
    }

    return xs;
}

Intersections Cylinder::localIntersect(const Ray& ray) const {
    constexpr double kEps = 1e-6;

    const double ox = ray.getOrigin().x;
    const double oz = ray.getOrigin().z;
    const double dx = ray.getDirection().x;
    const double dz = ray.getDirection().z;

    const double a = dx * dx + dz * dz;

    // Strahl parallel zur y-Achse -> keine Mantelflächen-Schnitte (nur evtl. Deckflächen)
    if (std::fabs(a) < kEps) {
        return intersectCaps(ray);
    }

    const double b = 2.0 * (ox * dx + oz * dz);
    const double c = ox * ox + oz * oz - 1.0;

    const double discriminant = b * b - 4.0 * a * c;
    Intersections xs;

    if (discriminant >= 0.0) {
        const double sqrtDisc = std::sqrt(discriminant);
        double t0 = (-b - sqrtDisc) / (2.0 * a);
        double t1 = (-b + sqrtDisc) / (2.0 * a);

        if (t0 > t1) {
            std::swap(t0, t1);
        }

        const double y0 = ray.getOrigin().y + t0 * ray.getDirection().y;
        if (y0 > minimum && y0 < maximum) {
            xs.add(Intersection(t0, this));
        }

        const double y1 = ray.getOrigin().y + t1 * ray.getDirection().y;
        if (y1 > minimum && y1 < maximum) {
            xs.add(Intersection(t1, this));
        }
    }

    // Deckflächen hinzufügen (falls geschlossen)
    Intersections capHits = intersectCaps(ray);
    for (const auto& i : capHits.getIntersections()) {
        xs.add(i);
    }

    return xs;
}

Vector Cylinder::localNormalAt(const Point& localPoint) const {
    constexpr double kEps = 1e-6;

    const double dist = localPoint.x * localPoint.x + localPoint.z * localPoint.z;

    // Deckflächen
    if (closed && dist < 1.0 && localPoint.y >= maximum - kEps) {
        return Vector(0.0, 1.0, 0.0);
    }
    if (closed && dist < 1.0 && localPoint.y <= minimum + kEps) {
        return Vector(0.0, -1.0, 0.0);
    }

    // Mantelfläche
    return Vector(localPoint.x, 0.0, localPoint.z);
}

