/**
 * SceneTests.cpp
 *
 * Unit-Tests für die Scene-Klasse
 *
 * Testet:
 * - Erstellung leerer Szenen
 * - Hinzufügen von Objekten
 * - Zugriff auf Objekte per Name
 * - Schnittpunktberechnung mit der Szene
 * - Standard-Testszene
 */
#include <gtest/gtest.h>
#include "../src/Scene/Scene.h"
#include "../src/Shape/Sphere.h"
#include "../src/Ray/Ray.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"
#include "../src/Matrix/Matrix.h"

/**
 * Test: Erstellen einer leeren Szene
 */
TEST(SceneTest, CreatingAScene) {
    Scene scene;

    EXPECT_EQ(scene.getObjectCount(), 0);
    EXPECT_TRUE(scene.getObjects().empty());
}

/**
 * Test: Hinzufügen unbenannter Objekte zur Szene
 */
TEST(SceneTest, AddingUnnamedObjectsToScene) {
    Scene scene;
    Sphere* sphere = new Sphere();

    scene.addObject(sphere);

    EXPECT_EQ(scene.getObjectCount(), 1);
    EXPECT_TRUE(scene.contains(sphere));

    // Cleanup
    delete sphere;
}

/**
 * Test: Hinzufügen benannter Objekte zur Szene
 */
TEST(SceneTest, AddingNamedObjectsToScene) {
    Scene scene;
    Sphere* sphere = new Sphere("Sphere");

    scene.addObject(sphere);

    EXPECT_TRUE(scene.contains("Sphere"));
    EXPECT_EQ(scene.getObjectCount(), 1);
    EXPECT_TRUE(scene.contains(sphere));

    // Cleanup
    delete sphere;
}

/**
 * Test: Zugriff auf Objekte per Name
 */
TEST(SceneTest, GettingObjectByName) {
    Scene scene;
    Sphere* sphere = new Sphere("MySphere");

    scene.addObject(sphere);

    Shape* retrieved = scene.getObject("MySphere");
    EXPECT_EQ(retrieved, sphere);

    Shape* notFound = scene.getObject("NonExistent");
    EXPECT_EQ(notFound, nullptr);

    // Cleanup
    delete sphere;
}

/**
 * Test: Mehrere Objekte in der Szene
 */
TEST(SceneTest, AddingMultipleObjectsToScene) {
    Scene scene;
    Sphere* sphere1 = new Sphere("Sphere1");
    Sphere* sphere2 = new Sphere("Sphere2");
    Sphere* sphere3 = new Sphere("Sphere3");

    scene.addObject(sphere1);
    scene.addObject(sphere2);
    scene.addObject(sphere3);

    EXPECT_EQ(scene.getObjectCount(), 3);
    EXPECT_TRUE(scene.contains("Sphere1"));
    EXPECT_TRUE(scene.contains("Sphere2"));
    EXPECT_TRUE(scene.contains("Sphere3"));

    // Cleanup
    delete sphere1;
    delete sphere2;
    delete sphere3;
}

/**
 * Test: Schnittpunktberechnung mit der Szene
 *
 * Szene enthält:
 * - Einheitskugel im Ursprung
 * - Kugel im Ursprung, um Faktor 0.5 skaliert
 *
 * Strahl: von (0, 0, -5) in Richtung (0, 0, 1)
 *
 * Erwartete Schnittpunkte:
 * - t = 4.0   (Eintritt große Kugel)
 * - t = 4.5   (Eintritt kleine Kugel)
 * - t = 5.5   (Austritt kleine Kugel)
 * - t = 6.0   (Austritt große Kugel)
 */
TEST(SceneTest, IntersectSceneWithRay) {
    Scene scene;

    // Erste Kugel: Radius 1 (nicht transformiert)
    Sphere* sphere1 = new Sphere("sphere1");
    scene.addObject(sphere1);

    // Zweite Kugel: Radius 0.5 (um Hälfte skaliert)
    Sphere* sphere2 = new Sphere("sphere2");
    sphere2->setTransform(Matrix::scale(0.5, 0.5, 0.5));
    scene.addObject(sphere2);

    // Strahl von (0, 0, -5) in z-Richtung
    Ray ray(Point(0, 0, -5), Vector(0, 0, 1));

    // Schnittpunkte berechnen
    Intersections xs = scene.traceRay(ray);

    // Überprüfungen
    EXPECT_EQ(xs.count(), 4);
    EXPECT_DOUBLE_EQ(xs[0].t, 4.0);
    EXPECT_DOUBLE_EQ(xs[1].t, 4.5);
    EXPECT_DOUBLE_EQ(xs[2].t, 5.5);
    EXPECT_DOUBLE_EQ(xs[3].t, 6.0);

    // Cleanup
    delete sphere1;
    delete sphere2;
}

/**
 * Test: Standard-Testszene erstellen
 */
TEST(SceneTest, CreatingDefaultScene) {
    Scene* scene = Scene::defaultScene();

    EXPECT_EQ(scene->getObjectCount(), 2);
    EXPECT_TRUE(scene->contains("sphere1"));
    EXPECT_TRUE(scene->contains("sphere2"));

    // Cleanup
    for (auto* obj : scene->getObjects()) {
        delete obj;
    }
    delete scene;
}

/**
 * Test: Standard-Testszene hat korrekte Transformationen
 */
TEST(SceneTest, DefaultSceneHasCorrectTransformations) {
    Scene* scene = Scene::defaultScene();

    Shape* sphere1 = scene->getObject("sphere1");
    Shape* sphere2 = scene->getObject("sphere2");

    ASSERT_NE(sphere1, nullptr);
    ASSERT_NE(sphere2, nullptr);

    // Sphere1 sollte Einheitsmatrix haben
    EXPECT_EQ(sphere1->getTransform(), Matrix::identity(4));

    // Sphere2 sollte um 0.5 skaliert sein
    Matrix expected = Matrix::scale(0.5, 0.5, 0.5);
    EXPECT_EQ(sphere2->getTransform(), expected);

    // Cleanup
    for (auto* obj : scene->getObjects()) {
        delete obj;
    }
    delete scene;
}

/**
 * Test: Leere Szene liefert keine Schnittpunkte
 */
TEST(SceneTest, EmptySceneReturnsNoIntersections) {
    Scene scene;
    Ray ray(Point(0, 0, -5), Vector(0, 0, 1));

    Intersections xs = scene.traceRay(ray);

    EXPECT_EQ(xs.count(), 0);
    EXPECT_TRUE(xs.isEmpty());
}

/**
 * Test: Szene mit Objekten, die nicht getroffen werden
 */
TEST(SceneTest, SceneWithObjectsThatAreMissed) {
    Scene scene;
    Sphere* sphere = new Sphere("sphere");
    sphere->setTransform(Matrix::translate(10, 0, 0)); // Weit rechts
    scene.addObject(sphere);

    // Strahl geradeaus
    Ray ray(Point(0, 0, -5), Vector(0, 0, 1));
    Intersections xs = scene.traceRay(ray);

    EXPECT_EQ(xs.count(), 0);

    // Cleanup
    delete sphere;
}
