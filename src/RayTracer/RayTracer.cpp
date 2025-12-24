/**
 * RayTracer.cpp
 *
 * Implementierung der RayTracer-Klasse.
 */
#include "RayTracer.h"
#include <chrono>
#include <iostream>

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
    const auto start = std::chrono::steady_clock::now();

    // Für jedes Pixel des Bildes
    for (int y = 0; y < camera_->getHeight(); y++) {
        for (int x = 0; x < camera_->getWidth(); x++) {
            // 1. Erzeuge einen Strahl von der Kamera durch das Pixel
            Ray ray = camera_->rayForPixel(x, y);

            // 2. Berechne die Farbe mit Beleuchtung
            // Scene::colorAt() führt Strahlverfolgung und Beleuchtungsberechnung durch
            Color pixelColor = scene_->colorAt(ray);

            // Falls keine Lichtquellen in der Szene sind, verwende Fallback-Farben
            if (scene_->getLightCount() == 0) {
                // Alte Logik für Szenen ohne Beleuchtung
                Intersections intersections = scene_->traceRay(ray);
                const Intersection* hit = intersections.hit();

                if (hit != nullptr) {
                    pixelColor = hitColor_;
                } else {
                    pixelColor = missColor_;
                }
            }

            // 3. Schreibe die Farbe in den Canvas
            renderTarget_.setPixel(x, y, pixelColor);
        }
    }

    const auto end = std::chrono::steady_clock::now();
    const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Renderzeit: " << ns << " ns (" << ms << " ms)" << std::endl;
}
