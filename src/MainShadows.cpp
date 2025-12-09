/**
 * MainShadows.cpp
 *
 * Ray Tracer - Kapitel 10: Schattenberechnung
 *
 * Demonstriert Schattenberechnung mit verschiedenen Szenen.
 */
#include <iostream>
#include <memory>

// Platform-spezifische Includes für mkdir
#ifdef _WIN32
#include <direct.h>  // _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "Point/Point.h"
#include "Vector/Vector.h"
#include "Color/Color.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "RayTracer/RayTracer.h"
#include "Shape/Sphere.h"
#include "Shape/Shape.h"
#include "Matrix/Matrix.h"
#include "Material/Material.h"
#include "LightSource/LightSource.h"
#include "LightSource/PointLightSource.h"
#include "LightSource/DirectionalLightSource.h"

/**
 * Erstellt einen Ordner (falls nicht vorhanden)
 */
void createDirectory(const std::string& path) {
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0755);
#endif
}

/**
 * Erstellt die Standard-Szene mit drei Kugeln und einer Ebene
 */
Scene* createDefaultScene() {
    Scene* scene = new Scene();

    // Boden (große, abgeflachte Kugel als Ebene)
    Sphere* floor = new Sphere("floor");
    floor->setTransform(Matrix::scale(10, 0.01, 10));
    Material floorMat;
    floorMat.color = Color(1, 0.9, 0.9);
    floorMat.specular = 0.0;
    floor->setMaterial(floorMat);
    scene->addObject(floor);

    // Linke Wand
    Sphere* leftWall = new Sphere("left_wall");
    leftWall->setTransform(
        Matrix::translate(0, 0, 5) *
        Matrix::rotateY(-M_PI / 4.0) *
        Matrix::rotateX(M_PI / 2.0) *
        Matrix::scale(10, 0.01, 10)
    );
    leftWall->setMaterial(floorMat);
    scene->addObject(leftWall);

    // Rechte Wand
    Sphere* rightWall = new Sphere("right_wall");
    rightWall->setTransform(
        Matrix::translate(0, 0, 5) *
        Matrix::rotateY(M_PI / 4.0) *
        Matrix::rotateX(M_PI / 2.0) *
        Matrix::scale(10, 0.01, 10)
    );
    rightWall->setMaterial(floorMat);
    scene->addObject(rightWall);

    // Mittlere Kugel (größte)
    Sphere* middleSphere = new Sphere("middle_sphere");
    middleSphere->setTransform(Matrix::translate(-0.5, 1, 0.5));
    Material middleMat;
    middleMat.color = Color(0.1, 1, 0.5);
    middleMat.diffuse = 0.7;
    middleMat.specular = 0.3;
    middleSphere->setMaterial(middleMat);
    scene->addObject(middleSphere);

    // Rechte Kugel
    Sphere* rightSphere = new Sphere("right_sphere");
    rightSphere->setTransform(
        Matrix::translate(1.5, 0.5, -0.5) *
        Matrix::scale(0.5, 0.5, 0.5)
    );
    Material rightMat;
    rightMat.color = Color(0.5, 1, 0.1);
    rightMat.diffuse = 0.7;
    rightMat.specular = 0.3;
    rightSphere->setMaterial(rightMat);
    scene->addObject(rightSphere);

    // Linke Kugel
    Sphere* leftSphere = new Sphere("left_sphere");
    leftSphere->setTransform(
        Matrix::translate(-1.5, 0.33, -0.75) *
        Matrix::scale(0.33, 0.33, 0.33)
    );
    Material leftMat;
    leftMat.color = Color(1, 0.8, 0.1);
    leftMat.diffuse = 0.7;
    leftMat.specular = 0.3;
    leftSphere->setMaterial(leftMat);
    scene->addObject(leftSphere);

    return scene;
}

/**
 * Test 1: Standard-Szene mit Schatten (Punktlichtquelle)
 */
void renderSceneWithShadows() {
    std::cout << "\n=== Test 1: Szene mit Schatten (Punktlichtquelle) ===\n";
    std::cout << "  Rendering: shadows1-default-scene.ppm\n";

    // Szene erstellen
    Scene* scene = createDefaultScene();

    // Punktlichtquelle hinzufügen (höhere Intensität wegen Attenuation)
    PointLightSource* light = new PointLightSource(
        Point(-10, 10, -10),
        Color(1, 1, 1),
        200.0  // Höhere Intensität wegen Attenuation
    );
    scene->addLight(light);

    // Kamera
    Camera camera(
        800, 800, 60.0,
        Point(0, 1.5, -5.0),
        Point(0, 1, 0),
        Vector(0, 1, 0)
    );

    // Rendern
    RayTracer raytracer(scene, &camera);
    raytracer.render();

    // Speichern
    raytracer.getRenderTarget().save("shadows1-default-scene");

    delete light;
    delete scene;
}

