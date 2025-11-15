/**
 * SphereTests.cpp
 *
 * Unit-Tests für die Sphere Klasse
 *
 * Tests basierend auf dem Universitätsskript Kapitel 4
 */
#include <gtest/gtest.h>
#include "../src/Shape/Sphere.h"
#include "../src/Ray/Ray.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"
#include "../src/Intersection/Intersections.h"
#include <cmath>

// ============================================================================
// Feature: Sphere
// ============================================================================

/**
 * Scenario: a ray intersects a sphere at two points
 */
TEST(SphereTest, RayIntersectsSphereAtTwoPoints) {
    // Given ray is a Ray(point(0, 0, -5), vector(0, 0, 1))
    Ray ray(Point(0, 0, -5), Vector(0, 0, 1));

    // And sphere is a Sphere
    Sphere sphere;

    // When xs is intersect(sphere, ray)
    Intersections xs = sphere.intersect(ray);

    // Then xs.count = 2
    EXPECT_EQ(xs.count(), 2);

    // And xs[0].t = 4.0
    EXPECT_DOUBLE_EQ(xs[0].t, 4.0);

    // And xs[1].t = 6.0
    EXPECT_DOUBLE_EQ(xs[1].t, 6.0);
}

/**
 * Scenario: intersect sets the object on the intersection
 */
TEST(SphereTest, IntersectSetsObjectOnIntersection) {
    // Given ray is a Ray(point(0, 0, -5), vector(0, 0, 1))
    Ray ray(Point(0, 0, -5), Vector(0, 0, 1));

    // And sphere is a Sphere
    Sphere sphere;

    // When xs is intersect(sphere, ray)
    Intersections xs = sphere.intersect(ray);

    // Then xs.count = 2
    EXPECT_EQ(xs.count(), 2);

    // And xs[0].shape = sphere
    EXPECT_EQ(xs[0].shape, &sphere);

    // And xs[1].shape = sphere
    EXPECT_EQ(xs[1].shape, &sphere);
}

/**
 * Scenario: a ray intersects a sphere at a tangent
 */
TEST(SphereTest, RayIntersectsSphereAtTangent) {
    // Given ray is a Ray(point(0, 1, -5), vector(0, 0, 1))
    Ray ray(Point(0, 1, -5), Vector(0, 0, 1));

    // And sphere is a Sphere
    Sphere sphere;

    // When xs is intersect(sphere, ray)
    Intersections xs = sphere.intersect(ray);

    // Then xs.count = 2
    EXPECT_EQ(xs.count(), 2);

    // And xs[0].t = 5.0
    EXPECT_DOUBLE_EQ(xs[0].t, 5.0);

    // And xs[1].t = 5.0
    EXPECT_DOUBLE_EQ(xs[1].t, 5.0);
}

/**
 * Scenario: a ray misses a sphere
 */
TEST(SphereTest, RayMissesSphere) {
    // Given ray is a Ray(point(0, 2, -5), vector(0, 0, 1))
    Ray ray(Point(0, 2, -5), Vector(0, 0, 1));

    // And sphere is a Sphere
    Sphere sphere;

    // When xs is intersect(sphere, ray)
    Intersections xs = sphere.intersect(ray);

    // Then xs.count = 0
    EXPECT_EQ(xs.count(), 0);
}

/**
 * Scenario: a ray originates inside a sphere
 */
TEST(SphereTest, RayOriginatesInsideSphere) {
    // Given ray is a Ray(point(0, 0, 0), vector(0, 0, 1))
    Ray ray(Point(0, 0, 0), Vector(0, 0, 1));

    // And sphere is a Sphere
    Sphere sphere;

    // When xs is intersect(sphere, ray)
    Intersections xs = sphere.intersect(ray);

    // Then xs.count = 2
    EXPECT_EQ(xs.count(), 2);

    // And xs[0].t = -1.0
    EXPECT_DOUBLE_EQ(xs[0].t, -1.0);

    // And xs[1].t = 1.0
    EXPECT_DOUBLE_EQ(xs[1].t, 1.0);
}

