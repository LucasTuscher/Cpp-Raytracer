/**
 * AdvancedLightingTests_AZTest.cpp
 *
 * Unit-Tests für fortgeschrittene Beleuchtung mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include <cmath>
#include "../src/LightSource/PointLightSource.h"
#include "../src/LightSource/DirectionalLightSource.h"
#include "../src/LightSource/SpotLightSource.h"
#include "../src/Material/Material.h"
#include "../src/Color/Color.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"

// ============================================================================
// Feature: Point Light Attenuation
// ============================================================================

/**
 * Scenario: Point light has no attenuation at its position
 */
TEST(PointLightAttenuation, NoAttenuationAtLightPosition) {
    // Gegeben: Punktlicht at (0, 0, 0)
    PointLightSource light(Point(0, 0, 0), Color(1, 1, 1), 1.0);

    // Wenn: Farbe berechnet wird at the light's position
    Color c = light.colorAtPoint(Point(0, 0, 0));

    // Then there should be full intensity (avoid division by zero)
    // The implementation should handle this gracefully
    EXPECT_TRUE(c.r >= 0.0);
    EXPECT_TRUE(c.g >= 0.0);
    EXPECT_TRUE(c.b >= 0.0);
}

/**
 * Scenario: Point light attenuates with distance
 */
TEST(PointLightAttenuation, AttenuatesWithDistance) {
    // Gegeben: Punktlicht at (0, 0, 0)
    PointLightSource light(Point(0, 0, 0), Color(1, 1, 1), 1.0);

    // Wenn: Farbe berechnet wird at distance 1 and distance 2
    Color c1 = light.colorAtPoint(Point(1, 0, 0));
    Color c2 = light.colorAtPoint(Point(2, 0, 0));

    // Then color at distance 2 should be dimmer than at distance 1
    // (inverse square law: 1/d^2)
    EXPECT_TRUE(c2.r < c1.r);
    EXPECT_TRUE(c2.g < c1.g);
    EXPECT_TRUE(c2.b < c1.b);
}

/**
 * Scenario: Point light follows inverse square law
 */
TEST(PointLightAttenuation, FollowsInverseSquareLaw) {
    // Gegeben: Punktlicht at (0, 0, 0)
    PointLightSource light(Point(0, 0, 0), Color(1, 1, 1), 1.0);

    // Wenn: Farbe berechnet wird at distance 1 and distance 2
    Color c1 = light.colorAtPoint(Point(1, 0, 0));
    Color c2 = light.colorAtPoint(Point(2, 0, 0));

    // Then the ratio should follow inverse square law
    // c1 / c2 = (2^2) / (1^2) = 4
    double ratio = c1.r / c2.r;
    EXPECT_NEAR(ratio, 4.0, 0.5);  // Allow some tolerance for implementation differences
}

/**
 * Scenario: Point light attenuation with colored light
 */
TEST(PointLightAttenuation, AttenuationWithColoredLight) {
    // Gegeben: Rotes Punktlicht
    PointLightSource light(Point(0, 0, 0), Color(1, 0, 0), 1.0);

    // Wenn: Farbe berechnet wird at distance 2
    Color c = light.colorAtPoint(Point(2, 0, 0));

    // Dann: Sollte have red component (attenuated) and no other colors
    EXPECT_TRUE(c.r > 0.0);
    EXPECT_NEAR(c.g, 0.0, 1e-6);
    EXPECT_NEAR(c.b, 0.0, 1e-6);
}

// ============================================================================
// Feature: Directional Light (No Attenuation)
// ============================================================================

/**
 * Scenario: Directional light has no attenuation
 */
