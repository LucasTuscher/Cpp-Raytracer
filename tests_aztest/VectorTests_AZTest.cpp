/**
 * VectorTests_AZTest.cpp
 *
 * Unit-Tests für die Vector-Klasse mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include <cmath>
#include "../src/Vector/Vector.h"

// ============================================================================
// Feature: Vector
// ============================================================================

/**
 * Scenario: A new vector has w value of 0.0
 */
TEST(Vector, NewVectorHasWValueOfZero) {
    // Given vector is a Vector
    Vector vector;

    // Then vector.w = 0.0
    EXPECT_NEAR(vector.w, 0.0, 1e-10);
}

/**
 * Scenario: A new vector has all components zero
 */
TEST(Vector, NewVectorHasAllComponentsZero) {
    // Given vector is a Vector
    Vector vector;

    // Then vector.x = 0.0
    EXPECT_NEAR(vector.x, 0.0, 1e-10);

    // And vector.y = 0.0
    EXPECT_NEAR(vector.y, 0.0, 1e-10);

    // And vector.z = 0.0
    EXPECT_NEAR(vector.z, 0.0, 1e-10);
}

/**
 * Scenario: Standard constructor with three parameters
 */
TEST(Vector, StandardConstructor) {
    // Given vector is a Vector(0.1, 2.3, 9.6)
    Vector vector(0.1, 2.3, 9.6);

    // Then vector.x = 0.1
    EXPECT_NEAR(vector.x, 0.1, 1e-10);

    // And vector.y = 2.3
    EXPECT_NEAR(vector.y, 2.3, 1e-10);

    // And vector.z = 9.6
    EXPECT_NEAR(vector.z, 9.6, 1e-10);

    // And vector.w = 0.0
    EXPECT_NEAR(vector.w, 0.0, 1e-10);
}

/**
 * Scenario: Adding two vectors
 */
TEST(Vector, AddingTwoVectors) {
    // Given vector is a Vector(3.0, -2.0, 5.0)
    Vector vector(3.0, -2.0, 5.0);

    // And vector1 is a Vector(-2.0, 3.0, 1.0)
    Vector vector1(-2.0, 3.0, 1.0);

    // When result is vector + vector1
    Vector result = vector + vector1;

    // Then result = Vector(1.0, 1.0, 6.0)
    EXPECT_TRUE(result == Vector(1.0, 1.0, 6.0));
}

/**
 * Scenario: Subtracting two vectors
 */
TEST(Vector, SubtractingTwoVectors) {
    // Given vector is a Vector(3, 2, 1)
    Vector vector(3, 2, 1);

    // And vector1 is a Vector(5, 6, 7)
    Vector vector1(5, 6, 7);

    // When result is vector - vector1
    Vector result = vector - vector1;

    // Then result = Vector(-2.0, -4.0, -6.0)
    EXPECT_TRUE(result == Vector(-2.0, -4.0, -6.0));
}

/**
 * Scenario: Subtracting a vector from the zero vector
 */
TEST(Vector, SubtractingVectorFromZeroVector) {
    // Given vector is the zero vector
    Vector vector = Vector::ZERO();

    // And vector1 is a Vector(1, -2, 3)
    Vector vector1(1, -2, 3);

    // When result is vector - vector1
    Vector result = vector - vector1;

    // Then result = Vector(-1.0, 2.0, -3.0)
    EXPECT_TRUE(result == Vector(-1.0, 2.0, -3.0));
}

/**
 * Scenario: Negating a vector
 */
TEST(Vector, NegatingVector) {
    // Given vector is a Vector(1, -2, 3)
    Vector vector(1, -2, 3);

    // When result is -vector
    Vector result = -vector;

    // Then result = Vector(-1, 2, -3)
    EXPECT_TRUE(result == Vector(-1, 2, -3));
}

/**
 * Scenario: Multiplying a vector by a scalar
 */
