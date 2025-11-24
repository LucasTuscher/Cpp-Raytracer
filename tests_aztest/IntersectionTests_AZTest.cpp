/**
 * IntersectionTests_AZTest.cpp
 *
 * Unit-Tests für die Intersection und Intersections Klassen mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include "../src/Intersection/Intersection.h"
#include "../src/Intersection/Intersections.h"
#include "../src/Shape/Sphere.h"

// ============================================================================
// Feature: Intersection
// ============================================================================

/**
 * Scenario: An intersection encapsulates t and object
 */
TEST(Intersection, IntersectionEncapsulatesTAndObject) {
    // Given sphere is a Sphere
    Sphere sphere;

    // When i1 is an Intersection(3.5, sphere)
    Intersection i1(3.5, &sphere);

    // Then i1.t = 3.5
    EXPECT_NEAR(i1.t, 3.5, 1e-10);

    // And i1.shape = sphere
    EXPECT_TRUE(i1.shape == &sphere);
}

/**
 * Scenario: Comparing intersections
 */
TEST(Intersection, ComparingIntersections) {
    // Given sphere is a Sphere
    Sphere sphere;

    // When i1 is an Intersection(3.5, sphere)
    Intersection i1(3.5, &sphere);

    // And i2 is an Intersection(0.3, sphere)
    Intersection i2(0.3, &sphere);

    // Then i1 > i2
    EXPECT_TRUE(i1 > i2);
    EXPECT_FALSE(i2 > i1);

    // And i2 < i1
    EXPECT_TRUE(i2 < i1);
    EXPECT_FALSE(i1 < i2);
}

// ============================================================================
// Feature: Intersections
// ============================================================================

/**
 * Scenario: Aggregating intersections
 */
TEST(Intersections, AggregatingIntersections) {
    // Given sphere is a Sphere
    Sphere sphere;

    // And i1 is an Intersection(1, sphere)
    Intersection i1(1.0, &sphere);

    // And i2 is an Intersection(2, sphere)
    Intersection i2(2.0, &sphere);

    // When xs is Intersections(i1, i2)
    Intersections xs({i1, i2});

    // Then xs.count = 2
    EXPECT_TRUE(xs.count() == 2);

    // And xs[0].t = 1.0
    EXPECT_NEAR(xs[0].t, 1.0, 1e-10);

    // And xs[1].t = 2.0
    EXPECT_NEAR(xs[1].t, 2.0, 1e-10);
}

/**
 * Scenario: Sorting intersections
 */
TEST(Intersections, SortingIntersections) {
    // Given sphere is a Sphere
    Sphere sphere;

    // And i1 is an Intersection(2, sphere)
    Intersection i1(2.0, &sphere);

    // And i2 is an Intersection(1, sphere)
    Intersection i2(1.0, &sphere);

    // When xs is Intersections(i1, i2) - note: unsorted order
    Intersections xs({i1, i2});

    // Then xs.count = 2
    EXPECT_TRUE(xs.count() == 2);

    // And xs[0].t = 1.0 - should be sorted!
    EXPECT_NEAR(xs[0].t, 1.0, 1e-10);

    // And xs[1].t = 2.0
    EXPECT_NEAR(xs[1].t, 2.0, 1e-10);
}

/**
 * Scenario: The hit, when all intersections have positive t
 */
TEST(Intersections, HitWhenAllIntersectionsPositive) {
    // Given sphere is a Sphere
    Sphere sphere;

    // And i1 is an Intersection(1, sphere)
    Intersection i1(1.0, &sphere);

    // And i2 is an Intersection(2, sphere)
    Intersection i2(2.0, &sphere);

    // And xs is Intersections(i2, i1)
    Intersections xs({i2, i1});

    // When i is hit(xs)
    const Intersection* i = xs.hit();

    // Then i = i1
    EXPECT_TRUE(i != nullptr);
    EXPECT_NEAR(i->t, 1.0, 1e-10);
}

/**
 * Scenario: The hit, when some intersections have negative t
 */
TEST(Intersections, HitWhenSomeIntersectionsNegative) {
    // Given sphere is a Sphere
    Sphere sphere;

    // And i1 is an Intersection(-1, sphere)
    Intersection i1(-1.0, &sphere);

    // And i2 is an Intersection(1, sphere)
    Intersection i2(1.0, &sphere);

    // And xs is Intersections(i2, i1)
    Intersections xs({i2, i1});

    // When i is hit(xs)
    const Intersection* i = xs.hit();

    // Then i = i2
    EXPECT_TRUE(i != nullptr);
    EXPECT_NEAR(i->t, 1.0, 1e-10);
}

/**
 * Scenario: The hit, when all intersections have negative t
 */
TEST(Intersections, HitWhenAllIntersectionsNegative) {
    // Given sphere is a Sphere
    Sphere sphere;

    // And i1 is an Intersection(-2, sphere)
    Intersection i1(-2.0, &sphere);

    // And i2 is an Intersection(-1, sphere)
    Intersection i2(-1.0, &sphere);

    // And xs is Intersections(i2, i1)
    Intersections xs({i2, i1});

    // When i is hit(xs)
    const Intersection* i = xs.hit();

    // Then i is null
    EXPECT_TRUE(i == nullptr);
}

/**
 * Scenario: The hit is always the lowest non-negative intersection
 */
TEST(Intersections, HitIsLowestNonNegative) {
    // Given sphere is a Sphere
    Sphere sphere;

    // And i1 is an Intersection(5, sphere)
    Intersection i1(5.0, &sphere);

    // And i2 is an Intersection(7, sphere)
    Intersection i2(7.0, &sphere);

    // And i3 is an Intersection(-3, sphere)
    Intersection i3(-3.0, &sphere);

    // And i4 is an Intersection(2, sphere)
    Intersection i4(2.0, &sphere);

    // And xs is Intersections(i1, i2, i3, i4)
    Intersections xs({i1, i2, i3, i4});

    // When i is hit(xs)
    const Intersection* i = xs.hit();

    // Then i = i4
    EXPECT_TRUE(i != nullptr);
    EXPECT_NEAR(i->t, 2.0, 1e-10);
}
