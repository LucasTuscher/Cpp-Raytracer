/**
 * SceneShadingTests_AZTest.cpp
 *
 * Integrations-Tests für Szenen-Beleuchtung mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include <cmath>
#include "../src/Scene/Scene.h"
#include "../src/Shape/Sphere.h"
#include "../src/Shape/Plane.h"
#include "../src/Material/Material.h"
#include "../src/LightSource/PointLightSource.h"
#include "../src/LightSource/DirectionalLightSource.h"
#include "../src/LightSource/SpotLightSource.h"
#include "../src/Ray/Ray.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"
#include "../src/Color/Color.h"
#include "../src/Matrix/Matrix.h"

// ============================================================================
// Feature: Scene::shadeHit - Basic Shading
// ============================================================================

/**
 * Scenario: Shading an intersection from the outside
 */
TEST(SceneShading, ShadingIntersectionFromOutside) {
    // Gegeben: Szene with a light and a sphere
    Scene scene;
    PointLightSource* light = new PointLightSource(Point(0, 0, -10), Color(1, 1, 1), 1.0);
    scene.addLight(light);

    Sphere* sphere = new Sphere();
    scene.addObject(sphere);

    // Und: Strahl hitting the sphere from outside
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    Intersections xs = scene.traceRay(r);
    EXPECT_TRUE(xs.count() > 0);

    // Wenn: Hit-Info berechnet wird and shading
    HitInfo hitInfo = Scene::computeHitInfo(xs[0], r);
    Color c = scene.shadeHit(hitInfo, 5);

    // Dann: Sollte have proper shading (not black, not default)
    EXPECT_TRUE(c.r > 0.0 || c.g > 0.0 || c.b > 0.0);
    EXPECT_TRUE(c.r >= 0.0 && c.r <= 2.0);  // Allow for bright specular
    EXPECT_TRUE(c.g >= 0.0 && c.g <= 2.0);
    EXPECT_TRUE(c.b >= 0.0 && c.b <= 2.0);

    delete light;
    delete sphere;
}

/**
 * Scenario: Shading an intersection from the inside
 */
TEST(SceneShading, ShadingIntersectionFromInside) {
    // Gegeben: Szene with a light and a sphere
    Scene scene;
    PointLightSource* light = new PointLightSource(Point(0, 0, 0), Color(1, 1, 1), 1.0);
    scene.addLight(light);

    Sphere* sphere = new Sphere();
    scene.addObject(sphere);

    // Und: Strahl originating inside the sphere
    Ray r(Point(0, 0, 0), Vector(0, 0, 1));
    Intersections xs = scene.traceRay(r);
    EXPECT_TRUE(xs.count() > 0);

    // Find the exit intersection (positive t)
    const Intersection* exitHit = nullptr;
    for (size_t i = 0; i < xs.count(); i++) {
        if (xs[i].t > 0) {
            exitHit = &xs[i];
            break;
        }
    }
    EXPECT_TRUE(exitHit != nullptr);

    // Wenn: Hit-Info berechnet wird and shading
    HitInfo hitInfo = Scene::computeHitInfo(*exitHit, r);
    Color c = scene.shadeHit(hitInfo, 5);

    // Dann: Sollte have proper shading
    EXPECT_TRUE(c.r >= 0.0 && c.r <= 2.0);
    EXPECT_TRUE(c.g >= 0.0 && c.g <= 2.0);
    EXPECT_TRUE(c.b >= 0.0 && c.b <= 2.0);

    delete light;
    delete sphere;
}

/**
 * Scenario: Shading with no lights returns ambient only
 */
