/**
 * MainLighting.cpp
 *
 * Ray Tracer - Kapitel 7: Beleuchtung
 *
 * Demonstriert das Phong-Beleuchtungsmodell mit verschiedenen Szenen.
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
#include "Material/Material.h"
#include "LightSource/PointLightSource.h"

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
 * Test 1: Einzelne Kugel mit Phong-Beleuchtung
 */
void renderSimpleLitSphere() {
    std::cout << "\n=== Test 1: Einfache beleuchtete Kugel ===\n";
    std::cout << "  Rendering: lighting1-simple-sphere.ppm\n";

    // Szene erstellen
    Scene* scene = new Scene();

    // Kugel im Ursprung mit pink-violettem Material
    Sphere* sphere = new Sphere("sphere");
    Material mat;
    mat.color = Color(1.0, 0.2, 1.0);
    mat.ambient = 0.1;
    mat.diffuse = 0.9;
    mat.specular = 0.9;
    mat.shininess = 200.0;
    sphere->setMaterial(mat);
    scene->addObject(sphere);

    // Lichtquelle links oben hinten
    PointLightSource* light = new PointLightSource(
        Point(-10, 10, -10),
        Color(1, 1, 1),
        1.0
    );
    scene->addLight(light);

    // Kamera
    Camera camera(800, 800, 90.0,
                  Point(0, 0, -2), Point(0, 0, 0), Vector(0, 1, 0));

    // Rendering
    RayTracer raytracer(scene, &camera);
    raytracer.render();
    raytracer.getRenderTarget().save("lighting1-simple-sphere");

    // Cleanup
    delete sphere;
    delete light;
    delete scene;

    std::cout << "  Fertig!\n";
}

/**
 * Test 2: Phong-Komponenten einzeln (Ambient, Diffuse, Specular)
 */
void renderPhongComponents() {
    std::cout << "\n=== Test 2: Phong-Komponenten einzeln ===\n";

    // Lichtquelle und Kamera für alle drei Tests
    Point lightPos(-10, 10, -10);
    Point cameraPos(0, 0, -2);

    // Test 2a: Nur Ambient
    {
        std::cout << "  Rendering: lighting2a-ambient-only.ppm\n";
        Scene* scene = new Scene();

        Sphere* sphere = new Sphere("sphere");
        Material mat;
        mat.color = Color(1.0, 0.2, 1.0);
        mat.ambient = 0.1;
        mat.diffuse = 0.0;  // Kein diffuser Anteil
        mat.specular = 0.0; // Kein spekularer Anteil
        mat.shininess = 200.0;
        sphere->setMaterial(mat);
        scene->addObject(sphere);

        PointLightSource* light = new PointLightSource(lightPos, Color(1, 1, 1), 1.0);
        scene->addLight(light);

        Camera camera(800, 800, 90.0, cameraPos, Point(0, 0, 0), Vector(0, 1, 0));
        RayTracer raytracer(scene, &camera);
        raytracer.render();
        raytracer.getRenderTarget().save("lighting2a-ambient-only");

        delete sphere;
        delete light;
        delete scene;
    }

    // Test 2b: Nur Diffuse
    {
        std::cout << "  Rendering: lighting2b-diffuse-only.ppm\n";
        Scene* scene = new Scene();

        Sphere* sphere = new Sphere("sphere");
        Material mat;
        mat.color = Color(1.0, 0.2, 1.0);
        mat.ambient = 0.0;  // Kein ambienter Anteil
        mat.diffuse = 0.9;
        mat.specular = 0.0; // Kein spekularer Anteil
        mat.shininess = 200.0;
        sphere->setMaterial(mat);
        scene->addObject(sphere);

        PointLightSource* light = new PointLightSource(lightPos, Color(1, 1, 1), 1.0);
        scene->addLight(light);

        Camera camera(800, 800, 90.0, cameraPos, Point(0, 0, 0), Vector(0, 1, 0));
        RayTracer raytracer(scene, &camera);
        raytracer.render();
        raytracer.getRenderTarget().save("lighting2b-diffuse-only");

        delete sphere;
        delete light;
        delete scene;
    }

    // Test 2c: Nur Specular
    {
        std::cout << "  Rendering: lighting2c-specular-only.ppm\n";
        Scene* scene = new Scene();

        Sphere* sphere = new Sphere("sphere");
        Material mat;
        mat.color = Color(1.0, 0.2, 1.0);
        mat.ambient = 0.0;  // Kein ambienter Anteil
        mat.diffuse = 0.0;  // Kein diffuser Anteil
        mat.specular = 0.9;
        mat.shininess = 200.0;
        sphere->setMaterial(mat);
        scene->addObject(sphere);

        PointLightSource* light = new PointLightSource(lightPos, Color(1, 1, 1), 1.0);
        scene->addLight(light);

        Camera camera(800, 800, 90.0, cameraPos, Point(0, 0, 0), Vector(0, 1, 0));
        RayTracer raytracer(scene, &camera);
        raytracer.render();
        raytracer.getRenderTarget().save("lighting2c-specular-only");

        delete sphere;
        delete light;
        delete scene;
    }

    // Test 2d: Alle Komponenten zusammen
    {
        std::cout << "  Rendering: lighting2d-all-combined.ppm\n";
        Scene* scene = new Scene();

        Sphere* sphere = new Sphere("sphere");
        Material mat;
        mat.color = Color(1.0, 0.2, 1.0);
        mat.ambient = 0.1;
        mat.diffuse = 0.9;
        mat.specular = 0.9;
        mat.shininess = 200.0;
        sphere->setMaterial(mat);
        scene->addObject(sphere);

        PointLightSource* light = new PointLightSource(lightPos, Color(1, 1, 1), 1.0);
        scene->addLight(light);

        Camera camera(800, 800, 90.0, cameraPos, Point(0, 0, 0), Vector(0, 1, 0));
        RayTracer raytracer(scene, &camera);
        raytracer.render();
        raytracer.getRenderTarget().save("lighting2d-all-combined");

        delete sphere;
        delete light;
        delete scene;
    }

    std::cout << "  Fertig!\n";
}

