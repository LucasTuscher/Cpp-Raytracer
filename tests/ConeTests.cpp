/**
 * ConeTests.cpp
 *
 * Unit-Tests für die Cone Klasse
 *
 * Tests basierend auf dem Universitätsskript Kapitel 8.5
 */
#include <gtest/gtest.h>
#include "../src/Shape/Cone.h"
#include "../src/Ray/Ray.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"

struct ConeRayHitCase {
    Point origin;
    Vector direction;
    size_t expectedCount;
    double t0;
    double t1;
};

TEST(ConeTest, IntersectingConeWithRay) {
    const std::vector<ConeRayHitCase> cases = {
        {Point(0, 0, -5), Vector(0, 0, 1), 2, 5, 5},
        {Point(0, 0, -5), Vector(1, 1, 1), 2, 8.66025, 8.66025},
        {Point(1, 1, -5), Vector(-0.5, -1, 1), 2, 4.55006, 49.44994},
    };

    for (const auto& c : cases) {
        Cone cone;
        Ray ray(c.origin, c.direction.normalized());
        Intersections xs = cone.intersect(ray);

        EXPECT_EQ(xs.count(), c.expectedCount);

        const double eps = 1e-5;
        EXPECT_NEAR(xs[0].t, c.t0, eps);
        EXPECT_NEAR(xs[1].t, c.t1, eps);
    }
}

TEST(ConeTest, IntersectingConeWithRayParallelToHalf) {
    Cone cone;
    Ray ray(Point(0, 0, -1), Vector(0, 1, 1).normalized());
    Intersections xs = cone.intersect(ray);

    EXPECT_EQ(xs.count(), 1);
    EXPECT_NEAR(xs[0].t, 0.35355, 1e-5);
}

TEST(ConeTest, IntersectingConesEndCaps) {
    struct Case {
        Point origin;
        Vector direction;
        size_t expectedCount;
    };

    const std::vector<Case> cases = {
        {Point(0, 0, -5), Vector(0, 1, 0), 0},
        {Point(0, 0, -0.25), Vector(0, 1, 1), 2},
        {Point(0, 0, -0.25), Vector(0, 1, 0), 4},
    };

    for (const auto& c : cases) {
        Cone cone;
        cone.minimum = -0.5;
        cone.maximum = 0.5;
        cone.closed = true;

        Ray ray(c.origin, c.direction.normalized());
        Intersections xs = cone.intersect(ray);
        EXPECT_EQ(xs.count(), c.expectedCount);
    }
}

TEST(ConeTest, ComputingNormalVectorOnCone) {
    struct Case {
        Point point;
        Vector normal;
    };

    const std::vector<Case> cases = {
        {Point(0, 0, 0), Vector(0, 0, 0)},
        {Point(1, 1, 1), Vector(1, -1.41421, 1)},
        {Point(-1, -1, 0), Vector(-1, 1, 0)},
        {Point(0.5, -2.0, 0.5), Vector(0, -1, 0)},
        {Point(0.5, 2.0, 0.5), Vector(0, 1, 0)},
    };

    Cone cone;
    cone.minimum = -2.0;
    cone.maximum = 2.0;
    cone.closed = true;

    const double eps = 1e-5;
    for (const auto& c : cases) {
        Vector n = cone.localNormalAt(c.point);
        EXPECT_NEAR(n.x, c.normal.x, eps);
        EXPECT_NEAR(n.y, c.normal.y, eps);
        EXPECT_NEAR(n.z, c.normal.z, eps);
    }
}