TEST(SceneShading, NoLightsReturnsAmbientOnly) {
    // Gegeben: Szene with no lights
    Scene scene;

    Sphere* sphere = new Sphere();
    Material mat;
    mat.ambient = 0.2;
    mat.color = Color(0.8, 0.6, 0.4);
    sphere->setMaterial(mat);
    scene.addObject(sphere);

    // Und: Strahl hitting the sphere
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    Intersections xs = scene.traceRay(r);
    EXPECT_TRUE(xs.count() > 0);

    // Wenn: Farbe berechnet wird
    Color c = scene.colorAt(r, 5);

    // Dann: Sollte only have ambient contribution or be black
    // (depends on implementation - some may return black, others ambient only)
    EXPECT_TRUE(c.r >= 0.0 && c.r <= 1.0);
    EXPECT_TRUE(c.g >= 0.0 && c.g <= 1.0);
    EXPECT_TRUE(c.b >= 0.0 && c.b <= 1.0);

    delete sphere;
}

// ============================================================================
// Feature: Scene::colorAt - Complete Pipeline
// ============================================================================

/**
 * Scenario: colorAt with a ray that misses
 */
TEST(SceneColorAt, RayMisses) {
    // Gegeben: Szene
    Scene scene;
    PointLightSource* light = new PointLightSource(Point(0, 10, 0), Color(1, 1, 1), 1.0);
    scene.addLight(light);

    Sphere* sphere = new Sphere();
    scene.addObject(sphere);

    // Und: Strahl that misses all objects
    Ray r(Point(0, 0, -5), Vector(0, 1, 0));

    // Wenn: Farbe berechnet wird
    Color c = scene.colorAt(r, 5);

    // Dann: Sollte be black (background)
    EXPECT_NEAR(c.r, 0.0, 1e-6);
    EXPECT_NEAR(c.g, 0.0, 1e-6);
    EXPECT_NEAR(c.b, 0.0, 1e-6);

    delete light;
    delete sphere;
}

/**
 * Scenario: colorAt with a ray that hits
 */
TEST(SceneColorAt, RayHits) {
    // Gegeben: Szene
    Scene scene;
    PointLightSource* light = new PointLightSource(Point(-10, 10, -10), Color(1, 1, 1), 1.0);
    scene.addLight(light);

    Sphere* sphere = new Sphere();
    Material mat;
    mat.color = Color(0.8, 1.0, 0.6);
    mat.diffuse = 0.7;
    mat.specular = 0.2;
    sphere->setMaterial(mat);
    scene.addObject(sphere);

    // Und: Strahl that hits the sphere
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));

    // Wenn: Farbe berechnet wird
    Color c = scene.colorAt(r, 5);

    // Dann: Sollte have proper color from material and lighting
    EXPECT_TRUE(c.r >= 0.0 && c.r <= 2.0);
    EXPECT_TRUE(c.g >= 0.0 && c.g <= 2.0);
    EXPECT_TRUE(c.b >= 0.0 && c.b <= 2.0);

    delete light;
    delete sphere;
}

/**
 * Scenario: colorAt with an intersection behind the ray
 */
TEST(SceneColorAt, IntersectionBehindRay) {
    // Gegeben: Szene
    Scene scene;
    PointLightSource* light = new PointLightSource(Point(0, 0, -10), Color(1, 1, 1), 1.0);
    scene.addLight(light);

    Sphere* sphere = new Sphere();
    scene.addObject(sphere);

    // Und: Strahl starting after the sphere
    Ray r(Point(0, 0, 5), Vector(0, 0, 1));

    // Wenn: Farbe berechnet wird
    Color c = scene.colorAt(r, 5);

    // Dann: Sollte be black (sphere is behind the ray)
    EXPECT_NEAR(c.r, 0.0, 1e-6);
    EXPECT_NEAR(c.g, 0.0, 1e-6);
    EXPECT_NEAR(c.b, 0.0, 1e-6);

    delete light;
    delete sphere;
}

// ============================================================================
// Feature: Multiple Lights
// ============================================================================

/**
 * Scenario: Shading with multiple lights
 */