/**
 * Test 2: Szene mit gerichteter Lichtquelle (keine Attenuation)
 */
void renderSceneWithDirectionalLight() {
    std::cout << "\n=== Test 2: Szene mit gerichteter Lichtquelle ===\n";
    std::cout << "  Rendering: shadows2-directional-light.ppm\n";

    // Szene erstellen
    Scene* scene = createDefaultScene();

    // Gerichtete Lichtquelle
    DirectionalLightSource* light = new DirectionalLightSource(
        Vector(1, -1, 1).normalized(),
        Color(1, 1, 1),
        1.0
    );
    scene->addLight(light);

    // Kamera
    Camera camera(
        800, 800, 60.0,
        Point(0, 1.5, -5.0),
        Point(0, 1, 0),
        Vector(0, 1, 0)
    );

    // Rendern
    RayTracer raytracer(scene, &camera);
    raytracer.render();

    // Speichern
    raytracer.getRenderTarget().save("shadows2-directional-light");

    delete light;
    delete scene;
}

/**
 * Test 3: Vergleich - Verschiedene Lichtpositionen
 */
void renderShadowComparison() {
    std::cout << "\n=== Test 3: Vergleich verschiedener Lichtpositionen ===\n";

    Point lightPositions[] = {
        Point(-10, 10, -10),
        Point(10, 10, -10),
        Point(0, 10, 0)
    };

    std::string filenames[] = {
        "shadows3a-light-left",
        "shadows3b-light-right",
        "shadows3c-light-above"
    };

    for (int i = 0; i < 3; i++) {
        std::cout << "  Rendering: " << filenames[i] << ".ppm\n";

        Scene* scene = createDefaultScene();

        PointLightSource* light = new PointLightSource(
            lightPositions[i],
            Color(1, 1, 1),
            200.0
        );
        scene->addLight(light);

        Camera camera(
            400, 400, 60.0,
            Point(0, 1.5, -5.0),
            Point(0, 1, 0),
            Vector(0, 1, 0)
        );

        RayTracer raytracer(scene, &camera);
        raytracer.render();
        raytracer.getRenderTarget().save(filenames[i]);

        delete light;
        delete scene;
    }
}

/**
 * Test 4: Szene mit mehreren Lichtquellen und Schatten
 */
void renderMultipleLightsWithShadows() {
    std::cout << "\n=== Test 4: Mehrere Lichtquellen mit Schatten ===\n";
    std::cout << "  Rendering: shadows4-multiple-lights.ppm\n";

    // Szene erstellen
    Scene* scene = createDefaultScene();

    // Erste Lichtquelle (von links)
    PointLightSource* light1 = new PointLightSource(
        Point(-5, 5, -5),
        Color(1, 0.9, 0.9),
        100.0
    );
    scene->addLight(light1);

    // Zweite Lichtquelle (von rechts)
    PointLightSource* light2 = new PointLightSource(
        Point(5, 5, -5),
        Color(0.9, 0.9, 1),
        100.0
    );
    scene->addLight(light2);

    // Kamera
    Camera camera(
        800, 800, 60.0,
        Point(0, 1.5, -5.0),
        Point(0, 1, 0),
        Vector(0, 1, 0)
    );

    // Rendern
    RayTracer raytracer(scene, &camera);
    raytracer.render();

    // Speichern
    raytracer.getRenderTarget().save("shadows4-multiple-lights");

    delete light1;
    delete light2;
    delete scene;
}

/**
 * Hauptprogramm
 */
int main() {
    std::cout << "\n========================================\n";
    std::cout << "  Ray Tracer - Kapitel 10: Schatten\n";
    std::cout << "========================================\n";

    // Output-Ordner erstellen
    createDirectory("output");

    // Tests ausführen
    renderSceneWithShadows();
    renderSceneWithDirectionalLight();
    renderShadowComparison();
    renderMultipleLightsWithShadows();

    std::cout << "\n=== Alle Bilder erfolgreich gerendert! ===\n";
    std::cout << "Bilder wurden im aktuellen Verzeichnis gespeichert.\n\n";

    return 0;
}
