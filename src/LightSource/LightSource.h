#pragma once
#include "../Color/Color.h"

/**
 * LightSource-Klasse (Abstrakte Basisklasse)
 *
 * Basisklasse für alle Lichtquellen im Ray Tracer.
 *
 * Alle Lichtquellen haben:
 * - Eine Farbe (color)
 * - Eine Intensität (intensity)
 *
 * Unterschiedliche Lichtquellen-Typen können zusätzliche Eigenschaften haben
 * (z.B. Position für Punktlichtquellen, Richtung für gerichtete Lichtquellen).
 */
class LightSource {
protected:
    Color color_;      // Farbe des Lichts
    double intensity_; // Intensität des Lichts (normalerweise 1.0)

public:
    /**
     * Konstruktor
     * @param color Farbe des Lichts (Standard: Weiß)
     * @param intensity Intensität des Lichts (Standard: 1.0)
     */
    LightSource(const Color& color = Color(1, 1, 1), double intensity = 1.0)
        : color_(color), intensity_(intensity) {}

    /**
     * Virtueller Destruktor für korrekte Polymorphie
     */
    virtual ~LightSource() = default;

    /**
     * Gibt die Farbe der Lichtquelle zurück
     * @return Farbe
     */
    Color getColor() const { return color_; }

    /**
     * Gibt die Intensität der Lichtquelle zurück
     * @return Intensität
     */
    double getIntensity() const { return intensity_; }

    /**
     * Setzt die Farbe der Lichtquelle
     * @param color Neue Farbe
     */
    void setColor(const Color& color) { color_ = color; }

    /**
     * Setzt die Intensität der Lichtquelle
     * @param intensity Neue Intensität
     */
    void setIntensity(double intensity) { intensity_ = intensity; }
};
