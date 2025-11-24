/**
 * Scene.cpp
 *
 * Implementierung der Scene-Klasse zur Verwaltung von 3D-Szenen.
 */
#include "Scene.h"
#include "../Shape/Sphere.h"
#include "../Matrix/Matrix.h"
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
 * Erstellt eine Standard-Testszene
 */
Scene* Scene::defaultScene() {
    Scene* scene = new Scene();

    // Erste Kugel: Nicht transformiert (Einheitskugel im Ursprung)
    Sphere* sphere1 = new Sphere("sphere1");
    scene->addObject(sphere1);

    // Zweite Kugel: Um die Hälfte in alle Richtungen skaliert
    Sphere* sphere2 = new Sphere("sphere2");
    sphere2->setTransform(Matrix::scale(0.5, 0.5, 0.5));
    scene->addObject(sphere2);

    return scene;
}
