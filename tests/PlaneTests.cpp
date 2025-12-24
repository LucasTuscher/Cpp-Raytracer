/**
 * PlaneTests.cpp
 *
 * Unit-Tests für die Plane Klasse
 *
 * Tests basierend auf dem Universitätsskript Kapitel 8.2
 */
#include <gtest/gtest.h>
#include "../src/Shape/Plane.h"
#include "../src/Ray/Ray.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"

TEST(PlaneTest, NormalOfPlaneIsConstantEverywhere) {
    Plane plane;
    Vector n1 = plane.normalAt(Point(0, 0, 0));
    Vector n2 = plane.normalAt(Point(10, 0, -10));
    Vector n3 = plane.normalAt(Point(-5, 0, 150));

    EXPECT_EQ(n1, Vector(0, 1, 0));
    EXPECT_EQ(n2, Vector(0, 1, 0));
    EXPECT_EQ(n3, Vector(0, 1, 0));
}

TEST(PlaneTest, IntersectWithRayParallelToPlane) {
    Plane plane;
    Ray ray(Point(0, 10, 0), Vector(0, 0, 1));
    Intersections xs = plane.localIntersect(ray);
    EXPECT_EQ(xs.count(), 0);
}

TEST(PlaneTest, IntersectWithCoplanarRay) {
    Plane plane;
    Ray ray(Point(0, 0, 0), Vector(0, 0, 1));
    Intersections xs = plane.localIntersect(ray);
    EXPECT_EQ(xs.count(), 0);
}

TEST(PlaneTest, RayIntersectingPlaneFromAbove) {
    Plane plane;
    Ray ray(Point(0, 1, 0), Vector(0, -1, 0));
    Intersections xs = plane.localIntersect(ray);

    EXPECT_EQ(xs.count(), 1);
    EXPECT_DOUBLE_EQ(xs[0].t, 1.0);
    EXPECT_EQ(xs[0].shape, &plane);
}

TEST(PlaneTest, RayIntersectingPlaneFromBelow) {
    Plane plane;
    Ray ray(Point(0, -1, 0), Vector(0, 1, 0));
    Intersections xs = plane.localIntersect(ray);

    EXPECT_EQ(xs.count(), 1);
    EXPECT_DOUBLE_EQ(xs[0].t, 1.0);
    EXPECT_EQ(xs[0].shape, &plane);
}

