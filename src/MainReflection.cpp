/**
 * MainReflection.cpp
 *
 * Ray Tracer - Kapitel 11: Reflexion
 *
 * Rendert eine Szene mit reflektierendem Boden und reflektierender Kugel.
 * Zusätzlich werden Varianten mit begrenzter Rekursionstiefe erzeugt.
 */
#include <iostream>
#include <memory>
#include <string>

#ifdef _WIN32
#include <direct.h>
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
#include "Canvas/Canvas.h"
#include "Intersection/Intersections.h"
#include "Intersection/Intersection.h"
#include "HitInfo/HitInfo.h"
#include "Shape/Sphere.h"
#include "Shape/Plane.h"
#include "Matrix/Matrix.h"
#include "Material/Material.h"
#include "LightSource/PointLightSource.h"

static void createDirectory(const std::string& path) {
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0755);
#endif
}

/**
 * Erstellt die Reflexions-Szene aus dem Beispiel:
 * - Reflektierender Boden
 * - Eine große türkisfarbene Kugel
 * - Eine kleine gelbe Kugel links
 * - Eine grüne, stark reflektierende Kugel rechts
 */
static Scene* createReflectionScene(double reflectiveFloor = 0.5, double reflectiveRight = 0.9) {
    Scene* scene = new Scene();

    // Boden als Ebene (y = 0)
    Plane* floor = new Plane("floor");
    Material floorMat;
    floorMat.color = Color(0.18, 0.18, 0.18);
    floorMat.ambient = 0.06;
    floorMat.diffuse = 0.9;
    floorMat.specular = 0.0;
    floorMat.reflectivity = reflectiveFloor;
    floor->setMaterial(floorMat);
    scene->addObject(floor);

    // Rückwand als vertikale Ebene (normal nach +z, steht bei z = 5)
    Plane* wall = new Plane("back_wall");
    wall->setTransform(Matrix::translate(0, 0, 5) * Matrix::rotateX(M_PI / 2.0));
    Material wallMat;
    wallMat.color = Color(0.14, 0.14, 0.14);
    wallMat.ambient = 0.04;
    wallMat.diffuse = 0.7;
    wallMat.specular = 0.0;
    wallMat.reflectivity = 0.0;
    wall->setMaterial(wallMat);
    scene->addObject(wall);

    // Große türkisfarbene Kugel
    Sphere* big = new Sphere("big_sphere");
    big->setTransform(Matrix::translate(-0.4, 1.0, 0.6) * Matrix::scale(1.1, 1.1, 1.1));
    Material bigMat;
    bigMat.color = Color(0.18, 0.78, 0.78);
    bigMat.diffuse = 0.8;
    bigMat.specular = 0.35;
    bigMat.shininess = 120.0;
    big->setMaterial(bigMat);
    scene->addObject(big);

    // Kleine gelbe Kugel links
    Sphere* smallLeft = new Sphere("small_left");
    smallLeft->setTransform(
        Matrix::translate(-1.5, 0.22, -1.2) *
        Matrix::scale(0.26, 0.26, 0.26)
    );
    Material smallLeftMat;
    smallLeftMat.color = Color(0.95, 0.82, 0.1);
    smallLeftMat.diffuse = 0.7;
    smallLeftMat.specular = 0.35;
    smallLeft->setMaterial(smallLeftMat);
    scene->addObject(smallLeft);

    // Grüne reflektierende Kugel rechts
    Sphere* right = new Sphere("right_sphere");
    right->setTransform(
        Matrix::translate(1.05, 0.45, -0.15) *
        Matrix::scale(0.5, 0.5, 0.5)
    );
    Material rightMat;
    rightMat.color = Color(0.55, 0.95, 0.4);
    rightMat.diffuse = 0.8;
    rightMat.specular = 0.4;
    rightMat.shininess = 200.0;
    rightMat.reflectivity = reflectiveRight;
    right->setMaterial(rightMat);
    scene->addObject(right);

    // Key-Licht (hell, links oben hinten)
    PointLightSource* light = new PointLightSource(
        Point(-10, 10, -8),
        Color(1, 1, 1),
        1.2
    );
    scene->addLight(light);

    // Sanftes Fill-Licht von rechts
    PointLightSource* fill = new PointLightSource(
        Point(8, 8, -6),
        Color(0.75, 0.8, 0.85),
        0.3
    );
    scene->addLight(fill);

    // Back-Light von hinten oben, leicht warm, schwach
    PointLightSource* rim = new PointLightSource(
        Point(0, 12, 6),
        Color(1.0, 0.95, 0.9),
        0.25
    );
    scene->addLight(rim);

    return scene;
}

static void renderImageWithDepth(Scene* scene, const Camera& camera, int depth, const std::string& filename, const Color& background) {
    Canvas canvas(camera.getWidth(), camera.getHeight());
    for (int y = 0; y < camera.getHeight(); ++y) {
        for (int x = 0; x < camera.getWidth(); ++x) {
            Ray ray = camera.rayForPixel(x, y);
            // Manuell hit prüfen, um Hintergrundfarbe setzen zu können
            Intersections xs = scene->traceRay(ray);
            const Intersection* hit = xs.hit();
            if (hit == nullptr) {
                canvas.setPixel(x, y, background);
                continue;
            }
            HitInfo hitInfo = Scene::computeHitInfo(*hit, ray);
            Color color = scene->shadeHit(hitInfo, depth);
            canvas.setPixel(x, y, color);
        }
    }
    canvas.save(filename);
}

static void renderReflectionScenes() {
    std::cout << "\n=== Kapitel 11: Reflexion ===\n";

    // Kamera für alle Varianten (leicht erhöht, Blick nach unten)
    Camera camera(1200, 600, 40.0,
                  Point(0, 1.3, -6.2), Point(0, 1.0, 0.6), Vector(0, 1, 0));
    Color background(0.1, 0.1, 0.1);

    // Standard-Szene
    {
        Scene* scene = createReflectionScene();
        renderImageWithDepth(scene, camera, 5, "reflection-default", background);

        for (auto* obj : scene->getObjects()) delete obj;
        for (auto* light : scene->getLights()) delete light;
        delete scene;
    }

    // Varianten mit begrenzter Rekursionstiefe (2, 3, 10)
    const int depths[] = {2, 3, 10};
    for (int depth : depths) {
        Scene* scene = createReflectionScene();
        std::string name = "reflection-depth-" + std::to_string(depth);
        renderImageWithDepth(scene, camera, depth, name, background);

        for (auto* obj : scene->getObjects()) delete obj;
        for (auto* light : scene->getLights()) delete light;
        delete scene;
    }
}

int main() {
    createDirectory("output");
    try {
        renderReflectionScenes();
    } catch (const std::exception& e) {
        std::cerr << "FEHLER: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
