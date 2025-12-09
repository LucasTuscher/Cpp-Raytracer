/**
 * TestSimpleLighting.cpp - Minimaler Test der Beleuchtung
 */
#include <iostream>
#include "Point/Point.h"
#include "Vector/Vector.h"
#include "Color/Color.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "RayTracer/RayTracer.h"
#include "Shape/Sphere.h"
#include "Material/Material.h"
#include "LightSource/PointLightSource.h"

int main() {
    std::cout << "=== Minimal Lighting Test ===\n";

    Scene* scene = new Scene();

    // Eine einfache Kugel
    Sphere* sphere = new Sphere("sphere");
    Material mat;
    mat.color = Color(1.0, 0, 0);  // Rot
    mat.ambient = 0.1;
    mat.diffuse = 0.9;
    mat.specular = 0.9;
    mat.shininess = 200.0;
    sphere->setMaterial(mat);
    scene->addObject(sphere);

    // Lichtquelle direkt von vorne
    PointLightSource* light = new PointLightSource(
        Point(0, 0, -10),
        Color(1, 1, 1),
        1.0
    );
    scene->addLight(light);

    // Sehr kleine Kamera
    Camera camera(100, 100, 60.0,
                  Point(0, 0, -5), Point(0, 0, 0), Vector(0, 1, 0));

    std::cout << "Rendering...\n";
    RayTracer raytracer(scene, &camera);
    raytracer.render();
    raytracer.getRenderTarget().save("test-simple-lighting");
    std::cout << "Saved: test-simple-lighting.ppm\n";

    // Prüfe einige Pixel manuell
    const Canvas& canvas = raytracer.getRenderTarget();
    Color centerPixel = canvas.getPixel(50, 50);
    std::cout << "Center pixel color: R=" << centerPixel.r
              << " G=" << centerPixel.g
              << " B=" << centerPixel.b << "\n";

    delete sphere;
    delete light;
    delete scene;

    return 0;
}
