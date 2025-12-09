/**
 * ShadowTests_AZTest.cpp
 *
 * Unit-Tests für die Schattenberechnung mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include "../src/Scene/Scene.h"
#include "../src/Point/Point.h"
#include "../src/LightSource/PointLightSource.h"

// ============================================================================
// Feature: Shadow Computation
// ============================================================================

/**
 * Scenario: There is no shadow when nothing is collinear with point and light
 */
TEST(Shadow, NoShadowWhenNothingBetween) {
    // Given scene is the default scene
    Scene* scene = Scene::defaultScene();

    // And point is a Point(0, 10, 0)
    Point point(0, 10, 0);

    // And light is the first light in the scene
    LightSource* light = scene->getFirstLight();

    // Then isInShadow(scene, point, light) is false
    EXPECT_FALSE(scene->isInShadow(point, light));

    // Cleanup
    delete scene;
}

/**
 * Scenario: The shadow when an object is between the point and the light
 */
TEST(Shadow, ShadowWhenObjectBetween) {
    // Given scene is the default scene
    Scene* scene = Scene::defaultScene();

    // And point is a Point(10, -10, 10)
    Point point(10, -10, 10);

    // And light is the first light in the scene
    LightSource* light = scene->getFirstLight();

    // Then isInShadow(scene, point, light) is true
    EXPECT_TRUE(scene->isInShadow(point, light));

    // Cleanup
    delete scene;
}

/**
 * Scenario: There is no shadow when an object is behind the light
 */
TEST(Shadow, NoShadowWhenObjectBehindLight) {
    // Given scene is the default scene
    Scene* scene = Scene::defaultScene();

    // And point is a Point(-20, 20, -20)
    Point point(-20, 20, -20);

    // And light is the first light in the scene
    LightSource* light = scene->getFirstLight();

    // Then isInShadow(scene, point, light) is false
    EXPECT_FALSE(scene->isInShadow(point, light));

    // Cleanup
    delete scene;
}

/**
 * Scenario: There is no shadow when an object is behind the point
 */
TEST(Shadow, NoShadowWhenObjectBehindPoint) {
    // Given scene is the default scene
    Scene* scene = Scene::defaultScene();

    // And point is a Point(-2, 2, -2)
    Point point(-2, 2, -2);

    // And light is the first light in the scene
    LightSource* light = scene->getFirstLight();

    // Then isInShadow(scene, point, light) is false
    EXPECT_FALSE(scene->isInShadow(point, light));

    // Cleanup
    delete scene;
}
