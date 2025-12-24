#include "Cone.h"
#include "../Intersection/Intersection.h"
#include <algorithm>
#include <cmath>

Cone::Cone()
    : Shape(),
      minimum(-std::numeric_limits<double>::infinity()),
      maximum(std::numeric_limits<double>::infinity()),
      closed(false) {
}

Cone::Cone(const std::string& name)
    : Shape(name),
      minimum(-std::numeric_limits<double>::infinity()),
      maximum(std::numeric_limits<double>::infinity()),
      closed(false) {
}

bool Cone::checkCap(const Ray& ray, double t, double radius) {
    const double x = ray.getOrigin().x + t * ray.getDirection().x;
    const double z = ray.getOrigin().z + t * ray.getDirection().z;
    return (x * x + z * z) <= (radius * radius);
}

Intersections Cone::intersectCaps(const Ray& ray) const {
    constexpr double kEps = 1e-6;

    if (!closed) {
        return Intersections();
    }

    if (std::fabs(ray.getDirection().y) < kEps) {
        return Intersections();
    }

    Intersections xs;

    // Untere Deckfläche: y = minimum, Radius = |minimum|
    double t = (minimum - ray.getOrigin().y) / ray.getDirection().y;
    if (checkCap(ray, t, std::fabs(minimum))) {
        xs.add(Intersection(t, this));
    }

    // Obere Deckfläche: y = maximum, Radius = |maximum|
    t = (maximum - ray.getOrigin().y) / ray.getDirection().y;
    if (checkCap(ray, t, std::fabs(maximum))) {
        xs.add(Intersection(t, this));
    }

    return xs;
}

Intersections Cone::localIntersect(const Ray& ray) const {
    constexpr double kEps = 1e-6;

    const double ox = ray.getOrigin().x;
    const double oy = ray.getOrigin().y;
    const double oz = ray.getOrigin().z;
    const double dx = ray.getDirection().x;
    const double dy = ray.getDirection().y;
    const double dz = ray.getDirection().z;

    const double a = dx * dx - dy * dy + dz * dz;
    const double b = 2.0 * (ox * dx - oy * dy + oz * dz);
    const double c = ox * ox - oy * oy + oz * oz;

    Intersections xs;

    if (std::fabs(a) < kEps) {
        // Strahl parallel zu einer Kegelhälfte -> nur ein Schnittpunkt möglich
        if (std::fabs(b) >= kEps) {
            const double t = -c / (2.0 * b);
            const double y = oy + t * dy;
            if (y > minimum && y < maximum) {
                xs.add(Intersection(t, this));
            }
        }
    } else {
        double discriminant = b * b - 4.0 * a * c;
        // Numerische Stabilität: Tangentenfälle können durch Rundungsfehler leicht negative Werte liefern.
        if (discriminant < 0.0 && discriminant > -1e-12) {
            discriminant = 0.0;
        }

        if (discriminant >= 0.0) {
            const double sqrtDisc = std::sqrt(discriminant);
            double t0 = (-b - sqrtDisc) / (2.0 * a);
            double t1 = (-b + sqrtDisc) / (2.0 * a);

            if (t0 > t1) {
                std::swap(t0, t1);
            }

            const double y0 = oy + t0 * dy;
            if (y0 > minimum && y0 < maximum) {
                xs.add(Intersection(t0, this));
            }

            const double y1 = oy + t1 * dy;
            if (y1 > minimum && y1 < maximum) {
                xs.add(Intersection(t1, this));
            }
        }
    }

    // Deckflächen hinzufügen (falls geschlossen)
    Intersections capHits = intersectCaps(ray);
    for (const auto& i : capHits.getIntersections()) {
        xs.add(i);
    }

    return xs;
}

Vector Cone::localNormalAt(const Point& localPoint) const {
    constexpr double kEps = 1e-6;

    const double dist = localPoint.x * localPoint.x + localPoint.z * localPoint.z;

    // Deckflächen
    if (closed) {
        const double rMax = std::fabs(maximum);
        if (dist < rMax * rMax && localPoint.y >= maximum - kEps) {
            return Vector(0.0, 1.0, 0.0);
        }

        const double rMin = std::fabs(minimum);
        if (dist < rMin * rMin && localPoint.y <= minimum + kEps) {
            return Vector(0.0, -1.0, 0.0);
        }
    }

    // Mantelfläche
    double y = std::sqrt(dist);
    if (localPoint.y > 0.0) {
        y = -y;
    }
    return Vector(localPoint.x, y, localPoint.z);
}
