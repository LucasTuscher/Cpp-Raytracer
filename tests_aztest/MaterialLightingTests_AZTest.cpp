/**
 * MaterialLightingTests_AZTest.cpp
 *
 * Unit-Tests für Material und Beleuchtung mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include <cmath>
#include "../src/Material/Material.h"
#include "../src/Color/Color.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"
#include "../src/LightSource/PointLightSource.h"
#include "../src/LightSource/DirectionalLightSource.h"

// ============================================================================
// Feature: Material Properties
// ============================================================================

/**
 * Scenario: The default material
 */
TEST(Material, DefaultMaterial) {
    // Gegeben: Standard-Material
    Material m;

    // Dann: Standard-Werte
    EXPECT_NEAR(m.color.r, 1.0, 1e-6);
    EXPECT_NEAR(m.color.g, 1.0, 1e-6);
    EXPECT_NEAR(m.color.b, 1.0, 1e-6);
    EXPECT_NEAR(m.ambient, 0.1, 1e-6);
    EXPECT_NEAR(m.diffuse, 0.9, 1e-6);
    EXPECT_NEAR(m.specular, 0.9, 1e-6);
    EXPECT_NEAR(m.shininess, 200.0, 1e-6);
    EXPECT_NEAR(m.reflectivity, 0.0, 1e-6);
}

/**
 * Scenario: Material equality comparison
 */
TEST(Material, EqualityComparison) {
    // Gegeben: Zwei identische Materialien
    Material m1(Color(0.8, 0.6, 0.4), 0.2, 0.7, 0.8, 150.0, 0.5);
    Material m2(Color(0.8, 0.6, 0.4), 0.2, 0.7, 0.8, 150.0, 0.5);

    // Dann: Materialien sind gleich
    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 != m2);
}

/**
 * Scenario: Material inequality comparison
 */
TEST(Material, InequalityComparison) {
    // Gegeben: Zwei verschiedene Materialien
    Material m1(Color(0.8, 0.6, 0.4), 0.2, 0.7, 0.8, 150.0, 0.5);
    Material m2(Color(0.8, 0.6, 0.4), 0.2, 0.7, 0.8, 200.0, 0.5);

    // Dann: Materialien sind ungleich
    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m1 != m2);
}

// ============================================================================
// Feature: Phong Lighting - Ambient Component
// ============================================================================

/**
 * Scenario: Lighting with the eye between the light and the surface
 */
TEST(PhongLighting, EyeBetweenLightAndSurface) {
    // Gegeben: Material und Position
    Material m;
    Point position(0, 0, 0);

    Vector eyeVector(0, 0, -1);
    Vector normalVector(0, 0, -1);

    PointLightSource light(Point(0, 0, -1), Color(1, 1, 1), 1.0);

    // Wenn: Phong-Beleuchtung berechnet wird
    Color result = m.phongLighting(&light, position, eyeVector, normalVector, false);

    // Dann: Ergebnis = (0.1 + 0.9 + 0.9) = 1.9
    EXPECT_NEAR(result.r, 1.9, 1e-5);
    EXPECT_NEAR(result.g, 1.9, 1e-5);
    EXPECT_NEAR(result.b, 1.9, 1e-5);
}

/**
 * Scenario: Lighting with the eye between light and surface, eye offset 45 degrees
 */
TEST(PhongLighting, EyeBetweenLightAndSurfaceEyeOffset45) {
    // Gegeben: Material
    Material m;
    Point position(0, 0, 0);

    Vector eyeVector(0, std::sqrt(2)/2, -std::sqrt(2)/2);
    Vector normalVector(0, 0, -1);

    PointLightSource light(Point(0, 0, -1), Color(1, 1, 1), 1.0);

    // Wenn: Phong-Beleuchtung berechnet wird
    Color result = m.phongLighting(&light, position, eyeVector, normalVector, false);

    // Dann: Ergebnis = Ambient + Diffuse (kein Specular)
    EXPECT_NEAR(result.r, 1.0, 1e-5);
    EXPECT_NEAR(result.g, 1.0, 1e-5);
    EXPECT_NEAR(result.b, 1.0, 1e-5);
}

