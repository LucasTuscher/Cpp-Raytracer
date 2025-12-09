#include "Plane.h"
#include "../Intersection/Intersection.h"
#include <cmath>

Intersections Plane::localIntersect(const Ray& ray) const {
    const double epsilon = 1e-6;

    // Strahl parallel zur Ebene? -> kein Schnitt
    if (std::fabs(ray.getDirection().y) < epsilon) {
        return Intersections();
    }

    double t = -ray.getOrigin().y / ray.getDirection().y;
    Intersection i(t, this);
    return Intersections({i});
}

Vector Plane::localNormalAt(const Point& /*localPoint*/) const {
    return Vector(0, 1, 0);
}
