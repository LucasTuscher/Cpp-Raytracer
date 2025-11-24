/**
 * CameraTests.cpp
 *
 * Unit-Tests für die Camera-Klasse
 *
 * Testet:
 * - Kamera-Konstruktion mit verschiedenen Parametern
 * - Berechnung der Pixelgröße
 * - Strahl-Generierung für verschiedene Pixel
 * - View-Transformation
 */
#include <gtest/gtest.h>
#include <cmath>
#include "../src/Camera/Camera.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"
#include "../src/Matrix/Matrix.h"
#include "../src/Ray/Ray.h"

// Hilfsfunktion für Gleitkomma-Vergleiche (static, um Linker-Fehler zu vermeiden)
static bool approxEqual(double a, double b, double epsilon = 1e-5) {
    return std::abs(a - b) < epsilon;
}

/**
 * Test: Konstruieren einer Kamera
 */
TEST(CameraTest, ConstructingACamera) {
    int hsize = 160;
    int vsize = 120;
    double fieldOfView = 90.0;

    Camera camera(hsize, vsize, fieldOfView);

    EXPECT_EQ(camera.getWidth(), 160);
    EXPECT_EQ(camera.getHeight(), 120);

    // FOV wird in Radiant konvertiert
    double expectedFov = 90.0 * M_PI / 180.0;
    EXPECT_DOUBLE_EQ(camera.getFov(), expectedFov);

    // Transform sollte Einheitsmatrix sein
    EXPECT_EQ(camera.getTransform(), Matrix::identity(4));
}

/**
 * Test: Pixelgröße für horizontales Canvas
 *
 * Bei einem horizontalen Canvas (Breite > Höhe) wird die Breite
 * durch den FOV bestimmt.
 */
TEST(CameraTest, PixelSizeForHorizontalCanvas) {
    Camera camera(200, 125, 90.0);

    // Erwartete Pixelgröße: 0.01
    EXPECT_TRUE(approxEqual(camera.getPixelSize(), 0.01));
}

/**
 * Test: Pixelgröße für vertikales Canvas
 *
 * Bei einem vertikalen Canvas (Höhe > Breite) wird die Höhe
 * durch den FOV bestimmt.
 */
TEST(CameraTest, PixelSizeForVerticalCanvas) {
    Camera camera(125, 200, 90.0);

    // Erwartete Pixelgröße: 0.01
    EXPECT_TRUE(approxEqual(camera.getPixelSize(), 0.01));
}

/**
 * Test: Strahl durch Bildmitte konstruieren
 *
 * Ein Strahl durch die Mitte des Canvas sollte:
 * - Ursprung: Kameraposition (0, 0, 0)
 * - Richtung: Entlang negativer z-Achse (0, 0, -1)
 */
TEST(CameraTest, ConstructingRayThroughCenterOfCanvas) {
    Camera camera(201, 101, 90.0);

    Ray ray = camera.rayForPixel(100, 50);

    EXPECT_EQ(ray.getOrigin(), Point(0, 0, 0));
    EXPECT_EQ(ray.getDirection(), Vector(0, 0, -1));
}

/**
 * Test: Strahl durch Ecke des Canvas konstruieren
 *
 * Ein Strahl durch die obere linke Ecke sollte diagonal verlaufen.
 */
TEST(CameraTest, ConstructingRayThroughCornerOfCanvas) {
    Camera camera(201, 101, 90.0);

    Ray ray = camera.rayForPixel(0, 0);

    EXPECT_EQ(ray.getOrigin(), Point(0, 0, 0));

    // Erwartete Richtung (normalisiert)
    Vector expectedDir(0.66519, 0.33259, -0.66851);

    EXPECT_TRUE(approxEqual(ray.getDirection().x, expectedDir.x));
    EXPECT_TRUE(approxEqual(ray.getDirection().y, expectedDir.y));
    EXPECT_TRUE(approxEqual(ray.getDirection().z, expectedDir.z));
}

/**
 * Test: Strahl konstruieren wenn Kamera transformiert ist
 *
 * Wenn die Kamera transformiert wird (Rotation + Translation),
 * sollten die Strahlen entsprechend angepasst werden.
 */
TEST(CameraTest, ConstructingRayWhenCameraIsTransformed) {
    Camera camera(201, 101, 90.0);

    // Kamera rotieren (45° um y) und verschieben
    Matrix transform = Matrix::rotateY(M_PI / 4.0) * Matrix::translate(0, -2, 5);
    camera.setTransform(transform);

    Ray ray = camera.rayForPixel(100, 50);

    // Erwarteter Ursprung: (0, 2, -5) nach Transformation
    Point expectedOrigin(0, 2, -5);
    EXPECT_TRUE(approxEqual(ray.getOrigin().x, expectedOrigin.x));
    EXPECT_TRUE(approxEqual(ray.getOrigin().y, expectedOrigin.y));
    EXPECT_TRUE(approxEqual(ray.getOrigin().z, expectedOrigin.z));

    // Erwartete Richtung: 45° gedreht
    double sqrtHalf = std::sqrt(2.0) / 2.0;
    Vector expectedDir(sqrtHalf, 0, -sqrtHalf);

    EXPECT_TRUE(approxEqual(ray.getDirection().x, expectedDir.x));
    EXPECT_TRUE(approxEqual(ray.getDirection().y, expectedDir.y));
    EXPECT_TRUE(approxEqual(ray.getDirection().z, expectedDir.z));
}

