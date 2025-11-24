/**
 * RayTracer.cpp
 *
 * Implementierung der RayTracer-Klasse.
 */
#include "RayTracer.h"

/**
 * Konstruktor mit Standard-Farben
 */
RayTracer::RayTracer(const Scene* scene, const Camera* camera)
    : scene_(scene),
      camera_(camera),
      renderTarget_(camera->getWidth(), camera->getHeight()),
      hitColor_(1.0, 0.5, 0.0),      // Orange für Treffer
      missColor_(0.39, 0.58, 0.93) { // Cornflower Blue für Fehltreffer
}

/**
 * Konstruktor mit benutzerdefinierten Farben
 */
RayTracer::RayTracer(const Scene* scene, const Camera* camera,
                     const Color& hitColor, const Color& missColor)
    : scene_(scene),
      camera_(camera),
      renderTarget_(camera->getWidth(), camera->getHeight()),
      hitColor_(hitColor),
      missColor_(missColor) {
}

/**
 * Rendert die Szene
 */
void RayTracer::render() {
    // Für jedes Pixel des Bildes
    for (int y = 0; y < camera_->getHeight(); y++) {
        for (int x = 0; x < camera_->getWidth(); x++) {
            // 1. Erzeuge einen Strahl von der Kamera durch das Pixel
            Ray ray = camera_->rayForPixel(x, y);

            // 2. Berechne Schnittpunkte mit allen Objekten der Szene
            Intersections intersections = scene_->traceRay(ray);

            // 3. Bestimme den sichtbaren Schnittpunkt
            const Intersection* hit = intersections.hit();

            // 4. Berechne die Farbe für dieses Pixel
            Color pixelColor;
            if (hit != nullptr) {
                // Treffer: Verwende Hit-Farbe
                pixelColor = hitColor_;
            } else {
                // Kein Treffer: Verwende Miss-Farbe (Hintergrund)
                pixelColor = missColor_;
            }

            // 5. Schreibe die Farbe in den Canvas
            renderTarget_.setPixel(x, y, pixelColor);
        }
    }
}