/**
 * Scenario: a sphere is behind a ray
 */
TEST(SphereTest, SphereIsBehindRay) {
    // Given ray is a Ray(point(0, 0, 5), vector(0, 0, 1))
    Ray ray(Point(0, 0, 5), Vector(0, 0, 1));

    // And sphere is a Sphere
    Sphere sphere;

    // When xs is intersect(sphere, ray)
    Intersections xs = sphere.intersect(ray);

    // Then xs.count = 2
    EXPECT_EQ(xs.count(), 2);

    // And xs[0].t = -6.0
    EXPECT_DOUBLE_EQ(xs[0].t, -6.0);

    // And xs[1].t = -4.0
    EXPECT_DOUBLE_EQ(xs[1].t, -4.0);
}

// ============================================================================
// Normal Vector Tests
// ============================================================================

/**
 * Scenario: the normal on a sphere at a point on the x axis
 */
TEST(SphereTest, NormalOnSphereAtPointOnXAxis) {
    // Given sphere is a Sphere
    Sphere sphere;

    // When normal is normalAt(sphere, point(1, 0, 0))
    Vector normal = sphere.normalAt(Point(1, 0, 0));

    // Then normal = vector(1, 0, 0)
    EXPECT_DOUBLE_EQ(normal.x, 1.0);
    EXPECT_DOUBLE_EQ(normal.y, 0.0);
    EXPECT_DOUBLE_EQ(normal.z, 0.0);

    // And normal = normalize(normal)
    EXPECT_DOUBLE_EQ(normal.magnitude(), 1.0);
}

/**
 * Scenario: the normal on a sphere at a point on the y axis
 */
TEST(SphereTest, NormalOnSphereAtPointOnYAxis) {
    // Given sphere is a Sphere
    Sphere sphere;

    // When normal is normalAt(sphere, point(0, 1, 0))
    Vector normal = sphere.normalAt(Point(0, 1, 0));

    // Then normal = vector(0, 1, 0)
    EXPECT_DOUBLE_EQ(normal.x, 0.0);
    EXPECT_DOUBLE_EQ(normal.y, 1.0);
    EXPECT_DOUBLE_EQ(normal.z, 0.0);

    // And normal = normalize(normal)
    EXPECT_DOUBLE_EQ(normal.magnitude(), 1.0);
}

/**
 * Scenario: the normal on a sphere at a point on the z axis
 */
TEST(SphereTest, NormalOnSphereAtPointOnZAxis) {
    // Given sphere is a Sphere
    Sphere sphere;

    // When normal is normalAt(sphere, point(0, 0, 1))
    Vector normal = sphere.normalAt(Point(0, 0, 1));

    // Then normal = vector(0, 0, 1)
    EXPECT_DOUBLE_EQ(normal.x, 0.0);
    EXPECT_DOUBLE_EQ(normal.y, 0.0);
    EXPECT_DOUBLE_EQ(normal.z, 1.0);

    // And normal = normalize(normal)
    EXPECT_DOUBLE_EQ(normal.magnitude(), 1.0);
}

/**
 * Scenario: the normal on a sphere at a non axial point
 */
TEST(SphereTest, NormalOnSphereAtNonAxialPoint) {
    // Given sphere is a Sphere
    Sphere sphere;

    // When normal is normalAt(sphere, point(0.57735, 0.57735, 0.57735))
    double val = 0.57735;
    Vector normal = sphere.normalAt(Point(val, val, val));

    // Then normal = vector(0.57735, 0.57735, 0.57735)
    // Use a small epsilon for comparison due to floating point precision
    const double epsilon = 1e-5;
    EXPECT_NEAR(normal.x, val, epsilon);
    EXPECT_NEAR(normal.y, val, epsilon);
    EXPECT_NEAR(normal.z, val, epsilon);

    // And normal = normalize(normal)
    EXPECT_NEAR(normal.magnitude(), 1.0, epsilon);
}
