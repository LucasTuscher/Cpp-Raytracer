/**
 * Scene.cpp
 *
 * Implementierung der Scene-Klasse zur Verwaltung von 3D-Szenen.
 */
#include "Scene.h"
#include "../Shape/Sphere.h"
#include "../Matrix/Matrix.h"
#include "../Color/Color.h"
#include "../Texture/Texture.h"
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

    // UV-Koordinaten am Schnittpunkt berechnen (für Texturen)
    UV uv = shape->uvAt(point);

    return HitInfo(t, shape, point, eyeVector, normal, uv);
}

/**
 * Berechnet die Farbe an einem Schnittpunkt mit Beleuchtung
 */
Color Scene::shadeHit(const HitInfo& hitInfo, int depth) const {
    // Akkumuliere Farben von allen Lichtquellen
    Color surfaceColor = Color(0, 0, 0);

    // Hole Material des getroffenen Objekts
    const Material& material = hitInfo.shape->getMaterial();

    // Basis-/Texturfarbe am Schnittpunkt (einmal pro Hit berechnen)
    Color surfaceColorAtPoint = material.color;
    if (material.texture != nullptr) {
        // Texturfarbe wird mit material.color getintet (material.color == Weiß -> unverändert)
        surfaceColorAtPoint = material.texture->sample(hitInfo.uv, hitInfo.point) * material.color;
    }

    // Für jede Lichtquelle in der Szene
    for (const auto& light : lights_) {
        // Schatten prüfen
        bool inShadow = isInShadow(hitInfo.pointPlus, light);

        // Berechne Beleuchtung mit Phong-Modell
        Color lightContribution = material.phongLighting(
            light,
            hitInfo.point,
            hitInfo.eyeVector,
            hitInfo.normal,
            inShadow,
            &surfaceColorAtPoint
        );

        // Addiere zur Gesamtfarbe
        surfaceColor = surfaceColor + lightContribution;
    }

    // Berechne reflektierte Farbe und füge sie hinzu
    Color reflectedContribution = reflectedColor(hitInfo, depth);
    double reflectivity = material.reflectivity;
    reflectivity = std::clamp(reflectivity, 0.0, 1.0);
    surfaceColor = surfaceColor * (1.0 - reflectivity) + reflectedContribution;

    return surfaceColor;
}

/**
 * Berechnet die reflektierte Farbe an einem Schnittpunkt
 */
Color Scene::reflectedColor(const HitInfo& hitInfo, int depth) const {
    // Wenn Rekursionstiefe erschöpft ist, keine weitere Reflexion
    if (depth <= 0) {
        return Color(0, 0, 0);
    }

    // Hole Material-Reflexivität
    const Material& material = hitInfo.shape->getMaterial();
    double reflectivity = material.reflectivity;

    // Wenn Material nicht reflektiert, gib schwarz zurück
    if (reflectivity <= 0.0) {
        return Color(0, 0, 0);
    }

    // Erstelle reflektierten Strahl vom Schnittpunkt aus
    // Verwende pointPlus um Selbst-Intersektionen zu vermeiden (shadow acne)
    Ray reflectedRay(hitInfo.pointPlus, hitInfo.reflectVector);

    // Berechne Farbe des reflektierten Strahls (mit reduzierter Tiefe)
    Color reflectedColor = colorAt(reflectedRay, depth - 1);

    // Multipliziere mit Reflexivität des Materials
    return reflectedColor * reflectivity;
}

/**
 * Berechnet die Farbe für einen Strahl
 */
Color Scene::colorAt(const Ray& ray, int depth) const {
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

    // 5. Berechne Farbe mit Beleuchtung und Reflexion
    return shadeHit(hitInfo, depth);
}

/**
 * Prüft, ob ein Punkt im Schatten einer Lichtquelle liegt
 */
bool Scene::isInShadow(const Point& point, const LightSource* light) const {
    if (light == nullptr) {
        return false;
    }

    // Richtung und Distanz zur Lichtquelle
    Vector directionToLight = light->directionFromPoint(point);
    double distanceToLight = light->distanceToPoint(point);

    // Kleines Epsilon, um Self-Intersections zu vermeiden
    Point shadowOrigin = point + directionToLight * 1e-4;
    Ray shadowRay(shadowOrigin, directionToLight);

    // Schnittpunkte entlang des Schattenstrahls
    Intersections xs = traceRay(shadowRay);
    const Intersection* hit = xs.hit();

    // Wenn kein Hit: kein Schatten
    if (hit == nullptr) {
        return false;
    }

    // Bei gerichteten Lichtern ist distanceToLight = inf, jeder positive Hit bedeutet Schatten
    return hit->t >= 0.0 && hit->t < distanceToLight;
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
        200.0  // Höhere Intensität wegen Attenuation (inverse square law)
    );
    scene->addLight(light);

    return scene;
}
