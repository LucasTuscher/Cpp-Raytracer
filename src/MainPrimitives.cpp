/**
 * MainPrimitives.cpp
 *
 * Ray Tracer - Kapitel 8: Weitere Geometrische Primitive
 *
 * Rendert Beispiel-/Testbilder für:
 * - Plane
 * - Cube
 * - Cylinder
 * - Cone
 */
#include <iostream>
#include <stdexcept>

// Platform-spezifische Includes für mkdir
#ifdef _WIN32
#include <direct.h> // _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "Camera/Camera.h"
#include "Color/Color.h"
#include "Material/Material.h"
#include "Matrix/Matrix.h"
#include "Point/Point.h"
#include "RayTracer/RayTracer.h"
#include "Scene/Scene.h"
#include "Shape/Cone.h"
#include "Shape/Cube.h"
#include "Shape/Cylinder.h"
#include "Shape/Plane.h"
#include "Shape/Sphere.h"
#include "Vector/Vector.h"
#include "LightSource/PointLightSource.h"

// Point lights use inverse-square attenuation, so these demo scenes need a higher
// intensity to remain visible at typical distances.
static constexpr double kDemoPointLightIntensity = 240.0;

static void createDirectory(const std::string& path) {
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0755);
#endif
}

static void saveOrThrow(const Canvas& canvas, const std::string& filename) {
    if (!canvas.save(filename)) {
        throw std::runtime_error("Konnte PPM nicht speichern: " + filename);
    }
}

static void cleanupScene(Scene& scene) {
    for (auto* obj : scene.getObjects()) {
        delete obj;
    }
    for (auto* light : scene.getLights()) {
        delete light;
    }
}

static void renderPlaneWithSpheres() {
    std::cout << "\n=== Kapitel 8: Plane (Boden) + Kugeln ===\n";
    std::cout << "  Rendering: primitives8-plane-floor.ppm\n";

    Scene scene;

    // Boden
    Plane* floor = new Plane("floor");
    Material floorMat;
    floorMat.color = Color(1.0, 0.9, 0.9);
    floorMat.ambient = 0.1;
    floorMat.diffuse = 0.9;
    floorMat.specular = 0.0;
    floorMat.shininess = 50.0;
    floor->setMaterial(floorMat);
    scene.addObject(floor);

    // Drei Kugeln (ähnlich Kapitel 6 Test 2)
    Sphere* sphere1 = new Sphere("sphere1");
    sphere1->setTransform(Matrix::translate(-0.5, 1.0, 0.5));
    Material mat1;
    mat1.color = Color(0.8, 0.1, 0.1);
    mat1.ambient = 0.1;
    mat1.diffuse = 0.7;
    mat1.specular = 0.4;
    mat1.shininess = 150.0;
    sphere1->setMaterial(mat1);
    scene.addObject(sphere1);

    Sphere* sphere2 = new Sphere("sphere2");
    sphere2->setTransform(Matrix::translate(1.5, 0.5, -0.5) * Matrix::scale(0.5, 0.5, 0.5));
    Material mat2;
    mat2.color = Color(0.1, 0.8, 0.2);
    mat2.ambient = 0.1;
    mat2.diffuse = 0.7;
    mat2.specular = 0.3;
    mat2.shininess = 80.0;
    sphere2->setMaterial(mat2);
    scene.addObject(sphere2);

    Sphere* sphere3 = new Sphere("sphere3");
    sphere3->setTransform(Matrix::translate(-1.5, 0.33, -0.75) * Matrix::scale(0.33, 0.33, 0.33));
    Material mat3;
    mat3.color = Color(0.2, 0.3, 0.95);
    mat3.ambient = 0.1;
    mat3.diffuse = 0.8;
    mat3.specular = 0.2;
    mat3.shininess = 60.0;
    sphere3->setMaterial(mat3);
    scene.addObject(sphere3);

    // Licht
    scene.addLight(new PointLightSource(Point(-5, 5, -5), Color(1, 1, 1), kDemoPointLightIntensity));

    // Kamera
    Camera camera(800, 400, 60.0, Point(0.0, 1.5, -5.0), Point(0.0, 1.0, 0.0), Vector(0, 1, 0));
    RayTracer raytracer(&scene, &camera);
    raytracer.render();
    saveOrThrow(raytracer.getRenderTarget(), "primitives8-plane-floor");

    cleanupScene(scene);
}

