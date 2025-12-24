/**
 * MainTexturedSphere.cpp
 *
 * Demo: Image Texture + UV-Mapping (Sphere)
 *
 * Usage:
 *   ./raytracer_textures [path/to/texture.ppm]
 *
 * If no texture path is provided, a small embedded PPM texture is used.
 */
#include <iostream>
#include <memory>
#include <sstream>

// Platform-spezifische Includes für mkdir
#ifdef _WIN32
#include <direct.h>  // _mkdir
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
#include "Shape/Sphere.h"
#include "Texture/Image.h"
#include "Texture/ImageTexture.h"
#include "Vector/Vector.h"
#include "LightSource/PointLightSource.h"

// Point lights use inverse-square attenuation, so these demo scenes need a higher
// intensity to remain visible at typical distances (~17 units).
static constexpr double kDemoPointLightIntensity = 200.0;

static void createDirectory(const std::string& path) {
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0755);
#endif
}

static std::shared_ptr<ImageTexture> loadTextureFromArgs(int argc, char** argv) {
    if (argc >= 2) {
        return std::make_shared<ImageTexture>(std::string(argv[1]));
    }

    // 8x4 embedded texture (P3)
    static const char* kEmbeddedPPM = R"PPM(
P3
8 4
255
255 255 0   255 0 0     255 255 0   255 0 0     255 255 0   255 0 0     255 255 0   255 0 0
0 255 0     0 255 255   0 255 0     0 255 255   0 255 0     0 255 255   0 255 0     0 255 255
0 0 255     255 0 255   0 0 255     255 0 255   0 0 255     255 0 255   0 0 255     255 0 255
255 255 255 0 0 0       255 255 255 0 0 0       255 255 255 0 0 0       255 255 255 0 0 0
)PPM";

    std::istringstream ppm(kEmbeddedPPM);
    return std::make_shared<ImageTexture>(Image::loadPPM(ppm));
}

int main(int argc, char** argv) {
    createDirectory("output");

    Scene scene;

    Sphere* sphere = new Sphere("textured_sphere");
    Material mat;
    mat.color = Color(1, 1, 1); // tint (white = no tint)
    mat.texture = loadTextureFromArgs(argc, argv);
    mat.ambient = 0.1;
    mat.diffuse = 0.9;
    mat.specular = 0.3;
    mat.shininess = 100.0;
    sphere->setMaterial(mat);
    scene.addObject(sphere);

    PointLightSource* light = new PointLightSource(
        Point(-10, 10, -10),
        Color(1, 1, 1),
        kDemoPointLightIntensity
    );
    scene.addLight(light);

    Camera camera(800, 800, 60.0,
                  Point(0, 0, -3), Point(0, 0, 0), Vector(0, 1, 0));

    RayTracer raytracer(&scene, &camera);
    raytracer.render();
    raytracer.getRenderTarget().save("textured-sphere");

    delete sphere;
    delete light;

    std::cout << "Rendered: output/textured-sphere.ppm\n";
    return 0;
}

