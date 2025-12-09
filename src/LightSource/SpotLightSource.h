#pragma once
#include "LightSource.h"
#include "../Point/Point.h"
#include "../Vector/Vector.h"
#include <cmath>
#include <algorithm>

/**
 * SpotLightSource-Klasse
 *
 * Repräsentiert ein Spotlight (Scheinwerfer), das einen Lichtkegel erzeugt.
 * Das Spotlight hat eine Position, eine Richtung und zwei Winkel:
 * - totalAngleDegrees: Der totale Öffnungswinkel des Lichtkegels
 * - fallOffStartDegrees: Der Winkel, ab dem die Intensität abnimmt
 *
 * Eigenschaften:
 * - Position im Raum
 * - Richtung des Lichtkegels
 * - Öffnungswinkel
 * - FallOff-Startwinkel
 * - Farbe des Lichts
 * - Intensität des Lichts
 *
 * Die Intensität nimmt vom Zentrum zum Rand des Lichtkegels ab.
 */
class SpotLightSource : public LightSource {
private:
    Point position_;              // Position der Lichtquelle
    Vector direction_;            // Richtung des Lichtkegels (normalisiert)
    double totalAngleDegrees_;    // Totaler Öffnungswinkel in Grad
    double fallOffStartDegrees_;  // Startwinkel für FallOff in Grad

public:
    /**
     * Konstruktor
     * @param position Position der Lichtquelle
     * @param direction Richtung des Lichtkegels (wird automatisch normalisiert)
     * @param totalAngleDegrees Totaler Öffnungswinkel in Grad
     * @param fallOffStartDegrees Startwinkel für FallOff in Grad
     * @param color Farbe des Lichts (Standard: Weiß)
     * @param intensity Intensität des Lichts (Standard: 1.0)
     */
    SpotLightSource(const Point& position,
                   const Vector& direction,
                   double totalAngleDegrees,
                   double fallOffStartDegrees,
                   const Color& color = Color(1, 1, 1),
                   double intensity = 1.0)
        : LightSource(color, intensity),
          position_(position),
          direction_(direction.normalized()),
          totalAngleDegrees_(totalAngleDegrees),
          fallOffStartDegrees_(fallOffStartDegrees) {}

    /**
     * Destruktor
     */
    ~SpotLightSource() override = default;

    /**
     * Gibt die Position der Lichtquelle zurück
     * @return Position
     */
    Point getPosition() const { return position_; }

    /**
     * Gibt die Richtung der Lichtquelle zurück
     * @return Richtung (normalisiert)
     */
    Vector getDirection() const { return direction_; }

    /**
     * Gibt den totalen Öffnungswinkel zurück
     * @return Öffnungswinkel in Grad
     */
    double getTotalAngle() const { return totalAngleDegrees_; }

    /**
     * Gibt den FallOff-Startwinkel zurück
     * @return FallOff-Startwinkel in Grad
     */
    double getFallOffStart() const { return fallOffStartDegrees_; }

    /**
     * Spotlights sind nicht gerichtet (verhalten sich wie Punktlichtquellen)
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
     * Berücksichtigt den Lichtkegel und FallOff
     * @param p Der Punkt auf der Oberfläche
     * @return Die Farbe des Lichts am Punkt
     */
    Color colorAtPoint(const Point& p) const override {
        // Vektor von Lichtquelle zum Punkt
        Vector toPoint = directionToPoint(p);

        // Winkel zwischen Spotlight-Richtung und Vektor zum Punkt
        double cosAngle = Vector::dot(direction_, toPoint);
        double angleDegrees = std::acos(cosAngle) * 180.0 / M_PI;

        // Halbe Winkel für Vergleiche
        double halfTotalAngle = totalAngleDegrees_ / 2.0;
        double halfFallOffStart = fallOffStartDegrees_ / 2.0;

        // Außerhalb des Lichtkegels?
        if (angleDegrees > halfTotalAngle) {
            return Color(0, 0, 0); // Schwarz (keine Beleuchtung)
        }

        // Im zentralen, voll beleuchteten Bereich?
        if (angleDegrees < halfFallOffStart) {
            return color_ * intensity_;
        }

        // Im FallOff-Bereich: Intensität nimmt ab
        // Berechne den Anteil im FallOff-Bereich (0 = am Rand, 1 = am Anfang)
        double fallOffRange = halfTotalAngle - halfFallOffStart;
        double distanceFromFallOffStart = angleDegrees - halfFallOffStart;
        double factor = 1.0 - (distanceFromFallOffStart / fallOffRange);

        // Quadriere den Faktor für einen realistischeren Effekt
        factor = factor * factor;

        return color_ * intensity_ * factor;
    }

    /**
     * Gleichheitsvergleich
     * @param other Die zu vergleichende Lichtquelle
     * @return true wenn beide Lichtquellen gleich sind
     */
    bool operator==(const SpotLightSource& other) const {
        return position_ == other.position_ &&
               direction_ == other.direction_ &&
               std::abs(totalAngleDegrees_ - other.totalAngleDegrees_) < 1e-6 &&
               std::abs(fallOffStartDegrees_ - other.fallOffStartDegrees_) < 1e-6 &&
               color_ == other.color_ &&
               std::abs(intensity_ - other.intensity_) < 1e-6;
    }

    /**
     * Ungleichheitsvergleich
     * @param other Die zu vergleichende Lichtquelle
     * @return true wenn die Lichtquellen unterschiedlich sind
     */
    bool operator!=(const SpotLightSource& other) const {
        return !(*this == other);
    }
};
