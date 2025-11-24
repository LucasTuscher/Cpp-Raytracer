/**
 * CameraTests_AZTest.cpp
 *
 * Unit-Tests für die Camera-Klasse mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include <cmath>
#include "../src/Camera/Camera.h"
#include "../src/Point/Point.h"
#include "../src/Vector/Vector.h"
#include "../src/Matrix/Matrix.h"
#include "../src/Ray/Ray.h"

// ============================================================================
// Feature: Camera
// ============================================================================

/**
 * Scenario: constructing a camera
 */
TEST(Camera, ConstructingACamera) {
    // Given hsize is 160
    int hsize = 160;

    // And vsize is 120
    int vsize = 120;

    // And fieldOfView is 90
    double fieldOfView = 90.0;

    // When camera is a Camera(hsize, vsize, fieldOfView)
    Camera camera(hsize, vsize, fieldOfView);

    // Then the width of the camera = 160
    EXPECT_EQ(camera.getWidth(), 160);

    // And the height of the camera = 120
    EXPECT_EQ(camera.getHeight(), 120);

    // And the fov of the camera = 90 (in radians)
    double expectedFov = 90.0 * M_PI / 180.0;
    EXPECT_NEAR(camera.getFov(), expectedFov, 1e-10);

    // And the camera transform = the identity matrix
    Matrix identity = Matrix::identity(4);
    EXPECT_TRUE(camera.getTransform() == identity);
}

/**
 * Scenario: the pixel size for a horizontal canvas
 */
TEST(Camera, PixelSizeForHorizontalCanvas) {
    // Given camera is a Camera(200, 125, 90)
    Camera camera(200, 125, 90.0);

    // Then the pixelSize of the camera = 0.01
    EXPECT_NEAR(camera.getPixelSize(), 0.01, 1e-5);
}

/**
 * Scenario: the pixel size for a vertical canvas
 */
TEST(Camera, PixelSizeForVerticalCanvas) {
    // Given camera is a Camera(125, 200, 90)
    Camera camera(125, 200, 90.0);

    // Then the pixelSize of the camera = 0.01
    EXPECT_NEAR(camera.getPixelSize(), 0.01, 1e-5);
}

/**
 * Scenario: constructing a ray through the center of the canvas
 */
TEST(Camera, RayThroughCenterOfCanvas) {
    // Given camera is a Camera(201, 101, 90)
    Camera camera(201, 101, 90.0);

    // When ray is rayForPixel(camera, 100, 50)
    Ray ray = camera.rayForPixel(100, 50);

    // Then ray.origin = point(0, 0, 0)
    Point origin = ray.getOrigin();
    EXPECT_NEAR(origin.x, 0.0, 1e-10);
    EXPECT_NEAR(origin.y, 0.0, 1e-10);
    EXPECT_NEAR(origin.z, 0.0, 1e-10);

    // And ray.direction = vector(0, 0, -1)
    Vector direction = ray.getDirection();
    EXPECT_NEAR(direction.x, 0.0, 1e-10);
    EXPECT_NEAR(direction.y, 0.0, 1e-10);
    EXPECT_NEAR(direction.z, -1.0, 1e-10);
}

/**
 * Scenario: constructing a ray through a corner of the canvas
 */
TEST(Camera, RayThroughCornerOfCanvas) {
    // Given camera is a Camera(201, 101, 90)
    Camera camera(201, 101, 90.0);

    // When ray is rayForPixel(camera, 0, 0)
    Ray ray = camera.rayForPixel(0, 0);

    // Then ray.origin = point(0, 0, 0)
    Point origin = ray.getOrigin();
    EXPECT_NEAR(origin.x, 0.0, 1e-10);
    EXPECT_NEAR(origin.y, 0.0, 1e-10);
    EXPECT_NEAR(origin.z, 0.0, 1e-10);

    // And ray.direction = vector(0.66519, 0.33259, -0.66851)
    EXPECT_NEAR(ray.getDirection().x, 0.66519, 1e-4);
    EXPECT_NEAR(ray.getDirection().y, 0.33259, 1e-4);
    EXPECT_NEAR(ray.getDirection().z, -0.66851, 1e-4);
}

/**
 * Scenario: constructing a ray when the camera is transformed
 */
TEST(Camera, RayWhenCameraIsTransformed) {
    // Given camera is a Camera(201, 101, 90)
    Camera camera(201, 101, 90.0);

    // When camera.transform = rotation_y(0.785398) * translation(0, -2, 5)
    Matrix transform = Matrix::rotateY(M_PI / 4.0) * Matrix::translate(0, -2, 5);
    camera.setTransform(transform);

    // And ray is rayForPixel(camera, 100, 50)
    Ray ray = camera.rayForPixel(100, 50);

    // Then ray.origin = point(0, 2, -5)
    EXPECT_NEAR(ray.getOrigin().x, 0.0, 1e-4);
    EXPECT_NEAR(ray.getOrigin().y, 2.0, 1e-4);
    EXPECT_NEAR(ray.getOrigin().z, -5.0, 1e-4);

    // And ray.direction = vector(0.707106, 0, -0.707106)
    double sqrtHalf = std::sqrt(2.0) / 2.0;
    EXPECT_NEAR(ray.getDirection().x, sqrtHalf, 1e-4);
    EXPECT_NEAR(ray.getDirection().y, 0.0, 1e-4);
    EXPECT_NEAR(ray.getDirection().z, -sqrtHalf, 1e-4);
}

/**
 * Scenario: camera with external view transformation
 */
