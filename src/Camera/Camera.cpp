/**
 * Camera.cpp
 *
 * Implementierung der Camera-Klasse für Ray Tracing.
 */
#include "Camera.h"
#include <cmath>

// Konvertierung von Grad zu Radiant
#define DEG_TO_RAD(deg) ((deg) * M_PI / 180.0)

/**
 * Berechnet die internen Kamera-Parameter
 */
void Camera::computePixelSize() {
    // 1. Seitenverhältnis berechnen
    double aspectRatio = static_cast<double>(width_) / static_cast<double>(height_);

    // 2. halfView berechnen: Die Hälfte der Bildebene
    // Die Bildebene ist 1 Einheit von der Kamera entfernt
    // tan(fov/2) gibt uns die halbe Höhe bzw. Breite
    double halfView = std::tan(fov_ / 2.0);

    // 3. halfWidth und halfHeight basierend auf Seitenverhältnis berechnen
    if (aspectRatio >= 1.0) {
        // Querformat oder quadratisch
        halfWidth_ = halfView;
        halfHeight_ = halfView / aspectRatio;
    } else {
        // Hochformat
        halfWidth_ = halfView * aspectRatio;
        halfHeight_ = halfView;
    }

    // 4. Pixelgröße berechnen (für quadratische Pixel)
    pixelSize_ = (2.0 * halfWidth_) / width_;
}

/**
 * Konstruktor mit Einheitsmatrix
 */
Camera::Camera(int width, int height, double fov)
    : width_(width),
      height_(height),
      fov_(DEG_TO_RAD(fov)),
      transform_(Matrix::identity(4)) {
    computePixelSize();
}

/**
 * Konstruktor mit externer View-Transformation
 */
Camera::Camera(int width, int height, double fov, const Matrix& viewTransform)
    : width_(width),
      height_(height),
      fov_(DEG_TO_RAD(fov)),
      transform_(viewTransform) {
    computePixelSize();
}

/**
 * Konstruktor mit Position, LookAt und Up
 */
Camera::Camera(int width, int height, double fov,
               const Point& position, const Point& lookAt, const Vector& up)
    : width_(width),
      height_(height),
      fov_(DEG_TO_RAD(fov)),
      transform_(Matrix::viewTransform(position, lookAt, up)) {
    computePixelSize();
}

/**
 * Generiert einen Strahl für ein bestimmtes Pixel
 */
Ray Camera::rayForPixel(int pixelX, int pixelY) const {
    // 1. Offset vom Rand des Canvas zum Pixelzentrum bestimmen
    double xOffset = (pixelX + 0.5) * pixelSize_;
    double yOffset = (pixelY + 0.5) * pixelSize_;

    // 2. Koordinaten des Pixels in Weltkoordinaten berechnen
    // Da die VPN durch die Mitte des Canvas geht, muss der Offset
    // von der Hälfte der Breite/Höhe subtrahiert werden
    // Wichtig: x geht von links nach rechts, daher halfWidth - xOffset
    // y geht von oben nach unten, daher halfHeight - yOffset
    double worldX = halfWidth_ - xOffset;
    double worldY = halfHeight_ - yOffset;

    // 3. Punkt auf der Bildebene und Kameraursprung transformieren
    // Die Bildebene liegt bei z = -1 (eine Einheit vor der Kamera)
    Matrix inverseTransform = transform_.inverse();
    Point pixel = inverseTransform * Point(worldX, worldY, -1.0);
    Point origin = inverseTransform * Point(0.0, 0.0, 0.0);

    // 4. Richtung des Strahls berechnen
    Vector direction = (pixel - origin).normalized();

    // 5. Strahl zurückgeben
    return Ray(origin, direction);
}