/**
 * Test: Kamera mit externer View-Transformation konstruieren
 */
TEST(CameraTest, ConstructingCameraWithViewTransform) {
    Matrix viewTransform = Matrix::viewTransform(
        Point(0, 0, -5),  // position
        Point(0, 0, 0),   // lookAt
        Vector(0, 1, 0)   // up
    );

    Camera camera(100, 100, 90.0, viewTransform);

    EXPECT_EQ(camera.getTransform(), viewTransform);
}

/**
 * Test: Kamera mit Position, LookAt und Up konstruieren
 */
TEST(CameraTest, ConstructingCameraWithPositionLookAtUp) {
    Point position(1, 2, 3);
    Point lookAt(4, 5, 6);
    Vector up(0, 1, 0);

    Camera camera(100, 100, 90.0, position, lookAt, up);

    // View-Transform sollte automatisch berechnet werden
    Matrix expectedTransform = Matrix::viewTransform(position, lookAt, up);
    EXPECT_EQ(camera.getTransform(), expectedTransform);
}

/**
 * Test: Verschiedene Bildgrößen
 */
TEST(CameraTest, DifferentImageSizes) {
    // Quadratisch
    Camera camera1(400, 400, 60.0);
    EXPECT_EQ(camera1.getWidth(), 400);
    EXPECT_EQ(camera1.getHeight(), 400);

    // Querformat
    Camera camera2(800, 600, 60.0);
    EXPECT_EQ(camera2.getWidth(), 800);
    EXPECT_EQ(camera2.getHeight(), 600);

    // Hochformat
    Camera camera3(600, 800, 60.0);
    EXPECT_EQ(camera3.getWidth(), 600);
    EXPECT_EQ(camera3.getHeight(), 800);
}

/**
 * Test: Verschiedene FOV-Werte
 */
TEST(CameraTest, DifferentFieldOfViewValues) {
    Camera camera1(100, 100, 45.0);
    EXPECT_DOUBLE_EQ(camera1.getFov(), 45.0 * M_PI / 180.0);

    Camera camera2(100, 100, 90.0);
    EXPECT_DOUBLE_EQ(camera2.getFov(), 90.0 * M_PI / 180.0);

    Camera camera3(100, 100, 120.0);
    EXPECT_DOUBLE_EQ(camera3.getFov(), 120.0 * M_PI / 180.0);
}

/**
 * Test: Strahlen an verschiedenen Pixel-Positionen
 *
 * Testet, dass Strahlen für verschiedene Pixel korrekt generiert werden.
 */
TEST(CameraTest, RaysAtDifferentPixelPositions) {
    Camera camera(200, 200, 90.0);

    // Pixel links oben (0, 0)
    Ray ray1 = camera.rayForPixel(0, 0);
    EXPECT_EQ(ray1.getOrigin(), Point(0, 0, 0));
    EXPECT_GT(ray1.getDirection().x, 0); // Nach rechts
    EXPECT_GT(ray1.getDirection().y, 0); // Nach oben

    // Pixel rechts oben (199, 0)
    Ray ray2 = camera.rayForPixel(199, 0);
    EXPECT_EQ(ray2.getOrigin(), Point(0, 0, 0));
    EXPECT_LT(ray2.getDirection().x, 0); // Nach links
    EXPECT_GT(ray2.getDirection().y, 0); // Nach oben

    // Pixel links unten (0, 199)
    Ray ray3 = camera.rayForPixel(0, 199);
    EXPECT_EQ(ray3.getOrigin(), Point(0, 0, 0));
    EXPECT_GT(ray3.getDirection().x, 0); // Nach rechts
    EXPECT_LT(ray3.getDirection().y, 0); // Nach unten

    // Pixel rechts unten (199, 199)
    Ray ray4 = camera.rayForPixel(199, 199);
    EXPECT_EQ(ray4.getOrigin(), Point(0, 0, 0));
    EXPECT_LT(ray4.getDirection().x, 0); // Nach links
    EXPECT_LT(ray4.getDirection().y, 0); // Nach unten
}

/**
 * Test: Aspect Ratio Berechnung
 *
 * Die Kamera sollte das Seitenverhältnis korrekt berücksichtigen.
 */
TEST(CameraTest, AspectRatioCalculation) {
    // 16:9 Querformat
    Camera camera1(1920, 1080, 60.0);
    double pixelSize1 = camera1.getPixelSize();

    // 9:16 Hochformat
    Camera camera2(1080, 1920, 60.0);
    double pixelSize2 = camera2.getPixelSize();

    // Pixelgröße sollte ungefähr gleich sein (quadratische Pixel)
    EXPECT_TRUE(approxEqual(pixelSize1, pixelSize2, 1e-4));
}
