#pragma once
#include "../Point/Point.h"
#include "../Vector/Vector.h"

// Forward declaration
class Matrix;

/**
 * Ray-Klasse
 * 
 * Repräsentiert einen Strahl im 3D-Raum.
 * Ein Strahl hat einen Ursprungspunkt O und eine Richtung d.
 * 
 * Alle Punkte auf dem Strahl können durch die Parameterform beschrieben werden:
 * P(t) = O + t * d
 * 
 * wobei t ein Parameter ist:
 * - t = 0: Ursprungspunkt
 * - t > 0: Punkte in Strahlrichtung
 * - t < 0: Punkte entgegen der Strahlrichtung
 */
class Ray {
private:
    Point origin;      // Ursprungspunkt des Strahls
    Vector direction;  // Richtungsvektor des Strahls

public:
    /**
     * Konstruktor: Strahl aus Ursprung und Richtung
     * 
     * Der Richtungsvektor wird so übernommen wie er angegeben wird.
     * 
     * @param origin Ursprungspunkt
     * @param direction Richtungsvektor
     */
    Ray(const Point& origin, const Vector& direction);

    /**
     * Konstruktor: Strahl aus Ursprung und Zielpunkt
     * 
     * Der Richtungsvektor wird aus den beiden Punkten berechnet
     * und automatisch normalisiert.
     * 
     * @param origin Ursprungspunkt
     * @param target Zielpunkt
     */
    static Ray fromPoints(const Point& origin, const Point& target);

    /**
     * Gibt den Ursprungspunkt zurück
     * @return Ursprungspunkt des Strahls
     */
    Point getOrigin() const { return origin; }

    /**
     * Gibt den Richtungsvektor zurück
     * @return Richtungsvektor des Strahls
     */
    Vector getDirection() const { return direction; }

    /**
     * Berechnet einen Punkt auf dem Strahl für gegebenen Parameter
     * 
     * P(t) = origin + t * direction
     * 
     * @param t Parameter (Abstand vom Ursprung in Richtungseinheiten)
     * @return Punkt auf dem Strahl
     */
    Point pointAt(double t) const;

    /**
     * Gleichheitsvergleich
     * @param r Der zu vergleichende Strahl
     * @return true wenn Ursprung und Richtung gleich sind
     */
    bool operator==(const Ray& r) const {
        return origin == r.origin && direction == r.direction;
    }

    /**
     * Ungleichheitsvergleich
     * @param r Der zu vergleichende Strahl
     * @return true wenn Ursprung oder Richtung unterschiedlich sind
     */
    bool operator!=(const Ray& r) const {
        return !(*this == r);
    }

    /**
     * Transformiert den Strahl mit einer Transformationsmatrix
     *
     * Multipliziert Ursprung und Richtung mit der gegebenen Matrix.
     * WICHTIG: Der Richtungsvektor wird NICHT normalisiert!
     * Bei Skalierungen wird der Strahl gestreckt oder gestaucht.
     *
     * @param trafo Transformationsmatrix
     * @return Transformierter Strahl
     */
    Ray transform(const Matrix& trafo) const;
};
