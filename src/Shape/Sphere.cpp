/**
 * Sphere.cpp
 *
 * Implementierung der Sphere-Klasse (Einheitskugel im Ursprung).
 */
#include "Sphere.h"
#include "../Intersection/Intersection.h"
#include <cmath>

/**
 * Berechnet die Schnittpunkte zwischen der Kugel und einem Strahl im lokalen Koordinatensystem
 *
 * Algorithmus:
 * Eine Kugel im Ursprung mit Radius 1 hat die Gleichung: x² + y² + z² = 1
 * Ein Strahl ist: P(t) = origin + t * direction
 *
 * Einsetzen ergibt eine quadratische Gleichung:
 * at² + bt + c = 0
 *
 * wobei:
 * a = direction · direction
 * b = 2 * (origin · direction)
 * c = origin · origin - 1
 *
 * Die Diskriminante bestimmt die Anzahl der Schnittpunkte:
 * discriminant = b² - 4ac
 *
 * - discriminant < 0: Kein Schnittpunkt
 * - discriminant = 0: Ein Schnittpunkt (Tangente)
 * - discriminant > 0: Zwei Schnittpunkte
 */
Intersections Sphere::localIntersect(const Ray& ray) const {
    // Vektor vom Kugelzentrum (0,0,0) zum Strahlursprung
    Vector sphereToRay = ray.getOrigin() - Point(0, 0, 0);

    // Koeffizienten der quadratischen Gleichung
    double a = Vector::dot(ray.getDirection(), ray.getDirection());
    double b = 2.0 * Vector::dot(ray.getDirection(), sphereToRay);
    double c = Vector::dot(sphereToRay, sphereToRay) - 1.0;

    // Diskriminante berechnen
    double discriminant = b * b - 4.0 * a * c;

    // Kein Schnittpunkt
    if (discriminant < 0.0) {
        return Intersections(); // Leere Liste
    }

    // Schnittpunkte berechnen
    double sqrtDiscriminant = std::sqrt(discriminant);
    double t1 = (-b - sqrtDiscriminant) / (2.0 * a);
    double t2 = (-b + sqrtDiscriminant) / (2.0 * a);

    // Erstelle Intersection-Objekte
    // Wichtig: this-Pointer wird als const Shape* übergeben
    Intersection i1(t1, this);
    Intersection i2(t2, this);

    // Rückgabe sortierte Liste (Intersections sortiert automatisch)
    return Intersections({i1, i2});
}

/**
 * Berechnet den Normalenvektor an einem Punkt auf der Kugeloberfläche
 *
 * Für eine Einheitskugel im Ursprung ist die Normale am Punkt P
 * gleich dem Ortsvektor von P (da die Kugel im Ursprung liegt und Radius 1 hat).
 *
 * Der Vektor wird normalisiert zurückgegeben.
 */
Vector Sphere::normalAt(const Point& point) const {
    // Für eine Einheitskugel im Ursprung: Normale = Punkt - Zentrum
    Vector normal = point - Point(0, 0, 0);

    // Normalisieren (sollte eigentlich schon Länge 1 haben, aber sicherheitshalber)
    return normal.normalized();
}