static void renderCubeExample() {
    std::cout << "\n=== Kapitel 8: Cube (Würfel) Beispiel ===\n";
    std::cout << "  Rendering: primitives8-cube-example.ppm\n";

    Scene scene;

    // Boden
    Plane* floor = new Plane("floor");
    Material floorMat;
    floorMat.color = Color(1.0, 0.9, 0.9);
    floorMat.ambient = 0.1;
    floorMat.diffuse = 0.9;
    floorMat.specular = 0.0;
    floor->setMaterial(floorMat);
    scene.addObject(floor);

    // Kugel 1
    Sphere* sphere1 = new Sphere("sphere1");
    sphere1->setTransform(Matrix::translate(-0.5, 1.0, 0.5));
    Material mat1;
    mat1.color = Color(0.8, 0.1, 0.1);
    mat1.ambient = 0.1;
    mat1.diffuse = 0.6;
    mat1.specular = 0.9;
    mat1.shininess = 200.0;
    sphere1->setMaterial(mat1);
    scene.addObject(sphere1);

    // Würfel an Stelle einer Kugel (Position (1.5, 0.5, -0.5), halb so groß)
    Cube* cube = new Cube("cube");
    cube->setTransform(Matrix::translate(1.5, 0.5, -0.5) * Matrix::scale(0.5, 0.5, 0.5));
    Material cubeMat;
    cubeMat.color = Color(0.9, 0.8, 0.2);
    cubeMat.ambient = 0.1;
    cubeMat.diffuse = 0.7;
    cubeMat.specular = 0.2;
    cubeMat.shininess = 50.0;
    cube->setMaterial(cubeMat);
    scene.addObject(cube);

    // Kugel 3
    Sphere* sphere3 = new Sphere("sphere3");
    sphere3->setTransform(Matrix::translate(-1.5, 0.33, -0.75) * Matrix::scale(0.33, 0.33, 0.33));
    Material mat3;
    mat3.color = Color(0.2, 0.3, 0.95);
    mat3.ambient = 0.1;
    mat3.diffuse = 0.8;
    mat3.specular = 0.2;
    mat3.shininess = 60.0;
    sphere3->setMaterial(mat3);
    scene.addObject(sphere3);

    scene.addLight(new PointLightSource(Point(-5, 5, -5), Color(1, 1, 1), kDemoPointLightIntensity));

    Camera camera(800, 400, 60.0, Point(0.0, 1.5, -5.0), Point(0.0, 1.0, 0.0), Vector(0, 1, 0));
    RayTracer raytracer(&scene, &camera);
    raytracer.render();
    saveOrThrow(raytracer.getRenderTarget(), "primitives8-cube-example");

    cleanupScene(scene);
}

