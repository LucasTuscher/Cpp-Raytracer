#pragma once

// Forward declaration
class Shape;

/**
 * Intersection-Klasse
 *
 * Speichert die Daten eines Schnittpunktes zwischen einem Strahl und einem Objekt.
 *
 * Ein Schnittpunkt enthält:
 * - t: Der Parameterwert entlang des Strahls (P(t) = origin + t * direction)
 * - shape: Verweis auf das geschnittene Objekt
 */
class Intersection {
public:
    double t;           // Parameterwert entlang des Strahls
    const Shape* shape; // Verweis auf das geschnittene Objekt

    /**
     * Konstruktor
     * @param t Parameterwert entlang des Strahls
     * @param shape Verweis auf das geschnittene Objekt
     */
    Intersection(double t, const Shape* shape);

    /**
     * Gleichheitsvergleich
     * @param other Die zu vergleichende Intersection
     * @return true wenn t-Wert und Shape übereinstimmen
     */
    bool operator==(const Intersection& other) const;

    /**
     * Ungleichheitsvergleich
     * @param other Die zu vergleichende Intersection
     * @return true wenn t-Wert oder Shape unterschiedlich sind
     */
    bool operator!=(const Intersection& other) const;

    /**
     * Kleiner-Vergleich (für Sortierung nach t-Wert)
     * @param other Die zu vergleichende Intersection
     * @return true wenn dieser t-Wert kleiner ist
     */
    bool operator<(const Intersection& other) const;

    /**
     * Größer-Vergleich
     * @param other Die zu vergleichende Intersection
     * @return true wenn dieser t-Wert größer ist
     */
    bool operator>(const Intersection& other) const;

    /**
     * Kleiner-gleich-Vergleich
     * @param other Die zu vergleichende Intersection
     * @return true wenn dieser t-Wert kleiner oder gleich ist
     */
    bool operator<=(const Intersection& other) const;

    /**
     * Größer-gleich-Vergleich
     * @param other Die zu vergleichende Intersection
     * @return true wenn dieser t-Wert größer oder gleich ist
     */
    bool operator>=(const Intersection& other) const;
};
