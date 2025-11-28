/**
 * Scene.cpp
 *
 * Implementierung der Scene-Klasse zur Verwaltung von 3D-Szenen.
 */
#include "Scene.h"
#include "../Shape/Sphere.h"
#include "../Matrix/Matrix.h"
#include "../Color/Color.h"
#include <algorithm>

/**
 * Konstruktor
 */
Scene::Scene() {
}

/**
 * Destruktor
 */
Scene::~Scene() {
    // Hinweis: Die Szene löscht NICHT die Objekte, da sie nur Zeiger speichert
    // Der Aufrufer ist für das Speichermanagement verantwortlich
}

/**
 * Fügt ein Objekt zur Szene hinzu
 */
void Scene::addObject(Shape* shape) {
    if (shape == nullptr) {
        return;
    }

    // In Liste hinzufügen
    objects_.push_back(shape);

    // In Map hinzufügen
    objectsByName_[shape->getName()] = shape;
}

/**
 * Prüft, ob ein Objekt mit dem gegebenen Namen existiert
 */
bool Scene::contains(const std::string& name) const {
    return objectsByName_.find(name) != objectsByName_.end();
}

/**
 * Prüft, ob ein bestimmtes Objekt in der Szene enthalten ist
 */
bool Scene::contains(const Shape* shape) const {
    return std::find(objects_.begin(), objects_.end(), shape) != objects_.end();
}

/**
 * Gibt ein Objekt per Name zurück
 */
Shape* Scene::getObject(const std::string& name) const {
    auto it = objectsByName_.find(name);
    if (it != objectsByName_.end()) {
        return it->second;
    }
    return nullptr;
}

/**
 * Gibt die Anzahl der Objekte zurück
 */
size_t Scene::getObjectCount() const {
    return objects_.size();
}

/**
 * Gibt die Liste aller Objekte zurück
 */
const std::vector<Shape*>& Scene::getObjects() const {
    return objects_;
}

/**
 * Berechnet alle Schnittpunkte eines Strahls mit der Szene
 */
Intersections Scene::traceRay(const Ray& ray) const {
    // Sammle alle Schnittpunkte aller Objekte
    std::vector<Intersection> allIntersections;

    for (const auto& shape : objects_) {
        // Berechne Schnittpunkte mit diesem Objekt
        Intersections shapeIntersections = shape->intersect(ray);

        // Füge alle Schnittpunkte zur Gesamtliste hinzu
        const auto& intersections = shapeIntersections.getIntersections();
        allIntersections.insert(
            allIntersections.end(),
            intersections.begin(),
            intersections.end()
        );
    }

    // Erstelle Intersections-Objekt (wird automatisch sortiert)
    return Intersections(allIntersections);
}

/**
 * Fügt eine Lichtquelle zur Szene hinzu
 */
void Scene::addLight(LightSource* light) {
    if (light == nullptr) {
        return;
    }
    lights_.push_back(light);
}

/**
 * Prüft, ob eine bestimmte Lichtquelle in der Szene enthalten ist
 */
bool Scene::containsLight(const LightSource* light) const {
    return std::find(lights_.begin(), lights_.end(), light) != lights_.end();
}

/**
 * Gibt die Anzahl der Lichtquellen zurück
 */
size_t Scene::getLightCount() const {
    return lights_.size();
}

/**
 * Gibt die Liste aller Lichtquellen zurück
 */
const std::vector<LightSource*>& Scene::getLights() const {
    return lights_;
}

/**
 * Gibt die erste Lichtquelle zurück
 */
LightSource* Scene::getFirstLight() const {
    if (lights_.empty()) {
        return nullptr;
    }
    return lights_[0];
}

/**
 * Berechnet HitInfo aus einem Intersection und einem Ray
 */
HitInfo Scene::computeHitInfo(const Intersection& intersection, const Ray& ray) {
    // t-Wert und Form aus Intersection
    double t = intersection.t;
    const Shape* shape = intersection.shape;

    // Schnittpunkt berechnen: origin + direction * t
    Point point = ray.pointAt(t);

    // Eye-Vektor: Negierte Richtung des Strahls (zeigt zur Kamera)
    Vector eyeVector = -ray.getDirection();

    // Normale am Schnittpunkt berechnen
    Vector normal = shape->normalAt(point);

    return HitInfo(t, shape, point, eyeVector, normal);
}

/**
 * Berechnet die Farbe an einem Schnittpunkt mit Beleuchtung
 */
Color Scene::shadeHit(const HitInfo& hitInfo) const {
    // Akkumuliere Farben von allen Lichtquellen
    Color finalColor = Color(0, 0, 0);

    // Hole Material des getroffenen Objekts
    const Material& material = hitInfo.shape->getMaterial();

    // Für jede Lichtquelle in der Szene
    for (const auto& light : lights_) {
        // Berechne Beleuchtung mit Phong-Modell
        Color lightContribution = material.phongLighting(
            light,
            hitInfo.point,
            hitInfo.eyeVector,
            hitInfo.normal
        );

        // Addiere zur Gesamtfarbe
        finalColor = finalColor + lightContribution;
    }

    return finalColor;
}

/**
 * Berechnet die Farbe für einen Strahl
 */
Color Scene::colorAt(const Ray& ray) const {
    // 1. Finde alle Schnittpunkte
    Intersections intersections = traceRay(ray);

    // 2. Finde den ersten sichtbaren Schnittpunkt (kleinster positiver t-Wert)
    const Intersection* hit = intersections.hit();

    // 3. Wenn kein Schnittpunkt gefunden wurde, gib Hintergrundfarbe zurück
    if (hit == nullptr) {
        return Color(0, 0, 0); // Schwarz
    }

    // 4. Berechne HitInfo
    HitInfo hitInfo = computeHitInfo(*hit, ray);

    // 5. Berechne Farbe mit Beleuchtung
    return shadeHit(hitInfo);
}

/**
 * Erstellt eine Standard-Testszene
 */
Scene* Scene::defaultScene() {
    Scene* scene = new Scene();

    // Erste Kugel: Nicht transformiert (Einheitskugel im Ursprung)
    // Mit speziellem Material
    Sphere* sphere1 = new Sphere("sphere1");
    Material mat1;
    mat1.color = Color(0.8, 1.0, 0.6);
    mat1.diffuse = 0.7;
    mat1.specular = 0.2;
    sphere1->setMaterial(mat1);
    scene->addObject(sphere1);

    // Zweite Kugel: Um die Hälfte in alle Richtungen skaliert
    Sphere* sphere2 = new Sphere("sphere2");
    sphere2->setTransform(Matrix::scale(0.5, 0.5, 0.5));
    scene->addObject(sphere2);

    // Lichtquelle hinzufügen
    PointLightSource* light = new PointLightSource(
        Point(-10, 10, -10),
        Color(1, 1, 1),
        1.0
    );
    scene->addLight(light);

    return scene;
}