static void renderCylindersExample() {
    std::cout << "\n=== Kapitel 8: Cylinder (Zylinder) Beispiel ===\n";
    std::cout << "  Rendering: primitives8-cylinders.ppm\n";

    Scene scene;

    Plane* floor = new Plane("floor");
    Material floorMat;
    floorMat.color = Color(0.9, 0.9, 0.9);
    floorMat.ambient = 0.1;
    floorMat.diffuse = 0.9;
    floorMat.specular = 0.0;
    floor->setMaterial(floorMat);
    scene.addObject(floor);

    // Unendlicher Zylinder (grün)
    Cylinder* cylinder1 = new Cylinder("cylinder_infinite");
    cylinder1->setTransform(Matrix::translate(-1.4, 0.0, 0.0));
    Material c1;
    c1.color = Color(0.1, 0.9, 0.2);
    c1.ambient = 0.1;
    c1.diffuse = 0.7;
    c1.specular = 0.2;
    c1.shininess = 80.0;
    cylinder1->setMaterial(c1);
    scene.addObject(cylinder1);

    // Begrenzter, geschlossener Zylinder (gelb), -0.5..0.5 und danach skaliert
    Cylinder* cylinder2 = new Cylinder("cylinder_capped");
    cylinder2->minimum = -0.5;
    cylinder2->maximum = 0.5;
    cylinder2->closed = true;
    cylinder2->setTransform(Matrix::translate(1.4, 0.0, 0.0) * Matrix::scale(2.0, 2.0, 2.0));
    Material c2;
    c2.color = Color(0.95, 0.85, 0.1);
    c2.ambient = 0.1;
    c2.diffuse = 0.7;
    c2.specular = 0.2;
    c2.shininess = 40.0;
    cylinder2->setMaterial(c2);
    scene.addObject(cylinder2);

    scene.addLight(new PointLightSource(Point(-5, 5, -5), Color(1, 1, 1), kDemoPointLightIntensity));

    Camera camera(800, 400, 60.0, Point(0.0, 1.5, -5.0), Point(0.0, 1.0, 0.0), Vector(0, 1, 0));
    RayTracer raytracer(&scene, &camera);
    raytracer.render();
    saveOrThrow(raytracer.getRenderTarget(), "primitives8-cylinders");

    cleanupScene(scene);
}

static void renderConesExample() {
    std::cout << "\n=== Kapitel 8: Cone (Kreiskegel) Beispiel ===\n";
    std::cout << "  Rendering: primitives8-cones.ppm\n";

    Scene scene;

    Plane* floor = new Plane("floor");
    Material floorMat;
    floorMat.color = Color(0.95, 0.95, 0.95);
    floorMat.ambient = 0.1;
    floorMat.diffuse = 0.9;
    floorMat.specular = 0.0;
    floor->setMaterial(floorMat);
    scene.addObject(floor);

    // Begrenzter Kegel (links)
    Cone* cone1 = new Cone("cone1");
    cone1->minimum = -1.0;
    cone1->maximum = 0.0;
    cone1->closed = true;
    cone1->setTransform(Matrix::translate(-1.4, 1.0, 0.0) * Matrix::scale(0.7, 1.0, 0.7));
    Material m1;
    m1.color = Color(0.9, 0.2, 0.2);
    m1.ambient = 0.1;
    m1.diffuse = 0.7;
    m1.specular = 0.2;
    m1.shininess = 60.0;
    cone1->setMaterial(m1);
    scene.addObject(cone1);

    // Zweiter Kegel (rechts), stärker abgeschnitten
    Cone* cone2 = new Cone("cone2");
    cone2->minimum = -0.5;
    cone2->maximum = 0.5;
    cone2->closed = true;
    cone2->setTransform(Matrix::translate(1.4, 1.0, 0.0) * Matrix::scale(0.9, 1.0, 0.9));
    Material m2;
    m2.color = Color(0.2, 0.5, 0.95);
    m2.ambient = 0.1;
    m2.diffuse = 0.7;
    m2.specular = 0.2;
    m2.shininess = 60.0;
    cone2->setMaterial(m2);
    scene.addObject(cone2);

    scene.addLight(new PointLightSource(Point(-5, 5, -5), Color(1, 1, 1), kDemoPointLightIntensity));

    Camera camera(800, 400, 60.0, Point(0.0, 1.5, -5.0), Point(0.0, 1.0, 0.0), Vector(0, 1, 0));
    RayTracer raytracer(&scene, &camera);
    raytracer.render();
    saveOrThrow(raytracer.getRenderTarget(), "primitives8-cones");

    cleanupScene(scene);
}

