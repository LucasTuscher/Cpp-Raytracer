/**
 * MainLightSources.cpp
 *
 * Ray Tracer - Kapitel 9: Lichtquellen
 *
 * Demonstriert verschiedene Lichtquellen-Typen:
 * - DirectionalLight (gerichtete Lichtquelle)
 * - SpotLight (Spotlight)
 * - PointLight mit Attenuation (Lichtabschwächung)
 * - Mehrere Lichtquellen kombiniert
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
#include "LightSource/SpotLightSource.h"

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
 * (Die Ebene wird durch eine große, abgeflachte Kugel simuliert)
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

    return scene;
}

/**
 * Test 1: Gerichtete Lichtquelle mit verschiedenen Intensitäten
 */
void renderDirectionalLight() {
    std::cout << "\n=== Test 1: Gerichtete Lichtquelle (DirectionalLight) ===\n";

    double intensities[] = {0.5, 1.0, 10.0};
    std::string filenames[] = {
        "lightsources1-directional-0.5.ppm",
        "lightsources2-directional-1.0.ppm",
        "lightsources3-directional-10.0.ppm"
    };

    for (int i = 0; i < 3; i++) {
        std::cout << "  Rendering: " << filenames[i] << " (Intensität: " << intensities[i] << ")\n";

        // Szene erstellen
        Scene* scene = createDefaultScene();

        // Gerichtete Lichtquelle hinzufügen (Richtung: von rechts oben nach links unten)
        DirectionalLightSource* light = new DirectionalLightSource(
            Vector(10.0, -10.0, 10.0).normalized(),
            Color(1, 1, 1),
            intensities[i]
        );
        scene->addLight(light);

        // Kamera (width, height, fov, position, lookAt, up)
        Camera camera(
            400, 400, 60.0,
            Point(0, 1.5, -5.0),
            Point(0, 1, 0),
            Vector(0, 1, 0)
        );

        // Rendern
        RayTracer raytracer(scene, &camera);
        raytracer.render();

        // Speichern
        raytracer.getRenderTarget().save(filenames[i]);
        delete light;
        delete scene;
    }
}

/**
 * Test 2: Punktlichtquelle mit Attenuation (verschiedene Intensitäten)
 */
void renderPointLightWithAttenuation() {
    std::cout << "\n=== Test 2: Punktlichtquelle mit Attenuation ===\n";

    double intensities[] = {1.0, 10.0, 100.0, 200.0};
    std::string filenames[] = {
        "lightsources4-point-attenuation-1.ppm",
        "lightsources5-point-attenuation-10.ppm",
        "lightsources6-point-attenuation-100.ppm",
        "lightsources7-point-attenuation-200.ppm"
    };

    for (int i = 0; i < 4; i++) {
        std::cout << "  Rendering: " << filenames[i] << " (Intensität: " << intensities[i] << ")\n";

        // Szene erstellen
        Scene* scene = createDefaultScene();

        // Punktlichtquelle hinzufügen
        PointLightSource* light = new PointLightSource(
            Point(-10, 10, -10),
            Color(1, 1, 1),
            intensities[i]
        );
        scene->addLight(light);

        // Kamera (width, height, fov, position, lookAt, up)
        Camera camera(
            400, 400, 60.0,
            Point(0, 1.5, -5.0),
            Point(0, 1, 0),
            Vector(0, 1, 0)
        );

        // Rendern
        RayTracer raytracer(scene, &camera);
        raytracer.render();

        // Speichern
        raytracer.getRenderTarget().save(filenames[i]);
        delete light;
        delete scene;
    }
}

/**
 * Test 3: Spotlight
 */
void renderSpotLight() {
    std::cout << "\n=== Test 3: Spotlight ===\n";
    std::cout << "  Rendering: lightsources8-spotlight.ppm\n";

    // Szene erstellen
    Scene* scene = createDefaultScene();

    // Spotlight hinzufügen (Position, Richtung, Öffnungswinkel, FallOff-Start)
    Point spotPosition(-10, 10, -10);
    Vector spotDirection = (Point(0, 0, 0) - spotPosition).normalized();

    SpotLightSource* light = new SpotLightSource(
        spotPosition,
        spotDirection,
        45.0,  // Totaler Öffnungswinkel
        10.0,  // FallOff-Startwinkel
        Color(1, 1, 1),
        100.0
    );
    scene->addLight(light);

    // Kamera (width, height, fov, position, lookAt, up)
    Camera camera(
        400, 400, 60.0,
        Point(0, 1.5, -5.0),
        Point(0, 1, 0),
        Vector(0, 1, 0)
    );

    // Rendern
    RayTracer raytracer(scene, &camera);
    raytracer.render();

    // Speichern
    raytracer.getRenderTarget().save("lightsources8-spotlight");
    delete light;
    delete scene;
}

/**
 * Test 4: Mehrere Lichtquellen kombiniert
 */
void renderMultipleLights() {
    std::cout << "\n=== Test 4: Mehrere Lichtquellen kombiniert ===\n";
    std::cout << "  Rendering: lightsources9-multiple.ppm\n";

    // Szene erstellen
    Scene* scene = createDefaultScene();

    // 1. Gerichtete weiße Lichtquelle (Hauptbeleuchtung)
    DirectionalLightSource* mainLight = new DirectionalLightSource(
        Vector(0, -10, 10).normalized(),
        Color(1, 1, 1),
        0.5
    );
    scene->addLight(mainLight);

    // 2. Blaues Spotlight auf die kleine linke Kugel
    Point blueSpotPosition(-3, 5, -2);
    Vector blueSpotDirection = (Point(-1.5, 0.33, -0.75) - blueSpotPosition).normalized();
    SpotLightSource* blueSpot = new SpotLightSource(
        blueSpotPosition,
        blueSpotDirection,
        30.0,
        10.0,
        Color(0.3, 0.3, 1),  // Blau
        80.0
    );
    scene->addLight(blueSpot);

    // 3. Orangefarbenes Spotlight auf die rechte Kugel
    Point orangeSpotPosition(3, 5, -2);
    Vector orangeSpotDirection = (Point(1.5, 0.5, -0.5) - orangeSpotPosition).normalized();
    SpotLightSource* orangeSpot = new SpotLightSource(
        orangeSpotPosition,
        orangeSpotDirection,
        30.0,
        10.0,
        Color(1, 0.6, 0.2),  // Orange
        80.0
    );
    scene->addLight(orangeSpot);

    // Kamera (width, height, fov, position, lookAt, up)
    Camera camera(
        400, 400, 60.0,
        Point(0, 1.5, -5.0),
        Point(0, 1, 0),
        Vector(0, 1, 0)
    );

    // Rendern
    RayTracer raytracer(scene, &camera);
    raytracer.render();

    // Speichern
    raytracer.getRenderTarget().save("lightsources9-multiple");
    delete mainLight;
    delete blueSpot;
    delete orangeSpot;
    delete scene;
}

/**
 * Hauptprogramm
 */
int main() {
    std::cout << "\n========================================\n";
    std::cout << "  Ray Tracer - Kapitel 9: Lichtquellen\n";
    std::cout << "========================================\n";

    // Output-Ordner erstellen
    createDirectory("output");

    // Tests ausführen
    renderDirectionalLight();
    renderPointLightWithAttenuation();
    renderSpotLight();
    renderMultipleLights();

    std::cout << "\n=== Alle Bilder erfolgreich gerendert! ===\n";
    std::cout << "Bilder wurden im 'output/' Ordner gespeichert.\n\n";

    return 0;
}
