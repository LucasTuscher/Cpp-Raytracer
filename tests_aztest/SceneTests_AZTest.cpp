/**
 * SceneTests_AZTest.cpp
 *
 * Unit-Tests für die Scene-Klasse mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include "../src/Scene/Scene.h"
#include "../src/Shape/Sphere.h"
#include "../src/Ray/Ray.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"
#include "../src/Matrix/Matrix.h"

// ============================================================================
// Feature: Scene
// ============================================================================

/**
 * Scenario: creating a Scene
 */
TEST(Scene, CreatingAScene) {
    // Given scene is a Scene
    Scene scene;

    // Then scene contains no objects
    EXPECT_EQ(scene.getObjectCount(), 0);
    EXPECT_TRUE(scene.getObjects().empty());
}

/**
 * Scenario: adding unnamed objects to a scene
 */
TEST(Scene, AddingUnnamedObjectsToScene) {
    // Given scene is a Scene
    Scene scene;

    // And sphere is a Sphere
    Sphere* sphere = new Sphere();

    // When sphere is added to scene
    scene.addObject(sphere);

    // Then scene contains sphere as object
    EXPECT_TRUE(scene.contains(sphere));
    EXPECT_EQ(scene.getObjectCount(), 1);

    // Cleanup
    delete sphere;
}

/**
 * Scenario: adding named objects to a scene
 */
TEST(Scene, AddingNamedObjectsToScene) {
    // Given scene is a Scene
    Scene scene;

    // And sphere is a Sphere("Sphere")
    Sphere* sphere = new Sphere("Sphere");

    // When sphere is added to scene
    scene.addObject(sphere);

    // Then scene contains "Sphere"
    EXPECT_TRUE(scene.contains("Sphere"));

    // And scene contains 1 shapes
    EXPECT_EQ(scene.getObjectCount(), 1);

    // And scene contains sphere as object
    EXPECT_TRUE(scene.contains(sphere));

    // Cleanup
    delete sphere;
}

/**
 * Scenario: getting object by name
 */
TEST(Scene, GettingObjectByName) {
    // Given scene is a Scene
    Scene scene;

    // And sphere is a Sphere("MySphere")
    Sphere* sphere = new Sphere("MySphere");

    // When sphere is added to scene
    scene.addObject(sphere);

    // Then we can retrieve it by name
    Shape* retrieved = scene.getObject("MySphere");
    EXPECT_EQ(retrieved, sphere);

    // And non-existent names return nullptr
    Shape* notFound = scene.getObject("NonExistent");
    EXPECT_TRUE(notFound == nullptr);

    // Cleanup
    delete sphere;
}

/**
 * Scenario: intersect a scene with a ray
 */
TEST(Scene, IntersectSceneWithRay) {
    // Given scene is the default scene
    Scene scene;

    // First sphere: radius 1 (not transformed)
    Sphere* sphere1 = new Sphere("sphere1");
    scene.addObject(sphere1);

    // Second sphere: radius 0.5 (scaled by 0.5)
    Sphere* sphere2 = new Sphere("sphere2");
    sphere2->setTransform(Matrix::scale(0.5, 0.5, 0.5));
    scene.addObject(sphere2);

    // And ray is a Ray(point(0, 0, -5), vector(0, 0, 1))
    Ray ray(Point(0, 0, -5), Vector(0, 0, 1));

    // When xs is traceRay(scene, ray)
    Intersections xs = scene.traceRay(ray);

    // Then xs.count = 4
    EXPECT_EQ(xs.count(), 4);

    // And xs[0].t = 4
    EXPECT_NEAR(xs[0].t, 4.0, 1e-5);

    // And xs[1].t = 4.5
    EXPECT_NEAR(xs[1].t, 4.5, 1e-5);

    // And xs[2].t = 5.5
    EXPECT_NEAR(xs[2].t, 5.5, 1e-5);

    // And xs[3].t = 6
    EXPECT_NEAR(xs[3].t, 6.0, 1e-5);

    // Cleanup
    delete sphere1;
    delete sphere2;
}

/**
 * Scenario: default scene has correct properties
 */
TEST(Scene, DefaultSceneHasCorrectProperties) {
    // Given scene is Scene::defaultScene()
    Scene* scene = Scene::defaultScene();

    // Then scene contains 2 objects
    EXPECT_EQ(scene->getObjectCount(), 2);

    // And scene contains "sphere1"
    EXPECT_TRUE(scene->contains("sphere1"));

    // And scene contains "sphere2"
    EXPECT_TRUE(scene->contains("sphere2"));

    // And sphere1 has identity transform
    Shape* sphere1 = scene->getObject("sphere1");
    EXPECT_TRUE(sphere1 != nullptr);
    Matrix identity = Matrix::identity(4);
    EXPECT_TRUE(sphere1->getTransform() == identity);

    // And sphere2 is scaled by 0.5
    Shape* sphere2 = scene->getObject("sphere2");
    EXPECT_TRUE(sphere2 != nullptr);
    Matrix expectedTransform = Matrix::scale(0.5, 0.5, 0.5);
    EXPECT_TRUE(sphere2->getTransform() == expectedTransform);

    // Cleanup
    for (auto* obj : scene->getObjects()) {
        delete obj;
    }
    delete scene;
}

/**
 * Scenario: empty scene returns no intersections
 */
TEST(Scene, EmptySceneReturnsNoIntersections) {
    // Given scene is an empty Scene
    Scene scene;

    // And ray is a Ray(point(0, 0, -5), vector(0, 0, 1))
    Ray ray(Point(0, 0, -5), Vector(0, 0, 1));

    // When xs is traceRay(scene, ray)
    Intersections xs = scene.traceRay(ray);

    // Then xs is empty
    EXPECT_EQ(xs.count(), 0);
    EXPECT_TRUE(xs.isEmpty());
}

/**
 * Scenario: scene with objects that are missed
 */
TEST(Scene, SceneWithMissedObjects) {
    // Given scene is a Scene
    Scene scene;

    // And sphere is a Sphere translated far to the right
    Sphere* sphere = new Sphere("sphere");
    sphere->setTransform(Matrix::translate(10, 0, 0));
    scene.addObject(sphere);

    // And ray is a Ray pointing straight ahead
    Ray ray(Point(0, 0, -5), Vector(0, 0, 1));

    // When xs is traceRay(scene, ray)
    Intersections xs = scene.traceRay(ray);

    // Then xs is empty (ray misses the sphere)
    EXPECT_EQ(xs.count(), 0);

    // Cleanup
    delete sphere;
}

/**
 * Scenario: adding multiple objects to scene
 */
TEST(Scene, AddingMultipleObjects) {
    // Given scene is a Scene
    Scene scene;

    // And sphere1, sphere2, sphere3 are Spheres
    Sphere* sphere1 = new Sphere("Sphere1");
    Sphere* sphere2 = new Sphere("Sphere2");
    Sphere* sphere3 = new Sphere("Sphere3");

    // When all spheres are added to scene
    scene.addObject(sphere1);
    scene.addObject(sphere2);
    scene.addObject(sphere3);

    // Then scene contains 3 objects
    EXPECT_EQ(scene.getObjectCount(), 3);

    // And all spheres can be found by name
    EXPECT_TRUE(scene.contains("Sphere1"));
    EXPECT_TRUE(scene.contains("Sphere2"));
    EXPECT_TRUE(scene.contains("Sphere3"));

    // Cleanup
    delete sphere1;
    delete sphere2;
    delete sphere3;
}