/**
 * Test 3: Drei Kugeln mit verschiedenen Materialien
 */
void renderThreeSpheresWithMaterials() {
    std::cout << "\n=== Test 3: Drei Kugeln mit verschiedenen Materialien ===\n";
    std::cout << "  Rendering: lighting3-three-spheres.ppm\n";

    Scene* scene = new Scene();

    // Lichtquelle
    PointLightSource* light = new PointLightSource(
        Point(-10, 10, -10),
        Color(1, 1, 1),
        1.0
    );
    scene->addLight(light);

    // Kugel 1: Rot, glänzend
    Sphere* sphere1 = new Sphere("sphere1");
    sphere1->setTransform(Matrix::translate(-2.5, 0, 0));
    Material mat1;
    mat1.color = Color(1.0, 0.0, 0.0);
    mat1.ambient = 0.1;
    mat1.diffuse = 0.7;
    mat1.specular = 0.9;
    mat1.shininess = 300.0;
    sphere1->setMaterial(mat1);
    scene->addObject(sphere1);

    // Kugel 2: Grün, weniger glänzend
    Sphere* sphere2 = new Sphere("sphere2");
    sphere2->setTransform(Matrix::translate(0, 0, 0));
    Material mat2;
    mat2.color = Color(0.0, 1.0, 0.0);
    mat2.ambient = 0.1;
    mat2.diffuse = 0.9;
    mat2.specular = 0.3;
    mat2.shininess = 50.0;
    sphere2->setMaterial(mat2);
    scene->addObject(sphere2);

    // Kugel 3: Blau, matt
    Sphere* sphere3 = new Sphere("sphere3");
    sphere3->setTransform(Matrix::translate(2.5, 0, 0));
    Material mat3;
    mat3.color = Color(0.0, 0.0, 1.0);
    mat3.ambient = 0.1;
    mat3.diffuse = 0.9;
    mat3.specular = 0.1;
    mat3.shininess = 10.0;
    sphere3->setMaterial(mat3);
    scene->addObject(sphere3);

    // Kamera
    Camera camera(800, 400, 60.0,
                  Point(0, 0, -5), Point(0, 0, 0), Vector(0, 1, 0));

    // Rendering
    RayTracer raytracer(scene, &camera);
    raytracer.render();
    raytracer.getRenderTarget().save("lighting3-three-spheres");

    // Cleanup
    delete sphere1;
    delete sphere2;
    delete sphere3;
    delete light;
    delete scene;

    std::cout << "  Fertig!\n";
}

