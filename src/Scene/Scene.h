#pragma once
#include "../Shape/Shape.h"
#include "../Ray/Ray.h"
#include "../Intersection/Intersections.h"
#include "../Intersection/Intersection.h"
#include "../LightSource/LightSource.h"
#include "../LightSource/PointLightSource.h"
#include "../HitInfo/HitInfo.h"
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

    // Liste aller Lichtquellen
    std::vector<LightSource*> lights_;

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
     * Fügt eine Lichtquelle zur Szene hinzu
     *
     * Die Szene übernimmt KEINE Besitzrechte an der Lichtquelle.
     *
     * @param light Zeiger auf die hinzuzufügende Lichtquelle
     */
    void addLight(LightSource* light);

    /**
     * Prüft, ob eine bestimmte Lichtquelle in der Szene enthalten ist
     *
     * @param light Zeiger auf die zu suchende Lichtquelle
     * @return true wenn die Lichtquelle in der Szene enthalten ist
     */
    bool containsLight(const LightSource* light) const;

    /**
     * Gibt die Anzahl der Lichtquellen in der Szene zurück
     *
     * @return Anzahl der Lichtquellen
     */
    size_t getLightCount() const;

    /**
     * Gibt die Liste aller Lichtquellen zurück
     *
     * @return Referenz auf den Vektor aller Lichtquellen
     */
    const std::vector<LightSource*>& getLights() const;

    /**
     * Gibt die erste Lichtquelle zurück (für Szenen mit nur einer Lichtquelle)
     *
     * @return Zeiger auf die erste Lichtquelle oder nullptr wenn keine vorhanden
     */
    LightSource* getFirstLight() const;

    /**
     * Berechnet HitInfo aus einem Intersection und einem Ray
     *
     * Sammelt alle Informationen, die für die Beleuchtungsberechnung benötigt werden:
     * - t-Wert und geschnittenes Objekt (aus Intersection)
     * - Schnittpunkt (berechnet aus Ray und t)
     * - Eye-Vektor (negierte Ray-Richtung)
     * - Normale (berechnet am Schnittpunkt)
     * - UV-Koordinaten (für Textur-Sampling)
     *
     * @param intersection Die Intersection, die den Schnittpunkt beschreibt
     * @param ray Der Strahl, der das Objekt trifft
     * @return HitInfo mit allen berechneten Werten
     */
    static HitInfo computeHitInfo(const Intersection& intersection, const Ray& ray);

    /**
     * Berechnet die Farbe an einem Schnittpunkt mit Beleuchtung
     *
     * Verwendet das Phong-Beleuchtungsmodell mit allen Lichtquellen der Szene.
     *
     * @param hitInfo Informationen über den Schnittpunkt
     * @param depth Rekursionstiefe für Reflexionen (Standard: 5)
     * @return Die berechnete Farbe
     */
    Color shadeHit(const HitInfo& hitInfo, int depth = 5) const;

    /**
     * Berechnet die Farbe für einen Strahl
     *
     * Kombiniert Strahlverfolgung und Beleuchtungsberechnung:
     * 1. Findet den nächsten Schnittpunkt mit traceRay()
     * 2. Berechnet HitInfo mit computeHitInfo()
     * 3. Berechnet Farbe mit shadeHit()
     *
     * @param ray Der Strahl in Weltkoordinaten
     * @param depth Rekursionstiefe für Reflexionen (Standard: 5)
     * @return Die berechnete Farbe (schwarz wenn kein Schnittpunkt)
     */
    Color colorAt(const Ray& ray, int depth = 5) const;

    /**
     * Berechnet die reflektierte Farbe an einem Schnittpunkt
     *
     * Verfolgt einen reflektierten Strahl und berechnet dessen Farbwert.
     * Die Reflexivität des Materials bestimmt, wie stark der Beitrag ist.
     *
     * @param hitInfo Informationen über den Schnittpunkt
     * @param depth Verbleibende Rekursionstiefe
     * @return Die reflektierte Farbe (gewichtet mit Material-Reflexivität)
     */
    Color reflectedColor(const HitInfo& hitInfo, int depth) const;

    /**
     * Prüft, ob ein Punkt im Schatten einer Lichtquelle liegt.
     *
     * Sendet einen Schattenstrahl vom Punkt zur Lichtquelle und prüft, ob ein
     * Objekt davor liegt.
     */
    bool isInShadow(const Point& point, const LightSource* light) const;

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
