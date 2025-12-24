#include "Cube.h"
#include "../Intersection/Intersection.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace {
std::pair<double, double> checkAxis(double origin, double direction) {
    constexpr double kMin = -1.0;
    constexpr double kMax = 1.0;
    constexpr double kEps = 1e-6;

    const double tminNumerator = kMin - origin;
    const double tmaxNumerator = kMax - origin;

    double tmin;
    double tmax;

    if (std::fabs(direction) >= kEps) {
        tmin = tminNumerator / direction;
        tmax = tmaxNumerator / direction;
    } else {
        const double inf = std::numeric_limits<double>::infinity();
        tmin = tminNumerator * inf;
        tmax = tmaxNumerator * inf;
    }

    if (tmin > tmax) {
        std::swap(tmin, tmax);
    }

    return {tmin, tmax};
}
} // namespace

Intersections Cube::localIntersect(const Ray& ray) const {
    const auto [tminX, tmaxX] = checkAxis(ray.getOrigin().x, ray.getDirection().x);
    const auto [tminY, tmaxY] = checkAxis(ray.getOrigin().y, ray.getDirection().y);
    const auto [tminZ, tmaxZ] = checkAxis(ray.getOrigin().z, ray.getDirection().z);

    const double tmin = std::max({tminX, tminY, tminZ});
    const double tmax = std::min({tmaxX, tmaxY, tmaxZ});

    if (tmin > tmax) {
        return Intersections();
    }

    return Intersections({Intersection(tmin, this), Intersection(tmax, this)});
}

Vector Cube::localNormalAt(const Point& localPoint) const {
    const double absX = std::fabs(localPoint.x);
    const double absY = std::fabs(localPoint.y);
    const double absZ = std::fabs(localPoint.z);

    if (absX >= absY && absX >= absZ) {
        return Vector(localPoint.x > 0 ? 1.0 : -1.0, 0.0, 0.0);
    }

    if (absY >= absX && absY >= absZ) {
        return Vector(0.0, localPoint.y > 0 ? 1.0 : -1.0, 0.0);
    }

    return Vector(0.0, 0.0, localPoint.z > 0 ? 1.0 : -1.0);
}