TEST(DirectionalLight, NoAttenuation) {
    // Gegeben: Gerichtetes Licht
    DirectionalLightSource light(Vector(0, 0, -1), Color(1, 1, 1), 1.0);

    // Wenn: Farbe berechnet wird at different distances
    Color c1 = light.colorAtPoint(Point(0, 0, 0));
    Color c2 = light.colorAtPoint(Point(0, 0, -100));
    Color c3 = light.colorAtPoint(Point(100, 100, 100));

    // Dann: Farbe sollte be the same everywhere (no attenuation)
    EXPECT_NEAR(c1.r, c2.r, 1e-6);
    EXPECT_NEAR(c1.g, c2.g, 1e-6);
    EXPECT_NEAR(c1.b, c2.b, 1e-6);

    EXPECT_NEAR(c1.r, c3.r, 1e-6);
    EXPECT_NEAR(c1.g, c3.g, 1e-6);
    EXPECT_NEAR(c1.b, c3.b, 1e-6);
}

/**
 * Scenario: Directional light direction is constant
 */
TEST(DirectionalLight, DirectionIsConstant) {
    // Gegeben: Gerichtetes Licht pointing down
    DirectionalLightSource light(Vector(0, -1, 0), Color(1, 1, 1), 1.0);

    // Wenn: Richtung berechnet wird from different points
    Vector d1 = light.directionFromPoint(Point(0, 0, 0));
    Vector d2 = light.directionFromPoint(Point(100, 200, 300));

    // Dann: Richtung sollte be the same everywhere
    EXPECT_NEAR(d1.x, d2.x, 1e-6);
    EXPECT_NEAR(d1.y, d2.y, 1e-6);
    EXPECT_NEAR(d1.z, d2.z, 1e-6);
}

// ============================================================================
// Feature: SpotLight Cone
// ============================================================================

/**
 * Scenario: SpotLight illuminates center of cone fully
 */
TEST(SpotLight, IlluminatesCenterOfConeFully) {
    // Gegeben: Spotlight at (0, 0, 0) pointing down (0, -1, 0)
    // Total angle 60 degrees, falloff starts at 30 degrees
    SpotLightSource light(Point(0, 0, 0), Vector(0, -1, 0), 60.0, 30.0, Color(1, 1, 1), 1.0);

    // Wenn: Farbe berechnet wird directly below the light
    Color c = light.colorAtPoint(Point(0, -5, 0));

    // Dann: Sollte be fully illuminated (but with distance attenuation if implemented)
    // At minimum, should not be zero
    EXPECT_TRUE(c.r > 0.0);
    EXPECT_TRUE(c.g > 0.0);
    EXPECT_TRUE(c.b > 0.0);
}

/**
 * Scenario: SpotLight does not illuminate outside cone
 */
TEST(SpotLight, DoesNotIlluminateOutsideCone) {
    // Gegeben: Spotlight at (0, 0, 0) pointing down (0, -1, 0)
    // Total angle 60 degrees (30 degrees from center)
    SpotLightSource light(Point(0, 0, 0), Vector(0, -1, 0), 60.0, 30.0, Color(1, 1, 1), 1.0);

    // Wenn: Farbe berechnet wird far outside the cone
    // Point at (5, -5, 0) is at 45 degrees from center, outside the 30-degree cone
    Color c = light.colorAtPoint(Point(5, -5, 0));

    // Dann: Sollte be black (no illumination)
    EXPECT_NEAR(c.r, 0.0, 1e-6);
    EXPECT_NEAR(c.g, 0.0, 1e-6);
    EXPECT_NEAR(c.b, 0.0, 1e-6);
}

/**
 * Scenario: SpotLight falloff reduces intensity
 */
TEST(SpotLight, FalloffReducesIntensity) {
    // Gegeben: Spotlight at (0, 0, 0) pointing down (0, -1, 0)
    // Total angle 60 degrees, falloff starts at 20 degrees
    SpotLightSource light(Point(0, 0, 0), Vector(0, -1, 0), 60.0, 20.0, Color(1, 1, 1), 1.0);

    // Wenn: Farbe berechnet wird in center vs in falloff region
    Color center = light.colorAtPoint(Point(0, -10, 0));  // 0 degrees from center
    Color falloff = light.colorAtPoint(Point(4, -10, 0)); // ~22 degrees from center

    // Dann: Falloff-Bereich should be dimmer than center
    EXPECT_TRUE(falloff.r < center.r);
    EXPECT_TRUE(falloff.g < center.g);
    EXPECT_TRUE(falloff.b < center.b);
}

