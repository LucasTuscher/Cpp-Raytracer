/**
 * VectorTests.cpp
 * 
 * Unit-Tests für die Vector-Klasse.
 * Testet alle Operationen und Eigenschaften von Vektoren im 3D-Raum.
 * 
 * Basiert auf den Test-Szenarien aus dem Universitätsskript.
 */
#include <gtest/gtest.h>
#include "../src/Vector/Vector.h"
#include <cmath>

// Test: Ein neuer Vektor hat w-Wert von 0.0 (homogene Koordinaten)
TEST(VectorTest, NewVectorHasWValueOfZero) {
    Vector vector;
    EXPECT_DOUBLE_EQ(vector.w, 0.0);
}

// Test: Ein neuer Vektor hat alle Komponenten auf Null gesetzt
TEST(VectorTest, NewVectorHasAllComponentsZero) {
    Vector vector;
    EXPECT_DOUBLE_EQ(vector.x, 0.0);
    EXPECT_DOUBLE_EQ(vector.y, 0.0);
    EXPECT_DOUBLE_EQ(vector.z, 0.0);
}

// Test: Standard-Konstruktor mit drei Parametern
TEST(VectorTest, StandardConstructor) {
    Vector vector(0.1, 2.3, 9.6);
    EXPECT_DOUBLE_EQ(vector.x, 0.1);
    EXPECT_DOUBLE_EQ(vector.y, 2.3);
    EXPECT_DOUBLE_EQ(vector.z, 9.6);
    EXPECT_DOUBLE_EQ(vector.w, 0.0);
}

// Test: Addition zweier Vektoren
TEST(VectorTest, AddingTwoVectors) {
    Vector vector(3.0, -2.0, 5.0);
    Vector vector1(-2.0, 3.0, 1.0);
    Vector result = vector + vector1;
    EXPECT_TRUE(result == Vector(1.0, 1.0, 6.0));
}

// Test: Subtraktion zweier Vektoren
TEST(VectorTest, SubtractingTwoVectors) {
    Vector vector(3, 2, 1);
    Vector vector1(5, 6, 7);
    Vector result = vector - vector1;
    EXPECT_TRUE(result == Vector(-2.0, -4.0, -6.0));
}

// Test: Subtraktion eines Vektors vom Nullvektor
TEST(VectorTest, SubtractingVectorFromZeroVector) {
    Vector vector = Vector::ZERO();
    Vector vector1(1, -2, 3);
    Vector result = vector - vector1;
    EXPECT_TRUE(result == Vector(-1.0, 2.0, -3.0));
}

// Test: Negation eines Vektors (Richtungsumkehr)
TEST(VectorTest, NegatingVector) {
    Vector vector(1, -2, 3);
    Vector result = -vector;
    EXPECT_TRUE(result == Vector(-1, 2, -3));
}

// Test: Multiplikation eines Vektors mit einem Skalar
TEST(VectorTest, MultiplyingVectorByScalar) {
    Vector vector1(1, -2, 3);
    Vector result1 = vector1 * 3.5;
    EXPECT_TRUE(result1 == Vector(3.5, -7, 10.5));

    Vector vector2(1, -2, 3);
    Vector result2 = vector2 * 0.5;
    EXPECT_TRUE(result2 == Vector(0.5, -1, 1.5));
}

// Test: Division eines Vektors durch einen Skalar
TEST(VectorTest, DividingVectorByScalar) {
    Vector vector1(1, -2, 3);
    Vector result1 = vector1 / 2;
    EXPECT_TRUE(result1 == Vector(0.5, -1, 1.5));

    Vector vector2(0.25, -0.5, 0.75);
    Vector result2 = vector2 / 0.5;
    EXPECT_TRUE(result2 == Vector(0.5, -1, 1.5));
}

// Test: Ungültige Division durch Null wirft Exception
TEST(VectorTest, IllegalDivisionByZero) {
    Vector vector(0.1, 0.2, 0.3);
    EXPECT_THROW(vector / 0.0, std::runtime_error);
}

