#pragma once
#include "LightSource.h"
#include "../Point/Point.h"
#include "../Vector/Vector.h"
#include <cmath>
#include <limits>

/**
 * DirectionalLightSource-Klasse
 *
 * Repräsentiert eine gerichtete Lichtquelle (wie die Sonne), die Licht
 * in eine bestimmte Richtung ausstrahlt, aber keine feste Position hat.
 *
 * Eigenschaften:
 * - Richtung des Lichts (normalisiert)
 * - Farbe des Lichts
 * - Intensität des Lichts
 * - Keine Lichtabschwächung (Licht kommt aus unendlicher Entfernung)
 *
 * Gerichtete Lichtquellen werden oft für Sonnenlicht verwendet,
 * da die Sonne so weit entfernt ist, dass alle Lichtstrahlen parallel sind.
 */
class DirectionalLightSource : public LightSource {
private:
    Vector direction_; // Richtung des Lichts (normalisiert)

public:
    /**
     * Konstruktor
     * @param direction Richtung des Lichts (wird automatisch normalisiert)
     * @param color Farbe des Lichts (Standard: Weiß)
     * @param intensity Intensität des Lichts (Standard: 1.0)
     */
    DirectionalLightSource(const Vector& direction,
                          const Color& color = Color(1, 1, 1),
                          double intensity = 1.0)
        : LightSource(color, intensity), direction_(direction.normalized()) {}

    /**
     * Destruktor
     */
    ~DirectionalLightSource() override = default;

    /**
     * Gibt die Richtung der Lichtquelle zurück
     * @return Richtung (normalisiert)
     */
    Vector getDirection() const { return direction_; }

    /**
     * Setzt die Richtung der Lichtquelle
     * @param direction Neue Richtung (wird automatisch normalisiert)
     */
    void setDirection(const Vector& direction) { direction_ = direction.normalized(); }

    /**
     * Gerichtete Lichtquellen sind gerichtet
     * @return true
     */
    bool isDirectional() const override {
        return true;
    }

    /**
     * Berechnet den normalisierten Vektor vom Punkt zur Lichtquelle
     * Bei gerichteten Lichtquellen ist dies die negierte Lichtrichtung
     * @param p Der Punkt auf der Oberfläche (wird nicht verwendet)
     * @return Normalisierte negierte Lichtrichtung
     */
    Vector directionFromPoint(const Point& p) const override {
        (void)p; // Unterdrücke Compiler-Warnung für ungenutzten Parameter
        return -direction_;
    }

    /**
     * Berechnet den normalisierten Vektor von der Lichtquelle zum Punkt
     * Bei gerichteten Lichtquellen ist dies die Lichtrichtung
     * @param p Der Punkt auf der Oberfläche (wird nicht verwendet)
     * @return Normalisierte Lichtrichtung
     */
    Vector directionToPoint(const Point& p) const override {
        (void)p; // Unterdrücke Compiler-Warnung für ungenutzten Parameter
        return direction_;
    }

    /**
     * Berechnet den Abstand vom Punkt zur Lichtquelle
     * Bei gerichteten Lichtquellen ist dies unendlich
     * @param p Der Punkt auf der Oberfläche (wird nicht verwendet)
     * @return Infinity
     */
    double distanceToPoint(const Point& p) const override {
        (void)p; // Unterdrücke Compiler-Warnung für ungenutzten Parameter
        return std::numeric_limits<double>::infinity();
    }

    /**
     * Berechnet die Farbe des Lichts am Punkt p
     * Bei gerichteten Lichtquellen gibt es keine Abschwächung
     * @param p Der Punkt auf der Oberfläche (wird nicht verwendet)
     * @return Die Farbe des Lichts (ohne Abschwächung)
     */
    Color colorAtPoint(const Point& p) const override {
        (void)p; // Unterdrücke Compiler-Warnung für ungenutzten Parameter
        return color_ * intensity_;
    }

    /**
     * Gleichheitsvergleich
     * Vergleicht Richtung, Farbe und Intensität
     * @param other Die zu vergleichende Lichtquelle
     * @return true wenn beide Lichtquellen gleich sind
     */
    bool operator==(const DirectionalLightSource& other) const {
        return direction_ == other.direction_ &&
               color_ == other.color_ &&
               std::abs(intensity_ - other.intensity_) < 1e-6;
    }

    /**
     * Ungleichheitsvergleich
     * @param other Die zu vergleichende Lichtquelle
     * @return true wenn die Lichtquellen unterschiedlich sind
     */
    bool operator!=(const DirectionalLightSource& other) const {
        return !(*this == other);
    }
};
