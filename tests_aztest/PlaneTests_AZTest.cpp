/**
 * PlaneTests_AZTest.cpp
 *
 * Unit-Tests für die Plane Klasse mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include "../src/Shape/Plane.h"
#include "../src/Ray/Ray.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"

TEST(Plane, NormalOfPlaneIsConstantEverywhere) {
    Plane plane;
    Vector n1 = plane.normalAt(Point(0, 0, 0));
    Vector n2 = plane.normalAt(Point(10, 0, -10));
    Vector n3 = plane.normalAt(Point(-5, 0, 150));

    EXPECT_TRUE(n1 == Vector(0, 1, 0));
    EXPECT_TRUE(n2 == Vector(0, 1, 0));
    EXPECT_TRUE(n3 == Vector(0, 1, 0));
}

TEST(Plane, IntersectWithRayParallelToPlane) {
    Plane plane;
    Ray ray(Point(0, 10, 0), Vector(0, 0, 1));
    Intersections xs = plane.localIntersect(ray);
    EXPECT_TRUE(xs.count() == 0);
}

TEST(Plane, IntersectWithCoplanarRay) {
    Plane plane;
    Ray ray(Point(0, 0, 0), Vector(0, 0, 1));
    Intersections xs = plane.localIntersect(ray);
    EXPECT_TRUE(xs.count() == 0);
}

TEST(Plane, RayIntersectingPlaneFromAbove) {
    Plane plane;
    Ray ray(Point(0, 1, 0), Vector(0, -1, 0));
    Intersections xs = plane.localIntersect(ray);

    EXPECT_TRUE(xs.count() == 1);
    EXPECT_NEAR(xs[0].t, 1.0, 1e-10);
    EXPECT_TRUE(xs[0].shape == &plane);
}

TEST(Plane, RayIntersectingPlaneFromBelow) {
    Plane plane;
    Ray ray(Point(0, -1, 0), Vector(0, 1, 0));
    Intersections xs = plane.localIntersect(ray);

    EXPECT_TRUE(xs.count() == 1);
    EXPECT_NEAR(xs[0].t, 1.0, 1e-10);
    EXPECT_TRUE(xs[0].shape == &plane);
}

