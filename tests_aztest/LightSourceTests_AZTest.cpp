/**
 * LightSourceTests_AZTest.cpp
 *
 * Unit-Tests für die LightSource-Klassenhierarchie mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include "../src/LightSource/LightSource.h"
#include "../src/LightSource/PointLightSource.h"
#include "../src/LightSource/DirectionalLightSource.h"
#include "../src/LightSource/SpotLightSource.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"
#include "../src/Color/Color.h"
#include <cmath>
#include <limits>

// ============================================================================
// Feature: PointLight
// ============================================================================

/**
 * Scenario: PointLight with standard parameters
 */
TEST(PointLight, StandardParameters) {
    // Given position is a Point(0, 0, 0)
    Point position(0, 0, 0);

    // And light is a PointLight(position)
    PointLightSource light(position);

    // And point is a Point(0, 1, 0)
    Point point(0, 1, 0);

    // Then isDirectional(light) = false
    EXPECT_FALSE(light.isDirectional());

    // And distanceToPoint(light, point) = 1.0
    EXPECT_NEAR(light.distanceToPoint(point), 1.0, 1e-6);

    // And directionToPoint(light, point) = Vector(0, 1, 0)
    Vector dirToPoint = light.directionToPoint(point);
    EXPECT_NEAR(dirToPoint.x, 0.0, 1e-6);
    EXPECT_NEAR(dirToPoint.y, 1.0, 1e-6);
    EXPECT_NEAR(dirToPoint.z, 0.0, 1e-6);

    // And directionFromPoint(light, point) = Vector(0, -1, 0)
    Vector dirFromPoint = light.directionFromPoint(point);
    EXPECT_NEAR(dirFromPoint.x, 0.0, 1e-6);
    EXPECT_NEAR(dirFromPoint.y, -1.0, 1e-6);
    EXPECT_NEAR(dirFromPoint.z, 0.0, 1e-6);

    // And colorAtPoint(light, point) = Color(1, 1, 1)
    // Note: With attenuation, at distance 1.0, the color is 1/(1²) = 1.0
    Color colorAtPoint = light.colorAtPoint(point);
    EXPECT_NEAR(colorAtPoint.r, 1.0, 1e-6);
    EXPECT_NEAR(colorAtPoint.g, 1.0, 1e-6);
    EXPECT_NEAR(colorAtPoint.b, 1.0, 1e-6);
}

/**
 * Scenario: PointLight with custom parameters
 */
TEST(PointLight, CustomParameters) {
    // Given position is a Point(0, 0, 0)
    Point position(0, 0, 0);

    // And color is a Color(0.2, 0.1, 0.3)
    Color color(0.2, 0.1, 0.3);

    // And intensity is 3.0
    double intensity = 3.0;

    // And light is a PointLight(position, color, intensity)
    PointLightSource light(position, color, intensity);

    // And point is a Point(0, 1, 0)
    Point point(0, 1, 0);

    // Then isDirectional(light) = false
    EXPECT_FALSE(light.isDirectional());

    // And distanceToPoint(light, point) = 1.0
    EXPECT_NEAR(light.distanceToPoint(point), 1.0, 1e-6);

    // And directionToPoint(light, point) = Vector(0, 1, 0)
    Vector dirToPoint = light.directionToPoint(point);
    EXPECT_NEAR(dirToPoint.x, 0.0, 1e-6);
    EXPECT_NEAR(dirToPoint.y, 1.0, 1e-6);
    EXPECT_NEAR(dirToPoint.z, 0.0, 1e-6);

    // And directionFromPoint(light, point) = Vector(0, -1, 0)
    Vector dirFromPoint = light.directionFromPoint(point);
    EXPECT_NEAR(dirFromPoint.x, 0.0, 1e-6);
    EXPECT_NEAR(dirFromPoint.y, -1.0, 1e-6);
    EXPECT_NEAR(dirFromPoint.z, 0.0, 1e-6);

    // And colorAtPoint(light, point) = Color(0.6, 0.3, 0.9)
    // With attenuation at distance 1.0: (0.2, 0.1, 0.3) * 3.0 * 1/(1²) = (0.6, 0.3, 0.9)
    Color colorAtPoint = light.colorAtPoint(point);
    EXPECT_NEAR(colorAtPoint.r, 0.6, 1e-6);
    EXPECT_NEAR(colorAtPoint.g, 0.3, 1e-6);
    EXPECT_NEAR(colorAtPoint.b, 0.9, 1e-6);
}

// ============================================================================
// Feature: DirectionalLight
// ============================================================================

/**
 * Scenario: DirectionalLight with standard parameters
 */
