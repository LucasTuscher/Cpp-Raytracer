/**
 * Main.cpp
 *
 * Ray Tracer - Kapitel 6: Szenen und Kamera
 *
 * Demonstriert die Grundstruktur des Raytracers mit verschiedenen Szenen und Kameras.
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
#include "Matrix/Matrix.h"

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
 * Test 1: Szene mit einer Kugel - Verschiedene Kameraperspektiven
 */
void renderSingleSphereTests() {
    std::cout << "\n=== Test 1: Einzelne Kugel mit verschiedenen Kameras ===\n";

    // Szene erstellen: Eine Kugel mit Radius 0.5 am Ursprung
    Scene scene;
    Sphere* sphere = new Sphere("sphere");
    sphere->setTransform(Matrix::scale(0.5, 0.5, 0.5));
    scene.addObject(sphere);

    // Test 1a: Kamera füllt Bild knapp bis zum Rand
    std::cout << "  Rendering: test1a-single-sphere-fullview.ppm\n";
    Camera camera1(800, 600, 7.7,
                   Point(0, 0, -10), Point(0, 0, 0), Vector(0, 1, 0));
    RayTracer raytracer1(&scene, &camera1);
    raytracer1.render();
    raytracer1.getRenderTarget().save("test1a-single-sphere-fullview");

    // Test 1b: Quadratisches Bild, Viertel der Kugel oben links
    std::cout << "  Rendering: test1b-single-sphere-quarter.ppm\n";
    Camera camera2(600, 600, 11.4,
                   Point(0, 0, -10), Point(1, 1, 0), Vector(0, 1, 0));
    RayTracer raytracer2(&scene, &camera2);
    raytracer2.render();
    raytracer2.getRenderTarget().save("test1b-single-sphere-quarter");

    // Test 1c: Kugel von Rand zu Rand mit schräger Kamera
    std::cout << "  Rendering: test1c-single-sphere-angled.ppm\n";
    Camera camera3(600, 600, 2.7,
                   Point(10, 10, -10), Point(0, 0, 0), Vector(0, 1, 0));
    RayTracer raytracer3(&scene, &camera3);
    raytracer3.render();
    raytracer3.getRenderTarget().save("test1c-single-sphere-angled");

    // Cleanup
    delete sphere;

    std::cout << "  Fertig!\n";
}

/**
 * Test 2: Szene mit drei Kugeln verschiedener Größen
 */
void renderThreeSpheresTest() {
    std::cout << "\n=== Test 2: Drei Kugeln verschiedener Größen ===\n";
    std::cout << "  Rendering: test2-three-spheres.ppm\n";

    // Szene erstellen
    Scene scene;

    // Kugel 1: Radius 1 an Position (-0.5, 1.0, 0.5)
    Sphere* sphere1 = new Sphere("sphere1");
    sphere1->setTransform(
        Matrix::translate(-0.5, 1.0, 0.5) * Matrix::scale(1.0, 1.0, 1.0)
    );
    scene.addObject(sphere1);

    // Kugel 2: Radius 0.5 an Position (1.5, 0.5, -0.5)
    Sphere* sphere2 = new Sphere("sphere2");
    sphere2->setTransform(
        Matrix::translate(1.5, 0.5, -0.5) * Matrix::scale(0.5, 0.5, 0.5)
    );
    scene.addObject(sphere2);

    // Kugel 3: Radius 0.33 an Position (-1.5, 0.33, -0.75)
    Sphere* sphere3 = new Sphere("sphere3");
    sphere3->setTransform(
        Matrix::translate(-1.5, 0.33, -0.75) * Matrix::scale(0.33, 0.33, 0.33)
    );
    scene.addObject(sphere3);

    // Kamera: Von oben schräg auf die Szene
    Camera camera(800, 400, 60.0,
                  Point(0.0, 1.5, -5.0), Point(0.0, 1.0, 0.0), Vector(0, 1, 0));

    // Rendering
    RayTracer raytracer(&scene, &camera);
    raytracer.render();
    raytracer.getRenderTarget().save("test2-three-spheres");

    // Cleanup
    delete sphere1;
    delete sphere2;
    delete sphere3;

    std::cout << "  Fertig!\n";
}