/**
 * Test 4: Variation von diffuse
 */
void renderDiffuseVariation() {
    std::cout << "\n=== Test 4: Variation von Diffuse (0.3 bis 0.9) ===\n";
    std::cout << "  Rendering: lighting4-diffuse-variation.ppm\n";

    Scene* scene = new Scene();

    // Lichtquelle
    PointLightSource* light = new PointLightSource(
        Point(-10, 10, -10),
        Color(1, 1, 1),
        1.0
    );
    scene->addLight(light);

    // Erstelle 4 Kugeln mit unterschiedlichen diffuse-Werten
    double diffuseValues[] = {0.3, 0.5, 0.7, 0.9};
    for (int i = 0; i < 4; i++) {
        Sphere* sphere = new Sphere("sphere" + std::to_string(i));
        sphere->setTransform(Matrix::translate(-3.0 + i * 2.0, 0, 0) *
                            Matrix::scale(0.8, 0.8, 0.8));
        Material mat;
        mat.color = Color(1.0, 0.8, 0.2);
        mat.ambient = 0.1;
        mat.diffuse = diffuseValues[i];
        mat.specular = 0.3;
        mat.shininess = 100.0;
        sphere->setMaterial(mat);
        scene->addObject(sphere);
    }

    // Kamera
    Camera camera(800, 400, 70.0,
                  Point(0, 0, -5), Point(0, 0, 0), Vector(0, 1, 0));

    // Rendering
    RayTracer raytracer(scene, &camera);
    raytracer.render();
    raytracer.getRenderTarget().save("lighting4-diffuse-variation");

    // Cleanup
    for (auto* obj : scene->getObjects()) {
        delete obj;
    }
    delete light;
    delete scene;

    std::cout << "  Fertig!\n";
}

/**
 * Test 5: Variation von Specular
 */
void renderSpecularVariation() {
    std::cout << "\n=== Test 5: Variation von Specular (0.3 bis 0.9) ===\n";
    std::cout << "  Rendering: lighting5-specular-variation.ppm\n";

    Scene* scene = new Scene();

    // Lichtquelle
    PointLightSource* light = new PointLightSource(
        Point(-10, 10, -10),
        Color(1, 1, 1),
        1.0
    );
    scene->addLight(light);

    // Erstelle 4 Kugeln mit unterschiedlichen specular-Werten
    double specularValues[] = {0.3, 0.5, 0.7, 0.9};
    for (int i = 0; i < 4; i++) {
        Sphere* sphere = new Sphere("sphere" + std::to_string(i));
        sphere->setTransform(Matrix::translate(-3.0 + i * 2.0, 0, 0) *
                            Matrix::scale(0.8, 0.8, 0.8));
        Material mat;
        mat.color = Color(1.0, 0.8, 0.2);
        mat.ambient = 0.1;
        mat.diffuse = 0.7;
        mat.specular = specularValues[i];
        mat.shininess = 100.0;
        sphere->setMaterial(mat);
        scene->addObject(sphere);
    }

    // Kamera
    Camera camera(800, 400, 70.0,
                  Point(0, 0, -5), Point(0, 0, 0), Vector(0, 1, 0));

    // Rendering
    RayTracer raytracer(scene, &camera);
    raytracer.render();
    raytracer.getRenderTarget().save("lighting5-specular-variation");

    // Cleanup
    for (auto* obj : scene->getObjects()) {
        delete obj;
    }
    delete light;
    delete scene;

    std::cout << "  Fertig!\n";
}

/**
 * Test 6: Variation von Shininess
 */
