#pragma once
#include "../Ray/Ray.h"
#include "../Vector/Vector.h"
#include "../Point/Point.h"
#include "../Matrix/Matrix.h"
#include "../Material/Material.h"
#include "../UV/UV.h"
#include <string>

// Forward declaration
class Intersections;

/**
 * Shape-Klasse (Abstrakte Basisklasse)
 *
 * Oberklasse für alle geometrischen Objekte im Ray Tracer.
 *
 * Alle geometrischen Objekte müssen folgende Funktionen implementieren:
 * - localIntersect(): Berechnung der Schnittpunkte im lokalen Koordinatensystem
 * - normalAt(): Berechnung des Normalenvektors an einem Punkt
 *
 * Standard-Eigenschaften:
 * - Objekte liegen in ihrer Standard-Größe und -Lage (z.B. Einheitskugel im Ursprung)
 * - Transformationen werden über Matrizen realisiert
 */
class Shape {
protected:
    Matrix transformation_;        // Transformationsmatrix (default: Einheitsmatrix)
    mutable Matrix* cachedInverse_; // Gecachte inverse Transformation
    mutable bool inverseCached_;   // Flag ob Inverse gecacht ist
    std::string name_;             // Name/ID des Objekts
    Material material_;            // Material des Objekts

private:
    static int nextId_;            // Statischer Zähler für automatische IDs

public:
    /**
     * Konstruktor: Initialisiert mit Einheitsmatrix und generiertem Namen
     */
    Shape()
        : transformation_(Matrix::identity(4)),
          cachedInverse_(nullptr),
          inverseCached_(false),
          name_("Shape_" + std::to_string(nextId_++)) {}

    /**
     * Konstruktor mit Namen
     * @param name Name/ID des Objekts
     */
    explicit Shape(const std::string& name)
        : transformation_(Matrix::identity(4)),
          cachedInverse_(nullptr),
          inverseCached_(false),
          name_(name) {}

    /**
     * Virtueller Destruktor für korrekte Polymorphie
     */
    virtual ~Shape() {
        if (cachedInverse_ != nullptr) {
            delete cachedInverse_;
        }
    }

    /**
     * Setzt die Transformationsmatrix des Objekts
     *
     * @param trafo Die neue Transformationsmatrix
     */
    void setTransform(const Matrix& trafo) {
        transformation_ = trafo;
        // Cache invalidieren
        if (cachedInverse_ != nullptr) {
            delete cachedInverse_;
            cachedInverse_ = nullptr;
        }
        inverseCached_ = false;
    }

    /**
     * Gibt die Transformationsmatrix zurück
     *
     * @return Transformationsmatrix
     */
    const Matrix& getTransform() const {
        return transformation_;
    }

    /**
     * Setzt den Namen des Objekts
     *
     * @param name Neuer Name
     */
    void setName(const std::string& name) {
        name_ = name;
    }

    /**
     * Gibt den Namen des Objekts zurück
     *
     * @return Name/ID
     */
    const std::string& getName() const {
        return name_;
    }

    /**
     * Setzt das Material des Objekts
     *
     * @param material Neues Material
     */
    void setMaterial(const Material& material) {
        material_ = material;
    }

    /**
     * Gibt das Material des Objekts zurück
     *
     * @return Material
     */
    Material& getMaterial() {
        return material_;
    }

    /**
     * Gibt das Material des Objekts zurück (const)
     *
     * @return Material
     */
    const Material& getMaterial() const {
        return material_;
    }

    /**
     * Berechnet die Schnittpunkte zwischen diesem Objekt und einem Strahl (Weltkoordinaten)
     *
     * Transformiert den Strahl ins lokale Koordinatensystem und ruft dann localIntersect() auf.
     *
     * @param worldRay Der Strahl in Weltkoordinaten
     * @return Intersections-Objekt mit allen Schnittpunkten (sortiert nach t-Wert)
     */
    Intersections intersect(const Ray& worldRay) const;

    /**
     * Berechnet die Schnittpunkte im lokalen Koordinatensystem
     *
     * Diese Methode muss von allen abgeleiteten Klassen implementiert werden.
     *
     * @param localRay Der Strahl im lokalen Koordinatensystem
     * @return Intersections-Objekt mit allen Schnittpunkten (sortiert nach t-Wert)
     */
    virtual Intersections localIntersect(const Ray& localRay) const = 0;

    /**
     * Berechnet den Normalenvektor an einem Punkt auf der Oberfläche (Weltkoordinaten)
     *
     * Transformiert den Punkt ins lokale Koordinatensystem, berechnet die lokale Normale
     * und transformiert diese zurück ins Weltkoordinatensystem.
     *
     * @param worldPoint Punkt auf der Oberfläche in Weltkoordinaten
     * @return Normalisierter Normalenvektor in Weltkoordinaten
     */
    Vector normalAt(const Point& worldPoint) const;

    /**
     * Berechnet UV-Koordinaten an einem Punkt auf der Oberfläche (Weltkoordinaten)
     *
     * Transformiert den Punkt ins lokale Koordinatensystem und ruft dann localUVAt() auf.
     *
     * @param worldPoint Punkt auf der Oberfläche in Weltkoordinaten
     * @return UV-Koordinaten (typisch im Bereich [0,1])
     */
    UV uvAt(const Point& worldPoint) const;

    /**
     * Berechnet den Normalenvektor im lokalen Koordinatensystem
     *
     * Diese Methode muss von allen abgeleiteten Klassen implementiert werden.
     *
     * @param localPoint Punkt auf der Oberfläche im lokalen Koordinatensystem
     * @return Normalisierter Normalenvektor im lokalen Koordinatensystem
     */
    virtual Vector localNormalAt(const Point& localPoint) const = 0;

    /**
     * Berechnet UV-Koordinaten im lokalen Koordinatensystem
     *
     * Diese Methode muss von allen abgeleiteten Klassen implementiert werden.
     *
     * @param localPoint Punkt auf der Oberfläche im lokalen Koordinatensystem
     * @return UV-Koordinaten
     */
    virtual UV localUVAt(const Point& localPoint) const = 0;

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
