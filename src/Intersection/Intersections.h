#pragma once
#include "Intersection.h"
#include <vector>
#include <initializer_list>

/**
 * Intersections-Klasse
 *
 * Speichert und verwaltet mehrere Intersection-Objekte.
 *
 * Features:
 * - Automatische Sortierung nach t-Werten
 * - Indizierter Zugriff auf einzelne Schnittpunkte
 * - Anzahl der Schnittpunkte abfragen
 * - Hit-Bestimmung (erster positiver Schnittpunkt)
 */
class Intersections {
private:
    std::vector<Intersection> intersections;

    /**
     * Sortiert die Intersections nach ihren t-Werten (aufsteigend)
     */
    void sort();

public:
    /**
     * Standard-Konstruktor: Leere Liste
     */
    Intersections();

    /**
     * Konstruktor mit initializer_list
     * @param list Liste von Intersection-Objekten
     */
    Intersections(std::initializer_list<Intersection> list);

    /**
     * Konstruktor mit Vektor
     * @param vec Vektor von Intersection-Objekten
     */
    Intersections(const std::vector<Intersection>& vec);

    /**
     * Fügt einen Schnittpunkt hinzu und sortiert die Liste neu
     * @param intersection Der hinzuzufügende Schnittpunkt
     */
    void add(const Intersection& intersection);

    /**
     * Gibt die Anzahl der Schnittpunkte zurück
     * @return Anzahl der Schnittpunkte
     */
    size_t count() const;

    /**
     * Prüft, ob die Liste leer ist
     * @return true wenn keine Schnittpunkte vorhanden sind
     */
    bool isEmpty() const;

    /**
     * Indizierter Zugriff auf Schnittpunkte (const)
     * @param index Index des Schnittpunkts
     * @return Referenz auf den Schnittpunkt
     */
    const Intersection& operator[](size_t index) const;

    /**
     * Indizierter Zugriff auf Schnittpunkte
     * @param index Index des Schnittpunkts
     * @return Referenz auf den Schnittpunkt
     */
    Intersection& operator[](size_t index);

    /**
     * Bestimmt den "Hit" - den ersten sichtbaren Schnittpunkt
     *
     * Der Hit ist der Schnittpunkt mit dem kleinsten positiven t-Wert.
     * Wenn alle t-Werte negativ sind, gibt es keinen Hit.
     *
     * @return Zeiger auf den Hit-Schnittpunkt oder nullptr wenn kein Hit
     */
    const Intersection* hit() const;

    /**
     * Gibt direkten Zugriff auf den internen Vektor (const)
     * @return Referenz auf den Vektor
     */
    const std::vector<Intersection>& getIntersections() const;
};
