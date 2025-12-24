/**
 * ShapeTests.cpp
 *
 * Unit-Tests für die Shape Basisklasse (Transformationen/Material)
 *
 * Tests basierend auf dem Universitätsskript Kapitel 8.1
 */
#include <gtest/gtest.h>
#include "../src/Shape/TestShape.h"
#include "../src/Matrix/Matrix.h"
#include "../src/Material/Material.h"

// ============================================================================
// Feature: Shape
// ============================================================================

TEST(ShapeTest, DefaultTransformationIsIdentity) {
    TestShape shape;
    EXPECT_EQ(shape.getTransform(), Matrix::identity(4));
}

TEST(ShapeTest, AssigningATransformation) {
    TestShape shape;
    Matrix transform = Matrix::translate(2, 3, 4);
    shape.setTransform(transform);
    EXPECT_EQ(shape.getTransform(), transform);
}

TEST(ShapeTest, DefaultMaterial) {
    TestShape shape;
    EXPECT_TRUE(shape.getMaterial() == Material());
}

TEST(ShapeTest, AssigningAMaterial) {
    TestShape shape;
    Material material;
    material.ambient = 1.0;
    shape.setMaterial(material);
    EXPECT_TRUE(shape.getMaterial() == material);
}

TEST(ShapeTest, IntersectingAScaledShapeWithARay) {
    Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
    TestShape shape;
    shape.setTransform(Matrix::scale(2, 2, 2));

    (void)shape.intersect(ray);

    EXPECT_EQ(shape.getSavedRay().getOrigin(), Point(0, 0, -2.5));
    EXPECT_EQ(shape.getSavedRay().getDirection(), Vector(0, 0, 0.5));
}

TEST(ShapeTest, IntersectingATranslatedShapeWithARay) {
    Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
    TestShape shape;
    shape.setTransform(Matrix::translate(5, 0, 0));

    (void)shape.intersect(ray);

    EXPECT_EQ(shape.getSavedRay().getOrigin(), Point(-5, 0, -5));
    EXPECT_EQ(shape.getSavedRay().getDirection(), Vector(0, 0, 1));
}

TEST(ShapeTest, ComputingTheNormalOnATranslatedShape) {
    TestShape shape;
    shape.setTransform(Matrix::translate(0, 1, 0));

    Vector normal = shape.normalAt(Point(0.0, 1.70711, -0.70711));

    const double eps = 1e-5;
    EXPECT_NEAR(normal.x, 0.0, eps);
    EXPECT_NEAR(normal.y, 0.707106, eps);
    EXPECT_NEAR(normal.z, -0.707106, eps);
}

TEST(ShapeTest, ComputingTheNormalOnATransformedShape) {
    TestShape shape;
    Matrix transform = Matrix::scale(1, 0.5, 1) * Matrix::rotateZ(0.628318);
    shape.setTransform(transform);

    Vector normal = shape.normalAt(Point(0.0, 0.707106, -0.707106));

    const double eps = 1e-5;
    EXPECT_NEAR(normal.x, 0.0, eps);
    EXPECT_NEAR(normal.y, 0.9701425, eps);
    EXPECT_NEAR(normal.z, -0.2425356, eps);
}