/**
 * Scenario: Lighting with eye opposite surface, light offset 45 degrees
 */
TEST(PhongLighting, EyeOppositeSurfaceLightOffset45) {
    // Gegeben: Material
    Material m;
    Point position(0, 0, 0);

    Vector eyeVector(0, 0, -1);
    Vector normalVector(0, 0, -1);

    PointLightSource light(Point(0, std::sqrt(2)/2, -std::sqrt(2)/2), Color(1, 1, 1), 1.0);

    // Wenn: Phong-Beleuchtung berechnet wird
    Color result = m.phongLighting(&light, position, eyeVector, normalVector, false);

    // Dann: Ergebnis = Ambient + Diffuse * cos(45°)
    EXPECT_NEAR(result.r, 0.1 + 0.9 * std::sqrt(2)/2, 1e-4);
    EXPECT_NEAR(result.g, 0.1 + 0.9 * std::sqrt(2)/2, 1e-4);
    EXPECT_NEAR(result.b, 0.1 + 0.9 * std::sqrt(2)/2, 1e-4);
}

/**
 * Scenario: Lighting with eye in the path of the reflection vector
 */
TEST(PhongLighting, EyeInPathOfReflectionVector) {
    // Gegeben: Material
    Material m;
    Point position(0, 0, 0);

    Vector eyeVector(0, -std::sqrt(2)/2, -std::sqrt(2)/2);
    Vector normalVector(0, 0, -1);

    PointLightSource light(Point(0, std::sqrt(2)/2, -std::sqrt(2)/2), Color(1, 1, 1), 1.0);

    // Wenn: Phong-Beleuchtung berechnet wird
    Color result = m.phongLighting(&light, position, eyeVector, normalVector, false);

    // Dann: Ergebnis = Ambient + Diffuse + Specular
    double expected = 0.1 + 0.9 * std::sqrt(2)/2 + 0.9;
    EXPECT_NEAR(result.r, expected, 1e-4);
    EXPECT_NEAR(result.g, expected, 1e-4);
    EXPECT_NEAR(result.b, expected, 1e-4);
}

/**
 * Scenario: Lighting with the light behind the surface
 */
TEST(PhongLighting, LightBehindSurface) {
    // Gegeben: Material
    Material m;
    Point position(0, 0, 0);

    Vector eyeVector(0, 0, -1);
    Vector normalVector(0, 0, -1);

    PointLightSource light(Point(0, 0, 1), Color(1, 1, 1), 1.0);

    // Wenn: Phong-Beleuchtung berechnet wird
    Color result = m.phongLighting(&light, position, eyeVector, normalVector, false);

    // Dann: Nur Ambient-Komponente
    EXPECT_NEAR(result.r, 0.1, 1e-5);
    EXPECT_NEAR(result.g, 0.1, 1e-5);
    EXPECT_NEAR(result.b, 0.1, 1e-5);
}

// ============================================================================
// Feature: Phong Lighting - Shadow Component
// ============================================================================

/**
 * Scenario: Lighting with the surface in shadow
 */
TEST(PhongLighting, SurfaceInShadow) {
    // Gegeben: Material
    Material m;
    Point position(0, 0, 0);

    Vector eyeVector(0, 0, -1);
    Vector normalVector(0, 0, -1);

    PointLightSource light(Point(0, 0, -1), Color(1, 1, 1), 1.0);

    // Wenn: Im Schatten
    Color result = m.phongLighting(&light, position, eyeVector, normalVector, true);

    // Dann: Nur Ambient-Komponente
    EXPECT_NEAR(result.r, 0.1, 1e-5);
    EXPECT_NEAR(result.g, 0.1, 1e-5);
    EXPECT_NEAR(result.b, 0.1, 1e-5);
}

// ============================================================================
// Feature: Phong Lighting - Colored Lights and Materials
// ============================================================================

