/**
 * CylinderTests_AZTest.cpp
 *
 * Unit-Tests für die Cylinder Klasse mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include "../src/Shape/Cylinder.h"
#include "../src/Ray/Ray.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"
#include <cmath>
#include <limits>

struct CylinderRayCase {
    Point origin;
    Vector direction;
    int expectedCount;
    double t0;
    double t1;
};

TEST(Cylinder, RayMissesCylinder) {
    const std::vector<std::pair<Point, Vector>> cases = {
        {Point(1, 0, 0), Vector(0, 1, 0)},
        {Point(0, 0, 0), Vector(0, 1, 0)},
        {Point(0, 0, -5), Vector(1, 1, 1)},
    };

    for (const auto& c : cases) {
        Cylinder cyl;
        Ray ray(c.first, c.second.normalized());
        Intersections xs = cyl.intersect(ray);
        EXPECT_TRUE(xs.count() == 0);
    }
}

TEST(Cylinder, RayStrikesCylinder) {
    const std::vector<CylinderRayCase> cases = {
        {Point(1, 0, -5), Vector(0, 0, 1), 2, 5, 5},
        {Point(0, 0, -5), Vector(0, 0, 1), 2, 4, 6},
        {Point(0.5, 0, -5), Vector(0.1, 1, 1), 2, 6.80798, 7.08872},
    };

    for (const auto& c : cases) {
        Cylinder cyl;
        Ray ray(c.origin, c.direction.normalized());
        Intersections xs = cyl.intersect(ray);

        EXPECT_TRUE(xs.count() == static_cast<size_t>(c.expectedCount));

        const double eps = 1e-5;
        EXPECT_NEAR(xs[0].t, c.t0, eps);
        EXPECT_NEAR(xs[1].t, c.t1, eps);
    }
}

TEST(Cylinder, NormalVectorOnCylinder) {
    struct NormalCase {
        Point point;
        Vector normal;
    };

    const std::vector<NormalCase> cases = {
        {Point(1, 0, 0), Vector(1, 0, 0)},
        {Point(0, 5, -1), Vector(0, 0, -1)},
        {Point(0, -2, 1), Vector(0, 0, 1)},
        {Point(-1, 1, 0), Vector(-1, 0, 0)},
    };

    for (const auto& c : cases) {
        Cylinder cyl;
        Vector normal = cyl.normalAt(c.point);
        EXPECT_TRUE(normal == c.normal);
    }
}

TEST(Cylinder, DefaultMinimumAndMaximum) {
    Cylinder cyl;
    EXPECT_TRUE(std::isinf(cyl.minimum));
    EXPECT_TRUE(std::isinf(cyl.maximum));
    EXPECT_TRUE(cyl.minimum < 0.0);
    EXPECT_TRUE(cyl.maximum > 0.0);
}

TEST(Cylinder, IntersectingAConstrainedCylinder) {
    struct Case {
        Point origin;
        Vector direction;
        size_t expectedCount;
    };

    const std::vector<Case> cases = {
        {Point(0, 1.5, 0), Vector(0.1, 1, 0), 0},
        {Point(0, 3, -5), Vector(0, 0, 1), 0},
        {Point(0, 0, -5), Vector(0, 0, 1), 0},
        {Point(0, 2, -5), Vector(0, 0, 1), 0},
        {Point(0, 1, -5), Vector(0, 0, 1), 0},
        {Point(0, 1.5, -2), Vector(0, 0, 1), 2},
    };

    for (const auto& c : cases) {
        Cylinder cyl;
        cyl.minimum = 1;
        cyl.maximum = 2;

        Ray ray(c.origin, c.direction.normalized());
        Intersections xs = cyl.intersect(ray);
        EXPECT_TRUE(xs.count() == c.expectedCount);
    }
}

TEST(Cylinder, DefaultClosedValueIsFalse) {
    Cylinder cyl;
    EXPECT_TRUE(!cyl.closed);
}

TEST(Cylinder, IntersectingCapsOfAClosedCylinder) {
    struct Case {
        Point origin;
        Vector direction;
        size_t expectedCount;
    };

    const std::vector<Case> cases = {
        {Point(0, 3, 0), Vector(0, -1, 0), 2},
        {Point(0, 3, -2), Vector(0, -1, 2), 2},
        {Point(0, 4, -2), Vector(0, -1, 1), 2},
        {Point(0, 0, -2), Vector(0, 1, 2), 2},
        {Point(0, -1, -2), Vector(0, 1, 1), 2},
    };

    for (const auto& c : cases) {
        Cylinder cyl;
        cyl.minimum = 1;
        cyl.maximum = 2;
        cyl.closed = true;

        Ray ray(c.origin, c.direction.normalized());
        Intersections xs = cyl.intersect(ray);
        EXPECT_TRUE(xs.count() == c.expectedCount);
    }
}

TEST(Cylinder, NormalVectorOnCylinderEndCaps) {
    struct Case {
        Point point;
        Vector normal;
    };

    const std::vector<Case> cases = {
        {Point(0, 1, 0), Vector(0, -1, 0)},
        {Point(0.5, 1, 0), Vector(0, -1, 0)},
        {Point(0, 1, 0.5), Vector(0, -1, 0)},
        {Point(0, 2, 0), Vector(0, 1, 0)},
        {Point(0.5, 2, 0), Vector(0, 1, 0)},
        {Point(0, 2, 0.5), Vector(0, 1, 0)},
    };

    Cylinder cyl;
    cyl.minimum = 1;
    cyl.maximum = 2;
    cyl.closed = true;

    for (const auto& c : cases) {
        Vector normal = cyl.normalAt(c.point);
        EXPECT_TRUE(normal == c.normal);
    }
}