TEST(DirectionalLight, StandardParameters) {
    // Given light is a DirectionalLight(vector(0, -1, 0))
    DirectionalLightSource light(Vector(0, -1, 0));

    // And point is a Point(0, 1, 0)
    Point point(0, 1, 0);

    // Then isDirectional(light) = true
    EXPECT_TRUE(light.isDirectional());

    // And distanceToPoint(light, point) = infinity
    EXPECT_TRUE(std::isinf(light.distanceToPoint(point)));

    // And directionToPoint(light, point) = Vector(0, -1, 0)
    Vector dirToPoint = light.directionToPoint(point);
    EXPECT_NEAR(dirToPoint.x, 0.0, 1e-6);
    EXPECT_NEAR(dirToPoint.y, -1.0, 1e-6);
    EXPECT_NEAR(dirToPoint.z, 0.0, 1e-6);

    // And directionFromPoint(light, point) = Vector(0, 1, 0)
    Vector dirFromPoint = light.directionFromPoint(point);
    EXPECT_NEAR(dirFromPoint.x, 0.0, 1e-6);
    EXPECT_NEAR(dirFromPoint.y, 1.0, 1e-6);
    EXPECT_NEAR(dirFromPoint.z, 0.0, 1e-6);

    // And colorAtPoint(light, point) = Color(1, 1, 1)
    Color colorAtPoint = light.colorAtPoint(point);
    EXPECT_NEAR(colorAtPoint.r, 1.0, 1e-6);
    EXPECT_NEAR(colorAtPoint.g, 1.0, 1e-6);
    EXPECT_NEAR(colorAtPoint.b, 1.0, 1e-6);
}

/**
 * Scenario: DirectionalLight with custom parameters
 */
TEST(DirectionalLight, CustomParameters) {
    // Given light is a DirectionalLight(vector(0, -1, 0), color(0.2, 0.1, 0.3), 3.0)
    DirectionalLightSource light(Vector(0, -1, 0), Color(0.2, 0.1, 0.3), 3.0);

    // And point is a Point(0, 1, 0)
    Point point(0, 1, 0);

    // Then isDirectional(light) = true
    EXPECT_TRUE(light.isDirectional());

    // And distanceToPoint(light, point) = infinity
    EXPECT_TRUE(std::isinf(light.distanceToPoint(point)));

    // And directionToPoint(light, point) = Vector(0, -1, 0)
    Vector dirToPoint = light.directionToPoint(point);
    EXPECT_NEAR(dirToPoint.x, 0.0, 1e-6);
    EXPECT_NEAR(dirToPoint.y, -1.0, 1e-6);
    EXPECT_NEAR(dirToPoint.z, 0.0, 1e-6);

    // And directionFromPoint(light, point) = Vector(0, 1, 0)
    Vector dirFromPoint = light.directionFromPoint(point);
    EXPECT_NEAR(dirFromPoint.x, 0.0, 1e-6);
    EXPECT_NEAR(dirFromPoint.y, 1.0, 1e-6);
    EXPECT_NEAR(dirFromPoint.z, 0.0, 1e-6);

    // And colorAtPoint(light, point) = Color(0.6, 0.3, 0.9)
    Color colorAtPoint = light.colorAtPoint(point);
    EXPECT_NEAR(colorAtPoint.r, 0.6, 1e-6);
    EXPECT_NEAR(colorAtPoint.g, 0.3, 1e-6);
    EXPECT_NEAR(colorAtPoint.b, 0.9, 1e-6);
}

// ============================================================================
// Feature: SpotLight
// ============================================================================

/**
 * Scenario: SpotLight with various parameters (center of beam)
 */
TEST(SpotLight, CenterOfBeam) {
    // Given position is a Point(0, 10, 0)
    Point position(0, 10, 0);

    // And direction is a Vector(0, -1, 0)
    Vector direction(0, -1, 0);

    // And color is a Color(1, 1, 1)
    Color color(1, 1, 1);

    // And intensity is 1.0
    double intensity = 1.0;

    // And point is point(0, 0, 0) - directly below light
    Point point(0, 0, 0);

    // Given light is a SpotLight(position, direction, 45.0, 15.0, color, intensity)
    SpotLightSource light(position, direction, 45.0, 15.0, color, intensity);

    // Then isDirectional(light) = false
    EXPECT_FALSE(light.isDirectional());

    // And distanceToPoint(light, point) = 10.0
    EXPECT_NEAR(light.distanceToPoint(point), 10.0, 1e-5);

    // And directionToPoint(light, point) = Vector(0, -1, 0)
    Vector dirToPoint = light.directionToPoint(point);
    EXPECT_NEAR(dirToPoint.x, 0.0, 1e-5);
    EXPECT_NEAR(dirToPoint.y, -1.0, 1e-5);
    EXPECT_NEAR(dirToPoint.z, 0.0, 1e-5);

    // And colorAtPoint(light, point) = Color(1.0, 1.0, 1.0) - center of beam, full intensity
    Color colorAtPoint = light.colorAtPoint(point);
    EXPECT_NEAR(colorAtPoint.r, 1.0, 1e-5);
    EXPECT_NEAR(colorAtPoint.g, 1.0, 1e-5);
    EXPECT_NEAR(colorAtPoint.b, 1.0, 1e-5);
}