/**
 * Scenario: Lighting with colored light source
 */
TEST(PhongLighting, ColoredLightSource) {
    // Given
    Material m;
    m.color = Color(1, 1, 1);  // White material
    Point position(0, 0, 0);

    Vector eyeVector(0, 0, -1);
    Vector normalVector(0, 0, -1);

    // Red light
    PointLightSource light(Point(0, 0, -1), Color(1, 0, 0), 1.0);

    // When
    Color result = m.phongLighting(&light, position, eyeVector, normalVector, false);

    // Then result should be reddish
    EXPECT_TRUE(result.r > result.g);
    EXPECT_TRUE(result.r > result.b);
}

/**
 * Scenario: Lighting with colored material
 */
TEST(PhongLighting, ColoredMaterial) {
    // Given
    Material m;
    m.color = Color(0, 0, 1);  // Blue material
    Point position(0, 0, 0);

    Vector eyeVector(0, 0, -1);
    Vector normalVector(0, 0, -1);

    PointLightSource light(Point(0, 0, -1), Color(1, 1, 1), 1.0);

    // When
    Color result = m.phongLighting(&light, position, eyeVector, normalVector, false);

    // Then result should be bluish (but specular is white)
    EXPECT_TRUE(result.b > 0.1);  // Has blue component from material
}

/**
 * Scenario: Lighting with colored light and colored material
 */
TEST(PhongLighting, ColoredLightAndMaterial) {
    // Given
    Material m;
    m.color = Color(1, 0.2, 0.4);  // Pinkish material
    Point position(0, 0, 0);

    Vector eyeVector(0, 0, -1);
    Vector normalVector(0, 0, -1);

    // Green light
    PointLightSource light(Point(0, 0, -1), Color(0, 1, 0), 1.0);

    // When
    Color result = m.phongLighting(&light, position, eyeVector, normalVector, false);

    // Then result should have reduced red (material red * light green = 0)
    // But green component should be present
    EXPECT_TRUE(result.g > result.r);
}

// ============================================================================
// Feature: Phong Lighting - Shininess Variations
// ============================================================================

/**
 * Scenario: Material with low shininess produces larger highlights
 */
TEST(PhongLighting, LowShininessProducesLargerHighlights) {
    // Gegeben: Zwei Materialien mit unterschiedlichem Shininess
    Material m1;
    m1.shininess = 10.0;

    Material m2;
    m2.shininess = 200.0;

    Point position(0, 0, 0);

    Vector eyeVector(0, -0.9, -0.4358);
    eyeVector = eyeVector.normalized();
    Vector normalVector(0, 0, -1);

    PointLightSource light(Point(0, std::sqrt(2)/2, -std::sqrt(2)/2), Color(1, 1, 1), 1.0);

    // Wenn: Phong-Beleuchtung berechnet wird
    Color result1 = m1.phongLighting(&light, position, eyeVector, normalVector, false);
    Color result2 = m2.phongLighting(&light, position, eyeVector, normalVector, false);

    // Dann: Niedriger Shininess hat breiteres Highlight
    EXPECT_TRUE(result1.r >= result2.r - 0.1);
}

/**
 * Scenario: Material with very high shininess produces tight highlights
 */
TEST(PhongLighting, HighShininessProducesTightHighlights) {
    // Gegeben: Material with high shininess
    Material m;
    m.shininess = 1000.0;

    Point position(0, 0, 0);

    Vector eyeVector(0, -0.1, -0.995);
    eyeVector = eyeVector.normalized();
    Vector normalVector(0, 0, -1);

    PointLightSource light(Point(0, std::sqrt(2)/2, -std::sqrt(2)/2), Color(1, 1, 1), 1.0);

    // Wenn: Phong-Beleuchtung berechnet wird
    Color result = m.phongLighting(&light, position, eyeVector, normalVector, false);

    // Dann: Specular sollte klein sein (enges Highlight)
    double diffuse = 0.9 * std::sqrt(2)/2;
    EXPECT_NEAR(result.r, 0.1 + diffuse, 0.2);
}