/**
 * Test 3: Raster von Kugeln (7x7 Gitter)
 */
void renderSphereGridTest() {
    std::cout << "\n=== Test 3: Raster von Kugeln (7x7 Gitter) ===\n";
    std::cout << "  Rendering: test3-sphere-grid.ppm\n";

    // Szene erstellen
    Scene scene;

    // Erstelle ein 7x7 Gitter von Kugeln
    int sphereCount = 0;
    for (int y = -3; y <= 3; y++) {
        for (int x = -3; x <= 3; x++) {
            std::string name = "sphere_" + std::to_string(sphereCount++);
            Sphere* sphere = new Sphere(name);

            // Transformation: Verschiebung zu (x, y, 0) und Skalierung auf Radius 0.4
            sphere->setTransform(
                Matrix::translate(x, y, 0) * Matrix::scale(0.4, 0.4, 0.4)
            );

            scene.addObject(sphere);
        }
    }

    // Kamera: Von vorne auf das Gitter
    Camera camera(400, 400, 90.0,
                  Point(0.0, 0.0, -5.0), Point(0.0, 0.0, 0.0), Vector(0, 1, 0));

    // Rendering
    RayTracer raytracer(&scene, &camera);
    raytracer.render();
    raytracer.getRenderTarget().save("test3-sphere-grid");

    // Cleanup
    for (auto* obj : scene.getObjects()) {
        delete obj;
    }

    std::cout << "  Fertig!\n";
}

/**
 * Test 4: Verwenden der Standard-Testszene
 */
void renderDefaultSceneTest() {
    std::cout << "\n=== Test 4: Standard-Testszene ===\n";
    std::cout << "  Rendering: test4-default-scene.ppm\n";

    // Standard-Testszene verwenden
    Scene* scene = Scene::defaultScene();

    // Einfache Kamera
    Camera camera(400, 400, 60.0,
                  Point(0.0, 0.0, -3.0), Point(0.0, 0.0, 0.0), Vector(0, 1, 0));

    // Rendering
    RayTracer raytracer(scene, &camera);
    raytracer.render();
    raytracer.getRenderTarget().save("test4-default-scene");

    // Cleanup
    for (auto* obj : scene->getObjects()) {
        delete obj;
    }
    delete scene;

    std::cout << "  Fertig!\n";
}

/**
 * Hauptprogramm
 */
int main() {
    std::cout << "============================================\n";
    std::cout << "  Ray Tracer - Kapitel 6: Szenen & Kamera\n";
    std::cout << "============================================\n";

    // Erstelle output-Ordner für die Bilder
    createDirectory("output");

    try {
        // Test 1: Einzelne Kugel mit verschiedenen Kameras
        renderSingleSphereTests();

        // Test 2: Drei Kugeln verschiedener Größen
        renderThreeSpheresTest();

        // Test 3: Raster von Kugeln
        renderSphereGridTest();

        // Test 4: Standard-Testszene
        renderDefaultSceneTest();

        std::cout << "\n============================================\n";
        std::cout << "  Alle Tests erfolgreich abgeschlossen!\n";
        std::cout << "============================================\n";
        std::cout << "\nErzeugte Dateien:\n";
        std::cout << "  - test1a-single-sphere-fullview.ppm\n";
        std::cout << "  - test1b-single-sphere-quarter.ppm\n";
        std::cout << "  - test1c-single-sphere-angled.ppm\n";
        std::cout << "  - test2-three-spheres.ppm\n";
        std::cout << "  - test3-sphere-grid.ppm\n";
        std::cout << "  - test4-default-scene.ppm\n";
        std::cout << "\nDie Bilder wurden im PPM-Format gespeichert.\n";

    } catch (const std::exception& e) {
        std::cerr << "\nFEHLER: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