TEST(Vector, MultiplyingVectorByScalar) {
    // Given vector1 is a Vector(1, -2, 3)
    Vector vector1(1, -2, 3);

    // When result1 is vector1 * 3.5
    Vector result1 = vector1 * 3.5;

    // Then result1 = Vector(3.5, -7, 10.5)
    EXPECT_TRUE(result1 == Vector(3.5, -7, 10.5));

    // Given vector2 is a Vector(1, -2, 3)
    Vector vector2(1, -2, 3);

    // When result2 is vector2 * 0.5
    Vector result2 = vector2 * 0.5;

    // Then result2 = Vector(0.5, -1, 1.5)
    EXPECT_TRUE(result2 == Vector(0.5, -1, 1.5));
}

/**
 * Scenario: Dividing a vector by a scalar
 */
TEST(Vector, DividingVectorByScalar) {
    // Given vector1 is a Vector(1, -2, 3)
    Vector vector1(1, -2, 3);

    // When result1 is vector1 / 2
    Vector result1 = vector1 / 2;

    // Then result1 = Vector(0.5, -1, 1.5)
    EXPECT_TRUE(result1 == Vector(0.5, -1, 1.5));

    // Given vector2 is a Vector(0.25, -0.5, 0.75)
    Vector vector2(0.25, -0.5, 0.75);

    // When result2 is vector2 / 0.5
    Vector result2 = vector2 / 0.5;

    // Then result2 = Vector(0.5, -1, 1.5)
    EXPECT_TRUE(result2 == Vector(0.5, -1, 1.5));
}

/**
 * Scenario: Illegal division by zero throws exception
 */
TEST(Vector, IllegalDivisionByZero) {
    // Given vector is a Vector(0.1, 0.2, 0.3)
    Vector vector(0.1, 0.2, 0.3);

    // When dividing vector by 0.0
    // Then an exception is thrown
    bool exceptionThrown = false;
    try {
        vector / 0.0;
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);
}

/**
 * Scenario: Computing magnitude of a vector
 */
TEST(Vector, ComputingMagnitudeOfVector) {
    // Given vector1 is a Vector(1, 0, 0)
    Vector vector1(1, 0, 0);
    // Then magnitude of vector1 = 1.0
    EXPECT_NEAR(vector1.magnitude(), 1.0, 1e-6);

    // Given vector2 is a Vector(0, 1, 0)
    Vector vector2(0, 1, 0);
    // Then magnitude of vector2 = 1.0
    EXPECT_NEAR(vector2.magnitude(), 1.0, 1e-6);

    // Given vector3 is a Vector(0, 0, 1)
    Vector vector3(0, 0, 1);
    // Then magnitude of vector3 = 1.0
    EXPECT_NEAR(vector3.magnitude(), 1.0, 1e-6);

    // Given vector4 is a Vector(1, 2, 3)
    Vector vector4(1, 2, 3);
    // Then magnitude of vector4 = 3.741657386
    EXPECT_NEAR(vector4.magnitude(), 3.741657386, 1e-6);

    // Given vector5 is a Vector(-1, -2, -3)
    Vector vector5(-1, -2, -3);
    // Then magnitude of vector5 = 3.741657386
    EXPECT_NEAR(vector5.magnitude(), 3.741657386, 1e-6);
}

/**
 * Scenario: Computing squared magnitude of a vector
 */
TEST(Vector, ComputingSquaredMagnitudeOfVector) {
    // Given vector1 is a Vector(1, 0, 0)
    Vector vector1(1, 0, 0);
    // Then squared magnitude of vector1 = 1.0
    EXPECT_NEAR(vector1.sqrMagnitude(), 1.0, 1e-6);

    // Given vector2 is a Vector(0, 1, 0)
    Vector vector2(0, 1, 0);
    // Then squared magnitude of vector2 = 1.0
    EXPECT_NEAR(vector2.sqrMagnitude(), 1.0, 1e-6);

    // Given vector3 is a Vector(0, 0, 1)
    Vector vector3(0, 0, 1);
    // Then squared magnitude of vector3 = 1.0
    EXPECT_NEAR(vector3.sqrMagnitude(), 1.0, 1e-6);

    // Given vector4 is a Vector(1, 2, 3)
    Vector vector4(1, 2, 3);
    // Then squared magnitude of vector4 = 14.0
    EXPECT_NEAR(vector4.sqrMagnitude(), 14.0, 1e-6);

    // Given vector5 is a Vector(-1, -2, -3)
    Vector vector5(-1, -2, -3);
    // Then squared magnitude of vector5 = 14.0
    EXPECT_NEAR(vector5.sqrMagnitude(), 14.0, 1e-6);
}

