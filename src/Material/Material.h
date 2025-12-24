#pragma once
#include "../Color/Color.h"
#include "../Point/Point.h"
#include "../Vector/Vector.h"
#include "../LightSource/LightSource.h"
#include "../LightSource/PointLightSource.h"
#include <memory>

// Forward declaration
class Texture;

/**
 * Material-Klasse
 *
 * Beschreibt die Oberflächeneigenschaften eines Objekts für die Beleuchtungsberechnung.
 *
 * Eigenschaften:
 * - color: Farbe der Oberfläche
 * - ambient: Ambienter Reflexionskoeffizient (Grundhelligkeit)
 * - diffuse: Diffuser Reflexionskoeffizient (matte Reflexion)
 * - specular: Spekularer Reflexionskoeffizient (glänzende Reflexion)
 * - shininess: Glanz (höhere Werte = kleinere Highlights)
 *
 * Diese Eigenschaften werden im Phong-Beleuchtungsmodell verwendet.
 */
class Material {
public:
    Color color;      // Farbe der Oberfläche
    std::shared_ptr<const Texture> texture; // Optional: Textur (überschreibt/ergänzt Farbe)
    double ambient;   // Ambienter Reflexionskoeffizient (0.0 - 1.0)
    double diffuse;   // Diffuser Reflexionskoeffizient (0.0 - 1.0)
    double specular;  // Spekularer Reflexionskoeffizient (0.0 - 1.0)
    double shininess; // Glanz (typisch: 10 - 200)
    double reflectivity; // Reflexivität (0.0 = keine Reflexion, 1.0 = perfekter Spiegel)

    /**
     * Standard-Konstruktor
     * Erstellt ein Material mit Standard-Werten
     */
    Material()
        : color(Color(1.0, 1.0, 1.0)),
          texture(nullptr),
          ambient(0.1),
          diffuse(0.9),
          specular(0.9),
          shininess(200.0),
          reflectivity(0.0) {}

    /**
     * Konstruktor mit allen Parametern
     */
    Material(const Color& color, double ambient, double diffuse, double specular, double shininess, double reflectivity = 0.0)
        : color(color),
          texture(nullptr),
          ambient(ambient),
          diffuse(diffuse),
          specular(specular),
          shininess(shininess),
          reflectivity(reflectivity) {}

    /**
     * Gleichheitsvergleich
     * @param other Das zu vergleichende Material
     * @return true wenn beide Materialien gleich sind
     */
    bool operator==(const Material& other) const {
        return color == other.color &&
               texture == other.texture &&
               std::abs(ambient - other.ambient) < 1e-6 &&
               std::abs(diffuse - other.diffuse) < 1e-6 &&
               std::abs(specular - other.specular) < 1e-6 &&
               std::abs(shininess - other.shininess) < 1e-6 &&
               std::abs(reflectivity - other.reflectivity) < 1e-6;
    }

    /**
     * Ungleichheitsvergleich
     * @param other Das zu vergleichende Material
     * @return true wenn die Materialien unterschiedlich sind
     */
    bool operator!=(const Material& other) const {
        return !(*this == other);
    }

    /**
     * Berechnet die Beleuchtung nach dem Phong-Modell
     *
     * Das Phong-Beleuchtungsmodell besteht aus drei Komponenten:
     * 1. Ambient: Grundhelligkeit (unabhängig von Lichtquellen)
     * 2. Diffuse: Matte Reflexion (abhängig vom Einfallswinkel)
     * 3. Specular: Glänzende Reflexion (Highlights)
     *
     * @param light Die Lichtquelle
     * @param point Der Punkt auf der Oberfläche
     * @param eyeVector Der Vektor vom Punkt zum Betrachter (normalisiert)
     * @param normalVector Der Normalenvektor am Punkt (normalisiert)
     * @param inShadow true wenn der Punkt im Schatten liegt
     * @return Die berechnete Farbe
     */
    Color phongLighting(const LightSource* light,
                       const Point& point,
                       const Vector& eyeVector,
                       const Vector& normalVector,
                       bool inShadow = false,
                       const Color* surfaceColor = nullptr) const;

    /**
     * Berechnet die Beleuchtung nach dem Blinn-Phong-Modell
     *
     * Das Blinn-Phong-Modell ist eine Approximation des Phong-Modells,
     * die etwas schneller ist. Anstatt den reflektierten Vektor zu berechnen,
     * wird die Winkelhalbierende zwischen Lichtvektor und Eye-Vektor verwendet.
     *
     * @param light Die Lichtquelle
     * @param point Der Punkt auf der Oberfläche
     * @param eyeVector Der Vektor vom Punkt zum Betrachter (normalisiert)
     * @param normalVector Der Normalenvektor am Punkt (normalisiert)
     * @param inShadow true wenn der Punkt im Schatten liegt
     * @return Die berechnete Farbe
     */
    Color blinnPhongLighting(const LightSource* light,
                            const Point& point,
                            const Vector& eyeVector,
                            const Vector& normalVector,
                            bool inShadow = false,
                            const Color* surfaceColor = nullptr) const;
};
