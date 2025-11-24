#pragma once
#include "../Scene/Scene.h"
#include "../Camera/Camera.h"
#include "../Canvas/Canvas.h"
#include "../Color/Color.h"

/**
 * RayTracer-Klasse
 *
 * Hauptklasse für das Ray Tracing.
 * Verbindet Szene und Kamera und erzeugt das gerenderte Bild.
 *
 * Der Basisalgorithmus:
 * 1. Für jedes Pixel des Bildes:
 *    a) Erzeuge einen Strahl von der Kamera durch das Pixel
 *    b) Berechne Schnittpunkte mit allen Objekten der Szene
 *    c) Bestimme den sichtbaren Schnittpunkt (kleinster positiver t-Wert)
 *    d) Berechne die Farbe für dieses Pixel
 *    e) Schreibe die Farbe in den Canvas
 * 2. Gib den Canvas zurück
 */
class RayTracer {
private:
    const Scene* scene_;     // Zeiger auf die zu rendernde Szene
    const Camera* camera_;   // Zeiger auf die Kamera
    Canvas renderTarget_;    // Das gerenderte Bild

    // Farben für einfaches Rendering (werden später durch Beleuchtungsmodell ersetzt)
    Color hitColor_;         // Farbe wenn ein Objekt getroffen wird
    Color missColor_;        // Farbe wenn kein Objekt getroffen wird

public:
    /**
     * Konstruktor
     *
     * @param scene Zeiger auf die zu rendernde Szene
     * @param camera Zeiger auf die Kamera
     */
    RayTracer(const Scene* scene, const Camera* camera);

    /**
     * Konstruktor mit benutzerdefinierten Farben
     *
     * @param scene Zeiger auf die zu rendernde Szene
     * @param camera Zeiger auf die Kamera
     * @param hitColor Farbe für Treffer
     * @param missColor Farbe für Fehltreffer
     */
    RayTracer(const Scene* scene, const Camera* camera,
              const Color& hitColor, const Color& missColor);

    /**
     * Rendert die Szene
     *
     * Führt den Ray Tracing-Algorithmus aus und füllt den Canvas.
     * Diese Methode kann längere Zeit in Anspruch nehmen.
     */
    void render();

    /**
     * Gibt den Canvas mit dem gerenderten Bild zurück
     *
     * @return Referenz auf den Canvas
     */
    Canvas& getRenderTarget() { return renderTarget_; }

    /**
     * Gibt den Canvas zurück (const)
     *
     * @return Const-Referenz auf den Canvas
     */
    const Canvas& getRenderTarget() const { return renderTarget_; }
};
