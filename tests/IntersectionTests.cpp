/**
 * IntersectionTests.cpp
 *
 * Unit-Tests für die Intersection und Intersections Klassen
 *
 * Tests basierend auf dem Universitätsskript Kapitel 4
 */
#include <gtest/gtest.h>
#include "../src/Intersection/Intersection.h"
#include "../src/Intersection/Intersections.h"
#include "../src/Shape/Sphere.h"

// ============================================================================
// Feature: Intersections
// ============================================================================

/**
 * Scenario: an intersection encapsulates t and object
 */
TEST(IntersectionTest, IntersectionEncapsulatesTAndObject) {
    // Given sphere is a Sphere
    Sphere sphere;

    // When i1 is an Intersection(3.5, sphere)
    Intersection i1(3.5, &sphere);

    // Then i1.t = 3.5
    EXPECT_DOUBLE_EQ(i1.t, 3.5);

    // And i1.shape = sphere
    EXPECT_EQ(i1.shape, &sphere);
}

/**
 * Scenario: comparing intersections
 */
TEST(IntersectionTest, ComparingIntersections) {
    // Given sphere is a Sphere
    Sphere sphere;

    // When i1 is an Intersection(3.5, sphere)
    Intersection i1(3.5, &sphere);

    // And i2 is an Intersection(0.3, sphere)
    Intersection i2(0.3, &sphere);

    // Then i1 > i2
    EXPECT_TRUE(i1 > i2);
    EXPECT_FALSE(i2 > i1);

    // Additional tests
    EXPECT_TRUE(i2 < i1);
    EXPECT_FALSE(i1 < i2);
}

/**
 * Scenario: aggregating intersections
 */
TEST(IntersectionsTest, AggregatingIntersections) {
    // Given sphere is a Sphere
    Sphere sphere;

    // And i1 is an Intersection(1, sphere)
    Intersection i1(1.0, &sphere);

    // And i2 is an Intersection(2, sphere)
    Intersection i2(2.0, &sphere);

    // When xs is Intersections(i1, i2)
    Intersections xs({i1, i2});

    // Then xs.count = 2
    EXPECT_EQ(xs.count(), 2);

    // And xs[0].t = 1.0
    EXPECT_DOUBLE_EQ(xs[0].t, 1.0);

    // And xs[1].t = 2.0
    EXPECT_DOUBLE_EQ(xs[1].t, 2.0);
}

/**
 * Scenario: sorting intersections
 */
TEST(IntersectionsTest, SortingIntersections) {
    // Given sphere is a Sphere
    Sphere sphere;

    // And i1 is an Intersection(2, sphere)
    Intersection i1(2.0, &sphere);

    // And i2 is an Intersection(1, sphere)
    Intersection i2(1.0, &sphere);

    // When xs is Intersections(i1, i2) - note: unsorted order
    Intersections xs({i1, i2});

    // Then xs.count = 2
    EXPECT_EQ(xs.count(), 2);

    // And xs[0].t = 1.0 - should be sorted!
    EXPECT_DOUBLE_EQ(xs[0].t, 1.0);

    // And xs[1].t = 2.0
    EXPECT_DOUBLE_EQ(xs[1].t, 2.0);
}

/**
 * Scenario: the hit, when all intersections have positive t
 */
TEST(IntersectionsTest, HitWhenAllIntersectionsPositive) {
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
    ASSERT_NE(i, nullptr);
    EXPECT_DOUBLE_EQ(i->t, 1.0);
}

/**
 * Scenario: the hit, when some intersections have negative t
 */
TEST(IntersectionsTest, HitWhenSomeIntersectionsNegative) {
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
    ASSERT_NE(i, nullptr);
    EXPECT_DOUBLE_EQ(i->t, 1.0);
}

/**
 * Scenario: the hit, when all intersections have negative t
 */
TEST(IntersectionsTest, HitWhenAllIntersectionsNegative) {
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
    EXPECT_EQ(i, nullptr);
}

/**
 * Scenario: the hit is always the lowest non-negative intersection
 */
TEST(IntersectionsTest, HitIsLowestNonNegative) {
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
    ASSERT_NE(i, nullptr);
    EXPECT_DOUBLE_EQ(i->t, 2.0);
}
