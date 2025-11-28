/**
 * Material.cpp
 *
 * Implementierung der Material-Klasse und Beleuchtungsberechnung.
 */
#include "Material.h"
#include <cmath>
#include <algorithm>

/**
 * Berechnet die Beleuchtung nach dem Phong-Modell
 *
 * Phong-Modell: I = ka*O + IL*O*kd*(n·l) + IL*ks*(v·r)^shininess
 *
 * wobei:
 * - ka: ambienter Reflexionskoeffizient
 * - kd: diffuser Reflexionskoeffizient
 * - ks: spekularer Reflexionskoeffizient
 * - O: Objektfarbe (material.color)
 * - IL: Lichtfarbe und -intensität
 * - n: Normalenvektor
 * - l: Vektor zur Lichtquelle
 * - v: Vektor zum Betrachter (eyeVector)
 * - r: Reflektierter Vektor von l an n
 */
Color Material::phongLighting(const LightSource* light,
                              const Point& point,
                              const Vector& eyeVector,
                              const Vector& normalVector) const {
    // Für Punktlichtquellen müssen wir die Position kennen
    const PointLightSource* pointLight = dynamic_cast<const PointLightSource*>(light);
    if (pointLight == nullptr) {
        // Wenn keine Punktlichtquelle, gib nur ambiente Beleuchtung zurück
        return color * ambient;
    }

    // Effektive Farbe: Objektfarbe * Lichtfarbe * Lichtintensität
    Color effectiveColor = color * light->getColor() * light->getIntensity();

    // Vektor zur Lichtquelle (normalisiert)
    Vector lightVector = (pointLight->getPosition() - point).normalized();

    // 1. Ambiente Komponente
    Color ambientContribution = effectiveColor * ambient;

    // Cosinus des Winkels zwischen Normale und Lichtvektor
    double lightDotNormal = Vector::dot(lightVector, normalVector);

    Color diffuseContribution = Color(0, 0, 0);
    Color specularContribution = Color(0, 0, 0);

    // Wenn Licht von der Rückseite kommt (lightDotNormal < 0),
    // gibt es keine diffuse und spekulare Reflexion
    if (lightDotNormal >= 0) {
        // 2. Diffuse Komponente
        diffuseContribution = effectiveColor * diffuse * lightDotNormal;

        // Reflektierter Vektor (Lichtvektor an Normale reflektiert)
        Vector reflectVector = Vector::reflect(-lightVector, normalVector);

        // Cosinus des Winkels zwischen reflektiertem Vektor und Augvektor
        double reflectDotEye = Vector::dot(reflectVector, eyeVector);

        // Wenn reflectDotEye < 0, liegt der Betrachter auf der "falschen" Seite
        // und sieht kein spekulares Highlight
        if (reflectDotEye > 0) {
            // 3. Spekulare Komponente
            double factor = std::pow(reflectDotEye, shininess);
            // Spekulare Reflexion verwendet nur die Lichtfarbe, nicht die Objektfarbe
            Color lightColor = light->getColor() * light->getIntensity();
            specularContribution = lightColor * specular * factor;
        }
    }

    // Alle Komponenten addieren
    return ambientContribution + diffuseContribution + specularContribution;
}