/**
 * Scenario: SpotLight - outside beam
 */
TEST(SpotLight, OutsideBeam) {
    Point position(0, 10, 0);
    Vector direction(0, -1, 0);
    Color color(1, 1, 1);
    double intensity = 1.0;

    // Point at (6, 0, 0) - outside the 45° cone
    Point point(6, 0, 0);

    SpotLightSource light(position, direction, 45.0, 15.0, color, intensity);

    EXPECT_FALSE(light.isDirectional());

    // Distance calculation: sqrt((6-0)² + (0-10)² + (0-0)²) = sqrt(36 + 100) = sqrt(136) ≈ 11.66190
    EXPECT_NEAR(light.distanceToPoint(point), 11.66190, 1e-5);

    // Direction: (6, -10, 0) normalized
    Vector dirToPoint = light.directionToPoint(point);
    EXPECT_NEAR(dirToPoint.x, 0.51449, 1e-5);
    EXPECT_NEAR(dirToPoint.y, -0.85749, 1e-5);
    EXPECT_NEAR(dirToPoint.z, 0.0, 1e-5);

    // Outside beam, should be black
    Color colorAtPoint = light.colorAtPoint(point);
    EXPECT_NEAR(colorAtPoint.r, 0.0, 1e-5);
    EXPECT_NEAR(colorAtPoint.g, 0.0, 1e-5);
    EXPECT_NEAR(colorAtPoint.b, 0.0, 1e-5);
}

/**
 * Scenario: SpotLight - inside central beam (before falloff)
 */
TEST(SpotLight, InsideCentralBeam) {
    Point position(0, 10, 0);
    Vector direction(0, -1, 0);
    Color color(1, 1, 1);
    double intensity = 1.0;

    // Point at (1, 0, 0) - inside the 15° central cone
    Point point(1, 0, 0);

    SpotLightSource light(position, direction, 45.0, 15.0, color, intensity);

    EXPECT_FALSE(light.isDirectional());

    // Distance calculation: sqrt((1-0)² + (0-10)² + (0-0)²) = sqrt(1 + 100) = sqrt(101) ≈ 10.04987
    EXPECT_NEAR(light.distanceToPoint(point), 10.04987, 1e-5);

    // Direction: (1, -10, 0) normalized
    Vector dirToPoint = light.directionToPoint(point);
    EXPECT_NEAR(dirToPoint.x, 0.09950, 1e-5);
    EXPECT_NEAR(dirToPoint.y, -0.99503, 1e-5);
    EXPECT_NEAR(dirToPoint.z, 0.0, 1e-5);

    // Inside central beam, full intensity
    Color colorAtPoint = light.colorAtPoint(point);
    EXPECT_NEAR(colorAtPoint.r, 1.0, 1e-5);
    EXPECT_NEAR(colorAtPoint.g, 1.0, 1e-5);
    EXPECT_NEAR(colorAtPoint.b, 1.0, 1e-5);
}

/**
 * Scenario: SpotLight - in falloff region
 */
TEST(SpotLight, InFalloffRegion) {
    Point position(0, 10, 0);
    Vector direction(0, -1, 0);
    Color color(1, 1, 1);
    double intensity = 1.0;

    // Point at (3, 0, 0) - in the falloff region (between 15° and 45°)
    Point point(3, 0, 0);

    SpotLightSource light(position, direction, 45.0, 15.0, color, intensity);

    EXPECT_FALSE(light.isDirectional());

    // Distance calculation: sqrt((3-0)² + (0-10)² + (0-0)²) = sqrt(9 + 100) = sqrt(109) ≈ 10.44030
    EXPECT_NEAR(light.distanceToPoint(point), 10.44030, 1e-5);

    // Direction: (3, -10, 0) normalized
    Vector dirToPoint = light.directionToPoint(point);
    EXPECT_NEAR(dirToPoint.x, 0.28734, 1e-5);
    EXPECT_NEAR(dirToPoint.y, -0.95782, 1e-5);
    EXPECT_NEAR(dirToPoint.z, 0.0, 1e-5);

    // In falloff region, partial intensity ≈ 0.14955 (with squared factor for realistic effect)
    Color colorAtPoint = light.colorAtPoint(point);
    EXPECT_NEAR(colorAtPoint.r, 0.14955, 1e-5);
    EXPECT_NEAR(colorAtPoint.g, 0.14955, 1e-5);
    EXPECT_NEAR(colorAtPoint.b, 0.14955, 1e-5);
}
