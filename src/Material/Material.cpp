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
                              const Vector& normalVector,
                              bool inShadow) const {
    // Lichtfarbe am Punkt (berücksichtigt Intensität und ggf. Attenuation)
    Color lightColorAtPoint = light->colorAtPoint(point);

    // Effektive Farbe: Objektfarbe * Lichtfarbe am Punkt
    Color effectiveColor = color * lightColorAtPoint;

    // 1. Ambiente Komponente (immer vorhanden, auch im Schatten)
    Color ambientContribution = effectiveColor * ambient;

    // Wenn der Punkt im Schatten liegt, gibt es nur die ambiente Komponente
    if (inShadow) {
        return ambientContribution;
    }

    // Vektor zur Lichtquelle (normalisiert)
    Vector lightVector = light->directionFromPoint(point);

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
            specularContribution = lightColorAtPoint * specular * factor;
        }
    }

    // Alle Komponenten addieren
    return ambientContribution + diffuseContribution + specularContribution;
}

/**
 * Berechnet die Beleuchtung nach dem Blinn-Phong-Modell
 *
 * Blinn-Phong verwendet die Winkelhalbierende (halfway vector) h
 * anstatt des reflektierten Vektors. Das macht die Berechnung etwas schneller.
 *
 * h = normalize(l + v)
 * Specularer Anteil: (n · h)^(4*shininess)
 */
Color Material::blinnPhongLighting(const LightSource* light,
                                   const Point& point,
                                   const Vector& eyeVector,
                                   const Vector& normalVector,
                                   bool inShadow) const {
    // Lichtfarbe am Punkt (berücksichtigt Intensität und ggf. Attenuation)
    Color lightColorAtPoint = light->colorAtPoint(point);

    // Effektive Farbe: Objektfarbe * Lichtfarbe am Punkt
    Color effectiveColor = color * lightColorAtPoint;

    // 1. Ambiente Komponente (immer vorhanden, auch im Schatten)
    Color ambientContribution = effectiveColor * ambient;

    // Wenn der Punkt im Schatten liegt, gibt es nur die ambiente Komponente
    if (inShadow) {
        return ambientContribution;
    }

    // Vektor zur Lichtquelle (normalisiert)
    Vector lightVector = light->directionFromPoint(point);

    // Cosinus des Winkels zwischen Normale und Lichtvektor
    double lightDotNormal = Vector::dot(lightVector, normalVector);

    Color diffuseContribution = Color(0, 0, 0);
    Color specularContribution = Color(0, 0, 0);

    // Wenn Licht von der Rückseite kommt (lightDotNormal < 0),
    // gibt es keine diffuse und spekulare Reflexion
    if (lightDotNormal >= 0) {
        // 2. Diffuse Komponente (identisch zu Phong)
        diffuseContribution = effectiveColor * diffuse * lightDotNormal;

        // Winkelhalbierende zwischen Lichtvektor und Eye-Vektor
        Vector halfwayVector = (lightVector + eyeVector).normalized();

        // Cosinus des Winkels zwischen Normale und Halfway-Vektor
        double halfwayDotNormal = Vector::dot(halfwayVector, normalVector);

        // Spekulare Komponente nur wenn halfwayDotNormal > 0
        if (halfwayDotNormal > 0) {
            // 3. Spekulare Komponente (Blinn-Phong)
            // Shininess wird mit 4 multipliziert, da der Winkel etwa halb so groß ist
            double factor = std::pow(halfwayDotNormal, shininess * 4.0);
            // Spekulare Reflexion verwendet nur die Lichtfarbe, nicht die Objektfarbe
            specularContribution = lightColorAtPoint * specular * factor;
        }
    }

    // Alle Komponenten addieren
    return ambientContribution + diffuseContribution + specularContribution;
}