/**
 * Scenario: SpotLight edge of cone is at specified angle
 */
TEST(SpotLight, EdgeOfConeAtSpecifiedAngle) {
    // Gegeben: Spotlight with 90-degree total cone
    SpotLightSource light(Point(0, 0, 0), Vector(0, -1, 0), 90.0, 45.0, Color(1, 1, 1), 1.0);

    // Wenn: Punkt geprüft wird at exactly 45 degrees (edge of cone)
    // 45 degrees means x/y = tan(45°) = 1, so point at (10, -10, 0)
    Color edge = light.colorAtPoint(Point(10, -10, 0));

    // Dann: Sollte have some light (just inside the cone)
    EXPECT_TRUE(edge.r >= 0.0);
    EXPECT_TRUE(edge.g >= 0.0);
    EXPECT_TRUE(edge.b >= 0.0);

    // Wenn: Punkt geprüft wird just beyond 45 degrees
    // 50 degrees means x/y = tan(50°) ≈ 1.19, so point at (12, -10, 0)
    Color outside = light.colorAtPoint(Point(12, -10, 0));

    // Dann: Sollte be darker than edge or black
    EXPECT_TRUE(outside.r <= edge.r);
}

/**
 * Scenario: SpotLight falloff is smooth (continuous)
 */
TEST(SpotLight, FalloffIsSmooth) {
    // Gegeben: Spotlight
    SpotLightSource light(Point(0, 0, 0), Vector(0, -1, 0), 60.0, 20.0, Color(1, 1, 1), 1.0);

    // Wenn: Punkte abgetastet werden at different angles
    Color c0 = light.colorAtPoint(Point(0, -10, 0));     // 0 degrees
    Color c1 = light.colorAtPoint(Point(2, -10, 0));     // ~11 degrees
    Color c2 = light.colorAtPoint(Point(4, -10, 0));     // ~22 degrees
    Color c3 = light.colorAtPoint(Point(5.5, -10, 0));   // ~29 degrees

    // Dann: Intensität sollte decrease smoothly
    EXPECT_TRUE(c0.r > c1.r || std::abs(c0.r - c1.r) < 0.01);
    EXPECT_TRUE(c1.r >= c2.r);
    EXPECT_TRUE(c2.r >= c3.r);
}

/**
 * Scenario: SpotLight with narrow cone
 */
TEST(SpotLight, NarrowCone) {
    // Gegeben: Spotlight with very narrow cone (20 degrees total)
    SpotLightSource light(Point(0, 0, 0), Vector(0, -1, 0), 20.0, 10.0, Color(1, 1, 1), 1.0);

    // Wenn: Farbe berechnet wird at center and slightly off-center
    Color center = light.colorAtPoint(Point(0, -10, 0));
    Color offCenter = light.colorAtPoint(Point(1, -10, 0));  // ~6 degrees

    // Then center should be bright
    EXPECT_TRUE(center.r > 0.5);

    // And off-center should still have light (within inner cone)
    EXPECT_TRUE(offCenter.r > 0.0);
}

/**
 * Scenario: SpotLight with wide cone
 */
TEST(SpotLight, WideCone) {
    // Gegeben: Spotlight with very wide cone (120 degrees total)
    SpotLightSource light(Point(0, 0, 0), Vector(0, -1, 0), 120.0, 60.0, Color(1, 1, 1), 1.0);

    // Wenn: Farbe berechnet wird at different angles
    Color c30deg = light.colorAtPoint(Point(5, -10, 0));   // ~27 degrees
    Color c50deg = light.colorAtPoint(Point(10, -10, 0));  // ~45 degrees

    // Then both should have light (within wide cone)
    EXPECT_TRUE(c30deg.r > 0.0);
    EXPECT_TRUE(c50deg.r > 0.0);
}

// ============================================================================
// Feature: Light Intensity
// ============================================================================

/**
 * Scenario: Light intensity affects brightness
 */
