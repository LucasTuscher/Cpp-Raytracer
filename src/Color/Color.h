#pragma once
#include <cmath>
#include <stdexcept>

/**
 * Color-Klasse
 * 
 * Repräsentiert eine Farbe im RGB-Farbraum.
 * Jede Komponente (Rot, Grün, Blau) hat normalerweise einen Wert zwischen 0.0 und 1.0,
 * kann aber bei Berechnungen auch außerhalb dieses Bereichs liegen.
 * 
 * Das RGB-Modell ist additiv:
 * - (0, 0, 0) = Schwarz (keine Farbe)
 * - (1, 1, 1) = Weiß (alle Farben)
 * - (1, 0, 0) = Rot
 * - (0, 1, 0) = Grün
 * - (0, 0, 1) = Blau
 * 
 * Wichtige Operationen:
 * - Addition: Lichter kombinieren
 * - Hadamard-Produkt: Farbfilterung, Materialfarben
 * - Clamping: Werte auf [0,1] beschränken für Ausgabe
 */
class Color {
public:
    // RGB-Komponenten (normalerweise 0.0 bis 1.0)
    double r, g, b;

    /**
     * Konstruktor
     * @param r Rot-Komponente (Standard: 0.0)
     * @param g Grün-Komponente (Standard: 0.0)
     * @param b Blau-Komponente (Standard: 0.0)
     */
    Color(double r = 0.0, double g = 0.0, double b = 0.0);

    /**
     * Addition: Farbe + Farbe
     * 
     * Addiert zwei Farben komponentenweise.
     * Wird verwendet um Lichtquellen zu kombinieren.
     * 
     * @param c Die zu addierende Farbe
     * @return Summenfarbe
     */
    Color operator+(const Color& c) const;
    
    /**
     * Multiplikation: Farbe * Skalar
     * 
     * Skaliert die Helligkeit der Farbe.
     * Werte > 1.0 machen die Farbe heller, < 1.0 dunkler.
     * 
     * @param s Skalierungsfaktor
     * @return Skalierte Farbe
     */
    Color operator*(double s) const;
    
    /**
     * Hadamard-Produkt: Farbe * Farbe
     * 
     * Multipliziert zwei Farben komponentenweise.
     * Wird für Farbfilterung und Materialfarben verwendet.
     * 
     * Beispiel: Weißes Licht (1,1,1) durch roten Filter (1,0,0) = Rot (1,0,0)
     * 
     * @param c Die zu multiplizierende Farbe
     * @return Produktfarbe
     */
    Color operator*(const Color& c) const;
    
    /**
     * Division: Farbe / Skalar
     * 
     * Teilt die Farbkomponenten durch einen Skalar.
     * 
     * @param s Divisor (darf nicht 0 sein)
     * @return Geteilte Farbe
     * @throws std::runtime_error bei Division durch Null
     */
    Color operator/(double s) const;
    
    /**
     * Clamping: Beschränkt Farbwerte auf [0, 1]
     * 
     * Schneidet Werte ab, die außerhalb des gültigen Bereichs liegen.
     * Notwendig vor der Bildausgabe.
     * 
     * Achtung: Kann Farbverhältnisse verfälschen!
     * Für bessere Ergebnisse sollte Tone Mapping verwendet werden.
     * 
     * @return Farbe mit Werten in [0, 1]
     */
    Color clamped() const;

    /**
     * Gleichheitsvergleich mit Epsilon-Toleranz
     * 
     * @param c Die zu vergleichende Farbe
     * @return true wenn die Farben (nahezu) gleich sind
     */
    bool operator==(const Color& c) const;
    
    /**
     * Ungleichheitsvergleich
     * 
     * @param c Die zu vergleichende Farbe
     * @return true wenn die Farben unterschiedlich sind
     */
    bool operator!=(const Color& c) const;
    
    /**
     * Vergleicht zwei Farben mit angegebener Toleranz
     * 
     * @param c Die zu vergleichende Farbe
     * @param eps Toleranz (Standard: 1e-6)
     * @return true wenn alle Komponenten innerhalb der Toleranz liegen
     */
    bool equals(const Color& c, double eps = 1e-6) const;

    // Vordefinierte Farben
    
    /**
     * @return Schwarz (0, 0, 0)
     */
    static Color BLACK();
    
    /**
     * @return Weiß (1, 1, 1)
     */
    static Color WHITE();
    
    /**
     * @return Rot (1, 0, 0)
     */
    static Color RED();
    
    /**
     * @return Grün (0, 1, 0)
     */
    static Color GREEN();
    
    /**
     * @return Blau (0, 0, 1)
     */
    static Color BLUE();
};
