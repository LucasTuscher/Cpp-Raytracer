#pragma once
#include "LightSource.h"
#include "../Point/Point.h"

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
