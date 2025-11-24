#pragma once
#include "../Point/Point.h"
#include "../Vector/Vector.h"
#include "../Matrix/Matrix.h"
#include "../Ray/Ray.h"
#include <cmath>

/**
 * Camera-Klasse
 *
 * Repräsentiert eine virtuelle Kamera für Ray Tracing.
 *
 * Die Kamera definiert:
 * - Auflösung des zu erzeugenden Bildes (width × height in Pixeln)
 * - Öffnungswinkel (field of view)
 * - Position und Orientierung (über View-Transformation)
 *
 * Die Kamera generiert Strahlen, die durch die Pixel der Bildebene gehen.
 * Per Konvention befindet sich die Bildebene eine Einheit vom Kameraursprung entfernt.
 */
class Camera {
private:
    int width_;          // Breite des Bildes in Pixeln
    int height_;         // Höhe des Bildes in Pixeln
    double fov_;         // Öffnungswinkel (field of view) in Radiant
    Matrix transform_;   // View-Transformationsmatrix

    // Interne Parameter (werden berechnet)
    double halfWidth_;   // Halbe Breite der Bildebene
    double halfHeight_;  // Halbe Höhe der Bildebene
    double pixelSize_;   // Größe eines Pixels auf der Bildebene

    /**
     * Berechnet die internen Kamera-Parameter
     *
     * Berechnet halfWidth, halfHeight und pixelSize basierend auf
     * den Kamera-Einstellungen (width, height, fov).
     */
    void computePixelSize();

public:
    /**
     * Konstruktor mit Einheitsmatrix als View-Transformation
     *
     * Erstellt eine Kamera mit:
     * - Position: (0, 0, 0)
     * - Blickrichtung: entlang negativer z-Achse
     * - Up-Vektor: positive y-Achse
     *
     * @param width Breite des Bildes in Pixeln
     * @param height Höhe des Bildes in Pixeln
     * @param fov Öffnungswinkel in Grad (wird intern in Radiant konvertiert)
     */
    Camera(int width, int height, double fov);

    /**
     * Konstruktor mit externer View-Transformation
     *
     * @param width Breite des Bildes in Pixeln
     * @param height Höhe des Bildes in Pixeln
     * @param fov Öffnungswinkel in Grad (wird intern in Radiant konvertiert)
     * @param viewTransform View-Transformationsmatrix
     */
    Camera(int width, int height, double fov, const Matrix& viewTransform);

    /**
     * Konstruktor mit Position, LookAt und Up
     *
     * Berechnet die View-Transformation automatisch.
     *
     * @param width Breite des Bildes in Pixeln
     * @param height Höhe des Bildes in Pixeln
     * @param fov Öffnungswinkel in Grad (wird intern in Radiant konvertiert)
     * @param position Standpunkt der Kamera
     * @param lookAt Punkt, auf den die Kamera schaut
     * @param up Vektor, der nach oben zeigt
     */
    Camera(int width, int height, double fov,
           const Point& position, const Point& lookAt, const Vector& up);

    /**
     * Gibt die Breite des Bildes zurück
     * @return Breite in Pixeln
     */
    int getWidth() const { return width_; }

    /**
     * Gibt die Höhe des Bildes zurück
     * @return Höhe in Pixeln
     */
    int getHeight() const { return height_; }

    /**
     * Gibt den Öffnungswinkel zurück
     * @return FOV in Radiant
     */
    double getFov() const { return fov_; }

    /**
     * Gibt die View-Transformationsmatrix zurück
     * @return Transformationsmatrix
     */
    const Matrix& getTransform() const { return transform_; }

    /**
     * Setzt die View-Transformationsmatrix
     * @param transform Neue Transformationsmatrix
     */
    void setTransform(const Matrix& transform) { transform_ = transform; }

    /**
     * Gibt die Pixelgröße zurück
     * @return Größe eines Pixels auf der Bildebene
     */
    double getPixelSize() const { return pixelSize_; }

    /**
     * Generiert einen Strahl für ein bestimmtes Pixel
     *
     * Der Strahl startet an der Kameraposition und geht durch
     * das Zentrum des angegebenen Pixels auf der Bildebene.
     *
     * @param pixelX x-Koordinate des Pixels (0 bis width-1)
     * @param pixelY y-Koordinate des Pixels (0 bis height-1)
     * @return Strahl in Weltkoordinaten
     */
    Ray rayForPixel(int pixelX, int pixelY) const;
};