static void renderAllPrimitivesScene() {
    std::cout << "\n=== Kapitel 8: Alle Primitive zusammen ===\n";
    std::cout << "  Rendering: primitives8-all.ppm\n";

    Scene scene;

    // Ebene (Boden)
    Plane* floor = new Plane("floor");
    Material floorMat;
    floorMat.color = Color(1.0, 0.9, 0.9);
    floorMat.ambient = 0.1;
    floorMat.diffuse = 0.9;
    floorMat.specular = 0.0;
    floor->setMaterial(floorMat);
    scene.addObject(floor);

    // Flacher Würfel als "Sockel"
    Cube* base = new Cube("base");
    base->setTransform(Matrix::translate(0, 0.25, 0) * Matrix::scale(5.0, 0.25, 1.0));
    Material baseMat;
    baseMat.color = Color(0.0, 0.1, 0.4);
    baseMat.ambient = 0.1;
    baseMat.diffuse = 0.9;
    baseMat.specular = 0.0;
    base->setMaterial(baseMat);
    scene.addObject(base);

    // Kugel (rot)
    Sphere* sphere = new Sphere("sphere");
    sphere->setTransform(Matrix::translate(-3.0, 1.5, 0.0));
    Material sphereMat;
    sphereMat.color = Color(1.0, 0.0, 0.0);
    sphereMat.ambient = 0.1;
    sphereMat.diffuse = 0.6;
    sphereMat.specular = 0.9;
    sphereMat.shininess = 200.0;
    sphere->setMaterial(sphereMat);
    scene.addObject(sphere);

    // Zylinder (0..2), geschlossen
    Cylinder* cylinder = new Cylinder("cylinder");
    cylinder->minimum = 0.0;
    cylinder->maximum = 2.0;
    cylinder->closed = true;
    cylinder->setTransform(Matrix::translate(-1.0, 0.5, 0.0) * Matrix::scale(0.5, 1.0, 0.5));
    Material cylinderMat;
    cylinderMat.color = Color(0.1, 1.0, 0.9);
    cylinderMat.ambient = 0.1;
    cylinderMat.diffuse = 0.3;
    cylinderMat.specular = 0.9;
    cylinderMat.shininess = 30.0;
    cylinder->setMaterial(cylinderMat);
    scene.addObject(cylinder);

    // Kegel (-1..0), geschlossen
    Cone* cone = new Cone("cone");
    cone->minimum = -1.0;
    cone->maximum = 0.0;
    cone->closed = true;
    cone->setTransform(Matrix::translate(1.0, 1.5, 0.0) * Matrix::scale(0.5, 1.0, 0.5));
    Material coneMat;
    coneMat.color = Color(0.9, 1.0, 0.1);
    coneMat.ambient = 0.1;
    coneMat.diffuse = 0.7;
    coneMat.specular = 0.9;
    coneMat.shininess = 60.0;
    cone->setMaterial(coneMat);
    scene.addObject(cone);

    // Rotierter Würfel
    Cube* cube = new Cube("rotated_cube");
    constexpr double degToRad = 3.14159265358979323846 / 180.0;
    const double angle = 45.0 * degToRad;
    cube->setTransform(
        Matrix::translate(3.0, 1.37, 0.0) *
        Matrix::rotateX(angle) *
        Matrix::rotateZ(angle) *
        Matrix::scale(0.5, 0.5, 0.5)
    );
    Material cubeMat;
    cubeMat.color = Color(1.0, 0.4, 0.0);
    cubeMat.ambient = 0.1;
    cubeMat.diffuse = 0.3;
    cubeMat.specular = 0.9;
    cubeMat.shininess = 30.0;
    cube->setMaterial(cubeMat);
    scene.addObject(cube);

    // Lichtquelle (weiß)
    scene.addLight(new PointLightSource(Point(-5, 5, -5), Color(1, 1, 1), 260.0));

    // Kamera
    Camera camera(800, 400, 60.0, Point(3.0, 4.0, -10.0), Point(0.0, 1.0, 0.0), Vector(0, 1, 0));
    RayTracer raytracer(&scene, &camera);
    raytracer.render();
    saveOrThrow(raytracer.getRenderTarget(), "primitives8-all");

    cleanupScene(scene);
}

int main() {
    createDirectory("output");

    try {
        renderPlaneWithSpheres();
        renderCubeExample();
        renderCylindersExample();
        renderConesExample();
        renderAllPrimitivesScene();
    } catch (const std::exception& e) {
        std::cerr << "FEHLER: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\nFertig! Bilder liegen in ./output\n";
    return 0;
}
