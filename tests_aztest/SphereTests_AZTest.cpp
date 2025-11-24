/**
 * SphereTests_AZTest.cpp
 *
 * Unit-Tests für die Sphere Klasse mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include <cmath>
#include "../src/Shape/Sphere.h"
#include "../src/Ray/Ray.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"
#include "../src/Intersection/Intersections.h"

// ============================================================================
// Feature: Sphere - Intersection Tests
// ============================================================================

/**
 * Scenario: A ray intersects a sphere at two points
 */
TEST(Sphere, RayIntersectsSphereAtTwoPoints) {
    // Given ray is a Ray(point(0, 0, -5), vector(0, 0, 1))
    Ray ray(Point(0, 0, -5), Vector(0, 0, 1));

    // And sphere is a Sphere
    Sphere sphere;

    // When xs is intersect(sphere, ray)
    Intersections xs = sphere.intersect(ray);

    // Then xs.count = 2
    EXPECT_TRUE(xs.count() == 2);

    // And xs[0].t = 4.0
    EXPECT_NEAR(xs[0].t, 4.0, 1e-10);

    // And xs[1].t = 6.0
    EXPECT_NEAR(xs[1].t, 6.0, 1e-10);
}

/**
 * Scenario: Intersect sets the object on the intersection
 */
TEST(Sphere, IntersectSetsObjectOnIntersection) {
    // Given ray is a Ray(point(0, 0, -5), vector(0, 0, 1))
    Ray ray(Point(0, 0, -5), Vector(0, 0, 1));

    // And sphere is a Sphere
    Sphere sphere;

    // When xs is intersect(sphere, ray)
    Intersections xs = sphere.intersect(ray);

    // Then xs.count = 2
    EXPECT_TRUE(xs.count() == 2);

    // And xs[0].shape = sphere
    EXPECT_TRUE(xs[0].shape == &sphere);

    // And xs[1].shape = sphere
    EXPECT_TRUE(xs[1].shape == &sphere);
}

/**
 * Scenario: A ray intersects a sphere at a tangent
 */
TEST(Sphere, RayIntersectsSphereAtTangent) {
    // Given ray is a Ray(point(0, 1, -5), vector(0, 0, 1))
    Ray ray(Point(0, 1, -5), Vector(0, 0, 1));

    // And sphere is a Sphere
    Sphere sphere;

    // When xs is intersect(sphere, ray)
    Intersections xs = sphere.intersect(ray);

    // Then xs.count = 2
    EXPECT_TRUE(xs.count() == 2);

    // And xs[0].t = 5.0
    EXPECT_NEAR(xs[0].t, 5.0, 1e-10);

    // And xs[1].t = 5.0
    EXPECT_NEAR(xs[1].t, 5.0, 1e-10);
}

/**
 * Scenario: A ray misses a sphere
 */
TEST(Sphere, RayMissesSphere) {
    // Given ray is a Ray(point(0, 2, -5), vector(0, 0, 1))
    Ray ray(Point(0, 2, -5), Vector(0, 0, 1));

    // And sphere is a Sphere
    Sphere sphere;

    // When xs is intersect(sphere, ray)
    Intersections xs = sphere.intersect(ray);

    // Then xs.count = 0
    EXPECT_TRUE(xs.count() == 0);
}

/**
 * Scenario: A ray originates inside a sphere
 */
TEST(Sphere, RayOriginatesInsideSphere) {
    // Given ray is a Ray(point(0, 0, 0), vector(0, 0, 1))
    Ray ray(Point(0, 0, 0), Vector(0, 0, 1));

    // And sphere is a Sphere
    Sphere sphere;

    // When xs is intersect(sphere, ray)
    Intersections xs = sphere.intersect(ray);

    // Then xs.count = 2
    EXPECT_TRUE(xs.count() == 2);

    // And xs[0].t = -1.0
    EXPECT_NEAR(xs[0].t, -1.0, 1e-10);

    // And xs[1].t = 1.0
    EXPECT_NEAR(xs[1].t, 1.0, 1e-10);
}

/**
 * Scenario: A sphere is behind a ray
 */
TEST(Sphere, SphereIsBehindRay) {
    // Given ray is a Ray(point(0, 0, 5), vector(0, 0, 1))
    Ray ray(Point(0, 0, 5), Vector(0, 0, 1));

    // And sphere is a Sphere
    Sphere sphere;

    // When xs is intersect(sphere, ray)
    Intersections xs = sphere.intersect(ray);

    // Then xs.count = 2
    EXPECT_TRUE(xs.count() == 2);

    // And xs[0].t = -6.0
    EXPECT_NEAR(xs[0].t, -6.0, 1e-10);

    // And xs[1].t = -4.0
    EXPECT_NEAR(xs[1].t, -4.0, 1e-10);
}

// ============================================================================
// Feature: Sphere - Normal Vector Tests
// ============================================================================

/**
 * Scenario: The normal on a sphere at a point on the x axis
 */
TEST(Sphere, NormalOnSphereAtPointOnXAxis) {
    // Given sphere is a Sphere
    Sphere sphere;

    // When normal is normalAt(sphere, point(1, 0, 0))
    Vector normal = sphere.normalAt(Point(1, 0, 0));

    // Then normal = vector(1, 0, 0)
    EXPECT_NEAR(normal.x, 1.0, 1e-10);
    EXPECT_NEAR(normal.y, 0.0, 1e-10);
    EXPECT_NEAR(normal.z, 0.0, 1e-10);

    // And normal = normalize(normal)
    EXPECT_NEAR(normal.magnitude(), 1.0, 1e-10);
}

/**
 * Scenario: The normal on a sphere at a point on the y axis
 */
TEST(Sphere, NormalOnSphereAtPointOnYAxis) {
    // Given sphere is a Sphere
    Sphere sphere;

    // When normal is normalAt(sphere, point(0, 1, 0))
    Vector normal = sphere.normalAt(Point(0, 1, 0));

    // Then normal = vector(0, 1, 0)
    EXPECT_NEAR(normal.x, 0.0, 1e-10);
    EXPECT_NEAR(normal.y, 1.0, 1e-10);
    EXPECT_NEAR(normal.z, 0.0, 1e-10);

    // And normal = normalize(normal)
    EXPECT_NEAR(normal.magnitude(), 1.0, 1e-10);
}

/**
 * Scenario: The normal on a sphere at a point on the z axis
 */
TEST(Sphere, NormalOnSphereAtPointOnZAxis) {
    // Given sphere is a Sphere
    Sphere sphere;

    // When normal is normalAt(sphere, point(0, 0, 1))
    Vector normal = sphere.normalAt(Point(0, 0, 1));

    // Then normal = vector(0, 0, 1)
    EXPECT_NEAR(normal.x, 0.0, 1e-10);
    EXPECT_NEAR(normal.y, 0.0, 1e-10);
    EXPECT_NEAR(normal.z, 1.0, 1e-10);

    // And normal = normalize(normal)
    EXPECT_NEAR(normal.magnitude(), 1.0, 1e-10);
}

/**
 * Scenario: The normal on a sphere at a non axial point
 */
TEST(Sphere, NormalOnSphereAtNonAxialPoint) {
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