void renderShininessVariation() {
    std::cout << "\n=== Test 6: Variation von Shininess (10 bis 190) ===\n";
    std::cout << "  Rendering: lighting6-shininess-variation.ppm\n";

    Scene* scene = new Scene();

    // Lichtquelle
    PointLightSource* light = new PointLightSource(
        Point(-10, 10, -10),
        Color(1, 1, 1),
        1.0
    );
    scene->addLight(light);

    // Erstelle 7 Kugeln mit unterschiedlichen shininess-Werten
    double shininessValues[] = {10, 40, 70, 100, 130, 160, 190};
    for (int i = 0; i < 7; i++) {
        Sphere* sphere = new Sphere("sphere" + std::to_string(i));
        sphere->setTransform(Matrix::translate(-6.0 + i * 2.0, 0, 0) *
                            Matrix::scale(0.8, 0.8, 0.8));
        Material mat;
        mat.color = Color(0.2, 0.8, 1.0);
        mat.ambient = 0.1;
        mat.diffuse = 0.7;
        mat.specular = 0.3;
        mat.shininess = shininessValues[i];
        sphere->setMaterial(mat);
        scene->addObject(sphere);
    }

    // Kamera
    Camera camera(1000, 300, 80.0,
                  Point(0, 0, -5), Point(0, 0, 0), Vector(0, 1, 0));

    // Rendering
    RayTracer raytracer(scene, &camera);
    raytracer.render();
    raytracer.getRenderTarget().save("lighting6-shininess-variation");

    // Cleanup
    for (auto* obj : scene->getObjects()) {
        delete obj;
    }
    delete light;
    delete scene;

    std::cout << "  Fertig!\n";
}

/**
 * Test 7: Standard-Szene mit Beleuchtung
 */
void renderDefaultSceneWithLighting() {
    std::cout << "\n=== Test 7: Standard-Szene mit Beleuchtung ===\n";
    std::cout << "  Rendering: lighting7-default-scene.ppm\n";

    // Standard-Szene (enthält bereits Materialien und Lichtquellen)
    Scene* scene = Scene::defaultScene();

    // Kamera
    Camera camera(800, 800, 60.0,
                  Point(0, 0, -3), Point(0, 0, 0), Vector(0, 1, 0));

    // Rendering
    RayTracer raytracer(scene, &camera);
    raytracer.render();
    raytracer.getRenderTarget().save("lighting7-default-scene");

    // Cleanup
    for (auto* obj : scene->getObjects()) {
        delete obj;
    }
    for (auto* light : scene->getLights()) {
        delete light;
    }
    delete scene;

    std::cout << "  Fertig!\n";
}

/**
 * Hauptprogramm
 */
int main() {
    std::cout << "============================================\n";
    std::cout << "  Ray Tracer - Kapitel 7: Beleuchtung\n";
    std::cout << "============================================\n";

    // Erstelle output-Ordner für die Bilder
    createDirectory("output");

    try {
        // Test 1: Einfache beleuchtete Kugel
        renderSimpleLitSphere();

        // Test 2: Phong-Komponenten einzeln
        renderPhongComponents();

        // Test 3: Drei Kugeln mit verschiedenen Materialien
        renderThreeSpheresWithMaterials();

        // Test 4: Variation von diffuse
        renderDiffuseVariation();

        // Test 5: Variation von specular
        renderSpecularVariation();

        // Test 6: Variation von shininess
        renderShininessVariation();

        // Test 7: Standard-Szene mit Beleuchtung
        renderDefaultSceneWithLighting();

        std::cout << "\n============================================\n";
        std::cout << "  Alle Tests erfolgreich abgeschlossen!\n";
        std::cout << "============================================\n";
        std::cout << "\nErzeugte Dateien:\n";
        std::cout << "  - lighting1-simple-sphere.ppm\n";
        std::cout << "  - lighting2a-ambient-only.ppm\n";
        std::cout << "  - lighting2b-diffuse-only.ppm\n";
        std::cout << "  - lighting2c-specular-only.ppm\n";
        std::cout << "  - lighting2d-all-combined.ppm\n";
        std::cout << "  - lighting3-three-spheres.ppm\n";
        std::cout << "  - lighting4-diffuse-variation.ppm\n";
        std::cout << "  - lighting5-specular-variation.ppm\n";
        std::cout << "  - lighting6-shininess-variation.ppm\n";
        std::cout << "  - lighting7-default-scene.ppm\n";
        std::cout << "\nDie Bilder wurden im PPM-Format gespeichert.\n";
        std::cout << "Sie befinden sich im 'output' Ordner.\n";

    } catch (const std::exception& e) {
        std::cerr << "\nFEHLER: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
