#pragma once
#include "../Color/Color.h"
#include <string>
#include <vector>

/**
 * Canvas-Klasse
 * 
 * Repräsentiert ein Bild als Pixelgitter.
 * Bietet Operationen zum Setzen/Lesen von Pixeln und Speichern in PNG-Format.
 * 
 * Koordinatensystem:
 * - x-Achse: von links nach rechts (0 bis width-1)
 * - y-Achse: von oben nach unten (0 bis height-1)
 * - Ursprung (0,0) ist oben links
 */
class Canvas {
private:
    int width;
    int height;
    std::string fileName;
    std::vector<Color> pixels;  // Pixel-Daten (row-major order)

public:
    /**
     * Konstruktor ohne Dateinamen
     * @param width Breite des Bildes
     * @param height Höhe des Bildes
     */
    Canvas(int width, int height);

    /**
     * Konstruktor mit Dateinamen
     * @param width Breite des Bildes
     * @param height Höhe des Bildes
     * @param fileName Name der Ausgabedatei (ohne Erweiterung)
     */
    Canvas(int width, int height, const std::string& fileName);

    /**
     * Gibt die Breite des Canvas zurück
     * @return Breite in Pixeln
     */
    int getWidth() const { return width; }

    /**
     * Gibt die Höhe des Canvas zurück
     * @return Höhe in Pixeln
     */
    int getHeight() const { return height; }

    /**
     * Gibt den Dateinamen zurück
     * @return Dateiname (leer wenn nicht gesetzt)
     */
    std::string getFileName() const { return fileName; }

    /**
     * Setzt einen Pixel auf eine bestimmte Farbe
     * @param x X-Koordinate (0 bis width-1)
     * @param y Y-Koordinate (0 bis height-1)
     * @param color Farbe des Pixels
     */
    void setPixel(int x, int y, const Color& color);

    /**
     * Liest die Farbe eines Pixels
     * @param x X-Koordinate (0 bis width-1)
     * @param y Y-Koordinate (0 bis height-1)
     * @return Farbe des Pixels
     */
    Color getPixel(int x, int y) const;

    /**
     * Speichert das Bild in eine PNG-Datei
     * 
     * Verwendet den gespeicherten Dateinamen oder generiert einen aus dem Timestamp.
     * Die Datei wird im aktuellen Verzeichnis gespeichert.
     * 
     * @return true bei Erfolg, false bei Fehler
     */
    bool save() const;

    /**
     * Speichert das Bild in eine PNG-Datei mit spezifischem Namen
     * @param filename Dateiname (ohne Erweiterung)
     * @return true bei Erfolg, false bei Fehler
     */
    bool save(const std::string& filename) const;

private:
    /**
     * Berechnet den Index im Pixel-Array
     * @param x X-Koordinate
     * @param y Y-Koordinate
     * @return Index im Array
     */
    int getIndex(int x, int y) const { return y * width + x; }

    /**
     * Generiert einen Dateinamen mit Timestamp
     * @param baseName Basisname (Standard: "raytracer")
     * @return Dateiname mit Timestamp
     */
    std::string generateTimestampFilename(const std::string& baseName = "raytracer") const;
};
