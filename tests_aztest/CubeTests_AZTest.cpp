/**
 * CubeTests_AZTest.cpp
 *
 * Unit-Tests für die Cube Klasse mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include "../src/Shape/Cube.h"
#include "../src/Ray/Ray.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"

struct CubeRayHitCase {
    Point origin;
    Vector direction;
    double t1;
    double t2;
};

struct CubeRayMissCase {
    Point origin;
    Vector direction;
};

struct CubeNormalCase {
    Point point;
    Vector normal;
};

TEST(Cube, RayIntersectsCube) {
    const std::vector<CubeRayHitCase> cases = {
        {Point(5, 0.5, 0), Vector(-1, 0, 0), 4, 6},   // +x
        {Point(-5, 0.5, 0), Vector(1, 0, 0), 4, 6},   // -x
        {Point(0.5, 5, 0), Vector(0, -1, 0), 4, 6},   // +y
        {Point(0.5, -5, 0), Vector(0, 1, 0), 4, 6},   // -y
        {Point(0.5, 0, 5), Vector(0, 0, -1), 4, 6},   // +z
        {Point(0.5, 0, -5), Vector(0, 0, 1), 4, 6},   // -z
        {Point(0, 0.5, 0), Vector(0, 0, 1), -1, 1},   // inside
    };

    for (const auto& c : cases) {
        Cube cube;
        Ray ray(c.origin, c.direction);
        Intersections xs = cube.intersect(ray);

        EXPECT_TRUE(xs.count() == 2);
        EXPECT_NEAR(xs[0].t, c.t1, 1e-10);
        EXPECT_NEAR(xs[1].t, c.t2, 1e-10);
    }
}

TEST(Cube, RayMissesCube) {
    const std::vector<CubeRayMissCase> cases = {
        {Point(-2, 0, 0), Vector(0.2673, 0.5345, 0.8018)},
        {Point(0, -2, 0), Vector(0.8018, 0.2673, 0.5345)},
        {Point(0, 0, -2), Vector(0.5345, 0.8018, 0.2673)},
        {Point(2, 0, 2), Vector(0, 0, -1)},
        {Point(0, 2, 2), Vector(0, -1, 0)},
        {Point(2, 2, 0), Vector(-1, 0, 0)},
    };

    for (const auto& c : cases) {
        Cube cube;
        Ray ray(c.origin, c.direction);
        Intersections xs = cube.intersect(ray);
        EXPECT_TRUE(xs.count() == 0);
    }
}

TEST(Cube, NormalOnSurfaceOfCube) {
    const std::vector<CubeNormalCase> cases = {
        {Point(1, 0.5, -0.8), Vector(1, 0, 0)},
        {Point(-1, -0.2, 0.9), Vector(-1, 0, 0)},
        {Point(-0.4, 1, -0.1), Vector(0, 1, 0)},
        {Point(0.3, -1, -0.7), Vector(0, -1, 0)},
        {Point(-0.6, 0.3, 1), Vector(0, 0, 1)},
        {Point(0.4, 0.4, -1), Vector(0, 0, -1)},
        {Point(1, 1, 1), Vector(1, 0, 0)},
        {Point(-1, -1, -1), Vector(-1, 0, 0)},
    };

    for (const auto& c : cases) {
        Cube cube;
        Vector normal = cube.normalAt(c.point);
        EXPECT_TRUE(normal == c.normal);
    }
}

