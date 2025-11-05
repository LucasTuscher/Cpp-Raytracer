/**
 * Color.cpp
 * 
 * Implementierung der Color-Klasse für RGB-Farbberechnungen.
 * Alle Operationen geben neue Color-Instanzen zurück (immutable).
 */
#include "Color.h"
#include <algorithm> // für std::clamp
#include <cmath>
#include <stdexcept>

// Konstruktor: Initialisiert RGB-Komponenten
Color::Color(double r, double g, double b) : r(r), g(g), b(b) {}

/**
 * Addition zweier Farben
 * 
 * Addiert die RGB-Komponenten komponentenweise.
 * Wird verwendet um mehrere Lichtquellen zu kombinieren.
 */
Color Color::operator+(const Color& c) const {
    return Color(r + c.r, g + c.g, b + c.b);
}

/**
 * Skalarmultiplikation
 * 
 * Multipliziert alle Farbkomponenten mit einem Skalar.
 * Wird verwendet um die Helligkeit zu ändern oder Lichtintensität anzupassen.
 */
Color Color::operator*(double s) const {
    return Color(r * s, g * s, b * s);
}

/**
 * Hadamard-Produkt (komponentenweise Multiplikation)
 * 
 * Multipliziert zwei Farben komponentenweise: (r1*r2, g1*g2, b1*b2)
 * 
 * Anwendungen:
 * - Farbfilterung (Licht durch farbiges Glas)
 * - Materialfarbe mit Lichtfarbe kombinieren
 * - Texturfarbe mit Beleuchtung multiplizieren
 */
Color Color::operator*(const Color& c) const {
    return Color(r * c.r, g * c.g, b * c.b);
}

/**
 * Division durch Skalar
 * 
 * Teilt alle Farbkomponenten durch einen Skalar.
 * Nützlich für Durchschnittsberechnungen (z.B. bei Anti-Aliasing).
 */
Color Color::operator/(double s) const {
    if (std::fabs(s) < 1e-12) {
        throw std::runtime_error("Division durch Null in Color::operator/"); 
    }
    return Color(r / s, g / s, b / s);
}

/**
 * Clamping: Beschränkt Farbwerte auf [0, 1]
 * 
 * Schneidet Werte ab, die außerhalb des gültigen Bereichs liegen:
 * - Werte < 0.0 werden auf 0.0 gesetzt
 * - Werte > 1.0 werden auf 1.0 gesetzt
 * 
 * Notwendig vor der Ausgabe in Bildformate.
 * 
 * Hinweis: Einfaches Clamping kann zu Farbverfälschungen führen.
 * Für bessere Ergebnisse sollte später Tone Mapping implementiert werden.
 */
Color Color::clamped() const {
    return Color(
        std::clamp(r, 0.0, 1.0),
        std::clamp(g, 0.0, 1.0),
        std::clamp(b, 0.0, 1.0)
    );
}

/**
 * Vergleicht zwei Farben mit Epsilon-Toleranz
 * 
 * Berücksichtigt Gleitkomma-Ungenauigkeiten durch Verwendung einer Toleranz.
 */
bool Color::equals(const Color& c, double eps) const {
    return std::fabs(r - c.r) <= eps &&
           std::fabs(g - c.g) <= eps &&
           std::fabs(b - c.b) <= eps;
}

// Gleichheitsoperator: Verwendet Standard-Epsilon
bool Color::operator==(const Color& c) const {
    return equals(c);
}

// Ungleichheitsoperator
bool Color::operator!=(const Color& c) const {
    return !equals(c);
}

// Vordefinierte Standardfarben
Color Color::BLACK() { return Color(0.0, 0.0, 0.0); }  // Schwarz
Color Color::WHITE() { return Color(1.0, 1.0, 1.0); }  // Weiß
Color Color::RED()   { return Color(1.0, 0.0, 0.0); }  // Rot
Color Color::GREEN() { return Color(0.0, 1.0, 0.0); }  // Grün
Color Color::BLUE()  { return Color(0.0, 0.0, 1.0); }  // Blau

/**
 * Konvertiert Farbe in INT_RGB Format (0xRRGGBB)
 * 
 * Wandelt die Farbkomponenten in 8-bit Integer-Werte um (0-255).
 * Die Werte werden vorher geclampet auf [0,1].
 */
int Color::toIntRGB() const {
    Color clamped = this->clamped();
    int red   = static_cast<int>(clamped.r * 255.0);
    int green = static_cast<int>(clamped.g * 255.0);
    int blue  = static_cast<int>(clamped.b * 255.0);
    
    return (red << 16) | (green << 8) | blue;
}

/**
 * Erstellt eine Farbe aus INT_RGB Format
 * 
 * Wandelt 8-bit Integer-Werte zurück in Farbkomponenten [0,1].
 */
Color Color::fromIntRGB(int rgb) {
    int red   = (rgb >> 16) & 0xFF;
    int green = (rgb >> 8) & 0xFF;
    int blue  = rgb & 0xFF;
    
    return Color(
        red / 255.0,
        green / 255.0,
        blue / 255.0
    );
}
