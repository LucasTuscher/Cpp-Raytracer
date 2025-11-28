#pragma once
#include "Shape.h"
#include "../Intersection/Intersections.h"

/**
 * Sphere-Klasse
 *
 * Repräsentiert eine Einheitskugel im Koordinatenursprung.
 *
 * Standard-Eigenschaften:
 * - Mittelpunkt: (0, 0, 0)
 * - Radius: 1
 *
 * Die Kugel liegt immer im Ursprung mit Radius 1.
 * Transformationen werden später über Matrizen realisiert.
 */
class Sphere : public Shape {
public:
    /**
     * Konstruktor: Erstellt eine Einheitskugel im Ursprung
     */
    Sphere() : Shape() {}

    /**
     * Konstruktor mit Namen
     * @param name Name/ID der Kugel
     */
    explicit Sphere(const std::string& name) : Shape(name) {}

    /**
     * Destruktor
     */
    ~Sphere() override = default;

    /**
     * Berechnet die Schnittpunkte zwischen der Kugel und einem Strahl im lokalen Koordinatensystem
     *
     * Mögliche Fälle:
     * - Kein Schnittpunkt: Leere Liste
     * - Tangente: Zwei identische Schnittpunkte
     * - Zwei Schnittpunkte: Strahl durchquert die Kugel
     * - Negative t-Werte: Schnittpunkte liegen hinter dem Strahlursprung
     *
     * @param localRay Der Strahl im lokalen Koordinatensystem
     * @return Intersections-Objekt mit allen Schnittpunkten (sortiert)
     */
    Intersections localIntersect(const Ray& localRay) const override;

    /**
     * Berechnet den Normalenvektor an einem Punkt auf der Kugeloberfläche (lokal)
     *
     * Für eine Einheitskugel im Ursprung ist die Normale am Punkt P
     * einfach der normalisierte Ortsvektor von P.
     *
     * @param localPoint Punkt auf der Kugeloberfläche im lokalen Koordinatensystem
     * @return Normalisierter Normalenvektor
     */
    Vector localNormalAt(const Point& localPoint) const override;
};