/**
 * Scenario: Normalizing a vector
 */
TEST(Vector, NormalizingVector) {
    // Given vector1 is a Vector(4, 0, 0)
    Vector vector1(4, 0, 0);

    // When result1 is vector1.normalized()
    Vector result1 = vector1.normalized();

    // Then result1 = Vector(1, 0, 0)
    EXPECT_TRUE(result1 == Vector(1, 0, 0));

    // Given vector2 is a Vector(1, 2, 3)
    Vector vector2(1, 2, 3);

    // When result2 is vector2.normalized()
    Vector result2 = vector2.normalized();

    // Then result2.x ≈ 0.26726
    EXPECT_NEAR(result2.x, 0.26726, 1e-5);

    // And result2.y ≈ 0.53452
    EXPECT_NEAR(result2.y, 0.53452, 1e-5);

    // And result2.z ≈ 0.80178
    EXPECT_NEAR(result2.z, 0.80178, 1e-5);
}

/**
 * Scenario: Magnitude of a normalized vector is 1.0
 */
TEST(Vector, MagnitudeOfNormalizedVector) {
    // Given vector is a Vector(1, 2, 3)
    Vector vector(1, 2, 3);

    // When vector1 is vector.normalized()
    Vector vector1 = vector.normalized();

    // Then magnitude of vector1 = 1.0
    EXPECT_NEAR(vector1.magnitude(), 1.0, 1e-6);
}

/**
 * Scenario: Dot product of two vectors
 */
TEST(Vector, DotProductOfTwoVectors) {
    // Given vector is a Vector(1, 2, 3)
    Vector vector(1, 2, 3);

    // And vector1 is a Vector(2, 3, 4)
    Vector vector1(2, 3, 4);

    // When result is Vector::dot(vector, vector1)
    double result = Vector::dot(vector, vector1);

    // Then result = 20.0
    EXPECT_NEAR(result, 20.0, 1e-10);
}

/**
 * Scenario: Cross product of two vectors
 */
TEST(Vector, CrossProductOfTwoVectors) {
    // Given vector is a Vector(1, 2, 3)
    Vector vector(1, 2, 3);

    // And vector1 is a Vector(2, 3, 4)
    Vector vector1(2, 3, 4);

    // When result1 is Vector::cross(vector, vector1)
    Vector result1 = Vector::cross(vector, vector1);

    // Then result1 = Vector(-1, 2, -1)
    EXPECT_TRUE(result1 == Vector(-1, 2, -1));

    // When result2 is Vector::cross(vector1, vector)
    Vector result2 = Vector::cross(vector1, vector);

    // Then result2 = Vector(1, -2, 1)
    EXPECT_TRUE(result2 == Vector(1, -2, 1));
}

/**
 * Scenario: Two vectors are equal (with epsilon tolerance)
 */
TEST(Vector, TwoVectorsAreEqual) {
    // Given vector is a Vector(-2.0, 3.0, 1.0)
    Vector vector(-2.0, 3.0, 1.0);

    // And vector1 is a Vector(-2.0, 3.0, 1.0)
    Vector vector1(-2.0, 3.0, 1.0);

    // Then vector == vector1
    EXPECT_TRUE(vector == vector1);
}

/**
 * Scenario: Two vectors are not equal
 */
TEST(Vector, TwoVectorsAreNotEqual) {
    // Given vector is a Vector(-2.0, 3.0, 1.0)
    Vector vector(-2.0, 3.0, 1.0);

    // And vector1 is a Vector(-5.0, 3.0, 3.0)
    Vector vector1(-5.0, 3.0, 3.0);

    // Then vector != vector1
    EXPECT_TRUE(vector != vector1);
}
