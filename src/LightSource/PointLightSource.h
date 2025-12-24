#pragma once
#include "LightSource.h"
#include "../Point/Point.h"
#include "../Vector/Vector.h"
#include <cmath>
#include <limits>

/**
 * PointLightSource-Klasse
 *
 * Repräsentiert eine Punktlichtquelle, die von einer bestimmten Position
 * gleichmäßig in alle Richtungen Licht ausstrahlt.
 *
 * Eigenschaften:
 * - Position im Raum
 * - Farbe des Lichts
 * - Intensität des Lichts
 *
 * Eine Punktlichtquelle ist das einfachste Lichtquellen-Modell im Ray Tracing.
 * Sie ist wenig realistisch (unendlich kleine Lichtquelle), aber einfach zu berechnen.
 */
class PointLightSource : public LightSource {
private:
    Point position_; // Position der Lichtquelle im Raum

public:
    /**
     * Konstruktor
     * @param position Position der Lichtquelle
     * @param color Farbe des Lichts (Standard: Weiß)
     * @param intensity Intensität des Lichts (Standard: 1.0)
     */
    PointLightSource(const Point& position,
                     const Color& color = Color(1, 1, 1),
                     double intensity = 1.0)
        : LightSource(color, intensity), position_(position) {}

    /**
     * Destruktor
     */
    ~PointLightSource() override = default;

    /**
     * Gibt die Position der Lichtquelle zurück
     * @return Position
     */
    Point getPosition() const { return position_; }

    /**
     * Setzt die Position der Lichtquelle
     * @param position Neue Position
     */
    void setPosition(const Point& position) { position_ = position; }

    /**
     * Punktlichtquellen sind nicht gerichtet
     * @return false
     */
    bool isDirectional() const override {
        return false;
    }

    /**
     * Berechnet den normalisierten Vektor vom Punkt zur Lichtquelle
     * @param p Der Punkt auf der Oberfläche
     * @return Normalisierter Vektor vom Punkt zur Lichtquelle
     */
    Vector directionFromPoint(const Point& p) const override {
        return (position_ - p).normalized();
    }

    /**
     * Berechnet den normalisierten Vektor von der Lichtquelle zum Punkt
     * @param p Der Punkt auf der Oberfläche
     * @return Normalisierter Vektor von der Lichtquelle zum Punkt
     */
    Vector directionToPoint(const Point& p) const override {
        return (p - position_).normalized();
    }

    /**
     * Berechnet den Abstand vom Punkt zur Lichtquelle
     * @param p Der Punkt auf der Oberfläche
     * @return Abstand zur Lichtquelle
     */
    double distanceToPoint(const Point& p) const override {
        return (position_ - p).magnitude();
    }

    /**
     * Berechnet die Farbe des Lichts am Punkt p
     * @param p Der Punkt auf der Oberfläche
     * @return Die Farbe des Lichts am Punkt
     */
    Color colorAtPoint(const Point& p) const override {
        // Inverse-square attenuation: intensity falls off with distance².
        // At the light position, avoid division by zero and return full intensity.
        double distance = distanceToPoint(p);
        if (distance <= 1e-12) {
            return color_ * intensity_;
        }

        double attenuation = 1.0 / (distance * distance);
        return color_ * intensity_ * attenuation;
    }

    /**
     * Gleichheitsvergleich
     * Vergleicht Position, Farbe und Intensität
     * @param other Die zu vergleichende Lichtquelle
     * @return true wenn beide Lichtquellen gleich sind
     */
    bool operator==(const PointLightSource& other) const {
        return position_ == other.position_ &&
               color_ == other.color_ &&
               std::abs(intensity_ - other.intensity_) < 1e-6;
    }

    /**
     * Ungleichheitsvergleich
     * @param other Die zu vergleichende Lichtquelle
     * @return true wenn die Lichtquellen unterschiedlich sind
     */
    bool operator!=(const PointLightSource& other) const {
        return !(*this == other);
    }
};