// ============================================================================
// Feature: Blinn-Phong Lighting
// ============================================================================

/**
 * Scenario: Blinn-Phong lighting with eye between light and surface
 */
TEST(BlinnPhongLighting, EyeBetweenLightAndSurface) {
    // Given
    Material m;
    Point position(0, 0, 0);

    Vector eyeVector(0, 0, -1);
    Vector normalVector(0, 0, -1);

    PointLightSource light(Point(0, 0, -1), Color(1, 1, 1), 1.0);

    // When
    Color result = m.blinnPhongLighting(&light, position, eyeVector, normalVector, false);

    // Then should have ambient + diffuse + specular
    // Blinn-Phong uses halfway vector and multiplies shininess by 4
    EXPECT_TRUE(result.r > 1.0);  // Has all three components
    EXPECT_TRUE(result.g > 1.0);
    EXPECT_TRUE(result.b > 1.0);
}

/**
 * Scenario: Blinn-Phong lighting with surface in shadow
 */
TEST(BlinnPhongLighting, SurfaceInShadow) {
    // Given
    Material m;
    Point position(0, 0, 0);

    Vector eyeVector(0, 0, -1);
    Vector normalVector(0, 0, -1);

    PointLightSource light(Point(0, 0, -1), Color(1, 1, 1), 1.0);

    // Wenn: Im Schatten
    Color result = m.blinnPhongLighting(&light, position, eyeVector, normalVector, true);

    // Then only ambient
    EXPECT_NEAR(result.r, 0.1, 1e-5);
    EXPECT_NEAR(result.g, 0.1, 1e-5);
    EXPECT_NEAR(result.b, 0.1, 1e-5);
}

/**
 * Scenario: Blinn-Phong produces similar results to Phong for aligned views
 */
TEST(BlinnPhongLighting, SimilarToPhongForAlignedViews) {
    // Gegeben: Material
    Material m;
    Point position(0, 0, 0);

    Vector eyeVector(0, 0, -1);
    Vector normalVector(0, 0, -1);

    PointLightSource light(Point(0, 0, -1), Color(1, 1, 1), 1.0);

    // Wenn: Beide Beleuchtungsmodelle berechnet werden
    Color phong = m.phongLighting(&light, position, eyeVector, normalVector, false);
    Color blinnPhong = m.blinnPhongLighting(&light, position, eyeVector, normalVector, false);

    // Dann: Ergebnisse sollten ähnlich sein
    EXPECT_NEAR(phong.r, blinnPhong.r, 0.5);
    EXPECT_NEAR(phong.g, blinnPhong.g, 0.5);
    EXPECT_NEAR(phong.b, blinnPhong.b, 0.5);
}

// ============================================================================
// Feature: Directional Light Source
// ============================================================================

/**
 * Scenario: Lighting with directional light source
 */
TEST(MaterialLighting, DirectionalLightSource) {
    // Gegeben: Material
    Material m;
    Point position(0, 0, 0);

    Vector eyeVector(0, 0, -1);
    Vector normalVector(0, 0, -1);

    DirectionalLightSource light(Vector(0, 0, 1), Color(1, 1, 1), 1.0);

    // Wenn: Phong-Beleuchtung berechnet wird
    Color result = m.phongLighting(&light, position, eyeVector, normalVector, false);

    // Then result should be independent of position
    EXPECT_NEAR(result.r, 1.9, 1e-5);
    EXPECT_NEAR(result.g, 1.9, 1e-5);
    EXPECT_NEAR(result.b, 1.9, 1e-5);

    // And at different position
    Point position2(100, 200, 300);
    Color result2 = m.phongLighting(&light, position2, eyeVector, normalVector, false);

    EXPECT_NEAR(result.r, result2.r, 1e-5);
    EXPECT_NEAR(result.g, result2.g, 1e-5);
    EXPECT_NEAR(result.b, result2.b, 1e-5);
}
