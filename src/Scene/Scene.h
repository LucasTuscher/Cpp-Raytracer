#pragma once
#include "../Shape/Shape.h"
#include "../Ray/Ray.h"
#include "../Intersection/Intersections.h"
#include <map>
#include <vector>
#include <memory>
#include <string>

/**
 * Scene-Klasse
 *
 * Repräsentiert eine vollständige 3D-Szene mit allen enthaltenen Objekten.
 *
 * Features:
 * - Verwaltung von geometrischen Objekten (Shapes)
 * - Objekte sind über Namen/IDs zugreifbar
 * - Schnittpunktberechnung mit allen Szenenobjekten
 * - Später: Lichtquellen und Materialien
 */
class Scene {
private:
    // Map für schnellen Zugriff auf Objekte per Name
    std::map<std::string, Shape*> objectsByName_;

    // Liste aller Objekte für Iteration
    std::vector<Shape*> objects_;

public:
    /**
     * Konstruktor: Erstellt eine leere Szene
     */
    Scene();

    /**
     * Destruktor: Gibt Speicher frei (wenn nötig)
     */
    ~Scene();

    /**
     * Fügt ein Objekt zur Szene hinzu
     *
     * Das Objekt wird sowohl in der Map als auch in der Liste gespeichert.
     * Die Szene übernimmt KEINE Besitzrechte am Objekt.
     *
     * @param shape Zeiger auf das hinzuzufügende Objekt
     */
    void addObject(Shape* shape);

    /**
     * Prüft, ob ein Objekt mit dem gegebenen Namen existiert
     *
     * @param name Name des zu suchenden Objekts
     * @return true wenn ein Objekt mit diesem Namen existiert
     */
    bool contains(const std::string& name) const;

    /**
     * Prüft, ob ein bestimmtes Objekt in der Szene enthalten ist
     *
     * @param shape Zeiger auf das zu suchende Objekt
     * @return true wenn das Objekt in der Szene enthalten ist
     */
    bool contains(const Shape* shape) const;

    /**
     * Gibt ein Objekt per Name zurück
     *
     * @param name Name des Objekts
     * @return Zeiger auf das Objekt oder nullptr wenn nicht gefunden
     */
    Shape* getObject(const std::string& name) const;

    /**
     * Gibt die Anzahl der Objekte in der Szene zurück
     *
     * @return Anzahl der Objekte
     */
    size_t getObjectCount() const;

    /**
     * Gibt die Liste aller Objekte zurück
     *
     * @return Referenz auf den Vektor aller Objekte
     */
    const std::vector<Shape*>& getObjects() const;

    /**
     * Berechnet alle Schnittpunkte eines Strahls mit der Szene
     *
     * Durchläuft alle Objekte und sammelt ihre Schnittpunkte.
     * Das Ergebnis ist nach t-Werten sortiert.
     *
     * @param ray Der Strahl in Weltkoordinaten
     * @return Intersections-Objekt mit allen Schnittpunkten (sortiert)
     */
    Intersections traceRay(const Ray& ray) const;

    /**
     * Erstellt eine Standard-Testszene
     *
     * Die Testszene enthält:
     * - Eine nicht transformierte Einheitskugel
     * - Eine Kugel im Ursprung, die um die Hälfte skaliert ist
     *
     * WICHTIG: Der Aufrufer ist für das Löschen der Szene
     * und aller Objekte verantwortlich!
     *
     * @return Zeiger auf die erstellte Testszene
     */
    static Scene* defaultScene();
};