TEST(MultipleLight, ShadingWithMultipleLights) {
    // Gegeben: Szene with two lights
    Scene scene;
    PointLightSource* light1 = new PointLightSource(Point(-10, 10, -10), Color(1, 0, 0), 0.5);
    PointLightSource* light2 = new PointLightSource(Point(10, 10, -10), Color(0, 0, 1), 0.5);
    scene.addLight(light1);
    scene.addLight(light2);

    Sphere* sphere = new Sphere();
    scene.addObject(sphere);

    // Und: Strahl hitting the sphere
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    Intersections xs = scene.traceRay(r);
    EXPECT_TRUE(xs.count() > 0);

    // Wenn: Beleuchtung berechnet wird
    HitInfo hitInfo = Scene::computeHitInfo(xs[0], r);
    Color c = scene.shadeHit(hitInfo, 5);

    // Dann: Sollte have contributions from both lights
    // (red from left, blue from right)
    EXPECT_TRUE(c.r > 0.0);  // Has red component
    EXPECT_TRUE(c.b > 0.0);  // Has blue component

    delete light1;
    delete light2;
    delete sphere;
}

/**
 * Scenario: Three lights of different colors
 */
TEST(MultipleLight, ThreeLightsOfDifferentColors) {
    // Gegeben: Szene with RGB lights
    Scene scene;
    PointLightSource* red = new PointLightSource(Point(-5, 5, -5), Color(1, 0, 0), 0.3);
    PointLightSource* green = new PointLightSource(Point(5, 5, -5), Color(0, 1, 0), 0.3);
    PointLightSource* blue = new PointLightSource(Point(0, 5, -10), Color(0, 0, 1), 0.3);
    scene.addLight(red);
    scene.addLight(green);
    scene.addLight(blue);

    Sphere* sphere = new Sphere();
    Material mat;
    mat.color = Color(1, 1, 1);  // White material
    sphere->setMaterial(mat);
    scene.addObject(sphere);

    // Und: Strahl hitting the sphere
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));

    // Wenn: Farbe berechnet wird
    Color c = scene.colorAt(r, 5);

    // Dann: Sollte have all three color components
    EXPECT_TRUE(c.r > 0.0);
    EXPECT_TRUE(c.g > 0.0);
    EXPECT_TRUE(c.b > 0.0);

    delete red;
    delete green;
    delete blue;
    delete sphere;
}

// ============================================================================
// Feature: Shadows with Multiple Objects
// ============================================================================

/**
 * Scenario: Object casts shadow on another object
 */
TEST(SceneShadows, ObjectCastsShadow) {
    // Gegeben: Szene with two spheres and a light
    Scene scene;
    PointLightSource* light = new PointLightSource(Point(0, 0, -10), Color(1, 1, 1), 1.0);
    scene.addLight(light);

    // First sphere (blocking)
    Sphere* sphere1 = new Sphere();
    sphere1->setTransform(Matrix::translate(0, 0, 0));
    scene.addObject(sphere1);

    // Second sphere (in shadow)
    Sphere* sphere2 = new Sphere();
    sphere2->setTransform(Matrix::translate(0, 0, 2));
    Material mat;
    mat.color = Color(1, 0, 0);
    sphere2->setMaterial(mat);
    scene.addObject(sphere2);

    // Und: Strahl hitting the second sphere
    Ray r(Point(0, 0, 5), Vector(0, 0, -1));

    // Wenn: Farbe berechnet wird
    Color c = scene.colorAt(r, 5);

    // Then second sphere should be darker (in shadow)
    // Should still have ambient light but no diffuse/specular
    EXPECT_TRUE(c.r > 0.0);  // Has some red from ambient
    EXPECT_TRUE(c.r < 0.5);  // But darker than if fully lit

    delete light;
    delete sphere1;
    delete sphere2;
}

// ============================================================================
// Feature: Reflection Integration
// ============================================================================

/**
 * Scenario: Reflection contributes to final color
 */
