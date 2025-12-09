/**
 * TestReflectionDebug.cpp
 *
 * Kleine Hilfsanwendung, um die Reflexions-Szene separat zu rendern.
 * Liefert eine PPM im Ordner output/ für schnellen visuellen Vergleich.
 */
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Point/Point.h"
#include "Vector/Vector.h"
#include "Color/Color.h"
#include "RayTracer/RayTracer.h"
#include "Shape/Plane.h"
#include "Shape/Sphere.h"
#include "Matrix/Matrix.h"
#include "Material/Material.h"
#include "LightSource/PointLightSource.h"
#include "Canvas/Canvas.h"
#include "Intersection/Intersections.h"
#include "Intersection/Intersection.h"
#include "HitInfo/HitInfo.h"
#include <iostream>

// Abbild der Scene aus MainReflection (ohne Rekursions-Varianten)
static Scene* createReflectionScene(double reflectiveFloor = 0.5, double reflectiveRight = 0.9) {
    Scene* scene = new Scene();

    Plane* floor = new Plane("floor");
    Material floorMat;
    floorMat.color = Color(0.12, 0.12, 0.12);
    floorMat.ambient = 0.05;
    floorMat.diffuse = 0.9;
    floorMat.specular = 0.0;
    floorMat.reflectivity = reflectiveFloor;
    floor->setMaterial(floorMat);
    scene->addObject(floor);

    Plane* wall = new Plane("back_wall");
    wall->setTransform(Matrix::translate(0, 0, 5) * Matrix::rotateX(M_PI / 2.0));
    Material wallMat;
    wallMat.color = Color(0.2, 0.2, 0.2);
    wallMat.ambient = 0.05;
    wallMat.diffuse = 0.8;
    wallMat.specular = 0.0;
    wall->setMaterial(wallMat);
    scene->addObject(wall);

    Sphere* big = new Sphere("big_sphere");
    big->setTransform(Matrix::translate(-0.4, 1.0, 0.6) * Matrix::scale(1.0, 1.0, 1.0));
    Material bigMat;
    bigMat.color = Color(0.2, 0.8, 0.8);
    bigMat.diffuse = 0.7;
    bigMat.specular = 0.3;
    bigMat.shininess = 120.0;
    big->setMaterial(bigMat);
    scene->addObject(big);

    Sphere* smallLeft = new Sphere("small_left");
    smallLeft->setTransform(
        Matrix::translate(-1.6, 0.33, -1.2) *
        Matrix::scale(0.33, 0.33, 0.33)
    );
    Material smallLeftMat;
    smallLeftMat.color = Color(1.0, 0.82, 0.1);
    smallLeftMat.diffuse = 0.7;
    smallLeftMat.specular = 0.3;
    smallLeft->setMaterial(smallLeftMat);
    scene->addObject(smallLeft);

    Sphere* right = new Sphere("right_sphere");
    right->setTransform(
        Matrix::translate(1.2, 0.45, -0.2) *
        Matrix::scale(0.5, 0.5, 0.5)
    );
    Material rightMat;
    rightMat.color = Color(0.4, 1.0, 0.4);
    rightMat.diffuse = 0.7;
    rightMat.specular = 0.3;
    rightMat.shininess = 200.0;
    rightMat.reflectivity = reflectiveRight;
    right->setMaterial(rightMat);
    scene->addObject(right);

    PointLightSource* key = new PointLightSource(
        Point(-10, 10, -10),
        Color(1, 1, 1),
        1.6
    );
    scene->addLight(key);

    PointLightSource* fill = new PointLightSource(
        Point(8, 6, -6),
        Color(0.7, 0.75, 0.8),
        0.7
    );
    scene->addLight(fill);

    return scene;
}

int main() {
    try {
        Scene* scene = createReflectionScene();
        Camera camera(1200, 600, 45.0,
                      Point(0, 1.3, -5.8), Point(0, 1.0, 0.6), Vector(0, 1, 0));
        Canvas canvas(camera.getWidth(), camera.getHeight());
        Color background(0.08, 0.08, 0.08);
        for (int y = 0; y < camera.getHeight(); ++y) {
            for (int x = 0; x < camera.getWidth(); ++x) {
                Ray ray = camera.rayForPixel(x, y);
                Intersections xs = scene->traceRay(ray);
                const Intersection* hit = xs.hit();
                if (hit == nullptr) {
                    canvas.setPixel(x, y, background);
                    continue;
                }
                HitInfo hitInfo = Scene::computeHitInfo(*hit, ray);
                Color color = scene->shadeHit(hitInfo, 5);
                canvas.setPixel(x, y, color);
            }
        }
        canvas.save("reflection-debug");

        for (auto* obj : scene->getObjects()) delete obj;
        for (auto* light : scene->getLights()) delete light;
        delete scene;

        std::cout << "Saved output/reflection-debug.ppm\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
