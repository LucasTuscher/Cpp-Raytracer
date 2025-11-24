/**
 * PointTests_AZTest.cpp
 *
 * Unit-Tests für die Point-Klasse mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"

// ============================================================================
// Feature: Point
// ============================================================================

/**
 * Scenario: A new point has w value of 1.0
 */
TEST(Point, NewPointHasWValueOfOne) {
    // Given point is a Point
    Point point;

    // Then point.w = 1.0
    EXPECT_NEAR(point.w, 1.0, 1e-10);
}

/**
 * Scenario: A new point has all components zero
 */
TEST(Point, NewPointHasAllComponentsZero) {
    // Given point is a Point
    Point point;

    // Then point.x = 0.0
    EXPECT_NEAR(point.x, 0.0, 1e-10);

    // And point.y = 0.0
    EXPECT_NEAR(point.y, 0.0, 1e-10);

    // And point.z = 0.0
    EXPECT_NEAR(point.z, 0.0, 1e-10);
}

/**
 * Scenario: Standard constructor with three parameters
 */
TEST(Point, StandardConstructor) {
    // Given point is a Point(0.1, 2.3, 9.6)
    Point point(0.1, 2.3, 9.6);

    // Then point.x = 0.1
    EXPECT_NEAR(point.x, 0.1, 1e-10);

    // And point.y = 2.3
    EXPECT_NEAR(point.y, 2.3, 1e-10);

    // And point.z = 9.6
    EXPECT_NEAR(point.z, 9.6, 1e-10);

    // And point.w = 1.0
    EXPECT_NEAR(point.w, 1.0, 1e-10);
}

/**
 * Scenario: Multiplying a point by a scalar
 */
TEST(Point, MultiplyingPointByScalar) {
    // Given point1 is a Point(1, -2, 3)
    Point point1(1, -2, 3);

    // When result1 is point1 * 3.5
    Point result1 = point1 * 3.5;

    // Then result1 = Point(3.5, -7, 10.5)
    EXPECT_TRUE(result1 == Point(3.5, -7, 10.5));

    // Given point2 is a Point(1, -2, 3)
    Point point2(1, -2, 3);

    // When result2 is point2 * 0.5
    Point result2 = point2 * 0.5;

    // Then result2 = Point(0.5, -1, 1.5)
    EXPECT_TRUE(result2 == Point(0.5, -1, 1.5));
}

/**
 * Scenario: Dividing a point by a scalar
 */
TEST(Point, DividingPointByScalar) {
    // Given point1 is a Point(1, -2, 3)
    Point point1(1, -2, 3);

    // When result1 is point1 / 2
    Point result1 = point1 / 2;

    // Then result1 = Point(0.5, -1, 1.5)
    EXPECT_TRUE(result1 == Point(0.5, -1, 1.5));

    // Given point2 is a Point(0.25, -0.5, 0.75)
    Point point2(0.25, -0.5, 0.75);

    // When result2 is point2 / 0.5
    Point result2 = point2 / 0.5;

    // Then result2 = Point(0.5, -1, 1.5)
    EXPECT_TRUE(result2 == Point(0.5, -1, 1.5));
}

/**
 * Scenario: Illegal division by zero throws exception
 */
TEST(Point, IllegalDivisionByZero) {
    // Given point is a Point(0.1, 0.2, 0.3)
    Point point(0.1, 0.2, 0.3);

    // When dividing point by 0.0
    // Then an exception is thrown
    bool exceptionThrown = false;
    try {
        point / 0.0;
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);
}

/**
 * Scenario: Adding a vector to a point
 */
TEST(Point, AddingVectorToPoint) {
    // Given point is a Point(3.0, -2.0, 5.0)
    Point point(3.0, -2.0, 5.0);

    // And vector is a Vector(-2.0, 3.0, 1.0)
    Vector vector(-2.0, 3.0, 1.0);

    // When result is point + vector
    Point result = point + vector;

    // Then result = Point(1.0, 1.0, 6.0)
    EXPECT_TRUE(result == Point(1.0, 1.0, 6.0));
}

/**
 * Scenario: Subtracting a vector from a point
 */
TEST(Point, SubtractingVectorFromPoint) {
    // Given point is a Point(3, 2, 1)
    Point point(3, 2, 1);

    // And vector is a Vector(5, 6, 7)
    Vector vector(5, 6, 7);

    // When result is point - vector
    Point result = point - vector;

    // Then result = Point(-2.0, -4.0, -6.0)
    EXPECT_TRUE(result == Point(-2.0, -4.0, -6.0));
}

/**
 * Scenario: Subtracting two points yields a vector
 */
TEST(Point, SubtractingTwoPoints) {
    // Given point is a Point(3, 2, 1)
    Point point(3, 2, 1);

    // And point1 is a Point(5, 6, 7)
    Point point1(5, 6, 7);

    // When result is point - point1
    Vector result = point - point1;

    // Then result = Vector(-2.0, -4.0, -6.0)
    EXPECT_TRUE(result == Vector(-2.0, -4.0, -6.0));
}

/**
 * Scenario: Two points are equal (with epsilon tolerance)
 */
TEST(Point, TwoPointsAreEqual) {
    // Given point is a Point(-2.0, 3.0, 1.0)
    Point point(-2.0, 3.0, 1.0);

    // And point1 is a Point(-2.0, 3.0, 1.0)
    Point point1(-2.0, 3.0, 1.0);

    // Then point == point1
    EXPECT_TRUE(point == point1);
}

/**
 * Scenario: Two points are not equal
 */
TEST(Point, TwoPointsAreNotEqual) {
    // Given point is a Point(-2.0, 3.0, 1.0)
    Point point(-2.0, 3.0, 1.0);

    // And point1 is a Point(-5.0, 3.0, 3.0)
    Point point1(-5.0, 3.0, 3.0);

    // Then point != point1
    EXPECT_TRUE(point != point1);
}

/**
 * Scenario: Component-wise minimum of two points
 */
TEST(Point, MinimumOfTwoPoints) {
    // Given point1 is a Point(1, 2, 3)
    Point point1(1, 2, 3);

    // And point2 is a Point(1, 2, 3)
    Point point2(1, 2, 3);

    // When result is Point::min(point1, point2)
    Point result = Point::min(point1, point2);

    // Then result = Point(1, 2, 3)
    EXPECT_TRUE(result == Point(1, 2, 3));

    // Given point3 is a Point(1, 2, 3)
    Point point3(1, 2, 3);

    // And point4 is a Point(3, 2, 1)
    Point point4(3, 2, 1);

    // When result2 is Point::min(point3, point4)
    Point result2 = Point::min(point3, point4);

    // Then result2 = Point(1, 2, 1)
    EXPECT_TRUE(result2 == Point(1, 2, 1));
}

/**
 * Scenario: Component-wise maximum of two points
 */
TEST(Point, MaximumOfTwoPoints) {
    // Given point1 is a Point(1, 2, 3)
    Point point1(1, 2, 3);

    // And point2 is a Point(1, 2, 3)
    Point point2(1, 2, 3);

    // When result is Point::max(point1, point2)
    Point result = Point::max(point1, point2);

    // Then result = Point(1, 2, 3)
    EXPECT_TRUE(result == Point(1, 2, 3));

    // Given point3 is a Point(1, 2, 3)
    Point point3(1, 2, 3);

    // And point4 is a Point(3, 2, 1)
    Point point4(3, 2, 1);

    // When result2 is Point::max(point3, point4)
    Point result2 = Point::max(point3, point4);

    // Then result2 = Point(3, 2, 3)
    EXPECT_TRUE(result2 == Point(3, 2, 3));
}
