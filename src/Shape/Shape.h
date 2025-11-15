#pragma once
#include "../Ray/Ray.h"
#include "../Vector/Vector.h"
#include "../Point/Point.h"

// Forward declaration
class Intersections;

/**
 * Shape-Klasse (Abstrakte Basisklasse)
 *
 * Oberklasse für alle geometrischen Objekte im Ray Tracer.
 *
 * Alle geometrischen Objekte müssen folgende Funktionen implementieren:
 * - intersect(): Berechnung der Schnittpunkte mit einem Strahl
 * - normalAt(): Berechnung des Normalenvektors an einem Punkt
 *
 * Standard-Eigenschaften:
 * - Objekte liegen in ihrer Standard-Größe und -Lage (z.B. Einheitskugel im Ursprung)
 * - Transformationen werden später über Matrizen realisiert
 */
class Shape {
public:
    /**
     * Virtueller Destruktor für korrekte Polymorphie
     */
    virtual ~Shape() = default;

    /**
     * Berechnet die Schnittpunkte zwischen diesem Objekt und einem Strahl
     *
     * Diese Methode muss von allen abgeleiteten Klassen implementiert werden.
     *
     * @param ray Der Strahl, mit dem geschnitten werden soll
     * @return Intersections-Objekt mit allen Schnittpunkten (sortiert nach t-Wert)
     */
    virtual Intersections intersect(const Ray& ray) const = 0;

    /**
     * Berechnet den Normalenvektor an einem Punkt auf der Oberfläche
     *
     * Diese Methode muss von allen abgeleiteten Klassen implementiert werden.
     * Es wird davon ausgegangen, dass der Punkt auf der Oberfläche liegt.
     *
     * @param point Punkt auf der Oberfläche des Objekts
     * @return Normalisierter Normalenvektor an diesem Punkt
     */
    virtual Vector normalAt(const Point& point) const = 0;

    /**
     * Gleichheitsvergleich (für Tests)
     * Vergleicht die Speicheradresse (Objekt-Identität)
     * @param other Das zu vergleichende Shape
     * @return true wenn es sich um dasselbe Objekt handelt
     */
    bool operator==(const Shape& other) const {
        return this == &other;
    }

    /**
     * Ungleichheitsvergleich
     * @param other Das zu vergleichende Shape
     * @return true wenn es sich um unterschiedliche Objekte handelt
     */
    bool operator!=(const Shape& other) const {
        return !(*this == other);
    }
};