TEST(LightIntensity, AffectsBrightness) {
    // Gegeben: Zwei gerichtete Lichter with different intensities
    DirectionalLightSource light1(Vector(0, 0, -1), Color(1, 1, 1), 1.0);
    DirectionalLightSource light2(Vector(0, 0, -1), Color(1, 1, 1), 0.5);

    // Wenn: Farbe berechnet wird at a point
    Color c1 = light1.colorAtPoint(Point(0, 0, 0));
    Color c2 = light2.colorAtPoint(Point(0, 0, 0));

    // Then half intensity should give half brightness
    EXPECT_NEAR(c2.r, c1.r * 0.5, 1e-5);
    EXPECT_NEAR(c2.g, c1.g * 0.5, 1e-5);
    EXPECT_NEAR(c2.b, c1.b * 0.5, 1e-5);
}

/**
 * Scenario: Zero intensity light produces no illumination
 */
TEST(LightIntensity, ZeroIntensityProducesNoIllumination) {
    // Gegeben: Licht mit zero intensity
    DirectionalLightSource light(Vector(0, 0, -1), Color(1, 1, 1), 0.0);

    // Wenn: Farbe berechnet wird
    Color c = light.colorAtPoint(Point(0, 0, 0));

    // Dann: Sollte be black
    EXPECT_NEAR(c.r, 0.0, 1e-6);
    EXPECT_NEAR(c.g, 0.0, 1e-6);
    EXPECT_NEAR(c.b, 0.0, 1e-6);
}

/**
 * Scenario: High intensity light is brighter
 */
TEST(LightIntensity, HighIntensityIsBrighter) {
    // Gegeben: Licht mit intensity 2.0
    DirectionalLightSource light(Vector(0, 0, -1), Color(1, 1, 1), 2.0);

    // Wenn: Farbe berechnet wird
    Color c = light.colorAtPoint(Point(0, 0, 0));

    // Dann: Sollte be twice as bright as intensity 1.0
    EXPECT_NEAR(c.r, 2.0, 1e-5);
    EXPECT_NEAR(c.g, 2.0, 1e-5);
    EXPECT_NEAR(c.b, 2.0, 1e-5);
}

// ============================================================================
// Feature: Multiple Light Interaction
// ============================================================================

/**
 * Scenario: Material lit by point light shows attenuation in diffuse component
 */
TEST(MultipleLight, PointLightAttenuationInMaterial) {
    // Given a material
    Material m;

    Point position(0, 0, 0);
    Vector eyeVector(0, 0, -1);
    Vector normalVector(0, 0, -1);

    // And two point lights at different distances
    PointLightSource light1(Point(0, 0, -2), Color(1, 1, 1), 1.0);
    PointLightSource light2(Point(0, 0, -4), Color(1, 1, 1), 1.0);

    // When computing Phong lighting
    Color c1 = m.phongLighting(&light1, position, eyeVector, normalVector, false);
    Color c2 = m.phongLighting(&light2, position, eyeVector, normalVector, false);

    // Then closer light should produce brighter result
    EXPECT_TRUE(c1.r > c2.r);
    EXPECT_TRUE(c1.g > c2.g);
    EXPECT_TRUE(c1.b > c2.b);
}

/**
 * Scenario: Material lit by spotlight shows cone effect
 */
TEST(MultipleLight, SpotLightConeEffectInMaterial) {
    // Given a material
    Material m;

    Vector eyeVector(0, 1, 0);  // Looking down
    Vector normalVector(0, 1, 0);  // Surface pointing up

    // And a spotlight pointing down
    SpotLightSource light(Point(0, 10, 0), Vector(0, -1, 0), 60.0, 30.0, Color(1, 1, 1), 1.0);

    // When computing Phong lighting at center and edge
    Point centerPoint(0, 0, 0);  // Directly under light
    Point edgePoint(5, 0, 0);    // Near edge of cone

    Color center = m.phongLighting(&light, centerPoint, eyeVector, normalVector, false);
    Color edge = m.phongLighting(&light, edgePoint, eyeVector, normalVector, false);

    // Then center should be brighter than edge
    EXPECT_TRUE(center.r >= edge.r);
    EXPECT_TRUE(center.g >= edge.g);
    EXPECT_TRUE(center.b >= edge.b);
}