// Test: Berechnung der Länge (Magnitude) eines Vektors
TEST(VectorTest, ComputingMagnitudeOfVector) {
    Vector vector1(1, 0, 0);
    EXPECT_NEAR(vector1.magnitude(), 1.0, 1e-6);

    Vector vector2(0, 1, 0);
    EXPECT_NEAR(vector2.magnitude(), 1.0, 1e-6);

    Vector vector3(0, 0, 1);
    EXPECT_NEAR(vector3.magnitude(), 1.0, 1e-6);

    Vector vector4(1, 2, 3);
    EXPECT_NEAR(vector4.magnitude(), 3.741657386, 1e-6);

    Vector vector5(-1, -2, -3);
    EXPECT_NEAR(vector5.magnitude(), 3.741657386, 1e-6);
}

// Test: Berechnung der quadrierten Länge eines Vektors
TEST(VectorTest, ComputingSquaredMagnitudeOfVector) {
    Vector vector1(1, 0, 0);
    EXPECT_NEAR(vector1.sqrMagnitude(), 1.0, 1e-6);

    Vector vector2(0, 1, 0);
    EXPECT_NEAR(vector2.sqrMagnitude(), 1.0, 1e-6);

    Vector vector3(0, 0, 1);
    EXPECT_NEAR(vector3.sqrMagnitude(), 1.0, 1e-6);

    Vector vector4(1, 2, 3);
    EXPECT_NEAR(vector4.sqrMagnitude(), 14.0, 1e-6);

    Vector vector5(-1, -2, -3);
    EXPECT_NEAR(vector5.sqrMagnitude(), 14.0, 1e-6);
}

// Test: Normalisierung eines Vektors (Einheitsvektor)
TEST(VectorTest, NormalizingVector) {
    Vector vector1(4, 0, 0);
    Vector result1 = vector1.normalized();
    EXPECT_TRUE(result1 == Vector(1, 0, 0));

    Vector vector2(1, 2, 3);
    Vector result2 = vector2.normalized();
    EXPECT_NEAR(result2.x, 0.26726, 1e-5);
    EXPECT_NEAR(result2.y, 0.53452, 1e-5);
    EXPECT_NEAR(result2.z, 0.80178, 1e-5);
}

// Test: Die Länge eines normalisierten Vektors ist 1.0
TEST(VectorTest, MagnitudeOfNormalizedVector) {
    Vector vector(1, 2, 3);
    Vector vector1 = vector.normalized();
    EXPECT_NEAR(vector1.magnitude(), 1.0, 1e-6);
}

// Test: Skalarprodukt (Dot Product) zweier Vektoren
TEST(VectorTest, DotProductOfTwoVectors) {
    Vector vector(1, 2, 3);
    Vector vector1(2, 3, 4);
    double result = Vector::dot(vector, vector1);
    EXPECT_DOUBLE_EQ(result, 20.0);
}

// Test: Kreuzprodukt (Cross Product) zweier Vektoren
TEST(VectorTest, CrossProductOfTwoVectors) {
    Vector vector(1, 2, 3);
    Vector vector1(2, 3, 4);
    
    Vector result1 = Vector::cross(vector, vector1);
    EXPECT_TRUE(result1 == Vector(-1, 2, -1));

    Vector result2 = Vector::cross(vector1, vector);
    EXPECT_TRUE(result2 == Vector(1, -2, 1));
}

// Test: Zwei Vektoren sind gleich (mit Epsilon-Toleranz)
TEST(VectorTest, TwoVectorsAreEqual) {
    Vector vector(-2.0, 3.0, 1.0);
    Vector vector1(-2.0, 3.0, 1.0);
    EXPECT_TRUE(vector == vector1);
}

// Test: Zwei Vektoren sind nicht gleich
TEST(VectorTest, TwoVectorsAreNotEqual) {
    Vector vector(-2.0, 3.0, 1.0);
    Vector vector1(-5.0, 3.0, 3.0);
    EXPECT_TRUE(vector != vector1);
}