TEST(Camera, CameraWithViewTransform) {
    // Given viewTransform is viewTransform(point(0, 0, -5), point(0, 0, 0), vector(0, 1, 0))
    Matrix viewTransform = Matrix::viewTransform(
        Point(0, 0, -5),
        Point(0, 0, 0),
        Vector(0, 1, 0)
    );

    // When camera is a Camera(100, 100, 90, viewTransform)
    Camera camera(100, 100, 90.0, viewTransform);

    // Then camera.transform = viewTransform
    EXPECT_TRUE(camera.getTransform() == viewTransform);
}

/**
 * Scenario: camera with position, lookAt and up
 */
TEST(Camera, CameraWithPositionLookAtUp) {
    // Given position is point(1, 2, 3)
    Point position(1, 2, 3);

    // And lookAt is point(4, 5, 6)
    Point lookAt(4, 5, 6);

    // And up is vector(0, 1, 0)
    Vector up(0, 1, 0);

    // When camera is a Camera(100, 100, 90, position, lookAt, up)
    Camera camera(100, 100, 90.0, position, lookAt, up);

    // Then camera.transform is the expected view transform
    Matrix expectedTransform = Matrix::viewTransform(position, lookAt, up);
    EXPECT_TRUE(camera.getTransform() == expectedTransform);
}

/**
 * Scenario: different image sizes
 */
TEST(Camera, DifferentImageSizes) {
    // Given camera1 is square (400x400)
    Camera camera1(400, 400, 60.0);
    EXPECT_EQ(camera1.getWidth(), 400);
    EXPECT_EQ(camera1.getHeight(), 400);

    // And camera2 is landscape (800x600)
    Camera camera2(800, 600, 60.0);
    EXPECT_EQ(camera2.getWidth(), 800);
    EXPECT_EQ(camera2.getHeight(), 600);

    // And camera3 is portrait (600x800)
    Camera camera3(600, 800, 60.0);
    EXPECT_EQ(camera3.getWidth(), 600);
    EXPECT_EQ(camera3.getHeight(), 800);
}

/**
 * Scenario: different field of view values
 */
TEST(Camera, DifferentFieldOfViewValues) {
    // Given camera1 has 45° FOV
    Camera camera1(100, 100, 45.0);
    EXPECT_NEAR(camera1.getFov(), 45.0 * M_PI / 180.0, 1e-10);

    // And camera2 has 90° FOV
    Camera camera2(100, 100, 90.0);
    EXPECT_NEAR(camera2.getFov(), 90.0 * M_PI / 180.0, 1e-10);

    // And camera3 has 120° FOV
    Camera camera3(100, 100, 120.0);
    EXPECT_NEAR(camera3.getFov(), 120.0 * M_PI / 180.0, 1e-10);
}

/**
 * Scenario: rays at different pixel positions
 */
TEST(Camera, RaysAtDifferentPixelPositions) {
    // Given camera is a Camera(200, 200, 90)
    Camera camera(200, 200, 90.0);

    // When ray1 is at pixel (0, 0) - top left
    Ray ray1 = camera.rayForPixel(0, 0);
    Point origin1 = ray1.getOrigin();
    EXPECT_NEAR(origin1.x, 0.0, 1e-10);
    EXPECT_NEAR(origin1.y, 0.0, 1e-10);
    EXPECT_NEAR(origin1.z, 0.0, 1e-10);
    EXPECT_GT(ray1.getDirection().x, 0); // Goes right
    EXPECT_GT(ray1.getDirection().y, 0); // Goes up

    // When ray2 is at pixel (199, 0) - top right
    Ray ray2 = camera.rayForPixel(199, 0);
    Point origin2 = ray2.getOrigin();
    EXPECT_NEAR(origin2.x, 0.0, 1e-10);
    EXPECT_NEAR(origin2.y, 0.0, 1e-10);
    EXPECT_NEAR(origin2.z, 0.0, 1e-10);
    EXPECT_LT(ray2.getDirection().x, 0); // Goes left
    EXPECT_GT(ray2.getDirection().y, 0); // Goes up

    // When ray3 is at pixel (0, 199) - bottom left
    Ray ray3 = camera.rayForPixel(0, 199);
    Point origin3 = ray3.getOrigin();
    EXPECT_NEAR(origin3.x, 0.0, 1e-10);
    EXPECT_NEAR(origin3.y, 0.0, 1e-10);
    EXPECT_NEAR(origin3.z, 0.0, 1e-10);
    EXPECT_GT(ray3.getDirection().x, 0); // Goes right
    EXPECT_LT(ray3.getDirection().y, 0); // Goes down

    // When ray4 is at pixel (199, 199) - bottom right
    Ray ray4 = camera.rayForPixel(199, 199);
    Point origin4 = ray4.getOrigin();
    EXPECT_NEAR(origin4.x, 0.0, 1e-10);
    EXPECT_NEAR(origin4.y, 0.0, 1e-10);
    EXPECT_NEAR(origin4.z, 0.0, 1e-10);
    EXPECT_LT(ray4.getDirection().x, 0); // Goes left
    EXPECT_LT(ray4.getDirection().y, 0); // Goes down
}

/**
 * Scenario: aspect ratio calculation
 */
TEST(Camera, AspectRatioCalculation) {
    // Given camera1 is 16:9 landscape (1920x1080)
    Camera camera1(1920, 1080, 60.0);
    double pixelSize1 = camera1.getPixelSize();

    // And camera2 is 9:16 portrait (1080x1920)
    Camera camera2(1080, 1920, 60.0);
    double pixelSize2 = camera2.getPixelSize();

    // Then pixel sizes should be approximately equal (square pixels)
    EXPECT_NEAR(pixelSize1, pixelSize2, 1e-4);
}