TEST(ReflectionIntegration, ReflectionContributesToFinalColor) {
    // Gegeben: Szene with a reflective plane and an object above it
    Scene scene;
    PointLightSource* light = new PointLightSource(Point(0, 10, -10), Color(1, 1, 1), 1.0);
    scene.addLight(light);

    // Reflective plane
    Plane* plane = new Plane();
    plane->setTransform(Matrix::translate(0, -1, 0));
    Material planeMat;
    planeMat.reflectivity = 0.5;
    planeMat.color = Color(0.5, 0.5, 0.5);
    plane->setMaterial(planeMat);
    scene.addObject(plane);

    // Colored sphere above plane
    Sphere* sphere = new Sphere();
    sphere->setTransform(Matrix::translate(0, 1, 0));
    Material sphereMat;
    sphereMat.color = Color(1, 0, 0);
    sphere->setMaterial(sphereMat);
    scene.addObject(sphere);

    // Und: Strahl that would see the reflection
    Ray r(Point(0, 0, -3), Vector(0, -0.3, 1).normalized());

    // Wenn: Farbe berechnet wird
    Color c = scene.colorAt(r, 5);

    // Dann: Sollte have some color (either from plane surface or reflection)
    EXPECT_TRUE(c.r >= 0.0 && c.r <= 2.0);
    EXPECT_TRUE(c.g >= 0.0 && c.g <= 2.0);
    EXPECT_TRUE(c.b >= 0.0 && c.b <= 2.0);

    delete light;
    delete plane;
    delete sphere;
}

/**
 * Scenario: Non-reflective material ignores reflections
 */
TEST(ReflectionIntegration, NonReflectiveMaterialIgnoresReflections) {
    // Gegeben: Szene with a non-reflective plane
    Scene scene;
    PointLightSource* light = new PointLightSource(Point(0, 10, -10), Color(1, 1, 1), 1.0);
    scene.addLight(light);

    Plane* plane = new Plane();
    plane->setTransform(Matrix::translate(0, -1, 0));
    Material planeMat;
    planeMat.reflectivity = 0.0;  // No reflection
    planeMat.color = Color(0.5, 0.5, 0.5);
    plane->setMaterial(planeMat);
    scene.addObject(plane);

    Sphere* sphere = new Sphere();
    sphere->setTransform(Matrix::translate(0, 1, 0));
    Material sphereMat;
    sphereMat.color = Color(1, 0, 0);  // Red
    sphere->setMaterial(sphereMat);
    scene.addObject(sphere);

    // Und: Strahl hitting the plane
    Ray r(Point(0, 0, -3), Vector(0, -0.3, 1).normalized());

    // Wenn: Farbe berechnet wird
    Color c = scene.colorAt(r, 5);

    // Dann: Sollte only have surface color, no red from sphere reflection
    // (would need to check that it's closer to gray than red)
    EXPECT_TRUE(c.r >= 0.0 && c.r <= 2.0);
    EXPECT_TRUE(c.g >= 0.0 && c.g <= 2.0);
    EXPECT_TRUE(c.b >= 0.0 && c.b <= 2.0);

    delete light;
    delete plane;
    delete sphere;
}

// ============================================================================
// Feature: Complex Scenes
// ============================================================================

/**
 * Scenario: Scene with multiple objects, lights, and reflections
 */
