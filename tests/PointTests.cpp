/**
 * PointTests.cpp
 * 
 * Unit-Tests für die Point-Klasse.
 * Testet alle Operationen und Eigenschaften von Punkten im 3D-Raum.
 * 
 * Basiert auf den Test-Szenarien aus dem Universitätsskript.
 */
#include <gtest/gtest.h>
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"

// Test: Ein neuer Punkt hat w-Wert von 1.0 (homogene Koordinaten)
TEST(PointTest, NewPointHasWValueOfOne) {
    Point point;
    EXPECT_DOUBLE_EQ(point.w, 1.0);
}

// Test: Ein neuer Punkt hat alle Komponenten auf Null gesetzt
TEST(PointTest, NewPointHasAllComponentsZero) {
    Point point;
    EXPECT_DOUBLE_EQ(point.x, 0.0);
    EXPECT_DOUBLE_EQ(point.y, 0.0);
    EXPECT_DOUBLE_EQ(point.z, 0.0);
}

// Test: Standard-Konstruktor mit drei Parametern
TEST(PointTest, StandardConstructor) {
    Point point(0.1, 2.3, 9.6);
    EXPECT_DOUBLE_EQ(point.x, 0.1);
    EXPECT_DOUBLE_EQ(point.y, 2.3);
    EXPECT_DOUBLE_EQ(point.z, 9.6);
    EXPECT_DOUBLE_EQ(point.w, 1.0);
}

// Test: Multiplikation eines Punktes mit einem Skalar
TEST(PointTest, MultiplyingPointByScalar) {
    Point point1(1, -2, 3);
    Point result1 = point1 * 3.5;
    EXPECT_TRUE(result1 == Point(3.5, -7, 10.5));

    Point point2(1, -2, 3);
    Point result2 = point2 * 0.5;
    EXPECT_TRUE(result2 == Point(0.5, -1, 1.5));
}

// Test: Division eines Punktes durch einen Skalar
TEST(PointTest, DividingPointByScalar) {
    Point point1(1, -2, 3);
    Point result1 = point1 / 2;
    EXPECT_TRUE(result1 == Point(0.5, -1, 1.5));

    Point point2(0.25, -0.5, 0.75);
    Point result2 = point2 / 0.5;
    EXPECT_TRUE(result2 == Point(0.5, -1, 1.5));
}

// Test: Ungültige Division durch Null wirft Exception
TEST(PointTest, IllegalDivisionByZero) {
    Point point(0.1, 0.2, 0.3);
    EXPECT_THROW(point / 0.0, std::runtime_error);
}

// Test: Addition eines Vektors zu einem Punkt
TEST(PointTest, AddingVectorToPoint) {
    Point point(3.0, -2.0, 5.0);
    Vector vector(-2.0, 3.0, 1.0);
    Point result = point + vector;
    EXPECT_TRUE(result == Point(1.0, 1.0, 6.0));
}

// Test: Subtraktion eines Vektors von einem Punkt
TEST(PointTest, SubtractingVectorFromPoint) {
    Point point(3, 2, 1);
    Vector vector(5, 6, 7);
    Point result = point - vector;
    EXPECT_TRUE(result == Point(-2.0, -4.0, -6.0));
}

// Test: Subtraktion zweier Punkte ergibt einen Vektor
TEST(PointTest, SubtractingTwoPoints) {
    Point point(3, 2, 1);
    Point point1(5, 6, 7);
    Vector result = point - point1;
    EXPECT_TRUE(result == Vector(-2.0, -4.0, -6.0));
}

// Test: Zwei Punkte sind gleich (mit Epsilon-Toleranz)
TEST(PointTest, TwoPointsAreEqual) {
    Point point(-2.0, 3.0, 1.0);
    Point point1(-2.0, 3.0, 1.0);
    EXPECT_TRUE(point == point1);
}

// Test: Zwei Punkte sind nicht gleich
TEST(PointTest, TwoPointsAreNotEqual) {
    Point point(-2.0, 3.0, 1.0);
    Point point1(-5.0, 3.0, 3.0);
    EXPECT_TRUE(point != point1);
}

// Test: Komponentenweises Minimum zweier Punkte
TEST(PointTest, MinimumOfTwoPoints) {
    Point point1(1, 2, 3);
    Point point2(1, 2, 3);
    Point result = Point::min(point1, point2);
    EXPECT_TRUE(result == Point(1, 2, 3));

    Point point3(1, 2, 3);
    Point point4(3, 2, 1);
    Point result2 = Point::min(point3, point4);
    EXPECT_TRUE(result2 == Point(1, 2, 1));
}

// Test: Komponentenweises Maximum zweier Punkte
TEST(PointTest, MaximumOfTwoPoints) {
    Point point1(1, 2, 3);
    Point point2(1, 2, 3);
    Point result = Point::max(point1, point2);
    EXPECT_TRUE(result == Point(1, 2, 3));

    Point point3(1, 2, 3);
    Point point4(3, 2, 1);
    Point result2 = Point::max(point3, point4);
    EXPECT_TRUE(result2 == Point(3, 2, 3));
}