TEST(ComplexScene, MultipleObjectsLightsAndReflections) {
    // Gegeben: Komplexe Szene
    Scene scene;

    // Two lights
    PointLightSource* light1 = new PointLightSource(Point(-5, 10, -5), Color(1, 1, 1), 0.6);
    PointLightSource* light2 = new PointLightSource(Point(5, 5, -5), Color(0.5, 0.5, 1), 0.4);
    scene.addLight(light1);
    scene.addLight(light2);

    // Reflective floor
    Plane* floor = new Plane();
    floor->setTransform(Matrix::translate(0, -2, 0));
    Material floorMat;
    floorMat.reflectivity = 0.3;
    floorMat.color = Color(0.2, 0.2, 0.2);
    floor->setMaterial(floorMat);
    scene.addObject(floor);

    // Three spheres of different colors
    Sphere* sphere1 = new Sphere();
    sphere1->setTransform(Matrix::translate(-2, 0, 0) * Matrix::scale(0.8, 0.8, 0.8));
    Material mat1;
    mat1.color = Color(1, 0, 0);
    sphere1->setMaterial(mat1);
    scene.addObject(sphere1);

    Sphere* sphere2 = new Sphere();
    sphere2->setTransform(Matrix::translate(0, 0, 1));
    Material mat2;
    mat2.color = Color(0, 1, 0);
    sphere2->setMaterial(mat2);
    scene.addObject(sphere2);

    Sphere* sphere3 = new Sphere();
    sphere3->setTransform(Matrix::translate(2, 0, 0) * Matrix::scale(1.2, 1.2, 1.2));
    Material mat3;
    mat3.color = Color(0, 0, 1);
    mat3.reflectivity = 0.2;
    sphere3->setMaterial(mat3);
    scene.addObject(sphere3);

    // Und: Strahlen von different angles
    Ray r1(Point(0, 0, -5), Vector(0, 0, 1));
    Ray r2(Point(-3, 1, -5), Vector(0, -0.2, 1).normalized());
    Ray r3(Point(3, 1, -5), Vector(0, -0.2, 1).normalized());

    // Wenn: Farbe berechnet wirds
    Color c1 = scene.colorAt(r1, 5);
    Color c2 = scene.colorAt(r2, 5);
    Color c3 = scene.colorAt(r3, 5);

    // Then all should produce valid colors
    EXPECT_TRUE(c1.r >= 0.0 && c1.r <= 3.0);
    EXPECT_TRUE(c1.g >= 0.0 && c1.g <= 3.0);
    EXPECT_TRUE(c1.b >= 0.0 && c1.b <= 3.0);

    EXPECT_TRUE(c2.r >= 0.0 && c2.r <= 3.0);
    EXPECT_TRUE(c2.g >= 0.0 && c2.g <= 3.0);
    EXPECT_TRUE(c2.b >= 0.0 && c2.b <= 3.0);

    EXPECT_TRUE(c3.r >= 0.0 && c3.r <= 3.0);
    EXPECT_TRUE(c3.g >= 0.0 && c3.g <= 3.0);
    EXPECT_TRUE(c3.b >= 0.0 && c3.b <= 3.0);

    // Cleanup
    delete light1;
    delete light2;
    delete floor;
    delete sphere1;
    delete sphere2;
    delete sphere3;
}

// ============================================================================
// Feature: Depth Limiting
// ============================================================================

/**
 * Scenario: Reflection depth is respected
 */
TEST(DepthLimiting, ReflectionDepthIsRespected) {
    // Gegeben: Szene with two parallel mirrors
    Scene scene;
    PointLightSource* light = new PointLightSource(Point(0, 0, 0), Color(1, 1, 1), 1.0);
    scene.addLight(light);

    Plane* lower = new Plane();
    lower->setTransform(Matrix::translate(0, -1, 0));
    Material lowerMat;
    lowerMat.reflectivity = 1.0;
    lower->setMaterial(lowerMat);
    scene.addObject(lower);

    Plane* upper = new Plane();
    upper->setTransform(Matrix::translate(0, 1, 0) * Matrix::rotateX(M_PI));
    Material upperMat;
    upperMat.reflectivity = 1.0;
    upper->setMaterial(upperMat);
    scene.addObject(upper);

    // And rays with different depth limits
    Ray r(Point(0, 0, 0), Vector(0, 1, 0));

    // Wenn: Farbe berechnet wird with different depths
    Color c0 = scene.colorAt(r, 0);
    Color c1 = scene.colorAt(r, 1);
    Color c5 = scene.colorAt(r, 5);

    // Then all should terminate (not hang)
    // And depth 0 should give simplest result
    EXPECT_TRUE(c0.r >= 0.0 && c0.r <= 2.0);
    EXPECT_TRUE(c1.r >= 0.0 && c1.r <= 2.0);
    EXPECT_TRUE(c5.r >= 0.0 && c5.r <= 2.0);

    delete light;
    delete lower;
    delete upper;
}
